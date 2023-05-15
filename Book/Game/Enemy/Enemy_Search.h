#pragma once
#include "Enemy.h"
class Enemy_Search:public Enemy
{
public:
	Enemy_Search();
	~Enemy_Search()override;
	bool Start();
	void Update();
	void Rotaition();
	void LightRotaition();
	void Render(RenderContext& rc);

	void Update_OnSearch();				// ���G
	void Update_OnCall();				// ����̓G���Ă�
	void Update_OnCallEnd();			// �Ăԍs���̏I����
	void Update_OnConfusion();			// �M���e�ɓ��������Ƃ�
		
private:
	//SpotLight m_spotLight;				//�X�|�b�g���C�g
	Quaternion m_rot;					//��]
	bool m_rotState=true;				//��]�̌����̐ؑ�
	float m_rotTime = 2.0f;
};