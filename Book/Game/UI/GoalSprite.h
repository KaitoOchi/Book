#pragma once

/// <summary>
/// 目標画像クラス。
/// </summary>
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
	/// 表示するスプライトを設定。
	/// </summary>
	void SetSpriteNum(const bool enable)
	{
		m_enableNum = enable;
	}

private:
	SpriteRender	m_goalSpriteRender[2];	//目標画像
	Vector3			m_goalPos;				//座標
	bool			m_enableNum;			//どちらを表示するか
	float			m_goalTimer = 0.0f;		//タイマー
	float			m_alphaTimer = 0.0f;	//透明度タイマー
};