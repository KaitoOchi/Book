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
	SpriteRender	m_countDownSpriteRender[4];		//�J�E���g�_�E���摜
	float			m_timer = 0.0f;					//�^�C�}�[
	int				m_count = 3;					//�J�E���g
};