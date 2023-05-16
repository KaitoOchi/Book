#pragma once

class Wall;
class BackGround;

namespace
{
	const int ENEMY_NUM_WIPE = 4;				//敵の数
}

class Wipe : public IGameObject
{
public:
	Wipe();
	~Wipe();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

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
	/// 敵の地点をリセット
	/// </summary>
	void Reset()
	{
		for (int i = 0; i < 3; i++) {
			m_enemy[i].moveSpeed[0] = m_bezierPos[0];
			m_enemy[i].moveSpeed[1] = m_bezierPos[0];
			m_enemy[i].moveSpeed[2] = m_bezierPos[1];
		}

		m_timer = 0.0f;
	}


private:
	struct EnemyStruct
	{
		ModelRender modelRender;	//モデル
		Vector3		moveSpeed[3];	//移動速度
	};

private:
	LevelRender			m_levelRender;
	AnimationClip*		m_enemyAnim = nullptr;
	BackGround*			m_backGround = nullptr;
	std::vector<Wall*>	m_stage;
	EnemyStruct			m_enemy[ENEMY_NUM_WIPE];		//敵の構造体
	Vector3				m_bezierPos[3];	//敵の移動座標
	float				m_timer = 0.0f;
};