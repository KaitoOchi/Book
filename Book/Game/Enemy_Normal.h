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

	void HeadToDestination();			// �ړI�n�Ɍ���������
	void Animation();					// �A�j���[�V����
	void Act();

private:

	ModelRender m_NormalModelRender;	// ���f�������_�[

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

	// �G�l�~�[�̃A�j���[�V�����X�e�[�g
	enum EnEnemyAnimationState
	{
		m_enEnemyAnimationState_Idle,	// �ҋ@
		m_enEnemyAnimationState_Walk,	// ����
		m_enEnemyAnimationState_Run,	// ����
		m_enEnemyAnimationState_Attack,	// �U��
		m_enEnemyAnimationState_Damege,	// ��e
	};
	// �A�j���[�V�����X�e�[�g
	EnEnemyAnimationState m_enEnemyAnimationState = m_enEnemyAnimationState_Walk;
};

