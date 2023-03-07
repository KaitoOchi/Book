/*!
 * @brief	シンプルなモデルシェーダー。
 */

////////////////////////////////////////////////
// 定数バッファ。
////////////////////////////////////////////////
//モデル用の定数バッファ
cbuffer ModelCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};

//ライト用の定数バッファ
cbuffer LightCb : register(b1) {

	//ディレクションライト用の定数バッファ
	float3 dirDirection;	//ライトの方向
	float3 dirColor;		//ライトのカラー
	float3 eyePos;			//視点の位置
	float3 ambient;			//環境光の強さ

	//ポイントライト用の定数バッファ
	float3 ptPosition;		//ポイントライトの位置
	float ptNum;			//ポイントライトの数
	float3 ptColor;			//ポイントライトのカラー
	float ptRange;			//ポイントライトの影響範囲
}


////////////////////////////////////////////////
// 構造体
////////////////////////////////////////////////
//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn{
	int4  Indices  	: BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};
//頂点シェーダーへの入力。
struct SVSIn{
	float4 pos 		: POSITION;		//モデルの頂点座標。
	float3 normal	: NORMAL;		//法線
	float3 tangent	: TANGENT;		//接ベクトル
	float3 biNormal : BINORMAL;		//従ベクトル
	float2 uv 		: TEXCOORD0;	//UV座標。
	SSkinVSIn skinVert;				//スキン用のデータ。
};
//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos 			: SV_POSITION;	//スクリーン空間でのピクセルの座標。
	float3 normal		: NORMAL;		//法線
	float3 tangent		: TANGENT;		//接ベクトル
	float3 biNormal		: BINORMAL;		//従ベクトル
	float2 uv 			: TEXCOORD0;	//uv座標。
	float3 worldPos		: TEXCOORD1;	//ワールド座標
};


////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);				//アルベドマップ
Texture2D<float4> g_normalMap : register(t1);			//法線マップ
StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。
sampler g_sampler : register(s0);	//サンプラステート。

///////////////////////////////////////////
// 関数宣言
///////////////////////////////////////////
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal);

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
/// 頂点シェーダーのコア関数。
/// </summary>
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
	psIn.worldPos = psIn.pos;
	psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);

	//頂点法線をピクセルシェーダーにわたす
	psIn.normal = mul(m, vsIn.normal);

	//ワールド空間に変換
	psIn.tangent = normalize(mul(mWorld, vsIn.tangent));
	psIn.biNormal = normalize(mul(mWorld, vsIn.biNormal));

	psIn.uv = vsIn.uv;

	return psIn;
}

/// <summary>
/// スキンなしメッシュ用の頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
	return VSMainCore(vsIn, false);
}

/// <summary>
/// スキンありメッシュの頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSSkinMain( SVSIn vsIn ) 
{
	return VSMainCore(vsIn, true);
}

/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
	//ディフューズマップをサンプリング
	float4 diffuseMap = g_albedo.Sample(g_sampler, psIn.uv);

	float3 normal = psIn.normal;

	//法線マップからタンジェントスペースの法線をサンプリングする
	float3 localNormal = g_normalMap.Sample(g_sampler, psIn.uv).xyz;
	localNormal = (localNormal - 0.5f) * 2.0f;

	//タンジェントスペースの法線をワールドスペースに変換する
	normal = psIn.tangent * localNormal.x + psIn.biNormal * localNormal.y + normal * localNormal.z;

	//拡散反射光を求める
	float3 diffDirection = CalcLambertDiffuse(dirDirection, dirColor, psIn.normal);

	//鏡面反射光を求める
	float3 specDirection = CalcPhongSpecular(dirDirection, dirColor, psIn.worldPos, psIn.normal);


	//サーフェイスに入射するポイントライトの光の向きを計算
	float3 ligDir = psIn.worldPos - ptPosition;
	//正規化する
	ligDir = normalize(ligDir);

	//減衰無しのLambert拡散反射光を計算する
	float3 diffPoint = CalcLambertDiffuse(
		ligDir,
		ptColor,
		psIn.normal
	);

	//減衰なしのPhong鏡面反射光を計算する
	float3 specPoint = CalcPhongSpecular(
		ligDir,
		ptColor,
		psIn.worldPos,
		psIn.normal
	);

	//距離による影響率を計算する
	//ポイントライトとの距離を計算する
	float distance = length(psIn.worldPos - ptPosition);

	//影響率は距離に比例して小さくなっていく
	float affect = 1.0f - 1.0f / ptRange * distance;

	//影響率がマイナスにならないように補正をかける
	if (affect < 0.0f) {
		affect = 0.0f;
	}

	//影響を指数関数的にする
	affect = pow(affect, 3.0f);

	//拡散反射光と鏡面反射光に減衰率を乗算して影響を弱める
	diffPoint *= affect;
	specPoint *= affect;

	//2つの反射光を合算して最終的な反射光を求める
	float3 diffuseLig = diffPoint + diffDirection;
	float3 specularLig = specPoint + specDirection;



	//拡散反射と鏡面反射と環境光を足して、最終的な光を求める
	float3 lig = diffuseLig + specularLig + ambient;



	float4 albedoColor = diffuseMap;

	albedoColor.xyz *= lig;

	return albedoColor;
}

/// <summary>
/// Lambert拡散反射光を計算する
/// </summary>
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal)
{
	//ピクセルの法線とライトの方向の内積を計算する
	float t = dot(normal, lightDirection);

	//内積の結果に-1を乗算する
	t *= -1.0f;

	if (t < 0.0f) {
		t = 0.0f;
	}

	//拡散反射光を求める
	return lightColor * t;
}

/// <summary>
/// Phong鏡面反射光を計算する
/// </summary>
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal)
{
	//反射ベクトルを求める
	float3 refVec = reflect(lightDirection, normal);

	//光があたったサーフェイスから視点に伸びるベクトルを求める
	worldPos = float3(0.0f, 0.0f, 0.0f);
	float3 toEye = eyePos - worldPos;

	//正規化する
	toEye = normalize(toEye);

	//鏡面反射の強さを求める
	float t = dot(refVec, toEye);

	if (t < 0.0f) {
		t = 0.0f;
	}

	//鏡面反射の強さを求める
	t = pow(t, 5.0f);

	//鏡面反射光を求める
	 return lightColor * t;
}