#pragma once

class Wall;
class BackGround;

namespace
{
	const Vector3	WIPE_POS_MAX = { 8.0f, 662.0f, 0.0f };			//ワイプの最大座標
	const Vector3	WIPE_POS_MIN = { -270.0f, 662.0f, 0.0f };		//ワイプの最小座標
	const Vector3	OUTLINE_POS_MAX = { -671.5f, -280.0f, 0.0f };	//輪郭画像の最大座標
	const Vector3	OUTLINE_POS_MIN = { -950.0f, -280.0f, 0.0f };	//輪郭画像の最小座標
	const int		ENEMY_NUM_WIPE = 4;								//敵の数
}

/// <summary>
/// ワイプクラス。
/// </summary>
class Wipe : public IGameObject
{
public:
	Wipe();
	~Wipe();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

public:
	/// <summary>
	/// 敵の地点をリセット
	/// </summary>
	void Reset()
	{
		//敵の座標をリセット
		for (int i = 0; i < 3; i++) {
			m_enemy[i].moveSpeed[0] = m_bezierPos[0];
			m_enemy[i].moveSpeed[1] = m_bezierPos[0];
			m_enemy[i].moveSpeed[2] = m_bezierPos[1];
		}
		
		//ワイプ座標を初期化
		m_wipePos = WIPE_POS_MIN;
		RenderingEngine::GetInstance()->GetWipeViewPort().TopLeftX = m_wipePos.x;
		
		//枠組み座標を初期化
		m_outlinePos = OUTLINE_POS_MIN;
		
		//ワイプを表示状態にする
		m_isWipe = true;

		//タイマーのリセット
		m_timer = 0.0f;
		m_outlineTimer = 0.0f;
	}

	/// <summary>
	/// ワイプの終了処理。
	/// </summary>
	void WipeEnd()
	{
		m_timer = 0.0f;
		m_isWipe = false;
	}

private:
	/// <summary>
	/// レベルレンダー処理。
	/// </summary>
	void LevelDesign();

	/// <summary>
	/// 敵の移動処理。
	/// </summary>
	void EnemyMove();

	/// <summary>
	/// 輪郭線画像の処理。
	/// </summary>
	void WipeOutline();

private:
	//敵の構造体
	struct EnemyStruct
	{
		ModelRender modelRender;	//モデル
		Vector3		moveSpeed[3];	//移動速度
	};

private:
	SpriteRender		m_outlineSpriteRender;		//ワイプの輪郭画像
	SpriteRender		m_warningSpriteRender;		//警告画像
	LevelRender			m_levelRender;				//レベルレンダー
	PointLight			m_pointLight;				//ポイントライト
	AnimationClip*		m_enemyAnim = nullptr;		//敵のアニメーション
	BackGround*			m_backGround = nullptr;		//地面
	std::vector<Wall*>	m_stage;					//ステージ
	EnemyStruct			m_enemy[ENEMY_NUM_WIPE];	//敵の構造体
	Vector3				m_bezierPos[3];				//敵の移動座標
	Vector3				m_outlinePos;				//輪郭線の座標
	Vector3				m_wipePos;					//ワイプの座標
	bool				m_isWipe = false;			//ワイプを表示するかどうか
	float				m_timer = 0.0f;				//タイマー
	float				m_outlineTimer = 0.0f;		//ワイプ用タイマー
	float				m_alpha = 0.0f;				//透明
};