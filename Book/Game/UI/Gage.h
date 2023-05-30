#pragma once
class Game;
class Enemy;
class Fade;
#include "Wipe.h"
class Enemy_Increase;
class Player3D;
class Gage:public IGameObject
{
public:
	Gage();
	~Gage();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// �x���x��グ��
	/// </summary>
	/// <param name="GageUp">�x���x�𑝂₷��</param>
	void GageUp(const int GageUp, const bool isEnemy);

	/// <summary>
	/// ワイプの終了処理。
	/// </summary>
	void SetWipeEnd()
	{
		m_wipe->WipeEnd();
		m_leverState = m_enLever_MAX;
	}

	enum EnLeverState
	{
		m_enLever_1,		//�x���x���x���P
		m_enLever_2,		//�x���x���x���Q
		m_enLever_3,		//�x���x���x���R
		m_enLever_MAX,		//�x���x���x��MAX
	};

	EnLeverState m_leverState = m_enLever_1;

private:
	
	/// <summary>
	/// �x���x�������
	/// </summary>
	void GageDown();

	/// <summary>
	/// �x���x���x����؂�ւ���
	/// </summary>
	void VigilaceLeverChange();
	
	void Gage_ColorChange(); 

	/// <summary>
	/// �x���x��MAX�̎��̏���
	/// </summary>
	void Gage_MAX();

	EnLeverState m_GetState = m_leverState;

	std::array<SpriteRender,4>m_LeverUPRender;					//�x���x���x���摜
	SpriteRender m_maxLeverRender;

	SpriteRender m_baseRender;									//���ɒu���摜
	std::array<SpriteRender,10>m_vigilanceRender;				//�x���x�摜
	bool m_isFind = false;										//�������ꂽ���ǂ���
	bool m_MaxEnd=true;
	int m_vigilanceGage = 0;									//�x���x�Q�[�W��
	float m_vigilanceTime = 2.0f;								//�x���x�ɓ���N�[���^�C��

	float m_Color=0.7f;											//�x���x�J���[						

	float m_HitTime = 5.0f;
	float m_vigilaceDownTime = 2.0f;							//�x���x�������N�[���^�C��

	Game* m_game = nullptr;
	Wipe* m_wipe = nullptr;
	Fade* m_fade = nullptr;
	Player3D* m_player3D = nullptr;
	Enemy_Increase* m_enemy_Increase = nullptr;					//エネミーの増加
};

