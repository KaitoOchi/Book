//ディレクションライト用の構造体
struct DirectionLig
{
	float3 dirDirection;	//ライトの方向
	float3 dirColor;		//ライトのカラー
	float3 eyePos;			//視点の位置
	float3 ambient;			//環境光の強さ
};

//ポイントライト用の構造体
struct PointLig
{
	float3 ptPosition;		//ポイントライトの位置
	float3 ptColor;			//ポイントライトのカラー
	float ptRange;			//ポイントライトの影響範囲
};

//スポットライト用の構造体
struct SpotLig
{
	float3 spPosition;		//スポットライトの位置
	float3 spColor;			//スポットライトのカラー
	float spRange;			//スポットライトの影響範囲
	float3 spDirection;		//スポットライトの方向
	float spAngle;			//スポットライトの射出角度
};

//半球ライト用の構造体
struct HemiLig
{
	float3 groundColor;		//照り返しのライト
	float3 skyColor;		//天球ライト
	float3 groundNormal;	//地面の法線
};

//シャドウ用の構造体
struct Shadow
{
	float3 lightPos;		//ライトの座標
	float4x4 mLVP;			//ライトビュープロジェクション行列
	int playerAnim2D;		//2Dプレイヤーアニメーションの番号
};
