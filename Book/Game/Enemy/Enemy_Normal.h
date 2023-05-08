#pragma once
#include "Enemy.h"
class Enemy_Normal:public Enemy
{
public:
	Enemy_Normal();
	~Enemy_Normal();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void Update_OnCraw();				// ����
	void Update_OnTracking();			// �ǐ�
	void Update_OnCalled();				// CALL���Ɏ��s
	void Update_OnBackBasedOn();		// �����Ԃɖ߂�
	void Update_OnConfusion();			// �M���e�ɓ��������Ƃ�
	void UpDate_OnListen();				// �����e���g�p�����Ƃ�
	void Update_OnCatch();				// �ߊl

private:

	Enemy* enemy;
};