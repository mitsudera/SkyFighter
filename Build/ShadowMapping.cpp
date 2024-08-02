#include "ShadowMapping.h"
#include "level.h"
#include "Main.h"
#include "Renderer.h"

ShadowMapping::ShadowMapping(Level* level)
{
	this->pLevel = level;
	this->quarity = 8192.0f;
	this->hw = 8192.0f;

	pos = XMFLOAT3(100.0f, 100.0f, 100.0f);
	at = XMFLOAT3(0.0f, 0.0f, 0.0f);
	up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	this->ShadowMap.Enable = TRUE;



}

ShadowMapping::~ShadowMapping()
{
}

void ShadowMapping::Init(void)
{
	Renderer* renderer = this->pLevel->GetMain()->GetRenderer();





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
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE; // 深度/ステンシル、シェーダ リソース ビューとして使用
	descDepth.CPUAccessFlags = 0;   // CPUからはアクセスしない
	descDepth.MiscFlags = 0;   // その他の設定なし
	renderer->GetDevice()->CreateTexture2D(
		&descDepth,         // 作成する2Dテクスチャの設定
		NULL,               // 
		&pShadowMapTexture);     // 作成したテクスチャを受け取る変数

	// 深度/ステンシル ビューの作成
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;            // ビューのフォーマット
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Flags = 0;
	descDSV.Texture2D.MipSlice = 0;
	renderer->GetDevice()->CreateDepthStencilView(
		pShadowMapTexture,         // 深度/ステンシル・ビューを作るテクスチャ
		&descDSV,             // 深度/ステンシル・ビューの設定
		&pShadowMapDSView); // 作成したビューを受け取る変数

	// シェーダ リソース ビューの作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = DXGI_FORMAT_R32_FLOAT; // フォーマット
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;  // 2Dテクスチャ
	srDesc.Texture2D.MostDetailedMip = 0;   // 最初のミップマップ レベル
	srDesc.Texture2D.MipLevels = -1;  // すべてのミップマップ レベル

	// シェーダ リソース ビューの作成
	renderer->GetDevice()->CreateShaderResourceView(
		pShadowMapTexture,          // アクセスするテクスチャ リソース
		&srDesc,               // シェーダ リソース ビューの設定
		&pShadowMapSRView);  // 受け取る変数





	// シャドウ マップの作成
	descDepth.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;  // フォーマット
	descDepth.Usage = D3D11_USAGE_DEFAULT;      // デフォルト使用法
	descDepth.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; //シェーダ リソース ビューとして使用
	renderer->GetDevice()->CreateTexture2D(
		&descDepth,         // 作成する2Dテクスチャの設定
		NULL,               // 
		&ShadowMapingTexture);     // 作成したテクスチャを受け取る変数



	// レンダーターゲットビューの設定
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0, sizeof(rtvDesc));
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	// レンダーターゲットビューの生成
	renderer->GetDevice()->CreateRenderTargetView(ShadowMapingTexture, &rtvDesc, &RenderTargetShadow);

	// シェーダ リソース ビューの作成
	srDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // フォーマット
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;  // 2Dテクスチャ
	srDesc.Texture2D.MostDetailedMip = 0;   // 最初のミップマップ レベル
	srDesc.Texture2D.MipLevels = -1;  // すべてのミップマップ レベル

	// シェーダ リソース ビューの作成
	renderer->GetDevice()->CreateShaderResourceView(
		ShadowMapingTexture,          // アクセスするテクスチャ リソース
		&srDesc,               // シェーダ リソース ビューの設定
		&pShadowMapSRViewBA);  // 受け取る変数



		// ビューポートの設定
	ViewPortShadowMap[0].TopLeftX = 0.0f;		// ビューポート領域の左上X座標。
	ViewPortShadowMap[0].TopLeftY = 0.0f;		// ビューポート領域の左上Y座標。
	ViewPortShadowMap[0].Width = quarity;	// ビューポート領域の幅
	ViewPortShadowMap[0].Height = quarity;	// ビューポート領域の高さ
	ViewPortShadowMap[0].MinDepth = 0.0f;		// ビューポート領域の深度値の最小値
	ViewPortShadowMap[0].MaxDepth = 1.0f;		// ビューポート領域の深度値の最大値








}

void ShadowMapping::Uninit(void)
{
}

void ShadowMapping::Update(void)
{
	Renderer* renderer= this->pLevel->GetMain()->GetRenderer();
}

void ShadowMapping::Draw(void)
{
	Renderer* renderer = pLevel->GetMain()->GetRenderer();
	renderer->SetShaderShadow();
	
	// 描画ターゲットのクリア
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// 背景色
	renderer->GetDeviceContext()->ClearRenderTargetView(
		RenderTargetShadow, // クリアする描画ターゲット
		ClearColor);         // クリアする値


	// 深度/ステンシルのクリア
	renderer->GetDeviceContext()->ClearDepthStencilView(pShadowMapDSView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// RSにビューポートを設定
	renderer->GetDeviceContext()->RSSetViewports(1, ViewPortShadowMap);


	//アルファ値も使う場合
	renderer->GetDeviceContext()->OMSetRenderTargets(1, &RenderTargetShadow, pShadowMapDSView);

	////デプスステンシルバッファのみでシャドウマッピングする場合
	//ID3D11RenderTargetView* pRender[1] = { NULL };
	//renderer->GetDeviceContext()->OMSetRenderTargets(1, pRender, pShadowMapDSView);

	XMMATRIX mtxShadowMapView = XMMatrixLookAtLH(XMLoadFloat3(&pos), XMLoadFloat3(&at), XMLoadFloat3(&up));
	renderer->SetViewMatrix(&mtxShadowMapView);
	XMMATRIX mtxShadowMapProj = XMMatrixOrthographicLH(hw, hw, 1.0f, 1000.0f);
	renderer->SetProjectionMatrix(&mtxShadowMapProj);

	XMMATRIX smwvp = XMMatrixTranspose(XMMatrixIdentity() * mtxShadowMapView * mtxShadowMapProj);

	XMStoreFloat4x4(&this->ShadowMap.SmView, mtxShadowMapView);
	XMStoreFloat4x4(&this->ShadowMap.SmProj, mtxShadowMapProj);
	XMStoreFloat4x4(&this->ShadowMap.SMWorldViewProj, smwvp);

	renderer->SetShadow(&this->ShadowMap);

	pLevel->DrawShadowObject();

	renderer->GetDeviceContext()->PSSetShaderResources(1, 1, &this->pShadowMapSRView);
	renderer->GetDeviceContext()->PSSetShaderResources(2, 1, &this->pShadowMapSRViewBA);

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
