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

static const float PI = 3.14159f;           //円周率
static const float2 CENTER = (0.5f, 0.5f);  //円の中央

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

/// <summary>
/// ピクセルシェーダーのコア関数。
/// </summary>
float4 PSMainCore(PSInput In)
{
    float4 albedo = albedoTexture.Sample(Sampler, In.uv) * mulColor;

    return albedo;
}

/// <summary>
/// 通常のエントリー関数。
/// </summary>
float4 PSMain(PSInput In) : SV_Target0
{
    return PSMainCore(In);
}

/// <summary>
/// クリップ用のエントリー関数。
/// </summary>
float4 PSClip(PSInput In) : SV_Target0
{
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

    return PSMainCore(In);
}

/// <summary>
/// 円形ゲージ用のエントリー関数。
/// </summary>
float4 PSCircleGauge(PSInput In) : SV_Target0
{
    //中心から上方向のベクトル
    //中心からピクセルの座標へのベクトル
    float2 Vector1 = { 0.0f, -1.0f };
    float2 Vector2 = In.uv - CENTER;
    Vector1 = normalize(Vector1);
    Vector2 = normalize(Vector2);

    //内積を求め、逆コサインを取る
    float Deg = acos(dot(Vector1, Vector2));
    Deg = abs(Deg);

    if (Vector2.x > Vector1.x){
        Deg = PI + (PI - Deg);
    }

    //小さければピクセルを破棄
    if (Deg >= clipSize.y){
        clip(-1);
    }

    return PSMainCore(In);
}

/// <summary>
/// UVスクロール用のエントリー関数。
/// </summary>
float4 PSMainUVScroll(PSInput In) : SV_Target0
{
    float2 offset = float2(clipSize.z, 0);

    if(In.uv.x > 1.0f - offset.x){
        offset.x -= 1.0f;
    }

    float4 albedo = albedoTexture.Sample(Sampler, In.uv + offset) * mulColor;

    return albedo;
}