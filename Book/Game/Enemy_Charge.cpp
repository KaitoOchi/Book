#include "stdafx.h"
#include "Enemy_Charge.h"

namespace 
{
	const float		LINEAR_COMPLETION = 0.2f;		// ���`�⊮�̃t���[����
	const float		STOP_TIMER = 5.0f;				// ���ߎ���
}

Enemy_Charge::Enemy_Charge()
{
}

Enemy_Charge::~Enemy_Charge()
{
}

bool Enemy_Charge::Start()
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
	m_enemyRender.Init("Assets/modelData/enemy/enemy.tkm", m_animationClips, m_enAnimationClip_Num, enModelUpAxisZ);
	m_enemyRender.SetScale(m_scale);
	m_enemyRender.SetPosition(m_position);
	m_enemyRender.SetRotation(m_rotation);

	Enemy::Start();

	// �p�X�ړ�
	m_point = &m_pointList[0];

	// ������쐬
	Enemy::SpotLight_New(m_position);

	return true;
}

void Enemy_Charge::Update()
{
	Act();			// �s���p�^�[��
	Animation();	// �A�j���[�V����

	// �X�V
	m_enemyRender.SetPosition(m_position);
	m_characterController.SetPosition(m_position);

	// �L�����N�^�[�R���g���[���[�����f���̈ʒu�Ɠ���
	Vector3 move = Vector3::Zero;
	m_position = m_characterController.Execute(move, g_gameTime->GetFrameDeltaTime());

	// �X�|�b�g���C�g
	Enemy::SpotLight_Serch(m_rotation, m_position);

	m_enemyRender.Update();
}

void Enemy_Charge::Act()
{
	Enemy::Act_HitFlashBullet();		// �M���e�ɓ��������Ƃ��̏���

	// �v���C���[�𔭌������Ƃ�
	if (Enemy::Act_SeachPlayer() == true) {

		Act_Charge(STOP_TIMER);		// �ːi�U��

		if (HitFlashBulletFlag == true || Enemy::Act_SeachPlayer() == false) {
			ChangeFlag = true;
		}
	}
	// �v���C���[���������@�܂��́@�M���e���q�b�g�����Ƃ�
	else if (HitFlashBulletFlag == true || Enemy::Act_SeachPlayer() == false) {

		if (ChangeFlag == true) {
			Enemy::Act_Loss();			// �ǐՍs������̐؂�ւ�
			ChangeFlag = false;			// ���x���񂾂����������s����
		}
		else {
			Enemy::Act_Craw();			// ����s��
		}
	}
}

void Enemy_Charge::Animation()
{
	// �A�j���[�V�����̃X�e�[�g
	switch (m_enEnemyAnimationState) {
		// �ҋ@
	case Enemy::m_enEnemyAnimationState_Idle:
		m_enemyRender.PlayAnimation(m_enAnimationClip_Idle, LINEAR_COMPLETION);
		break;
		// ����
	case Enemy::m_enEnemyAnimationState_Walk:
		m_enemyRender.PlayAnimation(m_enAnimationClip_Walk, LINEAR_COMPLETION);
		break;
		// ����
	case Enemy::m_enEnemyAnimationState_Run:
		m_enemyRender.PlayAnimation(m_enAnimationClip_Run, LINEAR_COMPLETION);
		break;
		// �U��
	case Enemy::m_enEnemyAnimationState_Attack:
		m_enemyRender.PlayAnimation(m_enAnimationClip_Attack, LINEAR_COMPLETION);
		break;
		// ��e
	case Enemy::m_enEnemyAnimationState_Damege:
		m_enemyRender.PlayAnimation(m_enAnimationClip_Damege, LINEAR_COMPLETION);
		break;
	}
}

void Enemy_Charge::Render(RenderContext& rc)
{
	// �`��
	m_enemyRender.Draw(rc);
}