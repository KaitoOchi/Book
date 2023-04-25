cbuffer cb : register(b0)
{
    float4x4 mvp;
    float4 mulColor;
};

cbuffer SpriteCB : register(b1)
{
    float3 clipSize;
    int clipMode;
};

struct VSInput
{
    float4 pos : POSITION;
    float2 uv  : TEXCOORD0;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD0;
    float clipmode : FOG;
};

Texture2D<float4> albedoTexture : register(t0); // アルベド
sampler Sampler : register(s0);

PSInput VSMain(VSInput In, uniform float clipM)
{
    PSInput psIn;
    psIn.pos = mul(mvp, In.pos);
    psIn.uv = In.uv;

    psIn.clipmode = clipM;
    return psIn;
}

PSInput VSMain1(VSInput vsIn)
{
	return VSMain(vsIn, 1.0f);
}

PSInput VSMain2(VSInput vsIn)
{
	return VSMain(vsIn, 2.0f);
}

PSInput VSMain3(VSInput vsIn)
{
	return VSMain(vsIn, 3.0f);
}

float4 PSMain(PSInput In) : SV_Target0
{
    float4 albedo = albedoTexture.Sample(Sampler, In.uv) * mulColor;

    //ゲームシーンなら
    if(In.clipmode == 1.0f){
        clip(In.pos.y - clipSize.x);
    }
    //タイトルシーンなら
    else if(In.clipmode == 2.0f){
        clip(clipSize.x - In.pos.x);
    }
    else if(In.clipmode == 3.0f){
        clip(clipSize.y - In.pos.x);
    }
    return albedo;
}
