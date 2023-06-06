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
	void Render(RenderContext& rc);

	void Update_OnSearch();				// ���G
	void Update_OnCallAroundEnemy();	// ����̓G���Ă�
	void Update_OnDizzy();				// �M���e�ɓ��������Ƃ�
	void Update_OnMissingPlayer();		// �v���C���[������������
		
private:
	bool m_rotState=true;				//��]�̌����̐ؑ�
	float m_rotTime = 2.0f;
};