#include "stdafx.h"
#include "Enemy_Normal.h"

namespace
{
	const float		LINEAR_COMPLETION = 0.5f;				// ���`�⊮�̃t���[����
}

Enemy_Normal::Enemy_Normal()
{
}

Enemy_Normal::~Enemy_Normal()
{
}

bool Enemy_Normal::Start()
{
	// �A�j���[�V�����̓ǂݍ���
	m_animationClips[m_enAnimationClip_Idle].Load("Assets/animData/enemy/idle.tka");
	m_animationClips[m_enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[m_enAnimationClip_Walk].Load("Assets/animData/enemy/walk.tka");
	m_animationClips[m_enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[m_enAnimationClip_Run].Load("Assets/animData/enemy/run.tka");
	m_animationClips[m_enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[m_enAnimationClip_Attack].Load("Assets/animData/enemy/attack1.tka");
	m_animationClips[m_enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClips[m_enAnimationClip_Damege].Load("Assets/animData/enemy/damege.tka");
	m_animationClips[m_enAnimationClip_Damege].SetLoopFlag(false);

	// ���f���̓ǂݍ���
	m_NormalModelRender.Init("Assets/modelData/enemy/enemy.tkm", m_animationClips, m_enAnimationClip_Num, enModelUpAxisZ);
	//m_NormalModelRender.Init("Assets/modelData/unityChan.tkm");
	m_NormalModelRender.SetScale(m_scale);
	m_NormalModelRender.SetPosition(m_position);
	m_NormalModelRender.SetRotation(m_rotation);

	Enemy::Start();

	return true;
}

void Enemy_Normal::Update()
{
	// �X�V
	//HeadToDestination();
	m_NormalModelRender.Update();
}

void Enemy_Normal::HeadToDestination()
{
	// �v���C���[�𔭌������Ƃ�
	if (Enemy::SeachPlayer() == true) {
		// �ǐՂɈڍs����
		m_enEnemyActState = m_enEnemyActState_Tracking;

		// �v���C���[�Ɍ������o�H���쐬


		// �v���C���[���m�ۂ����Ƃ�
		if (Enemy::CatchPlayer() == true) {
		}
	}
	else {
		// ����Ɉڍs����
		m_enEnemyActState = m_enEnemyActState_Craw;

		// ���߂̃p�X��T��
		// �p�X�ɖ߂����Ƃ���false�ɂ���
	}
}

void Enemy_Normal::Animation()
{
	// �A�j���[�V�����̃X�e�[�g
	switch (m_enEnemyAnimationState) {
		// �ҋ@
	case Enemy::m_enEnemyAnimationState_Idle:
		m_NormalModelRender.PlayAnimation(m_enAnimationClip_Idle, LINEAR_COMPLETION);
		break;
		// ����
	case Enemy::m_enEnemyAnimationState_Walk:
		m_NormalModelRender.PlayAnimation(m_enAnimationClip_Walk, LINEAR_COMPLETION);
		break;
		// ����
	case Enemy::m_enEnemyAnimationState_Run:
		m_NormalModelRender.PlayAnimation(m_enAnimationClip_Run, LINEAR_COMPLETION);
		break;
		// �U��
	case Enemy::m_enEnemyAnimationState_Attack:
		m_NormalModelRender.PlayAnimation(m_enAnimationClip_Attack, LINEAR_COMPLETION);
		break;
		// ��e
	case Enemy::m_enEnemyAnimationState_Damege:
		m_NormalModelRender.PlayAnimation(m_enAnimationClip_Damege, LINEAR_COMPLETION);
		break;
	}
}

void Enemy_Normal::Render(RenderContext& rc)
{
	// �`��
	m_NormalModelRender.Draw(rc);
}