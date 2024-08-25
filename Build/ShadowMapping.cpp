#include "ShadowMapping.h"
#include "level.h"
#include "gameobject.h"
#include "Main.h"
#include "Renderer.h"

ShadowMapping::ShadowMapping(Level* level)
{
	this->pLevel = level;
	this->quarity = 1024.0f*4.0f;
	this->quarityblur = 1024.0f*4.0f;
	this->hw = 128.0f;

	pos = XMFLOAT3(100.0f, 100.0f, 100.0f);
	at = XMFLOAT3(0.0f, 0.0f, 0.0f);
	up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	this->ShadowMap.Enable = TRUE;
	SetShadowMode(SHADOW_MODE::VARIANCE);
	m_VertexShaderShadow = nullptr;
	m_PixelShaderShadow = nullptr;

	m_VertexShaderShadow2D = nullptr;
	m_PixelShaderShadowX = nullptr;
	m_PixelShaderShadowY = nullptr;

	this->ShadowTarget = nullptr;

	this->dir=XMFLOAT3(0.0f, 1.0f, 0.0f);
	this->len = 100.0f;
	this->vNear = 10.0f;
	this->vFar = 1000.0f;
}

ShadowMapping::~ShadowMapping()
{
}

void ShadowMapping::Init(void)
{
	Renderer* renderer = this->pLevel->GetMain()->GetRenderer();

	HRESULT hr;

	renderer->CreateVSFile("shader.hlsl", "VS_SM", &m_VertexShaderShadow);
	renderer->CreatePSFile("shader.hlsl", "PS_SM", &m_PixelShaderShadow);

	renderer->CreateVSFile("shader.hlsl", "VS_2D", &m_VertexShaderShadow2D);
	renderer->CreatePSFile("shader.hlsl", "xpass", &m_PixelShaderShadowX);
	renderer->CreatePSFile("shader.hlsl", "ypass", &m_PixelShaderShadowY);


	// シャドウ マップの作成
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = (UINT)quarity;   // 幅
	descDepth.Height = (UINT)quarity;  // 高さ
	descDepth.MipLevels = 1;       // ミップマップ レベル数
	descDepth.ArraySize = 1;       // 配列サイズ
	descDepth.Format = DXGI_FORMAT_R32_TYPELESS;  // フォーマット
	descDepth.SampleDesc.Count = 1;  // マルチサンプリングの設定
	descDepth.SampleDesc.Quality = 0;  // マルチサンプリングの品質
	descDepth.Usage = D3D11_USAGE_DEFAULT;      // デフォルト使用法
	descDepth.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL; // 深度/ステンシル、シェーダ リソース ビューとして使用
	descDepth.CPUAccessFlags = 0;   // CPUからはアクセスしない
	descDepth.MiscFlags = 0;   // その他の設定なし
	hr = renderer->GetDevice()->CreateTexture2D(
		&descDepth,         // 作成する2Dテクスチャの設定
		nullptr,               // 
		&ShadowMapDS);     // 作成したテクスチャを受け取る変数

	// 深度/ステンシル ビューの作成
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;            // ビューのフォーマット
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Flags = 0;
	descDSV.Texture2D.MipSlice = 0;
	hr = renderer->GetDevice()->CreateDepthStencilView(
		ShadowMapDS,         // 深度/ステンシル・ビューを作るテクスチャ
		&descDSV,             // 深度/ステンシル・ビューの設定
		&ShadowMapDSView); // 作成したビューを受け取る変数

	// シェーダ リソース ビューの作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = DXGI_FORMAT_R32_FLOAT; // フォーマット
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;  // 2Dテクスチャ
	srDesc.Texture2D.MostDetailedMip = 0;   // 最初のミップマップ レベル
	srDesc.Texture2D.MipLevels = -1;  // すべてのミップマップ レベル

	// シェーダ リソース ビューの作成
	hr = renderer->GetDevice()->CreateShaderResourceView(
		ShadowMapDS,          // アクセスするテクスチャ リソース
		&srDesc,               // シェーダ リソース ビューの設定
		&ShadowMapDSSRView);  // 受け取る変数





	// シャドウ マップの作成
	descDepth.Format = DXGI_FORMAT_R32G32_TYPELESS;  // フォーマット
	descDepth.Usage = D3D11_USAGE_DEFAULT;      // デフォルト使用法
	descDepth.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; //シェーダ リソース ビューとして使用
	hr = renderer->GetDevice()->CreateTexture2D(
		&descDepth,         // 作成する2Dテクスチャの設定
		NULL,               // 
		&ShadowMapingTexture);     // 作成したテクスチャを受け取る変数



	// レンダーターゲットビューの設定
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0, sizeof(rtvDesc));
	rtvDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	
	// レンダーターゲットビューの生成


	hr = renderer->GetDevice()->CreateRenderTargetView(ShadowMapingTexture, &rtvDesc, &RenderTargetShadow);

	// シェーダ リソース ビューの作成
	srDesc.Format = DXGI_FORMAT_R32G32_FLOAT; // フォーマット
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;  // 2Dテクスチャ
	srDesc.Texture2D.MostDetailedMip = 0;   // 最初のミップマップ レベル
	srDesc.Texture2D.MipLevels = -1;  // すべてのミップマップ レベル
	
	// シェーダ リソース ビューの作成
	hr = renderer->GetDevice()->CreateShaderResourceView(
		ShadowMapingTexture,          // アクセスするテクスチャ リソース
		&srDesc,               // シェーダ リソース ビューの設定
		&ShadowMapSRView);  // 受け取る変数




	// Xシャドウ マップの作成
	descDepth.Width = quarityblur;
	descDepth.Height = quarityblur;

	descDepth.Format = DXGI_FORMAT_R32G32_TYPELESS;  // フォーマット
	descDepth.Usage = D3D11_USAGE_DEFAULT;      // デフォルト使用法
	descDepth.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; //シェーダ リソース ビューとして使用
	hr = renderer->GetDevice()->CreateTexture2D(
		&descDepth,         // 作成する2Dテクスチャの設定
		NULL,               // 
		&ShadowMapingTextureX);     // 作成したテクスチャを受け取る変数



	// レンダーターゲットビューの生成


	hr = renderer->GetDevice()->CreateRenderTargetView(ShadowMapingTextureX, &rtvDesc, &RenderTargetShadowX);


	// シェーダ リソース ビューの作成
	hr = renderer->GetDevice()->CreateShaderResourceView(
		ShadowMapingTextureX,          // アクセスするテクスチャ リソース
		&srDesc,               // シェーダ リソース ビューの設定
		&ShadowMapSRViewX);  // 受け取る変数


	// Yシャドウ マップの作成
	descDepth.Width = quarityblur;
	descDepth.Height = quarityblur;

	descDepth.Format = DXGI_FORMAT_R32G32_TYPELESS;  // フォーマット
	descDepth.Usage = D3D11_USAGE_DEFAULT;      // デフォルト使用法
	descDepth.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; //シェーダ リソース ビューとして使用
	hr = renderer->GetDevice()->CreateTexture2D(
		&descDepth,         // 作成する2Dテクスチャの設定
		NULL,               // 
		&ShadowMapingTextureY);     // 作成したテクスチャを受け取る変数



	

	// レンダーターゲットビューの生成


	hr = renderer->GetDevice()->CreateRenderTargetView(ShadowMapingTextureY, &rtvDesc, &RenderTargetShadowY);

	// シェーダ リソース ビューの作成
	hr = renderer->GetDevice()->CreateShaderResourceView(
		ShadowMapingTextureY,          // アクセスするテクスチャ リソース
		&srDesc,               // シェーダ リソース ビューの設定
		&ShadowMapSRViewY);  // 受け取る変数




		// ビューポートの設定
	ViewPortShadowMap[0].TopLeftX = 0.0f;		// ビューポート領域の左上X座標。
	ViewPortShadowMap[0].TopLeftY = 0.0f;		// ビューポート領域の左上Y座標。
	ViewPortShadowMap[0].Width = quarity;	// ビューポート領域の幅
	ViewPortShadowMap[0].Height = quarity;	// ビューポート領域の高さ
	ViewPortShadowMap[0].MinDepth = 0.0f;		// ビューポート領域の深度値の最小値
	ViewPortShadowMap[0].MaxDepth = 1.0f;		// ビューポート領域の深度値の最大値


	ViewPortShadowMap[1].TopLeftX = 0.0f;		// ビューポート領域の左上X座標。
	ViewPortShadowMap[1].TopLeftY = 0.0f;		// ビューポート領域の左上Y座標。
	ViewPortShadowMap[1].Width = quarityblur;	// ビューポート領域の幅
	ViewPortShadowMap[1].Height = quarityblur;	// ビューポート領域の高さ
	ViewPortShadowMap[1].MinDepth = 0.0f;		// ビューポート領域の深度値の最小値
	ViewPortShadowMap[1].MaxDepth = 1.0f;		// ビューポート領域の深度値の最大値




	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	renderer->GetDevice()->CreateBuffer(&bd, NULL, &this->VertexBuffer);

	D3D11_MAPPED_SUBRESOURCE msr;
	ID3D11DeviceContext* deviceContext = renderer->GetDeviceContext();
	renderer->GetDeviceContext()->Map(VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;


	// 指定された座標を中心に設定するプログラム

	// 頂点０番（左上の頂点）
	vertex[0].Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f,0.0f);

	// 頂点１番（右上の頂点）
	vertex[1].Position = XMFLOAT3(quarityblur, 0.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	// 頂点２番（左下の頂点）
	vertex[2].Position = XMFLOAT3(0.0f, quarityblur, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	// 頂点３番（右下の頂点）
	vertex[3].Position = XMFLOAT3(quarityblur, quarityblur, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);
	
	renderer->GetDeviceContext()->Unmap(VertexBuffer, 0);



}

void ShadowMapping::Uninit(void)
{

	if (m_VertexShaderShadow)			m_VertexShaderShadow->Release();
	if (m_PixelShaderShadow)			m_PixelShaderShadow->Release();
	if (m_VertexShaderShadow2D)			m_VertexShaderShadow2D->Release();
	if (m_PixelShaderShadowX)			m_PixelShaderShadowX->Release();
	if (m_PixelShaderShadowY)			m_PixelShaderShadowY->Release();
	if (ShadowMapDS) ShadowMapDS->Release();
	if (ShadowMapDSView) ShadowMapDSView->Release();
	if (ShadowMapDSSRView) ShadowMapDSSRView->Release();
	if (ShadowMapingTexture) ShadowMapingTexture->Release();
	if (RenderTargetShadow) RenderTargetShadow->Release();
	if (ShadowMapSRView) ShadowMapSRView->Release();
	if (ShadowMapingTextureX) ShadowMapingTextureX->Release();
	if (RenderTargetShadowX) RenderTargetShadowX->Release();
	if (ShadowMapSRViewX) ShadowMapSRViewX->Release();
	if (ShadowMapingTextureY) ShadowMapingTextureY->Release();
	if (RenderTargetShadowY) RenderTargetShadowY->Release();
	if (ShadowMapSRViewY) ShadowMapSRViewY->Release();
	if (VertexBuffer) VertexBuffer->Release();

}

void ShadowMapping::Update(void)
{
	Renderer* renderer= this->pLevel->GetMain()->GetRenderer();
	if (ShadowTarget!=nullptr)
	{
		this->at = ShadowTarget->GetTransFormComponent()->GetPosition();
		this->pos.x = at.x + dir.x * len;
		this->pos.y = at.y + dir.y * len;
		this->pos.z = at.z + dir.z * len;
	}
	
	
}

void ShadowMapping::Draw(void)
{
	Renderer* renderer = pLevel->GetMain()->GetRenderer();
	this->SetShaderShadow();
	
	// 描画ターゲットのクリア
	float ClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };	// 背景色
	renderer->GetDeviceContext()->ClearRenderTargetView(
		RenderTargetShadow, // クリアする描画ターゲット
		ClearColor);         // クリアする値

	ClearColor[0] = 0.0f;	// 背景色
	ClearColor[1] = 0.0f;	// 背景色


	// 深度/ステンシルのクリア
	renderer->GetDeviceContext()->ClearDepthStencilView(ShadowMapDSView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// RSにビューポートを設定
	renderer->GetDeviceContext()->RSSetViewports(1, ViewPortShadowMap);


	//アルファ値も使う場合
	renderer->GetDeviceContext()->OMSetRenderTargets(1, &RenderTargetShadow, ShadowMapDSView);

	//デプスステンシルバッファのみでシャドウマッピングする場合
	//ID3D11RenderTargetView* pRender[1] = { NULL };
	//renderer->GetDeviceContext()->OMSetRenderTargets(1, pRender, ShadowMapDSView);

	XMMATRIX mtxShadowMapView = XMMatrixLookAtLH(XMLoadFloat3(&this->pos), XMLoadFloat3(&this->at), XMLoadFloat3(&this->up));
	renderer->SetViewMatrix(&mtxShadowMapView);
	XMMATRIX mtxShadowMapProj = XMMatrixOrthographicLH(hw, hw, vNear, vFar);
	renderer->SetProjectionMatrix(&mtxShadowMapProj);

	XMMATRIX smwvp = XMMatrixTranspose(XMMatrixIdentity() * mtxShadowMapView * mtxShadowMapProj);

	XMStoreFloat4x4(&this->ShadowMap.SmView, mtxShadowMapView);
	XMStoreFloat4x4(&this->ShadowMap.SmProj, mtxShadowMapProj);
	XMStoreFloat4x4(&this->ShadowMap.SMWorldViewProj, smwvp);

	renderer->SetShadow(&this->ShadowMap);

	pLevel->DrawShadowObject();


	

	////レンダーターゲットから外さないとシェーダーリソースにバインドできない
	renderer->GetDeviceContext()->OMSetRenderTargets(0, nullptr, nullptr);


	renderer->GetDeviceContext()->PSSetShaderResources(1, 1, &this->ShadowMapDSSRView);


	// RSにビューポートを設定
	renderer->GetDeviceContext()->RSSetViewports(1, &ViewPortShadowMap[1]);

	this->SetWorldViewProjection2D();


	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	renderer->GetDeviceContext()->IASetVertexBuffers(0, 1, &this->VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	renderer->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	renderer->SetCullingMode(CULL_MODE_NONE);


	for (int i = 0; i < 1; i++)
	{

		// 描画ターゲットのクリア
		renderer->GetDeviceContext()->ClearRenderTargetView(
			RenderTargetShadowX, // クリアする描画ターゲット
			ClearColor);         // クリアする値



		this->SetShaderXpass();




		if (i == 0)
		{
			renderer->GetDeviceContext()->PSSetShaderResources(0, 1, &this->ShadowMapSRView);

		}
		else
		{

			renderer->GetDeviceContext()->PSSetShaderResources(0, 1, &this->ShadowMapSRViewY);

		}
		renderer->GetDeviceContext()->OMSetRenderTargets(1, &RenderTargetShadowX, nullptr);

		renderer->GetDeviceContext()->Draw(4, 0);

		this->SetShaderYpass();

		// 描画ターゲットのクリア
		renderer->GetDeviceContext()->ClearRenderTargetView(
			RenderTargetShadowY, // クリアする描画ターゲット
			ClearColor);         // クリアする値


		renderer->GetDeviceContext()->OMSetRenderTargets(1, &RenderTargetShadowY, nullptr);


		renderer->GetDeviceContext()->PSSetShaderResources(0, 1, &this->ShadowMapSRViewX);




		renderer->GetDeviceContext()->Draw(4, 0);



		////レンダーターゲットから外さないとシェーダーリソースにバインドできない
		renderer->GetDeviceContext()->OMSetRenderTargets(0, nullptr, nullptr);


	}





	////レンダーターゲットから外さないとシェーダーリソースにバインドできない
	renderer->GetDeviceContext()->OMSetRenderTargets(0, nullptr, nullptr);

	renderer->GetDeviceContext()->PSSetShaderResources(2, 1, &this->ShadowMapSRViewY);

}

void ShadowMapping::SetShadowMap(XMFLOAT3 pos, XMFLOAT3 at, XMFLOAT3 up)
{
	this->pos = pos;
	this->at = at;
	this->up = up;
}

void ShadowMapping::SetPos(XMFLOAT3 pos)
{
	this->pos = pos;
}

void ShadowMapping::SetShadowBuffer(void)
{
	
	this->pLevel->GetMain()->GetRenderer()->SetShadow(&this->ShadowMap);


}
void ShadowMapping::SetShaderShadow(void)
{
	Renderer* renderer = pLevel->GetMain()->GetRenderer();

	renderer->GetDeviceContext()->VSSetShader(m_VertexShaderShadow, NULL, 0);
	renderer->GetDeviceContext()->PSSetShader(m_PixelShaderShadow, NULL, 0);

}
void ShadowMapping::SetShaderXpass(void)
{
	Renderer* renderer = pLevel->GetMain()->GetRenderer();

	renderer->GetDeviceContext()->VSSetShader(m_VertexShaderShadow2D, NULL, 0);
	renderer->GetDeviceContext()->PSSetShader(m_PixelShaderShadowX, NULL, 0);

}
void ShadowMapping::SetShaderYpass(void)
{
	Renderer* renderer = pLevel->GetMain()->GetRenderer();

	renderer->GetDeviceContext()->VSSetShader(m_VertexShaderShadow2D, NULL, 0);
	renderer->GetDeviceContext()->PSSetShader(m_PixelShaderShadowY, NULL, 0);

}
void ShadowMapping::SetWorldViewProjection2D(void)
{
	Renderer* renderer = this->pLevel->GetMain()->GetRenderer();

	XMMATRIX world = XMMatrixIdentity();
	renderer->SetWorldMatrix(&world);

	XMMATRIX view = XMMatrixIdentity();
	renderer->SetViewMatrix(&view);


	XMMATRIX projection;
	projection = XMMatrixOrthographicOffCenterLH(0.0f, this->quarityblur, this->quarityblur, 0.0f, 0.0f, 1.0f);
	renderer->SetProjectionMatrix(&projection);

}

void ShadowMapping::SetShadowMode(SHADOW_MODE mode)
{
	this->ShadowMap.mode = mode;
}

void ShadowMapping::SetTarget(GameObject* gameObject)
{
	this->ShadowTarget = gameObject;
}

void ShadowMapping::SetForward(XMFLOAT3 dir)
{

	this->dir = XMFLOAT3Normalize(dir);
	

}

void ShadowMapping::SetLen(float len)
{
	this->len = len;
}

void ShadowMapping::SetNear(float f)
{
	this->vNear = f;
}

void ShadowMapping::SetFar(float f)
{
	this->vFar = f;

}
