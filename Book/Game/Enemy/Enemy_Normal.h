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

	void Animation();					// �A�j���[�V����
	void Update_OnCraw();				// ����
	void Update_OnTracking();			// �ǐ�
	void Update_OnCalled();				// CALL���Ɏ��s
	void Update_OnBackBasedOn();		// �����Ԃɖ߂�
	void Update_OnConfusion();			// ����
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