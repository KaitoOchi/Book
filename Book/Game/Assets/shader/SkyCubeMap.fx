//構造体
struct SVSIn
{
    float4 pos      : POSITION;
    float3 normal   : NORMAL;
};

struct SPSIn
{
    float4 pos      : SV_POSITION;
    float3 normal   : NORMAL;
};

//モデル用の定数バッファ
cbuffer ModelCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};


cbuffer SkyCubeCB : register(b1)
{
    float luminance;        //明るさ
};

//グローバル変数
Texture2D<float4>   g_albedo        : register(t0);
TextureCube<float4> g_skyCubeMap    : register(t10);
sampler             g_sampler       : register(s0);

/// <summary>
/// 頂点シェーダーのコア関数。
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
    SPSIn psIn;

    psIn.pos = mul(mWorld, vsIn.pos);
    psIn.pos = mul(mView, psIn.pos);
    psIn.pos = mul(mProj, psIn.pos);

    psIn.normal = vsIn.normal;

    return psIn;
}

float4 PSMain(SPSIn psIn) : SV_Target0
{
    float4 albedoColor;
    float3 normal = normalize(psIn.normal);
    albedoColor = g_skyCubeMap.Sample(g_sampler, normal * -1.0f) * luminance;

    return albedoColor;
}