#pragma once
#include "Coreminimal.h"

class Level;
class GameObject;

typedef enum 
{
	NORMAL,
	VARIANCE,


}SHADOW_MODE;

// 影用定数バッファ構造体
struct SHADOWMAP_CBUFFER
{

	XMFLOAT4X4 SMWorldViewProj; // ワールド×ビュー×透視変換行列(シャドウマップ用)

	XMFLOAT4X4 SmView;
	XMFLOAT4X4 SmProj;


	int			Enable;
	BOOL		mode;
	int			Dummy[14];
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
	void SetShaderShadow(void);
	void SetShaderXpass(void);
	void SetShaderYpass(void);
	void SetWorldViewProjection2D(void);
	void SetShadowMode(SHADOW_MODE mode);
	void SetTarget(GameObject* gameObject);
	void SetForward(XMFLOAT3 dir);
	void SetLen(float len);

	void SetNear(float f);
	void SetFar(float f);

private:

	GameObject* ShadowTarget;


	Level* pLevel;
	XMFLOAT3 dir;
	float len;
	XMFLOAT3 pos;
	XMFLOAT3 at;
	XMFLOAT3 up;

	float quarity;
	float quarityblur;
	float hw;

	float vNear;
	float vFar;

	ID3D11VertexShader* m_VertexShaderShadow;
	ID3D11PixelShader* m_PixelShaderShadow;

	ID3D11VertexShader* m_VertexShaderShadow2D;
	ID3D11PixelShader* m_PixelShaderShadowX;
	ID3D11PixelShader* m_PixelShaderShadowY;


	SHADOWMAP_CBUFFER	ShadowMap;

	ID3D11Texture2D* ShadowMapDS;			// シャドウ・マップ
	ID3D11DepthStencilView* ShadowMapDSView;	// 深度/ステンシル・ビュー
	ID3D11ShaderResourceView* ShadowMapDSSRView;	// シェーダ・リソース・ビュー

	ID3D11Texture2D* ShadowMapingTexture;
	ID3D11RenderTargetView* RenderTargetShadow;
	ID3D11ShaderResourceView* ShadowMapSRView;	// シェーダ・リソース・ビュー

	D3D11_VIEWPORT            ViewPortShadowMap[2];       // ビューポート
	

	ID3D11Texture2D* ShadowMapingTextureX;
	ID3D11RenderTargetView* RenderTargetShadowX;
	ID3D11ShaderResourceView* ShadowMapSRViewX;	// シェーダ・リソース・ビュー

	ID3D11Texture2D* ShadowMapingTextureY;
	ID3D11RenderTargetView* RenderTargetShadowY;
	ID3D11ShaderResourceView* ShadowMapSRViewY;	// シェーダ・リソース・ビュー

	ID3D11Buffer* VertexBuffer;


};

