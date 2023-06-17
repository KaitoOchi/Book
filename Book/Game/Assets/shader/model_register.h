
////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);					//アルベドマップ
Texture2D<float4> g_normalMap : register(t1);				//法線マップ
Texture2D<float4> g_specularMap : register(t2);				//スペキュラマップ
Texture2D<float4> g_shadowMap : register(t10); 				// シャドウマップ
Texture2D<float4> g_depthTexture : register(t11);			//深度テクスチャ

StructuredBuffer<float4x4> g_boneMatrix : register(t3);		//ボーン行列。
sampler g_sampler : register(s0);							//サンプラステート。
SamplerComparisonState g_shadowMapSampler : register(s1);	//シャドウマップサンプリング用のサンプラーステート

//ディザパターン
static const int pattern[4][4] = {
    { 0, 32,  8, 40},
    { 48, 16, 56, 24},
    { 12, 44,  4, 36},
    { 60, 28, 52, 20},
};
static const int anim_array_max = 4;		//アニメーション正方形の配列の数
