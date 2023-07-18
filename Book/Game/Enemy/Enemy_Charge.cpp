#include "stdafx.h"
#include "Enemy_Charge.h"

#include "GameManager.h"
#include "PlayerManagement.h"
#include "Gage.h"

namespace 
{
	const float		STOP_TIMER = 1.5f;				// ���ߎ���
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
	LoadAnimation();

	// ���f���̓ǂݍ���
	m_enemyRender.Init("Assets/modelData/enemy/enemy_charge.tkm", m_enAnimationClips, m_enAnimation_Num, enModelUpAxisZ, true, true, 2);

	Enemy::Start();

	m_enemyRender.SetScale(m_scale);
	m_enemyRender.SetPosition(m_position);
	m_enemyRender.SetRotation(m_rotation);

	// �p�X�̏������W��n��
	m_point = &m_pointList[0];

	return true;
}

void Enemy_Charge::Update()
{
	// �s���ł��邩���ׂ�
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
		// �ҋ@��Ԃɂ���
		SetEnemyAnimationState(m_enAnimationState_Idle);
		return;
	}

	if (GetHearedSoundBulletFlag() == true && GetHitFlashBulletFlag() == true) {
		// �ǐՂ�D�悷��
		SetHearedSoundBulletFlag(false);
	}

	if (GetEnemyActionState() == m_ActionState_Tracking && GetHearedSoundBulletFlag() == true) {
		// �M���e��D�悷��
		SetHearedSoundBulletFlag(false);
	}

	// �M���e�ɓ�������
	if (GetHitFlashBulletFlag() == true) {
		SetEnemyActionState(m_ActionState_Dizzy);
	}
	// �����e���g�p����
	if (GetHearedSoundBulletFlag() == true) {
		SetEnemyActionState(m_ActionState_Listen);
	}

	switch (GetEnemyActionState()) {
		// ���̏ꏊ�����񂷂�
	case m_ActionState_Craw:
		Update_OnCraw();
		break;
		// �v���C���[�Ɍ������ēːi����
	case m_ActionState_Charge:
		Update_OnCharge();
		break;
		// �ːi�I������
	case m_ActionState_ChargeEnd:
		Update_OnChargeEnd();
		break;
		// ���������v���C���[��T��
	case m_ActionState_Search_MissingPlayer:
		Update_OnSearchMissingPlayer();
		break;
		// Search�̍��W�߂��Ɍ�����
	case m_ActionState_Called:
		Update_OnCalled();
		break;
		// ���̃p�X�ɖ߂�
	case m_ActionState_BackBasedOn:
		Update_OnBackBasedOn();
		break;
		// ����
	case m_ActionState_Dizzy:
		Update_OnDizzy();
		break;
		// �������������ꏊ�Ɍ�����
	case m_ActionState_Listen:
		UpDate_OnListen();
		break;
	}

	Enemy::PlayAnimation();							// �A�j���[�V����

	m_enemyRender.SetPosition(m_position);
	m_characterController.SetPosition(m_position);

	// �L�����N�^�[�R���g���[���[�����f���̈ʒu�Ɠ���
	Vector3 move = Vector3::Zero;
	m_position = m_characterController.Execute(move, g_gameTime->GetFrameDeltaTime());

	Enemy::SpotLight_Serch(m_rotation, m_position);	// �X�|�b�g���C�g
	Enemy::Action_SeachPlayer();						// ���G

	m_enemyRender.Update();
}

void Enemy_Charge::Update_OnCraw()
{
	if (Action_CatchPlayer() == true) {
		SetEnemyActionState(m_ActionState_CatchPlayer);
	}

	Enemy::Action_CrawPath();
}

void Enemy_Charge::Update_OnCharge()
{
	if (Enemy::Action_CatchPlayer() == true) {
		SetEnemyActionState(m_ActionState_CatchPlayer);
	}

	Enemy::Action_ChargeStart(STOP_TIMER);
}

void Enemy_Charge::Update_OnChargeEnd()
{
	Enemy::Action_ChargeEnd();
}

void Enemy_Charge::Update_OnSearchMissingPlayer()
{
	Enemy::Action_SearchMissingPlayer();
}

void Enemy_Charge::Update_OnBackBasedOn()
{
	Enemy::Action_MissingPlayer();
}

void Enemy_Charge::Update_OnCalled()
{
	Enemy::Action_GoLocationListenSound(m_setPos);
}

void Enemy_Charge::Update_OnDizzy()
{
	Enemy::Action_HitFlashBullet();
}

void Enemy_Charge::UpDate_OnListen()
{
	Enemy::Action_GoLocationListenSound(m_itemPos);
}

void Enemy_Charge::Update_OnCatch()
{
	Enemy::Action_CatchPlayer();
}

void Enemy_Charge::Render(RenderContext& rc)
{
	if (GetNotDrawFlag() == false&&
		GetActiveFlag() == false) {
		m_enemyRender.Draw(rc);
	}
}