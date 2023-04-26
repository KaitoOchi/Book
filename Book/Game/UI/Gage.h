#pragma once
class Game;
class Enemy;
class Gage:public IGameObject
{
public:
	Gage();
	~Gage();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// </summary>
	/// <param name="GageUp">�x���x�𑝂₷��</param>
	void GageUp(int GageUp);
private:
	/// <summary>
	/// �x���x���グ��
	
	/// <summary>
	/// �x���x��������
	/// </summary>
	void GageDown();

	/// <summary>
	/// �x���x���x����؂�ւ���
	/// </summary>
	void VigilaceLeverChange();
	
	void Gage_ColorChange();
	enum EnLeverState
	{
		m_enLever_1,		//�x���x���x���P
		m_enLever_2,		//�x���x���x���Q
		m_enLever_3,		//�x���x���x���R
		m_enLever_MAX,		//�x���x���x��MAX
	};

	EnLeverState m_leverState= m_enLever_1;

	EnLeverState m_GetState = m_leverState;

	SpriteRender m_LeverUPRender[4];				//�x���x���x���摜
	SpriteRender m_baseRender;						//���ɒu���摜
	SpriteRender m_vigilanceRender[10];				//�x���x�摜
	
	float m_vigilanceTime = 2.0f;					//�x���x�ɓ���N�[���^�C��
	int m_vigilanceGage = 8;						//�x���x�Q�[�W��

	float m_Color=0.7f;							//�x���x�J���[						

	float m_vigilaceDownTime = 2.0f;				//�x���x��������N�[���^�C��

	Game* m_game = nullptr;
};
