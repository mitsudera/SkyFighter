

//*****************************************************************************
// 定数バッファ
//*****************************************************************************

// マトリクスバッファ
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

// マテリアルバッファ
struct MATERIAL
{
	float4		Ambient;
	float4		Diffuse;
	float4		Specular;
	float4		Emission;
	float		Shininess;
	int			noTexSampling;
	float		Dummy[2];//16byte境界用
};

cbuffer MaterialBuffer : register( b3 )
{
	MATERIAL	Material;
}

// ライト用バッファ
struct LIGHT
{
	float4		Direction[5];
	float4		Position[5];
	float4		Diffuse[5];
	float4		Ambient[5];
	float4		Attenuation[5];
	int4		Flags[5];
	int			Enable;
	int			Dummy[3];//16byte境界用
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
	float		Dummy[3];//16byte境界用
};


// フォグ用バッファ
cbuffer FogBuffer : register( b5 )
{
	FOG			Fog;
};


// 縁取り用バッファ
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
	matrix SMWorldViewProj; // ワールド×ビュー×透視変換行列(シャドウマップ用)

	int			Enable;
	int		Dummy[3];
};


// 影用バッファ
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



//大気散乱用バッファ
struct SCAT
{
	float3 Camera;				//カメラのポジション
	float  CameraHeight;		//カメラの高さ
	float  CameraHeight2;		//カメラの高さの2乗
	float3 LightPos;			// The direction vector to the light source
	float3 InvWaveLength;		//RGBの1/pow(wavelngth,4)
	float  OuterRadius;			//大気の半径
	float  OuterRadius2;		//大気の半径^2
	float  InnerRadius;			//惑星の半径
	float  InnerRadius2;		//惑星の半径^2
	float  KrESun;				//Kr*Esun
	float  KmESun;				//Km*Esun
	float  Kr4PI;				//Kr*4*PI
	float  Km4PI;				//Km*4*PI
	float  Scale;				//1/(OuterRadius-InnerRadius)
	float  ScaleDepth;			//スケールの深さ (つまり、大気の平均密度が見つかる高度)
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
// 頂点シェーダ
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



	//頂点座標　モデル座標系→透視座標系(シャドウマップ)
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



	//頂点座標　モデル座標系→透視座標系(シャドウマップ)
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
// グローバル変数
//*****************************************************************************
Texture2D		g_Texture : register( t0 );
Texture2D		g_ShadowMap : register(t1);
Texture2D		g_ShadowMapBA : register(t2);
SamplerState	smpWarp : register( s0 );
SamplerState	smpBorder : register( s1 );




float GetVarianceDirectionalShadowFactor(float4 shadowCoord) {
	float w = 1.0f / shadowCoord.w; // 頂点シェーダでGPUは勝手にwでx, y, zを割る。その再現
	float2 stex = float2((1.0f + shadowCoord.x * w) * 0.5f, (1.0f - shadowCoord.y * w) * 0.5f); // -1 ~ 1を 0 ~ 1にマッピング

	float2 depth = g_ShadowMap.Sample(smpBorder, stex.xy).xy;

	float depth_sq = depth.x * depth.x; // E(x)^2
	float variance = depth.y - depth_sq; // σ^2 = E(x^2) - E(x^2)
	variance = min(1.0f, max(0.0f, variance + 0.0001f));

	float fragDepth = shadowCoord.z * w;
	float md = fragDepth - depth.x; // t - μ
	float p = variance / (variance + (md * md)); // σ^2 / (σ^2 + (t - μ)^2)

	return max(p, fragDepth <= depth.x); // P(x >= t)を満たすときのみ
}

//=============================================================================
// ピクセルシェーダ
//=============================================================================
void PixelShaderPolygon( in  float4 inPosition		: SV_POSITION,
						 in  float4 inNormal		: NORMAL0,
						 in  float2 inTexCoord		: TEXCOORD0,
						 in  float4 inDiffuse		: COLOR0,
						 in  float4 inWorldPos		: POSITION0,
						 in	 float4 inPosSM			: POSITION1, // 頂点座標(シャドウマップの透視座標系)

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

			if (Light.Flags[i].y == 1)//yはon.off
			{
				if (Light.Flags[i].x == 1)//xはタイプ
				{
					lightDir = normalize(Light.Direction[i].xyz);
					float3 normal = inNormal.xyz;
					light = dot(lightDir, normal);//明るさ、正規化されたベクトル同士の内積なので-1.0～+1.0


					//影
					
					if (Shadow.Enable == 1)
					{

						// シャドウマップ

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

	//フォグ
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



	//縁取り
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
//// シャドウマップの描画
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


	//シャドウマップ用ピクセルシェーダー
	void PS_SM(
		in  float4 inPosition	: SV_POSITION,
		in  float2 inTexCoord	: TEXCOORD0,
		in  float4 inDiffuse	: COLOR0,

		out float4 outDiffuse	: SV_Target
		)
	{
		float4 color;
		if (Material.noTexSampling == 0)//テクスチャあり
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



	//パーティクル用ピクセルシェーダー
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






	//空用頂点シェーダー
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
		//大気散乱
		// カメラの頂点とその長さを取得する
		//(大気を通過する斜線の遠点)
		float3 v3Pos = inPosition.xyz;
		float3 v3Ray = v3Pos - Scat.Camera;
		v3Pos = normalize(v3Pos);
		float fFar = length(v3Ray);
		v3Ray /= fFar;



		// 光線の開始位置を計算してから、その散乱オフセットを計算します。
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

		// 散乱ループ変数を初期化する
		float fSampleLength = fFar / fSamples;
		float fScaledLength = fSampleLength * Scat.Scale;
		float3 v3SampleRay = v3Ray * fSampleLength;
		float3 v3SamplePoint = v3Start + v3SampleRay * 0.5;

		// ここでサンプル点をループする
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

	//空用ピクセルシェーダー
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

//		//コンピュートシェーダー
//	// パーティクル構造体
//	struct ParticleCompute
//	{
//		float3 pos;
//		float3 vel;
//		float life;
//	};
//
//	// CS設定
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
// ジオメトリシェーダ(ライティングするための法線の計算用)
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
		[maxvertexcount(3)]						//頂点数
	void GeometryShaderPolygon(
		triangle GS_IN In[3],                    // トライアングル リストを構成する頂点配列の入力情報
		inout TriangleStream<GS_OUT> TriStream   // 頂点情報を追加するためのストリームオブジェクト
	)
	{
		GS_OUT Out;

		int i;

		// もとの頂点を出力
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
			// 頂点を追加する
			TriStream.Append(Out);
		}
		//// 現在のストリップを終了し、新しいストリップを開始する。
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


