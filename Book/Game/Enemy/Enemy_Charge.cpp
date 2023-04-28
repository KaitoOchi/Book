#include "stdafx.h"
#include "Enemy_Charge.h"

namespace 
{
	const float		LINEAR_COMPLETION = 0.2f;		// ���`�⊮�̃t���[����
	const float		STOP_TIMER = 3.0f;				// ���ߎ���
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
	m_enemyRender.Init("Assets/modelData/enemy/enemy.tkm", m_animationClips, m_enAnimationClip_Num, enModelUpAxisZ, true, true);
	m_enemyRender.SetScale(m_scale);
	m_enemyRender.SetPosition(m_position);
	m_enemyRender.SetRotation(m_rotation);

	Enemy::Start();

	// �p�X�ړ�
	m_point = &m_pointList[0];

	//// ������쐬
	//Enemy::SpotLight_New(m_position, 1);

	return true;
}

void Enemy_Charge::Update()
{
	Enemy::SearchPass(CRAW);

	switch (m_ActState) {
		// ����
	case CRAW:
		Update_OnCraw();
		break;
		// �ːi
	case CHARGE:
		Update_OnCharge();
		break;
		// �Ă΂ꂽ�Ƃ�
	case CALLED:
		Update_OnCalled();
		break;
		// �����Ԃɖ߂�
	case BACKBASEDON:
		Update_OnBackBasedOn();
		break;
		// ����
	case CONFUSION:
		Update_OnConfusion();
		break;
		// �������Ȃ�
	case NOOP:
		return;
		break;
	}

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

void Enemy_Charge::Update_OnCraw()
{
	// ����

	Enemy::Act_Craw();					// ����s��

	// ����p�Ƀv���C���[������Ƃ�
	if (Enemy::Act_SeachPlayer() == true) {
		m_ActState = CHARGE;
	}

	// �M���e�����������Ƃ�
	if (m_HitFlashBulletFlag == true) {
		m_ActState = CONFUSION;
	}
}

void Enemy_Charge::Update_OnCharge()
{
	// �ːi

	Enemy::Act_Charge(STOP_TIMER);		// �ːi�U��
										// �֐����ŏ����Ԃɖ߂鏈�����L�q

	// �M���e�����������Ƃ�
	if (m_HitFlashBulletFlag == true) {
		m_ActState = CONFUSION;
	}
}

void Enemy_Charge::Update_OnBackBasedOn()
{
	// �ːi�ˏ���ւ̐؂�ւ�
	Enemy::Act_Loss();					// �ǐՍs������̐؂�ւ�
	m_ActState = CRAW;
}

void Enemy_Charge::Update_OnCalled()
{
	Enemy::Act_Called();

	// ����p�Ƀv���C���[������Ƃ�
	if (Enemy::Act_SeachPlayer() == true) {
		m_ActState = CHARGE;
	}
}

void Enemy_Charge::Update_OnConfusion()
{
	// ����

	Enemy::Act_HitFlashBullet();		// �M���e�ɓ��������Ƃ��̏���

	// �M���e�ɓ������Ă��Ȃ��Ƃ�
	if (m_HitFlashBulletFlag == false) {
		m_ActState = BACKBASEDON;
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
	m_enemyRender.Draw(rc);
}