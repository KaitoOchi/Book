#pragma once
#include "Enemy.h"
class Enemy_Search:public Enemy
{
public:
	Enemy_Search();
	~Enemy_Search();
	bool Start();
	void Update();
	void Rotaition();
	void LightRotaition();
	void Render(RenderContext& rc);

	void Update_OnSearch();				// ���G
	void Update_OnConfusion();			// ����
		
private:
	//SpotLight m_spotLight;				//�X�|�b�g���C�g
	Quaternion m_rot;					//��]
	bool m_rotState=true;				//��]�̌����̐ؑ�
	float m_rotTime = 2.0f;
};