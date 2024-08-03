#pragma once
#include "Coreminimal.h"

class Level;

// 影用定数バッファ構造体
struct SHADOWMAP_CBUFFER
{

	XMFLOAT4X4 SMWorldViewProj; // ワールド×ビュー×透視変換行列(シャドウマップ用)

	XMFLOAT4X4 SmView;
	XMFLOAT4X4 SmProj;


	int			Enable;
	int			Dummy[15];
};


class ShadowMapping
{
public:
	ShadowMapping(Level* level);
	~ShadowMapping();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetShadowMap(XMFLOAT3 pos, XMFLOAT3 at, XMFLOAT3 up);
	void SetPos(XMFLOAT3 pos);
	void SetShadowBuffer(void);

private:

	Level* pLevel;

	XMFLOAT3 pos;
	XMFLOAT3 at;
	XMFLOAT3 up;

	float quarity;
	float hw;



	SHADOWMAP_CBUFFER	ShadowMap;

	ID3D11Texture2D* ShadowMapDS;			// シャドウ・マップ
	ID3D11DepthStencilView* ShadowMapDSView;	// 深度/ステンシル・ビュー
	ID3D11ShaderResourceView* ShadowMapDSSRView;	// シェーダ・リソース・ビュー

	ID3D11Texture2D* ShadowMapingTexture;
	ID3D11RenderTargetView* RenderTargetShadow;
	ID3D11ShaderResourceView* ShadowMapSRView;	// シェーダ・リソース・ビュー

	D3D11_VIEWPORT            ViewPortShadowMap[1];       // ビューポート

};

