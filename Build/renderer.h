//=============================================================================
//
// �����_�����O���� [renderer.h]
// Author : 
//
//=============================================================================
#pragma once

//*********************************************************
// �}�N����`
//*********************************************************
#define LIGHT_MAX		(5)

enum LIGHT_TYPE
{
	LIGHT_TYPE_NONE,		//���C�g����
	LIGHT_TYPE_DIRECTIONAL,	//�f�B���N�V���i�����C�g
	LIGHT_TYPE_POINT,		//�|�C���g���C�g

	LIGHT_TYPE_NUM
};

enum BLEND_MODE
{
	BLEND_MODE_NONE,		//�u�����h����
	BLEND_MODE_ALPHABLEND,	//���u�����h
	BLEND_MODE_ADD,			//���Z�u�����h
	BLEND_MODE_SUBTRACT,	//���Z�u�����h

	BLEDD_MODE_NUM
};

enum CULL_MODE
{
	CULL_MODE_NONE,			//�J�����O����
	CULL_MODE_FRONT,		//�\�̃|���S����`�悵�Ȃ�(CW)
	CULL_MODE_BACK,			//���̃|���S����`�悵�Ȃ�(CCW)

	CULL_MODE_NUM
};


//*********************************************************
// �\����
//*********************************************************

// ���_�\����
struct VERTEX_3D
{
    XMFLOAT3	Position;
    XMFLOAT3	Normal;
    XMFLOAT4	Diffuse;
    XMFLOAT2	TexCoord;
};

// �}�e���A���\����
struct MATERIAL
{
	XMFLOAT4	Ambient;
	XMFLOAT4	Diffuse;
	XMFLOAT4	Specular;
	XMFLOAT4	Emission;
	float		Shininess;
	int			noTexSampling;
};

// ���C�g�\����
struct LIGHT {
	XMFLOAT3	Direction;	// ���C�g�̕���
	XMFLOAT3	Position;	// ���C�g�̈ʒu
	XMFLOAT4	Diffuse;	// �g�U���̐F
	XMFLOAT4	Ambient;	// �����̐F
	float		Attenuation;// ������
	int			Type;		// ���C�g��ʁE�L���t���O
	int			Enable;		// ���C�g��ʁE�L���t���O
};

// �t�H�O�\����
struct FOG {
	float		FogStart;	// �t�H�O�̊J�n����
	float		FogEnd;		// �t�H�O�̍ő勗��
	XMFLOAT4	FogColor;	// �t�H�O�̐F
};

struct SCAT
{

	XMFLOAT3 Camera;			//�J�����̃|�W�V����
	float  CameraHeight;		//�J�����̍���
	float  CameraHeight2;		//�J�����̍�����2��
	XMFLOAT3 InvWaveLength;		//RGB��1/pow(wavelngth,4)
	float OuterRadius;			//��C�̔��a
	float OuterRadius2;			//��C�̔��a^2
	float InnerRadius;			//�f���̔��a
	float InnerRadius2;			//�f���̔��a^2
	float KrESun;				//Kr*Esun
	float KmESun;				//Km*Esun
	float Kr4PI;				//Kr*4*PI
	float Km4PI;				//Km*4*PI
	float Scale;				//1/(OuterRadius-InnerRadius)
	float ScaleOverScaleDepth;	//Scale/ScaleDepth



};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitRenderer(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void UninitRenderer(void);

void Clear(void);
void Present(void);

ID3D11Device *GetDevice( void );
ID3D11DeviceContext *GetDeviceContext( void );

void SetDepthEnable( bool Enable );
void SetBlendState(BLEND_MODE bm);
void SetCullingMode(CULL_MODE cm);
void SetAlphaTestEnable(BOOL flag);

void SetWorldViewProjection2D( void );
void SetWorldMatrix( XMMATRIX *WorldMatrix );
void SetViewMatrix( XMMATRIX *ViewMatrix );
void SetProjectionMatrix( XMMATRIX *ProjectionMatrix );

void SetMaterial( MATERIAL material );

void SetLightEnable(BOOL flag);
void SetLight(int index, LIGHT* light);

void SetFogEnable(BOOL flag);
void SetFog(FOG* fog);
//
//void SetShadowEnable(BOOL flag);
//void SetShadow(SHADOW* shadow);

void DebugTextOut(char* text, int x, int y);

void SetFuchi(int flag);
void SetShaderCamera(XMFLOAT3 pos);
//void SetShaderLightPos(XMFLOAT3 pos);

void SetClearColor(float* color4);

HRESULT InitShadowMap(void);
HRESULT CreateShaderShadow(void);
void SetShadowEnable(BOOL flag);
void SetShadowBuffer(void);
void ICClear(void);

void DrawShadowMap(void);
void DrawShadowMap2(void);

void SetViewMatrix(XMMATRIX *ViewMatrix);
void SetProjectionMatrix(XMMATRIX *ProjectionMatrix);
void SetScat(void);
void SetSkyShader(void);
void SetPolygonShader(void);
void SetParticleShader(void);
void SetFieldShader(void);


void DrawShadowTex(void);
void SetSrvYpass(void);
