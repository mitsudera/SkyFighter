

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
	int			noDiffuseTex;
	int			noNormalTex;
	int			phong;
};

cbuffer MaterialBuffer : register( b3 )
{
	MATERIAL	Material;
}

// ライト用バッファ
struct LIGHT
{
	float4		Direction[8];
	float4		Position[8];
	float4		Diffuse[8];
	float4		Ambient[8];
	float4		Attenuation[8];
	int4		Flags[8];
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
    matrix wvp;
    matrix view;
    matrix proj;
    int enable;
    int dummy[3];
};

cbuffer ShadowBuffer : register(b8)
{
    SHADOW Shadow;
}

struct GAUSSIAN
{
    float4 weight1;
    float4 weight2;

};

cbuffer GaussianBuffer : register(b9)
{
    GAUSSIAN gaus;

}




//=============================================================================
// 頂点シェーダ
//=============================================================================
void VSmain( in  float4 inPosition		: POSITION0,
						  in  float4 inNormal		: NORMAL0,
						  in  float4 inDiffuse		: COLOR0,
						  in  float2 inTexCoord		: TEXCOORD0,

						  out float4 outPosition	: SV_POSITION,
						  out float4 outNormal		: NORMAL0,
						  out float2 outTexCoord	: TEXCOORD0,
						  out float4 outDiffuse		: COLOR0,
						  out float4 outWorldPos    : POSITION0,
                          out float4 outPosSM       : POSITION1)
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
    matrix SMWorldViewProj = mul(World, Shadow.wvp);
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);

    float4 pos4 = mul(inPosition, SMWorldViewProj);
    pos4.xyz = pos4.xyz / pos4.w;
    outPosSM.x = (pos4.x + 1.0) / 2.0;
    outPosSM.y = (-pos4.y + 1.0) / 2.0;
    outPosSM.z = pos4.z - 0.0002;

}



//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture2D Texture : register(t0);
Texture2D ShadowMap : register(t1);
Texture2D ShadowMapTex : register(t2);

SamplerState smpWrap : register(s0);
SamplerState smpBorder : register(s1);


//=============================================================================
// ピクセルシェーダ
//=============================================================================
void PSmain(in float4 inPosition : SV_POSITION,
						 in  float4 inNormal		: NORMAL0,
						 in  float2 inTexCoord		: TEXCOORD0,
						 in  float4 inDiffuse		: COLOR0,
						 in  float4 inWorldPos      : POSITION0,
						 in  float4 inPosSM          : POSITION1, // 頂点座標(シャドウマップの透視座標系)


						 out float4 outDiffuse		: SV_Target )
{
    float4 color;
    float sma = 1.0;
    bool shadow;
    					//影
    if (Shadow.enable == 1)
    {

				//		// シャドウマップ

        //float sm = ShadowMap.Sample(smpBorder, inPosSM.xy).r;
						



        float2 sm = ShadowMapTex.Sample(smpBorder, inPosSM.xy);

        if (inPosSM.z> sm.r)
        {

            sma = 0.5;

            sma = sma + (0.5 - (sm.g * 0.5));
        }
						

						

        //if (sm == 1.0)
        //{
        //    sma = 1.0;
        //}
        
        
 

    }
    else
    {
        sma = 1.0;
    }
 

    if (Material.noDiffuseTex == 0)
    {
        color = Texture.Sample(smpWrap, inTexCoord);

        color *= inDiffuse;
    }
    else
    {
        
        color = inDiffuse;
    }

    if (Light.Enable == 0)
    {
        color = color * Material.Diffuse;
    }
    else
    {
        if (Material.phong == false)
        {
            float4 tempColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
            float4 outColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

            for (int i = 0; i < 8; i++)
            {
                float3 lightDir;
                float light;

                if (Light.Flags[i].y == 1)
                {
                    if (Light.Flags[i].x == 1)
                    {
                        lightDir = normalize(Light.Direction[i].xyz);
                        light = dot(lightDir, inNormal.xyz);
                        

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

            color = outColor;
            color.a = inDiffuse.a * Material.Diffuse.a;
        }
        else
        {
            float4 tempColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
            float4 outColor = float4(0.0f, 0.0f, 0.0f, 0.0f);


 
            for (int i = 0; i < 8; i++)
            {
                float3 lightDir;
                float light;
                float3 iA;
                float3 iD;
                float3 iS;
                
                if (Light.Flags[i].y == 1)
                {
                    if (Light.Flags[i].x == 1)
                    {
                        lightDir = normalize(Light.Direction[i].xyz);
                        light = dot(lightDir, normalize(inNormal.xyz));
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

                        float3 r = 2.0 * inNormal.xyz * light - lightDir;

                        float3 v = normalize(Camera.xyz - inWorldPos.xyz);
                        
                        iA = Material.Ambient.xyz * Material.Ambient.w * Light.Ambient[i].xyz;
                        iD = color * Material.Diffuse * light * Light.Diffuse[i];

                        iS = pow(saturate(dot(r, v)), Material.Shininess) * Material.Specular.xyz;
                        
                        tempColor = float4(saturate((iA + iD + iS)), 1.0f);

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

            color = outColor;
            color.a = inDiffuse.a * Material.Diffuse.a;

        }
			

    }

    outDiffuse = color;
}

void VS_SM(
		in float4 inPosition : POSITION0,
		in float4 inNormal : NORMAL0,
		in float4 inDiffuse : COLOR0,
		in float2 inTexCoord : TEXCOORD0,

		out float4 outPosition : SV_POSITION,
		out float2 outTexCoord : TEXCOORD0,
		out float4 outDiffuse : COLOR0,
		out float4 outWorldPos : POSITION0
)
{
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    outPosition = mul(inPosition, wvp);


    outTexCoord = inTexCoord;

    outWorldPos = mul(inPosition, World);

    outDiffuse = inDiffuse;
}


//シャドウマップ用ピクセルシェーダー
void PS_SM(
		in float4 inPosition : SV_POSITION,
		in float2 inTexCoord : TEXCOORD0,
		in float4 inDiffuse : COLOR0,
		in float4 inWorldPos : POSITION0,


		out float4 outDiffuse : SV_Target
		)
{
    float4 color;
    if (Material.noDiffuseTex == 0)
    {
        color = Texture.Sample(smpWrap, inTexCoord);

        color *= inDiffuse;
    }
    else
    {
        
        color = inDiffuse;
    }
    
    color = color * Material.Diffuse;

    color.r = inPosition.z;
    color.g = color.a;
    
    outDiffuse = color;
}

void VS_2D(
		in float4 inPosition : POSITION0,
		in float4 inNormal : NORMAL0,
		in float4 inDiffuse : COLOR0,
		in float2 inTexCoord : TEXCOORD0,

		out float4 outPosition : SV_POSITION,
		out float2 outTexCoord : TEXCOORD0,
		out float4 outDiffuse : COLOR0,
		out float4 outWorldPos : POSITION0
)
{
    outPosition = inPosition;


    outTexCoord = inTexCoord;


    outDiffuse = inDiffuse;
}


void xpass(
		in float4 inPosition : SV_POSITION,
		in float4 inNormal : NORMAL0,
		in float2 inTexCoord : TEXCOORD0,
		in float4 inDiffuse : COLOR0,
		in float4 inWorldPos : POSITION0,

		out float4 outDiffuse : SV_Target)
{
    float x;
    float y;
    Texture.GetDimensions(x, y);

    float3 col = gaus.weight1.x * Texture.Sample(smpWrap, float2(inTexCoord) + float2(+1.0f / x, 0));
    col += gaus.weight1.y * (Texture.Sample(smpWrap, inTexCoord + float2(+3.0f / x, 0)) + Texture.Sample(smpWrap, inTexCoord + float2(-3.0f / x, 0)));
    col += gaus.weight1.z * (Texture.Sample(smpWrap, inTexCoord + float2(+5.0f / x, 0)) + Texture.Sample(smpWrap, inTexCoord + float2(-5.0f / x, 0)));
    col += gaus.weight1.w * (Texture.Sample(smpWrap, inTexCoord + float2(+7.0f / x, 0)) + Texture.Sample(smpWrap, inTexCoord + float2(-7.0f / x, 0)));
    col += gaus.weight2.x * (Texture.Sample(smpWrap, inTexCoord + float2(+9.0f / x, 0)) + Texture.Sample(smpWrap, inTexCoord + float2(-9.0f / x, 0)));
    col += gaus.weight2.y * (Texture.Sample(smpWrap, inTexCoord + float2(+11.0f / x, 0)) + Texture.Sample(smpWrap, inTexCoord + float2(-11.0f / x, 0)));
    col += gaus.weight2.z * (Texture.Sample(smpWrap, inTexCoord + float2(+13.0f / x, 0)) + Texture.Sample(smpWrap, inTexCoord + float2(-13.0f / x, 0)));
    col += gaus.weight2.w * (Texture.Sample(smpWrap, inTexCoord + float2(+15.0f / x, 0)) + Texture.Sample(smpWrap, inTexCoord + float2(-15.0f / x, 0)));



    

    outDiffuse = (col, 1.0f);


}