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
	/// �摜�̏������B
	/// </summary>
	void InitSprite(const bool change)
	{
		if (change) {
			m_goalSpriteRender.Init("Assets/sprite/UI/Gauge/image_target2.DDS", 207.0f, 60.0f);
		}
		else {
			m_goalSpriteRender.Init("Assets/sprite/UI/Gauge/image_target.DDS", 231.0f, 60.0f);
		}
	}

private:
	SpriteRender	m_goalSpriteRender;		//�ڕW�摜
	Vector3			m_goalPos;				//���W
	bool			m_reverse = false;		//���]�����邩�ǂ���
	float			m_goalTimer = 0.0f;		//�^�C�}�[
};