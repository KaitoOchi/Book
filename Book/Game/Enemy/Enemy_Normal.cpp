#include "stdafx.h"
#include "Enemy_Normal.h"
#include "GameManager.h"
#include "PlayerManagement.h"
#include "Gage.h"

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

	// �p�X�̏������W��n��
	m_point = &m_pointList[0];

	return true;
}

void Enemy_Normal::Update()
{
	// �s���ł��邩���ׂ�
	if (m_activeFlag == true)
	{
		Vector3 move = m_position;
		move.y -= 30000.0f;
		m_characterController.Execute(move, 1.0f);
		m_position = move;
		m_spotLight.SetPosition(move);
		m_spotLight.Update();
		return;
	}

	// �C�x���g��̏���
	if (m_NotDrawFlag == true) {
		if (m_Effect != nullptr) {
			m_Effect->Stop();
			DeleteGO(m_Effect);
		}
		return;
	}

	// �v���C���[��߂܂����Ƃ�
	if (m_ActionState == m_ActionState_CatchPlayer) {
		m_enAnimationState = m_enAnimationState_Idle;
		return;
	}

	if (m_HearedSoundBulletFlag == true && m_HitFlashBulletFlag == true) {
		// �M���e��D�悷��
		m_HearedSoundBulletFlag = false;
	}

	if (m_ActionState == m_ActionState_Tracking && m_HearedSoundBulletFlag == true) {
		// �ǐՂ�D�悷��
		m_HearedSoundBulletFlag = false;
	}

	// �M���e�ɓ��������Ƃ�
	if (m_HitFlashBulletFlag == true) {
		m_ActionState = m_ActionState_Dizzy;
	}
	// �����e���g�p���ꂽ�Ƃ�
	if (m_HearedSoundBulletFlag == true) {
		m_ActionState = m_ActionState_Listen;
	}

	switch (m_ActionState) {
	case m_ActionState_Craw:
		// �w�肳�ꂽ�͈͂̏���
		Update_OnCraw();
		break;
	case m_ActionState_Tracking:
		// �v���C���[��ǐՂ���
		Update_OnTracking();
		break;
	case m_ActionState_Move_MissingPositon:
		// �v���C���[���Ō�Ɍ������W�܂ňړ�����
		Update_OnMoveMissingPosition();
		break;
	case m_ActionState_Search_MissingPlayer:
		// ���������v���C���[��T��
		Update_OnSearchMissingPlayer();
		break;
	case m_ActionState_Called:
		// Search�̍��W�߂��܂ňړ�����
		Update_OnCalled();
		break;
	case m_ActionState_BackBasedOn:
		// ���̃p�X�ɖ߂�
		Update_OnBackBasedOn();
		break;
	case m_ActionState_Dizzy:
		// ����
		Update_OnDizzy();
		break;
	case m_ActionState_Listen:
		// �������������ꏊ�Ɍ�����
		UpDate_OnListen();
		break;
	}

	Enemy::PlayAnimation();							// �A�j���[�V����

	m_enemyRender.SetPosition(m_position);
	m_characterController.SetPosition(m_position);

	// �L�����N�^�[�R���g���[���[�����g�̍��W�Ɠ���
	Vector3 move = Vector3::Zero;
	m_position = m_characterController.Execute(move, g_gameTime->GetFrameDeltaTime());

	Enemy::SpotLight_Serch(m_rotation, m_position);	// �X�|�b�g���C�g
	Enemy::Action_SeachPlayer();						// ���G

	m_enemyRender.Update();
}

void Enemy_Normal::Update_OnCraw()
{
	if (Action_CatchPlayer() == true) {
		m_ActionState = m_ActionState_CatchPlayer;
	}

	Enemy::Action_CrawPath();
}

void Enemy_Normal::Update_OnTracking()
{
	if (Action_CatchPlayer() == true) {
		m_ActionState = m_ActionState_CatchPlayer;
	}

	Enemy::Action_TrackingPlayer();
}

void Enemy_Normal::Update_OnCalled()
{
	Enemy::Action_GoLocationListenSound(m_setPos);
}

void Enemy_Normal::Update_OnMoveMissingPosition()
{
	Enemy::Action_MoveMissingPosition();
}

void Enemy_Normal::Update_OnSearchMissingPlayer()
{
	Enemy::Action_SearchMissingPlayer();
}

void Enemy_Normal::Update_OnBackBasedOn()
{
	Enemy::Action_MissingPlayer();
}

void Enemy_Normal::Update_OnDizzy()
{
	Enemy::Action_HitFlashBullet();
}


void Enemy_Normal::UpDate_OnListen()
{
	// �����e���g�����Ƃ�
	Enemy::Action_GoLocationListenSound(m_itemPos);
}

void Enemy_Normal::Render(RenderContext& rc)
{
	// �`��
	if (m_NotDrawFlag == false&&
		m_activeFlag == false) {
		m_enemyRender.Draw(rc);
	}
}