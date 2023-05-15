#pragma once


class CountDown : public IGameObject
{
public:
	CountDown();
	~CountDown();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	std::array<SpriteRender,4> m_countDownSpriteRender;		//�J�E���g�_�E���摜
	float			m_timer = 1.0f;					//�^�C�}�[
	int				m_count = 4;					//�J�E���g
};