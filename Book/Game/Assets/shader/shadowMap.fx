// モデル用の定数バッファー
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
}

//シャドウ用の定数バッファ
cbuffer ShadowCB : register(b1)
{
	float3 lightPos;
    float4x4 mLVP;
    int playerAnim2D;
}

struct SSkinVSIn{
	int4  Indices  	: BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};

// 頂点シェーダーへの入力
struct SVSIn
{
    float4 pos : POSITION;  // モデルの頂点座標
    float3 normal : NORMAL; // 法線
    float2 uv : TEXCOORD0;  // UV座標
    SSkinVSIn skinVert;				//スキン用のデータ。
};

// ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos : SV_POSITION;   // スクリーン空間でのピクセルの座標
    float3 normal : NORMAL;     // 法線
    float2 uv : TEXCOORD0;      // uv座標
    float2 depth : TEXCOORD1;   // ライト空間での座標
};

///////////////////////////////////////////////////
// グローバル変数
///////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);      // アルベドマップ
Texture2D<float4> g_shadowMap : register(t10);  // シャドウマップ

StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。
sampler g_sampler : register(s0); 

/// <summary>
//スキン行列を計算する。
/// </summary>
float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
	float4x4 skinning = 0;
	float w = 0.0f;
	[unroll]
	for (int i = 0; i < 3; i++)
	{
		skinning += g_boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
		w += skinVert.Weights[i];
	}

	skinning += g_boneMatrix[skinVert.Indices[3]] * (1.0f - w);

	return skinning;
}

/// <summary>
/// 頂点シェーダー
/// <summary>
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
	SPSIn psIn;
	float4x4 m;
	if( hasSkin ){
		m = CalcSkinMatrix(vsIn.skinVert);
	}else{
		m = mWorld;
	}

    psIn.pos = mul(m, vsIn.pos);
    float3 worldPos = psIn.pos;
    psIn.pos = mul(mView, psIn.pos);
    psIn.pos = mul(mProj, psIn.pos);

    psIn.normal = mul(m, vsIn.normal);

    //頂点のライトから見た深度値と、ライトから見た深度値の2乗を計算する
    psIn.depth.x = length(worldPos - lightPos) / 1000.0f;
    psIn.depth.y = psIn.depth.x * psIn.depth.x;

    psIn.uv = vsIn.uv;

    return psIn;
}

SPSIn VSMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, false);
}

SPSIn VSSkinMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, true);
}

/// <summary>
/// シャドウマップ描画用のピクセルシェーダー
/// </summary>
float4 PSMainCore(SPSIn psIn)
{
	float2 uv;
	int x, y;
	x = playerAnim2D % 4;
	y = playerAnim2D / 4;
	uv.x = (psIn.uv.x / 4) + (x * 0.25);
	uv.y = (psIn.uv.y / 4) + (y * 0.25);
	float4 albedo = g_albedo.Sample(g_sampler, uv);

    if(albedo.w < 0.1f){
        clip(-1);
    }

    //ライトから見た深度値と、ライトから見た深度値の2乗を出力する
    return float4(psIn.depth.x, psIn.depth.y, 0.0f, 1.0f);
}

float4 PSMain(SPSIn psIn) : SV_Target0
{
    return PSMainCore(psIn);
}

float4 PSPlayer2D(SPSIn psIn) : SV_Target0
{
        return PSMainCore(psIn);
}