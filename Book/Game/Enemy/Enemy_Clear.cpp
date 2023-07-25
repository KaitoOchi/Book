#include "stdafx.h"
#include "Enemy_Clear.h"

#include "GameManager.h"
#include "PlayerManagement.h"
#include "Gage.h"

Enemy_Clear::Enemy_Clear()
{
}

Enemy_Clear::~Enemy_Clear()
{
}

bool Enemy_Clear::Start()
{
	m_clearFlag = true;

	Enemy::LoadAnimation();

	// �ʏ�̃��f����ǂݍ���
	m_enemyRender.Init("Assets/modelData/enemy/enemy_clear.tkm", m_enAnimationClips, m_enAnimation_Num, enModelUpAxisZ, true, true, ModelRender::enOutlineMode_Enemy);
	// �������`��p�̃��f����ǂݍ���
	m_clearModelRender.Init("Assets/modelData/enemy/enemy_clear.tkm", m_enAnimationClips, m_enAnimation_Num, enModelUpAxisZ, true, true, ModelRender::enOutlineMode_TranslucentEnemy);

	Enemy::Start();

	m_enemyRender.SetScale(m_scale);
	m_enemyRender.SetPosition(m_position);
	m_enemyRender.SetRotation(m_rotation);

	m_clearModelRender.SetScale(m_scale);
	m_clearModelRender.SetPosition(m_position);
	m_clearModelRender.SetRotation(m_rotation);

	// �p�X�̏������W��n��
	m_point = &m_pointList[0];

	return true;
}

void Enemy_Clear::Update()
{
	//�s���ł��邩���ׂ�
	if (GetActiveFlag() == true)
	{
		Vector3 move = m_position;
		move.y -= 30000.0f;
		m_characterController.Execute(move, 1.0f);
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

	if (GetActiveFlag() == m_ActionState_Tracking && GetHearedSoundBulletFlag() == true) {
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

	// �s���p�^�[��
	switch (GetEnemyActionState()) {
	case m_ActionState_Craw:
		Update_OnCraw();
		m_clearModelRender.PlayAnimation(m_enAnimation_Walk, 1.0f);
		break;
	case m_ActionState_Tracking:
		Update_OnTracking();
		m_clearModelRender.PlayAnimation(m_enAnimation_Run, 1.0f);
		break;
	case m_ActionState_Move_MissingPositon:
		Update_OnMoveMissingPosition();
		m_clearModelRender.PlayAnimation(m_enAnimation_Walk, 1.0f);
		break;
	case m_ActionState_Search_MissingPlayer:
		Update_OnSearchMissingPlayer();
		m_clearModelRender.PlayAnimation(m_enAnimation_Loss, 1.0f);
		break;
	case m_ActionState_Called:
		Update_OnCalled();
		m_clearModelRender.PlayAnimation(m_enAnimation_Run, 1.0f);
		break;
	case m_ActionState_BackBasedOn:
		Update_OnBackBasedOn();
		m_clearModelRender.PlayAnimation(m_enAnimation_Walk, 1.0f);
		break;
	case m_ActionState_Dizzy:
		Update_OnDizzy();
		m_clearModelRender.PlayAnimation(m_enAnimation_Dizzy, 1.0f);
		break;
	case m_ActionState_Listen:
		UpDate_OnListen();
		m_clearModelRender.PlayAnimation(m_enAnimation_Run, 1.0f);
		break;
	}

	Enemy::PlayAnimation();

	ClearChange();

	m_enemyRender.SetPosition(m_position);
	m_characterController.SetPosition(m_position);

	// �L�����N�^�[�R���g���[���[�����g�̍��W�Ɠ���
	Vector3 move = Vector3::Zero;
	m_position = m_characterController.Execute(move, g_gameTime->GetFrameDeltaTime());

	Enemy::SpotLight_Serch(m_rotation, m_position);
	Enemy::Action_SeachPlayer();

	m_enemyRender.Update();

	m_clearModelRender.SetPosition(m_position);
	m_clearModelRender.SetRotation(m_rotation);
	m_clearModelRender.Update();
}

void Enemy_Clear::Update_OnCraw()
{
	if (Action_CatchPlayer() == true) {
		SetEnemyActionState(m_ActionState_CatchPlayer);
	}

	Enemy::Action_CrawPath();
}

void Enemy_Clear::Update_OnTracking()
{
	if (Action_CatchPlayer() == true) {
		SetEnemyActionState(m_ActionState_CatchPlayer);
	}

	Enemy::Action_TrackingPlayer();
}

void Enemy_Clear::Update_OnCalled()
{
	Enemy::Action_GoLocationListenSound(m_gatherPosition);
}

void Enemy_Clear::Update_OnMoveMissingPosition()
{
	Enemy::Action_MoveMissingPosition();
}

void Enemy_Clear::Update_OnSearchMissingPlayer()
{
	Enemy::Action_SearchMissingPlayer();
}

void Enemy_Clear::Update_OnBackBasedOn()
{
	Enemy::Action_MissingPlayer();
}

void Enemy_Clear::Update_OnDizzy()
{
	Enemy::Action_HitFlashBullet();
}

void Enemy_Clear::UpDate_OnListen()
{
	Enemy::Action_GoLocationListenSound(m_itemPosition);
}

void Enemy_Clear::ClearChange()
{
	if (m_setActionState != GetEnemyActionState () && m_clearChangeTime >= 0.0f)
	{
		m_clearChangeTime -= g_gameTime->GetFrameDeltaTime();
		return;
	}

	if (GetEnemyActionState() == m_ActionState_Craw)
	{
		// ������
		m_clearFlag = true;
		m_setActionState = m_ActionState_Craw;
		m_clearChangeTime = 0.0f;
	}
	else if (GetEnemyActionState() == m_ActionState_Tracking)
	{
		// ����������
		m_clearFlag = false;
		m_setActionState = m_ActionState_Tracking;
		m_clearChangeTime = 1.0f;
	}
}

void Enemy_Clear::Render(RenderContext& rc)
{
	// �`��
	if (GetActiveFlag() == true)
	{
		Vector3 move{ 0.0f,-300.0f,0.0f };
		m_characterController.Execute(move, 1.0f);
		return;
	}

	if (GetNotDrawFlag() == true) {
		return;
	}

	// ���f���̐؂�ւ�
	if (m_clearFlag) {
		m_clearModelRender.Draw(rc);
	}
	else {
		m_enemyRender.Draw(rc);
	}
}