#include "stdafx.h"
#include "Enemy_Normal.h"

#include "GameManager.h"
namespace
{
	const float		LINEAR_COMPLETION = 0.2f;		// ���`�⊮�̃t���[����
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
	m_enemyRender.Init("Assets/modelData/enemy/enemy.tkm", m_animationClips, m_enAnimationClip_Num, enModelUpAxisZ, true, true, 2);
	m_enemyRender.SetScale(m_scale);
	m_enemyRender.SetPosition(m_position);
	m_enemyRender.SetRotation(m_rotation);

	Enemy::Start();

	m_point = &m_pointList[0];

	return true;
}

void Enemy_Normal::Update()
{
	Enemy::SearchPass(CRAW);

	// �M���e�ɓ�������
	if (Enemy::GetHitFlushBullet() == true) {
		m_ActState = CONFUSION;
	}
	// �����e���g�p����
	if (Enemy::GetHitSoundBullet() == true) {
		m_ActState = LISTEN;
	}

	switch (m_ActState) {
		// ����
	case CRAW:
		Update_OnCraw();
		break;
		// �ǐ�
	case TRACKING:
		Update_OnTracking();
		break;
		// �Ă΂ꂽ�Ƃ�
	case CALLED:
		Update_OnCalled();
		break;
		// �����Ԃɖ߂�
	case BACKBASEDON:
		Update_OnBackBasedOn();
		break;
		// �M���e�ɓ�������
	case CONFUSION:
		Update_OnConfusion();
		break;
		// �����e���g�p�����Ƃ�
	case LISTEN:
		UpDate_OnListen();
		// �ߊl
	case CATCH:
		Update_OnCatch();
		break;
	}

	Animation();					// �A�j���[�V����

	m_enemyRender.SetPosition(m_position);
	m_characterController.SetPosition(m_position);

	// �L�����N�^�[�R���g���[���[�����f���̈ʒu�Ɠ���
	Vector3 move = Vector3::Zero;
	m_position = m_characterController.Execute(move, g_gameTime->GetFrameDeltaTime());

	Enemy::SpotLight_Serch(m_rotation, m_position);

	m_enemyRender.Update();	// �X�V
}

void Enemy_Normal::Update_OnCraw()
{
	Enemy::Act_Craw();				// ����s��

	// ����p�Ƀv���C���[������Ƃ�
	if (Enemy::Act_SeachPlayer() == true) {
		m_ActState = TRACKING;
	}

	// �v���C���[��߂܂����Ƃ�
	if (Enemy::Act_CatchPlayer() == true) {
		m_ActState = CATCH;
	}
}

void Enemy_Normal::Update_OnTracking()
{
	Enemy::Act_Tracking();			// �ǐՍs��

	// ����p�Ƀv���C���[�����Ȃ��Ƃ�
	if (Enemy::Act_SeachPlayer() == false) {
		m_ActState = BACKBASEDON;
	}

	// �v���C���[��߂܂����Ƃ�
	if (Enemy::Act_CatchPlayer() == true) {
		m_ActState = CATCH;
	}
}

void Enemy_Normal::Update_OnCalled()
{
	Enemy::Act_Called();

	// ����p�Ƀv���C���[������Ƃ�
	if (Enemy::Act_SeachPlayer() == true) {
		m_ActState = TRACKING;
	}
}

void Enemy_Normal::Update_OnBackBasedOn()
{
	Enemy::Act_Loss();					// �ǐՍs������̐؂�ւ�
	m_ActState = CRAW;
}

void Enemy_Normal::Update_OnConfusion()
{
	// �M���e�ɓ��������Ƃ�

	Enemy::Act_HitFlashBullet();

	// �d���������Ă���Ƃ�
	if (Enemy::GetHitFlushBullet() == false) {
		m_ActState = BACKBASEDON;
	}
}

void Enemy_Normal::UpDate_OnListen()
{
	// �����e���g�����Ƃ�

	Enemy::Act_HitSoundBullet();

	// ���ʂ��I�������Ƃ�
	if (Enemy::GetHitSoundBullet() == false) {
		m_ActState = BACKBASEDON;
	}
}

void Enemy_Normal::Update_OnCatch()
{

	Enemy::Act_CatchPlayer();

	m_ActState = CRAW;
}

void Enemy_Normal::Animation()
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

void Enemy_Normal::Render(RenderContext& rc)
{
	// �`��
	m_enemyRender.Draw(rc);

	if (Enemy::Act_CatchPlayer() == true) {
		m_fontRender.Draw(rc);
	}
}