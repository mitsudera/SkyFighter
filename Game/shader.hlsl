

//*****************************************************************************
// �萔�o�b�t�@
//*****************************************************************************

// �}�g���N�X�o�b�t�@
cbuffer WorldBuffer : register( b0 )
{
	matrix World;
}

cbuffer ViewBuffer : register( b1 )
{
	matrix View;
}

cbuffer ProjectionBuffer : register( b2 )
{
	matrix Projection;
}

// �}�e���A���o�b�t�@
struct MATERIAL
{
	float4		Ambient;
	float4		Diffuse;
	float4		Specular;
	float4		Emission;
	float		Shininess;
	int			noTexSampling;
	float		Dummy[2];//16byte���E�p
};

cbuffer MaterialBuffer : register( b3 )
{
	MATERIAL	Material;
}

// ���C�g�p�o�b�t�@
struct LIGHT
{
	float4		Direction[5];
	float4		Position[5];
	float4		Diffuse[5];
	float4		Ambient[5];
	float4		Attenuation[5];
	int4		Flags[5];
	int			Enable;
	int			Dummy[3];//16byte���E�p
};

cbuffer LightBuffer : register( b4 )
{
	LIGHT		Light;
}

struct FOG
{
	float4		Distance;
	float4		FogColor;
	int			Enable;
	float		Dummy[3];//16byte���E�p
};


// �t�H�O�p�o�b�t�@
cbuffer FogBuffer : register( b5 )
{
	FOG			Fog;
};


// �����p�o�b�t�@
cbuffer Fuchi : register(b6)
{
	int			fuchi;
	int			fill[3];
};


cbuffer CameraBuffer : register(b7)
{
	float4 Camera;

}


struct SHADOW
{
	matrix SMWorldViewProj; // ���[���h�~�r���[�~�����ϊ��s��(�V���h�E�}�b�v�p)

	int			Enable;
	int		Dummy[3];
};


// �e�p�o�b�t�@
cbuffer ShadowBuffer : register(b8)
{
	SHADOW	Shadow;
};

cbuffer SMViewBuffer : register(b9)
{
	matrix SMView;
}

cbuffer SMProjectionBuffer : register(b10)
{
	matrix SMProjection;
}



//��C�U���p�o�b�t�@
struct SCAT
{
	float3 Camera;				//�J�����̃|�W�V����
	float  CameraHeight;		//�J�����̍���
	float  CameraHeight2;		//�J�����̍�����2��
	float3 LightPos;			// The direction vector to the light source
	float3 InvWaveLength;		//RGB��1/pow(wavelngth,4)
	float  OuterRadius;			//��C�̔��a
	float  OuterRadius2;		//��C�̔��a^2
	float  InnerRadius;			//�f���̔��a
	float  InnerRadius2;		//�f���̔��a^2
	float  KrESun;				//Kr*Esun
	float  KmESun;				//Km*Esun
	float  Kr4PI;				//Kr*4*PI
	float  Km4PI;				//Km*4*PI
	float  Scale;				//1/(OuterRadius-InnerRadius)
	float  ScaleDepth;			//�X�P�[���̐[�� (�܂�A��C�̕��ϖ��x�������鍂�x)
	float  ScaleOverScaleDepth;	//Scale/ScaleDepth
	float  Dummy[2];
};

cbuffer Scat : register(b11)
{
	SCAT	Scat;
}

struct GAUSSIAN
{
	float4 weight1;
	float4 weight2;
	float2 texsize;
	float  Dummy[2];

};

cbuffer GaussianBuffer : register(b12)
{
	GAUSSIAN gaus;

}


const int nSamples = 2;
const float fSamples = 2.0;




//=============================================================================
// ���_�V�F�[�_
//=============================================================================
void VertexShaderPolygon( in  float4 inPosition		: POSITION0,
						  in  float4 inNormal		: NORMAL0,
						  in  float4 inDiffuse		: COLOR0,
						  in  float2 inTexCoord		: TEXCOORD0,

						  out float4 outPosition	: SV_POSITION,
						  out float4 outNormal		: NORMAL0,
						  out float2 outTexCoord	: TEXCOORD0,
						  out float4 outDiffuse		: COLOR0,
						  out float4 outWorldPos    : POSITION0,
						  out float4 outPosSM		: POSITION1
)
{


	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	outPosition = mul(inPosition, wvp);

	outNormal = normalize(mul(float4(inNormal.xyz, 0.0f), World));

	outTexCoord = inTexCoord;

	outWorldPos = mul(inPosition, World);

	outDiffuse = inDiffuse;



	//���_���W�@���f�����W�n���������W�n(�V���h�E�}�b�v)
	matrix SMWorldViewProj= mul(World, Shadow.SMWorldViewProj);
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

	float4 pos4 = mul(inPosition, SMWorldViewProj);
	pos4.xyz = pos4.xyz / pos4.w;
	outPosSM.x = (pos4.x + 1.0) / 2.0;
	outPosSM.y = (-pos4.y + 1.0) / 2.0;
	outPosSM.z = pos4.z-0.00002;
	


}

void VertexShaderSprite( in  float4 inPosition		: POSITION0,
						  in  float4 inNormal		: NORMAL0,
						  in  float4 inDiffuse		: COLOR0,
						  in  float2 inTexCoord		: TEXCOORD0,

						  out float4 outPosition	: SV_POSITION,
						  out float4 outNormal		: NORMAL0,
						  out float2 outTexCoord	: TEXCOORD0,
						  out float4 outDiffuse		: COLOR0,
						  out float4 outWorldPos    : POSITION0,
						  out float4 outPosSM		: POSITION1
)
{


	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	outPosition = mul(inPosition, wvp);

	outNormal = normalize(mul(float4(inNormal.xyz, 0.0f), World));

	outTexCoord = inTexCoord;

	outWorldPos = mul(inPosition, World);

	outDiffuse = inDiffuse;



	//���_���W�@���f�����W�n���������W�n(�V���h�E�}�b�v)
	matrix SMWorldViewProj= mul(World, Shadow.SMWorldViewProj);
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

	float4 pos4 = mul(inPosition, SMWorldViewProj);
	pos4.xyz = pos4.xyz / pos4.w;
	outPosSM.x = (pos4.x + 1.0) / 2.0;
	outPosSM.y = (-pos4.y + 1.0) / 2.0;
	outPosSM.z = pos4.z-0.00002;
	


}



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
Texture2D		g_Texture : register( t0 );
Texture2D		g_ShadowMap : register(t1);
Texture2D		g_ShadowMapBA : register(t2);
SamplerState	smpWarp : register( s0 );
SamplerState	smpBorder : register( s1 );




float GetVarianceDirectionalShadowFactor(float4 shadowCoord) {
	float w = 1.0f / shadowCoord.w; // ���_�V�F�[�_��GPU�͏����w��x, y, z������B���̍Č�
	float2 stex = float2((1.0f + shadowCoord.x * w) * 0.5f, (1.0f - shadowCoord.y * w) * 0.5f); // -1 ~ 1�� 0 ~ 1�Ƀ}�b�s���O

	float2 depth = g_ShadowMap.Sample(smpBorder, stex.xy).xy;

	float depth_sq = depth.x * depth.x; // E(x)^2
	float variance = depth.y - depth_sq; // ��^2 = E(x^2) - E(x^2)
	variance = min(1.0f, max(0.0f, variance + 0.0001f));

	float fragDepth = shadowCoord.z * w;
	float md = fragDepth - depth.x; // t - ��
	float p = variance / (variance + (md * md)); // ��^2 / (��^2 + (t - ��)^2)

	return max(p, fragDepth <= depth.x); // P(x >= t)�𖞂����Ƃ��̂�
}

//=============================================================================
// �s�N�Z���V�F�[�_
//=============================================================================
void PixelShaderPolygon( in  float4 inPosition		: SV_POSITION,
						 in  float4 inNormal		: NORMAL0,
						 in  float2 inTexCoord		: TEXCOORD0,
						 in  float4 inDiffuse		: COLOR0,
						 in  float4 inWorldPos		: POSITION0,
						 in	 float4 inPosSM			: POSITION1, // ���_���W(�V���h�E�}�b�v�̓������W�n)

						 out float4 outDiffuse		: SV_Target )
{
	float4 color;
	float sma = 1.0;

	if (Material.noTexSampling == 0)
	{
		color = g_Texture.Sample(smpWarp, inTexCoord);
	}
	else
	{
		color = inDiffuse;
	}

	if (Light.Enable == 0)
	{
		color = color * inDiffuse;
	}
	else
	{
		float4 tempColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 outColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
		for (int i = 0; i < 5; i++)
		{


			float3 lightDir;
			float light;

			if (Light.Flags[i].y == 1)//y��on.off
			{
				if (Light.Flags[i].x == 1)//x�̓^�C�v
				{
					lightDir = normalize(Light.Direction[i].xyz);
					float3 normal = inNormal.xyz;
					light = dot(lightDir, normal);//���邳�A���K�����ꂽ�x�N�g�����m�̓��ςȂ̂�-1.0�`+1.0


					//�e
					
					if (Shadow.Enable == 1)
					{

						// �V���h�E�}�b�v

						float sm = g_ShadowMap.Sample(smpBorder, inPosSM.xy).r;
						
						//sm = sm + g_ShadowMap.Sample(smpBorder, inPosSM.xy + float2(0.0f, 2.0f/ gaus.texsize.y)).x;

						//sm = sm / 2;



						float4 sa = g_ShadowMap.Sample(smpBorder, inPosSM.xy);
						//sm = sa.r;

						if (inPosSM.z > sm)
						{

							sma =  0.5;

							sma = sma+ (0.5-(sa.a*0.5));
						}
						

						

						if (sm == 1.0)
						{
							sma = 1.0;
						}

					}
					else
					{
						sma = 1.0;
					}

					if (light < -0.1)
					{

						light = (0.5 - 0.5 * light) * sma;
						tempColor = color * Material.Diffuse * light * Light.Diffuse[i];
					}
					else
					{


						light = (0.5 - 0.5 * light);

						tempColor = color * Material.Diffuse * light * Light.Diffuse[i];

					}

					//light = (0.5 - 0.5 * light) * sma;

					//tempColor = color * Material.Diffuse * light * Light.Diffuse[i];


				}
				else if (Light.Flags[i].x == 2)
				{
					lightDir = normalize(Light.Position[i].xyz - inWorldPos.xyz);
					light = dot(lightDir, inNormal.xyz);

					tempColor = color * Material.Diffuse * light * Light.Diffuse[i];

					float distance = length(inWorldPos - Light.Position[i]);

					float att = saturate((Light.Attenuation[i].x - distance) / Light.Attenuation[i].x);
					tempColor *= att;
				}
				else
				{
					tempColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
				}

				outColor += tempColor;
			}
		}






		color.rgb = outColor;
		color.a = inDiffuse.a * Material.Diffuse.a;
	}

	//�t�H�O
	if (Fog.Enable == 1)
	{
		float z = inPosition.z*inPosition.w;
		float f = (Fog.Distance.y - z) / (Fog.Distance.y - Fog.Distance.x);
		f = saturate(f);
		outDiffuse = f * color + (1 - f)*Fog.FogColor;
		outDiffuse.a = color.a;
	}
	else
	{
		outDiffuse = color;
	}



	//�����
	if (fuchi == 1)
	{
		float angle = dot(normalize(inWorldPos.xyz - Camera.xyz), normalize(inNormal));
		//if ((angle < 0.5f)&&(angle > -0.5f))
		if (angle > -0.3f)
		{
			outDiffuse.rb  = 1.0f;
			outDiffuse.g = 0.0f;			
		}
	}
}
//
//	// **************************************************
//// �V���h�E�}�b�v�̕`��
//// **************************************************
	void VS_SM(
		in  float4 inPosition	: POSITION0,
		in  float4 inNormal		: NORMAL0,
		in  float4 inDiffuse	: COLOR0,
		in  float2 inTexCoord	: TEXCOORD0,

		out float4 outPosition	: SV_POSITION,
		out float2 outTexCoord	: TEXCOORD0,
		out float4 outDiffuse	: COLOR0)
	{
		matrix wvp;
		wvp = mul(World, View);
		wvp = mul(wvp, Projection);
		outPosition = mul(inPosition, wvp);
		outTexCoord = inTexCoord;
		outDiffuse = inDiffuse;
	}


	//�V���h�E�}�b�v�p�s�N�Z���V�F�[�_�[
	void PS_SM(
		in  float4 inPosition	: SV_POSITION,
		in  float2 inTexCoord	: TEXCOORD0,
		in  float4 inDiffuse	: COLOR0,

		out float4 outDiffuse	: SV_Target
		)
	{
		float4 color;
		if (Material.noTexSampling == 0)//�e�N�X�`������
		{
			color = g_Texture.Sample(smpWarp, inTexCoord);
		}
		else
		{
			color = inDiffuse;
		}

		color.r = inPosition.z;
		color.g = color.r * color.r;
		outDiffuse = color;
	}



	//�p�[�e�B�N���p�s�N�Z���V�F�[�_�[
	void PARTICLE_PS(
	in  float4 inPosition		: SV_POSITION,
	in  float4 inNormal : NORMAL0,
	in  float2 inTexCoord : TEXCOORD0,
	in  float4 inDiffuse : COLOR0,
	in  float4 inWorldPos : POSITION0,
	in	float4 inPosSM : POSITION1,

	out float4 outDiffuse : SV_Target)
{
	float4 color;

		
		color = g_Texture.Sample(smpWarp, inTexCoord)* inDiffuse;


		outDiffuse = color;


}






	//��p���_�V�F�[�_�[
	void VertexShaderSky(
		in  float4 inPosition		: POSITION0,
		in  float4 inNormal			: NORMAL0,
		in  float4 inDiffuse		: COLOR0,
		in  float2 inTexCoord		: TEXCOORD0,

		out float4 outPosition		: SV_POSITION,
		out float4 outNormal		: NORMAL0,
		out float2 outTexCoord		: TEXCOORD0,
		out float4 outDiffuse		: COLOR0,
		out float4 outDiffuse1		: COLOR1,
		out float4 outWorldPos		: POSITION0)
	{
		//��C�U��
		// �J�����̒��_�Ƃ��̒������擾����
		//(��C��ʉ߂���ΐ��̉��_)
		float3 v3Pos = inPosition.xyz;
		float3 v3Ray = v3Pos - Scat.Camera;
		v3Pos = normalize(v3Pos);
		float fFar = length(v3Ray);
		v3Ray /= fFar;



		// �����̊J�n�ʒu���v�Z���Ă���A���̎U���I�t�Z�b�g���v�Z���܂��B
		float3 v3Start = Scat.Camera;
		float fDepth = exp((Scat.InnerRadius - Scat.CameraHeight) * Scat.ScaleDepth);
		float fCameraAngle = dot(-v3Ray, v3Pos);
		float fLightAngle = dot(Scat.LightPos, v3Pos);
		float x = 1.0 - fCameraAngle;
		float scale = Scat.ScaleDepth * exp(-0.00287 + x * (0.459 + x * (3.83 + x * (-6.80 + x * 5.25))));
		float fCameraScale = scale;
		x = 1.0 - fLightAngle;
		scale = Scat.ScaleDepth * exp(-0.00287 + x * (0.459 + x * (3.83 + x * (-6.80 + x * 5.25))));
		float fLightScale = scale;
		float fCameraOffset = fDepth * fCameraScale;
		float fTemp = (fLightScale + fCameraScale);

		// �U�����[�v�ϐ�������������
		float fSampleLength = fFar / fSamples;
		float fScaledLength = fSampleLength * Scat.Scale;
		float3 v3SampleRay = v3Ray * fSampleLength;
		float3 v3SamplePoint = v3Start + v3SampleRay * 0.5;

		// �����ŃT���v���_�����[�v����
		float3 v3FrontColor = float3(0.0, 0.0, 0.0);
		float3 v3Attenuate;

		for (int i = 0; i < nSamples; i++)
		{
			float fHeight = length(v3SamplePoint);
			float fDepth = exp(Scat.ScaleOverScaleDepth * (Scat.InnerRadius - fHeight));
			float fScatter = fDepth * fTemp - fCameraOffset;
			v3Attenuate = exp(-fScatter * (Scat.InvWaveLength * Scat.Kr4PI + Scat.Km4PI));
			v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
			v3SamplePoint += v3SampleRay;
		}



		matrix wvp;
		wvp = mul(World, View);
		wvp = mul(wvp, Projection);
		outPosition = mul(inPosition, wvp);

		outNormal = normalize(mul(float4(inNormal.xyz, 0.0f), World));

		outTexCoord = inTexCoord;

		outWorldPos = mul(inPosition, World);

		outDiffuse.rgb = v3FrontColor * (Scat.InvWaveLength * Scat.KrESun + Scat.KmESun)*1000;
		outDiffuse1.rgb = v3Attenuate*1000;




	}

	//��p�s�N�Z���V�F�[�_�[
	void PixelShaderSky(
		in  float4 inPosition	: SV_POSITION,
		in  float4 inNormal		: NORMAL0,
		in  float2 inTexCoord	: TEXCOORD0,
		in  float4 inDiffuse	: COLOR0,
		in  float4 inDiffuse1	: COLOR1,
		in  float4 inWorldPos	: POSITION0,
	
		out float4 outDiffuse : SV_Target)
	{
		float4 color;

		float4 yone = float4(0.0, 1.0, 0.0,1.0);
		float angle = dot(yone, normalize(inNormal));
		angle = angle * 0.5;

		//if ((angle < 0.5f)&&(angle > -0.5f))

	
		color = g_Texture.Sample(smpWarp, inTexCoord);
		color = color*angle;
		outDiffuse = inDiffuse+ color* inDiffuse1;
	}

//		//�R���s���[�g�V�F�[�_�[
//	// �p�[�e�B�N���\����
//	struct ParticleCompute
//	{
//		float3 pos;
//		float3 vel;
//		float life;
//	};
//
//	// CS�ݒ�
//	struct CSInput
//	{
//		uint3 groupThread : SV_GroupThreadID;
//		uint3 group : SV_GroupID;
//		uint  groupIndex : SV_GroupIndex;
//		uint3 dispatch : SV_DispatchThreadID;
//	};
//
//	// In
//	StructuredBuffer<ParticleCompute> particle : register(t0);
//	// Out
//	RWStructuredBuffer<ParticleCompute> BufOut : register(u0);
//
//#define size_x    256
//#define size_y      1
//#define size_z      1
//
//	[numthreads(size_x, size_y, size_z)]
//	void CSFunc(const CSInput input)
//	{
//		int index = input.dispatch.x;
//
//		float3 result = particle[index].pos + particle[index].vel;
//
//		BufOut[index].pos = result;
//		BufOut[index].life = particle[index].life - 1.0f;
//		BufOut[index].vel = particle[index].vel;
//
//	}
//

	//void CSPA(
	//	in  uint3 groupThread : SV_GroupThreadID,
	//	in  uint3 group : SV_GroupID,
	//	in  uint groupIndex : SV_GroupIndex,
	//	in  uint3 dispatch : SV_DispatchThreadID,

	//	out	: SV_Target)
	//{
	//	int index = dispatch.x;

	//	float3 result = particle[index].pos + particle[index].vel;

	//	BufOut[index].pos = result;
	//	BufOut[index].life = particle[index].life - 1.0f;
	//	BufOut[index].vel = particle[index].vel;

	//}
//=============================================================================
// �W�I���g���V�F�[�_(���C�e�B���O���邽�߂̖@���̌v�Z�p)
//=============================================================================

	struct GS_IN
	{
		float4  Position	: SV_POSITION;
		float4  Normal		: NORMAL0;
		float2  TexCoord	: TEXCOORD0;
		float4  Diffuse		: COLOR0;
		float4  WorldPos	: POSITION0;
		float4  PosSM		: POSITION1;
	};

	struct GS_OUT
	{
		float4 Position	: SV_POSITION;
		float4 Normal	: NORMAL0;
		float2 TexCoord : TEXCOORD0;
		float4 Diffuse	: COLOR0;
		float4 WorldPos	: POSITION0;
		float4 PosSM	: POSITION1;
	};
		[maxvertexcount(3)]						//���_��
	void GeometryShaderPolygon(
		triangle GS_IN In[3],                    // �g���C�A���O�� ���X�g���\�����钸�_�z��̓��͏��
		inout TriangleStream<GS_OUT> TriStream   // ���_����ǉ����邽�߂̃X�g���[���I�u�W�F�N�g
	)
	{
		GS_OUT Out;

		int i;

		// ���Ƃ̒��_���o��
		for (i = 0; i < 3; i++)
		{
			Out.Position = In[i].Position;

			float3 vec1 = In[1].WorldPos.xyz - In[0].WorldPos.xyz;
			float3 vec2 = In[2].WorldPos.xyz - In[0].WorldPos.xyz;
			float4 nor;
			nor.xyz = normalize(cross(vec1, vec2));
			nor.w = 0.0f;
			Out.Normal = nor;
			Out.TexCoord = In[i].TexCoord;
			Out.Diffuse = In[i].Diffuse; 
			Out.WorldPos = In[i].WorldPos;
			Out.PosSM = In[i].PosSM;
			// ���_��ǉ�����
			TriStream.Append(Out);
		}
		//// ���݂̃X�g���b�v���I�����A�V�����X�g���b�v���J�n����B
		//TriStream.RestartStrip();
	}




	struct pixcelIn
	{
		float4 pos : SV_POSITION;
		float2 tex : TEXCOORD;
	};



	void xpass(
		in  float4 inPosition		: SV_POSITION,
		in  float4 inNormal : NORMAL0,
		in  float2 inTexCoord : TEXCOORD0,
		in  float4 inDiffuse : COLOR0,
		in  float4 inWorldPos : POSITION0,
		in	float4 inPosSM : POSITION1,

		out float4 outDiffuse : SV_Target)
	{

		float4 color;


		color = g_Texture.Sample(smpWarp, inTexCoord);


		outDiffuse = color;


	}


	////xpass
	//float4 xpass(pixcelIn In) : SV_Target
	//{
	//	//float MAP_WIDTH = gaus.texsize.x;


	//	//float3 col = gaus.weight1.x * g_Texture.Sample(smpWarp, float2(In.tex) + float2(+1.0f / MAP_WIDTH, 0));
	//	//col += gaus.weight1.y * (g_Texture.Sample(smpWarp, In.tex + float2(+3.0f / MAP_WIDTH, 0)) +g_Texture.Sample(smpWarp, In.tex + float2(-3.0f / MAP_WIDTH, 0)));
	//	//col += gaus.weight1.z * (g_Texture.Sample(smpWarp, In.tex + float2(+5.0f / MAP_WIDTH, 0)) +g_Texture.Sample(smpWarp, In.tex + float2(-5.0f / MAP_WIDTH, 0)));
	//	//col += gaus.weight1.w * (g_Texture.Sample(smpWarp, In.tex + float2(+7.0f / MAP_WIDTH, 0)) +g_Texture.Sample(smpWarp, In.tex + float2(-7.0f / MAP_WIDTH, 0)));
	//	//col += gaus.weight2.x * (g_Texture.Sample(smpWarp, In.tex + float2(+9.0f / MAP_WIDTH, 0)) +g_Texture.Sample(smpWarp, In.tex + float2(-9.0f / MAP_WIDTH, 0)));
	//	//col += gaus.weight2.y * (g_Texture.Sample(smpWarp, In.tex + float2(+11.0f / MAP_WIDTH, 0)) + g_Texture.Sample(smpWarp, In.tex + float2(-11.0f / MAP_WIDTH, 0)));
	//	//col += gaus.weight2.z * (g_Texture.Sample(smpWarp, In.tex + float2(+13.0f / MAP_WIDTH, 0)) + g_Texture.Sample(smpWarp, In.tex + float2(-13.0f / MAP_WIDTH, 0)));
	//	//col += gaus.weight2.w * (g_Texture.Sample(smpWarp, In.tex + float2(+15.0f / MAP_WIDTH, 0)) + g_Texture.Sample(smpWarp, In.tex + float2(-15.0f / MAP_WIDTH, 0)));

	//	float3 col = g_Texture.Sample(smpWarp, float2(In.tex)) ;

	//	return float4(col, 1.0f);
	//}
	// 
	// 
	// 
		
	
	

	void ypass(
		in  float4 inPosition		: SV_POSITION,
		in  float4 inNormal : NORMAL0,
		in  float2 inTexCoord : TEXCOORD0,
		in  float4 inDiffuse : COLOR0,
		in  float4 inWorldPos : POSITION0,
		in	float4 inPosSM : POSITION1,

		out float4 outDiffuse : SV_Target)
	{

		float4 color;


		color = g_Texture.Sample(smpWarp, inTexCoord);


		outDiffuse = color;


	}


	
	//ypass
		float4 ypass(pixcelIn In) : SV_Target
	{
		float MAP_HEIGHT = gaus.texsize.y;

		//float3 col = gaus.weight1.x * g_Texture.Sample(smpWarp, float2(In.tex));
		//col += gaus.weight1.y * (g_Texture.Sample(smpWarp, In.tex + float2(0, +2.0f / MAP_HEIGHT)) + g_Texture.Sample(smpWarp, In.tex + float2(0, -2.0f / MAP_HEIGHT)));
		//col += gaus.weight1.z * (g_Texture.Sample(smpWarp, In.tex + float2(0, +4.0f / MAP_HEIGHT)) + g_Texture.Sample(smpWarp, In.tex + float2(0, -4.0f / MAP_HEIGHT)));
		//col += gaus.weight1.w * (g_Texture.Sample(smpWarp, In.tex + float2(0, +6.0f / MAP_HEIGHT)) + g_Texture.Sample(smpWarp, In.tex + float2(0, -6.0f / MAP_HEIGHT)));
		//col += gaus.weight2.x * (g_Texture.Sample(smpWarp, In.tex + float2(0, +8.0f / MAP_HEIGHT)) + g_Texture.Sample(smpWarp, In.tex + float2(0, -8.0f / MAP_HEIGHT)));
		//col += gaus.weight2.y * (g_Texture.Sample(smpWarp, In.tex + float2(0, +10.0f / MAP_HEIGHT)) + g_Texture.Sample(smpWarp, In.tex + float2(0, -10.0f / MAP_HEIGHT)));
		//col += gaus.weight2.z * (g_Texture.Sample(smpWarp, In.tex + float2(0, +12.0f / MAP_HEIGHT)) + g_Texture.Sample(smpWarp, In.tex + float2(0, -12.0f / MAP_HEIGHT)));
		//col += gaus.weight2.w * (g_Texture.Sample(smpWarp, In.tex + float2(0, +14.0f / MAP_HEIGHT)) + g_Texture.Sample(smpWarp, In.tex + float2(0, -14.0f / MAP_HEIGHT)));
		

		float3 col = g_Texture.Sample(smpWarp, float2(In.tex));


		return float4(col, 1.0f);
	}


