#pragma once
#include "Enemy.h"
class Enemy_Charge : public Enemy
{
public:
	Enemy_Charge();
	~Enemy_Charge()override;

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void Update_OnCraw();				// ����
	void Update_OnCharge();				// �ːi
	void Update_OnChargeEnd();			// �ːi
	void Update_OnSearchMissingPlayer();// �v���C���[��T��
	void Update_OnCalled();				// CALL���Ɏ��s
	void Update_OnBackBasedOn();		// �����Ԃɖ߂�
	void Update_OnDizzy();			// �M���e�ɓ��������Ƃ�
	void UpDate_OnListen();				// �����e���g�p�����Ƃ�
	void Update_OnCatch();				// �ߊl
};