#pragma once

/// <summary>
/// ロゴクラス。
/// </summary>
class Logo : public IGameObject
{
public:
	Logo();
	~Logo();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	SpriteRender	m_logoSpriteRender;		//ロゴ画像
	float			m_timer = 0.0f;			//時間
};