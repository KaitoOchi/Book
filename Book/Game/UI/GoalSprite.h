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
	/// �\������X�v���C�g��ݒ�B
	/// </summary>
	void SetSpriteNum(const bool enable)
	{
		m_enableNum = enable;
	}

private:
	SpriteRender	m_goalSpriteRender[2];	//�ڕW�摜
	Vector3			m_goalPos;				//���W
	bool			m_reverse = false;		//���]�����邩�ǂ���
	bool			m_enableNum;			//�ǂ����\�����邩
	float			m_goalTimer = 0.0f;		//�^�C�}�[
};