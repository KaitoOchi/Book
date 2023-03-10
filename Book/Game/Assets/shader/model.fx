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
	float3 ptColor;			//ポイントライトのカラー
	float ptRange;			//ポイントライトの影響範囲

	//スポットライト用の定数バッファ
	float3 spPosition;		//スポットライトの位置
	float3 spColor;			//スポットライトのカラー
	float spRange;			//スポットライトの影響範囲
	float3 spDirection;		//スポットライトの方向
	float spAngle;			//スポットライトの射出角度
		
	//半球ライト用の定数バッファ
	float3 groundColor;		//照り返しのライト
	float3 skyColor;		//天球ライト
	float3 groundNormal;	//地面の法線
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
	float3 normalInView : TEXCOORD2;	//カメラ空間の法線
};


////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);				//アルベドマップ
Texture2D<float4> g_normalMap : register(t1);			//法線マップ
Texture2D<float4> g_specularMap : register(t2);			//スペキュラマップ

StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。
sampler g_sampler : register(s0);	//サンプラステート。

///////////////////////////////////////////
// 関数宣言
///////////////////////////////////////////
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal);
float3 CalcLigFromDirectionLight(SPSIn psIn);
float3 CalcLigFromPointLight(SPSIn psIn);
float3 CalcLigFromSpotLight(SPSIn psIn);
float CalcLim(float3 dirDirection, float3 normal, float3 normalInView);
float3 CalcHemiSphereLight(float3 normal, float3 groundColor, float3 skyColor, float3 groundNormal);
float3 CalcNormal(float3 normal, float3 tangent, float3 biNormal, float2 uv);
float3 CalcSpecular(float3 specLig, float2 uv);

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

	//カメラ空間の法線を求める
	psIn.normalInView = mul(mView, psIn.normal);

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

	if (diffuseMap.r == 0.0f && diffuseMap.g == 0.0f && diffuseMap.b == 0.0f) {
		diffuseMap.a = 0.0f;

		return diffuseMap;
	}

	//法線マップを求める
	float3 normal = CalcNormal(psIn.normal, psIn.tangent, psIn.biNormal, psIn.uv);

	//ディレクションライトを求める
	float3 directionLight = CalcLigFromDirectionLight(psIn);

	//ポイントライトを求める
	float3 pointLight = CalcLigFromPointLight(psIn);

	//スポットライトを求める
	float3 spotLight = CalcLigFromSpotLight(psIn);

	//半球ライトを求める
	float3 hemiLight = CalcHemiSphereLight(psIn.normal, groundColor, skyColor, groundNormal);

	//リムライトを求める
	float limPower = CalcLim(dirDirection, psIn.normal, psIn.normalInView);


	//最終的な反射光にリムライトの反射光を合算する
	float3 limColor = limPower * dirColor;

	//ディレクションライト、ポイントライト、スポットライト、環境光、リムライト、半球ライトを足して、最終的な光を求める
	float3 lig = directionLight 
				+ pointLight
				+ spotLight
				+ ambient
				+ limColor
				+ hemiLight;

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

/// <summary>
/// ディレクションライトを計算する
/// </summary>
float3 CalcLigFromDirectionLight(SPSIn psIn)
{
	//拡散反射光を求める
	float3 diffDirection = CalcLambertDiffuse(dirDirection, dirColor, psIn.normal);

	//鏡面反射光を求める
	float3 specDirection = CalcPhongSpecular(dirDirection, dirColor, psIn.worldPos, psIn.normal);

	//スペキュラマップを求める
	specDirection = CalcSpecular(specDirection, psIn.uv);

	return diffDirection + specDirection;
}

/// <summary>
/// ポイントライトを計算する
/// </summary>
float3 CalcLigFromPointLight(SPSIn psIn)
{
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

	return diffPoint + specPoint;
}

/// <summary>
/// スポットライトを計算する
/// </summary>
float3 CalcLigFromSpotLight(SPSIn psIn)
{
	//ピクセルの座標 - スポットライトの座標を計算
	float3 ligDir = psIn.worldPos - spPosition;
	//正規化して大きさ1のベクトルにする
	ligDir = normalize(ligDir);

	//減衰無しのLambert拡散反射光を計算する
	float3 diffSpotLight = CalcLambertDiffuse(
		ligDir,
		spColor,
		psIn.normal
	);

	//減衰無しのPhong鏡面反射光を計算する
	float3 specSpotLight = CalcPhongSpecular(
		ligDir,
		spColor,
		psIn.worldPos,
		psIn.normal
	);

	//スポットライトとの距離を計算する
	float3 distance = length(psIn.worldPos - spPosition);

	//影響率は距離に比例して小さくなっていく
	float affect = 1.0f - 1.0f / spRange * distance;

	//影響力がマイナスにならないように補正をかける
	if (affect < 0.0f) {
		affect = 0.0f;
	}
	//影響の仕方を指数関数的にする
	affect = pow(affect, 3.0f);

	//影響率を乗算して反射光を弱める
	diffSpotLight *= affect;
	specSpotLight *= affect;

	//入射光と射出方向の角度を求める
	float angle = dot(ligDir, spDirection);
	angle = abs(acos(angle));

	//角度による影響率を求める
	affect = 1.0f - 1.0f / spAngle * angle;

	//影響率がマイナスにならないように補正をかける
	if (affect < 0.0f) {
		affect = 0.0f;
	}
	//影響の仕方を指数関数的にする
	affect = pow(affect, 0.5f);

	//角度による影響率を反射光に乗算して、影響を弱める
	diffSpotLight *= affect;
	specSpotLight *= affect;

	return diffSpotLight + specSpotLight;

}

/// <summary>
/// リムライトを計算する
/// </summary>
float CalcLim(float3 dirDirection, float3 normal, float3 normalInView)
{
	//サーフェイスの法線と光の入射方向に依存するリムの強さを求める
	float power1 = 1.0f - max(0.0f, dot(dirDirection, normal));

	//サーフェイスの法線と視線の方向に依存するリムの強さを求める
	float power2 = 1.0f - max(0.0f, normalInView * -1.0f);

	//最終的なリムの強さを求める
	float limPow = power1 * power2;
	limPow = pow(limPow, 1.3f);

	return limPow;
}

/// <summary>
/// 半球ライトを計算する
/// </summary>
float3 CalcHemiSphereLight(float3 normal, float3 groundColor, float3 skyColor, float3 groundNormal)
{
	//サーフェイスの法線と地面の法線との内積を計算する
	float t = dot(normal, groundNormal);

	//内積の結果を0～1の範囲に変換する
	t = (t + 1.0f) / 2.0f;

	//地面色と天球色を補完率tで線形補間する
	float3 hemiLig = lerp(groundColor, skyColor, t);

	return hemiLig;
}

/// <summary>
/// 法線を計算する
/// </summary>
float3 CalcNormal(float3 normal, float3 tangent, float3 biNormal, float2 uv)
{
	//法線マップからタンジェントスペースの法線をサンプリングする
	float3 binSpcaeNormal = g_normalMap.Sample(g_sampler, uv).xyz;
	binSpcaeNormal = (binSpcaeNormal - 0.5f) * 2.0f;

	//タンジェントスペースの法線をワールドスペースに変換する
	float3 newNormal = tangent * binSpcaeNormal.x + biNormal * binSpcaeNormal.y + normal * binSpcaeNormal.z;

	return newNormal;
}

/// <summary>
/// スペキュラを計算
/// </summary>
float3 CalcSpecular(float3 specLig, float2 uv)
{
	//スペキュラマップからスペキュラ反射の強さをサンプリング
	float specPower = g_specularMap.Sample(g_sampler, uv).r;

	//鏡面反射の強さを鏡面反射光に乗算する
	specLig *= specPower * 10.0f;

	return specLig;
}