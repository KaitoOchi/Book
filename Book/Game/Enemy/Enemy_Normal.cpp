#include "stdafx.h"
#include "Enemy_Normal.h"
#include "PlayerManagement.h"

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
	Animation();

	// ���f���̓ǂݍ���
	m_enemyRender.Init("Assets/modelData/enemy/enemy_normal.tkm", m_enAnimationClips, m_enAnimation_Num, enModelUpAxisZ, true, true, 2);

	Enemy::Start();

	m_enemyRender.SetScale(m_scale);
	m_enemyRender.SetPosition(m_position);
	m_enemyRender.SetRotation(m_rotation);

	m_point = &m_pointList[0];

	return true;
}

void Enemy_Normal::Update()
{
	//行動できるか調べる
	if (m_activeFlag == true)
	{
		Vector3 move{ 0.0f,-3000.0f,0.0f };
		m_characterController.Execute(move, 1.0f);
		m_spotLight.SetPosition(move);
		m_spotLight.Update();
		return;
	}

	// �`�悵�Ȃ��t���O��true�̂Ƃ�
	if (m_NotDrawFlag == true) {
		if (m_soundEffect != nullptr) {
			m_soundEffect->Stop();
		}
		return;
	}
	// �f�t�H���g�ɖ߂��t���O��true�̂Ƃ�
	if (m_ChangeDefaultFlag == true) {
		m_ActState = CRAW;

		m_ChangeDefaultFlag = false;
	}

	// �M���e�ɓ��������@���@�����e�𕷂����Ƃ�
	if (m_HitSoundBulletFlag == true && m_HitFlashBulletFlag == true) {
		// �M���e��D�悷��
		m_HitSoundBulletFlag = false;
	}

	// �M���e�ɓ�������
	if (m_HitFlashBulletFlag == true) {
		m_ActState = CONFUSION;
	}
	// �����e��g�p����
	if (m_HitSoundBulletFlag == true) {
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
	case MISSING_MOVEPOSITON:
		Update_OnMoveMissingPosition();
		break;
	case MISSING_SEARCHPLAYER:
		Update_OnSearchMissingPlayer();
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
		// �����e��g�p�����Ƃ�
	case LISTEN:
		UpDate_OnListen();
		break;
	}

	Enemy::PlayAnimation();		// �A�j���[�V����

	m_enemyRender.SetPosition(m_position);
	m_characterController.SetPosition(m_position);

	// �L�����N�^�[�R���g���[���[����f���̈ʒu�Ɠ���
	Vector3 move = Vector3::Zero;
	m_position = m_characterController.Execute(move, g_gameTime->GetFrameDeltaTime());

	Enemy::SpotLight_Serch(m_rotation, m_position);

	// ����p
	Enemy::Act_SeachPlayer();

	m_enemyRender.Update();	// �X�V
}

void Enemy_Normal::Update_OnCraw()
{
	if (m_ActState == CATCH) {
		m_enAnimationState = IDLE;
		return;
	}

	// �v���C���[��߂܂����Ƃ�
	if (Act_CatchPlayer() == true) {
		m_ActState = CATCH;
	}

	Enemy::Act_Craw();				// ����s��
}

void Enemy_Normal::Update_OnTracking()
{
	// �v���C���[��߂܂����Ƃ�
	if (Act_CatchPlayer() == true) {
		m_ActState = CATCH;
	}

	Enemy::Act_Tracking();			// �ǐՍs��
}

void Enemy_Normal::Update_OnCalled()
{
	Enemy::Act_Called();
}

void Enemy_Normal::Update_OnMoveMissingPosition()
{
	// �v���C���[����������ʒu�܂ňړ�����
	Enemy::Act_MoveMissingPosition();
}

void Enemy_Normal::Update_OnSearchMissingPlayer()
{
	// �v���C���[��T��
	Enemy::Act_SearchMissingPlayer();
}

void Enemy_Normal::Update_OnBackBasedOn()
{
	Enemy::Act_Loss();
}

void Enemy_Normal::Update_OnConfusion()
{
	Enemy::Act_HitFlashBullet();		// �M���e�ɓ��������Ƃ��̏���
}


void Enemy_Normal::UpDate_OnListen()
{
	// �����e��g�����Ƃ�

	// �M���e��H����Ă���Ƃ��͎��s���Ȃ�
	if (m_HitFlashBulletFlag == true) {
		m_HitFlashBulletFlag = false;
		return;
	}

	Enemy::Act_HitSoundBullet();

	// ���ʂ��I�������Ƃ�
	if (m_HitSoundBulletFlag == false) {
		m_ActState = BACKBASEDON;
	}
}

void Enemy_Normal::Render(RenderContext& rc)
{
	// �`��
	if (m_NotDrawFlag == false&&
		m_activeFlag == false) {
		m_enemyRender.Draw(rc);
	}

	if (Enemy::Act_CatchPlayer() == true) {
		m_fontRender.Draw(rc);
	}
}