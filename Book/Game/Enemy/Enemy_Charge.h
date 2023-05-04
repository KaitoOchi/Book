#pragma once
#include "Enemy.h"
class Enemy_Charge : public Enemy
{
public:
	Enemy_Charge();
	~Enemy_Charge();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void Animation();					// �A�j���[�V����
	void Update_OnCraw();				// ����
	void Update_OnCharge();				// �ːi
	void Update_OnCalled();				// CALL���Ɏ��s
	void Update_OnBackBasedOn();		// �����Ԃɖ߂�
	void Update_OnConfusion();			// �M���e�ɓ��������Ƃ�
	void UpDate_OnListen();				// �����e���g�p�����Ƃ�
	void Update_OnCatch();				// �ߊl
private:

	Enemy* enemy;

	// �A�j���[�V�����N���b�v
	enum EnEnemyAnimationClip
	{
		m_enAnimationClip_Idle,			// �ҋ@
		m_enAnimationClip_Walk,			// ����
		m_enAnimationClip_Run,			// ����
		m_enAnimationClip_Attack,		// �U��
		m_enAnimationClip_Damege,		// ��e
		m_enAnimationClip_Num
	};
	// �A�j���[�V�����N���b�v
	AnimationClip m_animationClips[m_enAnimationClip_Num];
};