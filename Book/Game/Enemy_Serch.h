#pragma once
#include "Enemy.h"
class Enemy_Serch:public Enemy
{
public:
	Enemy_Serch();
	~Enemy_Serch();
	bool Start();
	void Update();
	void Rotaition();
	void LightRotaition();
	void Render(RenderContext& rc);
		
private:
	//SpotLight m_spotLight;				//�X�|�b�g���C�g
	Quaternion m_rot;					//��]
	bool m_rotState=true;				//��]�̌����̐ؑ�
	float m_rotTime = 2.0f;
};

