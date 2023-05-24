#pragma once


class GoalSprite : public IGameObject
{
public:
	GoalSprite();
	~GoalSprite();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

public:
	/// <summary>
	/// 画像の初期化。
	/// </summary>
	void InitSprite(const bool change)
	{
		if (change) {
			m_goalSpriteRender.Init("Assets/sprite/UI/Gauge/image_target2.DDS", 414.0f, 120.0f);
		}
		else {
			m_goalSpriteRender.Init("Assets/sprite/UI/Gauge/image_target.DDS", 459.0f, 120.0f);
		}
	}

private:
	SpriteRender	m_goalSpriteRender;		//目標画像
	Vector3			m_goalPos;				//座標
	bool			m_reverse = false;		//反転させるかどうか
	float			m_goalTimer = 0.0f;		//タイマー
};