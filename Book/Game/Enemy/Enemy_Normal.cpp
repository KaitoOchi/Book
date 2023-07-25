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
	Enemy::LoadAnimation();

	// ���f���̓ǂݍ���
	m_enemyRender.Init("Assets/modelData/enemy/enemy_normal.tkm", m_enAnimationClips, m_enAnimation_Num, enModelUpAxisZ, true, true, ModelRender::enOutlineMode_Enemy);

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
	if (GetActiveFlag() == true)
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
	if (GetNotDrawFlag() == true) {
		if (m_Effect != nullptr) {
			m_Effect->Stop();
			DeleteGO(m_Effect);
		}
		return;
	}

	// �v���C���[��߂܂����Ƃ�
	if (GetEnemyActionState() == m_ActionState_CatchPlayer) {
		SetEnemyAnimationState(m_enAnimationState_Idle);
		return;
	}

	if (GetHearedSoundBulletFlag() == true && GetHitFlashBulletFlag() == true) {
		// �M���e��D�悷��
		SetHearedSoundBulletFlag(false);
	}

	if (GetEnemyActionState() == m_ActionState_Tracking && GetHearedSoundBulletFlag() == true) {
		// �ǐՂ�D�悷��
		SetHearedSoundBulletFlag(false);
	}

	// �M���e�ɓ��������Ƃ�
	if (GetHitFlashBulletFlag() == true) {
		SetEnemyActionState(m_ActionState_Dizzy);
	}
	// �����e���g�p���ꂽ�Ƃ�
	if (GetHearedSoundBulletFlag() == true) {
		SetEnemyActionState(m_ActionState_Listen);
	}

	switch (GetEnemyActionState()) {
	case m_ActionState_Craw:
		Update_OnCraw();
		break;
	case m_ActionState_Tracking:
		Update_OnTracking();
		break;
	case m_ActionState_Move_MissingPositon:
		Update_OnMoveMissingPosition();
		break;
	case m_ActionState_Search_MissingPlayer:
		Update_OnSearchMissingPlayer();
		break;
	case m_ActionState_Called:
		Update_OnCalled();
		break;
	case m_ActionState_BackBasedOn:
		Update_OnBackBasedOn();
		break;
	case m_ActionState_Dizzy:
		Update_OnDizzy();
		break;
	case m_ActionState_Listen:
		UpDate_OnListen();
		break;
	}

	Enemy::PlayAnimation();

	m_enemyRender.SetPosition(m_position);
	m_characterController.SetPosition(m_position);

	// �L�����N�^�[�R���g���[���[�����g�̍��W�Ɠ���
	Vector3 move = Vector3::Zero;
	m_position = m_characterController.Execute(move, g_gameTime->GetFrameDeltaTime());

	Enemy::SpotLight_Serch(m_rotation, m_position);
	Enemy::Action_SeachPlayer();

	m_enemyRender.Update();
}

void Enemy_Normal::Update_OnCraw()
{
	if (Action_CatchPlayer() == true) {
		SetEnemyActionState(m_ActionState_CatchPlayer);
	}

	Enemy::Action_CrawPath();
}

void Enemy_Normal::Update_OnTracking()
{
	if (Action_CatchPlayer() == true) {
		SetEnemyActionState(m_ActionState_CatchPlayer);
	}

	Enemy::Action_TrackingPlayer();
}

void Enemy_Normal::Update_OnCalled()
{
	Enemy::Action_GoLocationListenSound(m_gatherPosition);
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
	Enemy::Action_GoLocationListenSound(m_itemPosition);
}

void Enemy_Normal::Render(RenderContext& rc)
{
	// �`��
	if (GetNotDrawFlag() == false &&
		GetActiveFlag() == false) {
		m_enemyRender.Draw(rc);
	}
}