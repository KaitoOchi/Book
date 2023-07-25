#include "stdafx.h"
#include "Enemy_Charge.h"

#include "GameManager.h"
#include "PlayerManagement.h"
#include "Gage.h"

namespace 
{
	const Vector3	ADD_LENGTH = { 50.0f, 0.0f, 50.0f };	// �ːi���ɒǉ����钷��
	const float		STOP_TIMER = 1.5f;						// ���ߎ���
	const float		MOVING_DISTANCE = 600.0f;				// �ːi����ړ�����
	const float		MOVE_SPEED = 3.0f;						// �ړ����x
	const float		ADD_SPEED = 1.3f;						// ��Z���x
}

Enemy_Charge::Enemy_Charge()
{
}

Enemy_Charge::~Enemy_Charge()
{
}

bool Enemy_Charge::Start()
{
	Enemy::LoadAnimation();

	// ���f���̓ǂݍ���
	m_enemyRender.Init("Assets/modelData/enemy/enemy_charge.tkm", m_enAnimationClips, m_enAnimation_Num, enModelUpAxisZ, true, true, ModelRender::enOutlineMode_Enemy);

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

	Enemy::PlayAnimation();

	m_enemyRender.SetPosition(m_position);
	m_characterController.SetPosition(m_position);

	// �L�����N�^�[�R���g���[���[�����f���̈ʒu�Ɠ���
	Vector3 move = Vector3::Zero;
	m_position = m_characterController.Execute(move, g_gameTime->GetFrameDeltaTime());

	Enemy::SpotLight_Serch(m_rotation, m_position);
	Enemy::Action_SeachPlayer();

	m_enemyRender.Update();
}

void Enemy_Charge::Action_ChargeStart(float time)
{
	// �ړ������̒��������ȏ�̂Ƃ�
	if (m_sumPosition.Length() >= MOVING_DISTANCE) {
		// �ːi���I������
		SetEnemyActionState(m_ActionState_ChargeEnd);
		m_sumPosition = Vector3::Zero;
		return;
	}

	// �^�C�}�[��true�̂Ƃ�
	if (Action_StopMove(time, m_TimerState_UntilTheCharge) == true) {

		// ��x�������s����
		if (m_canCharge == false) {
			// ���W���Q��
			m_chargeTergetPosition = m_playerManagement->GetPosition();

			// ���x�����s���Ȃ��悤��true�ɂ���
			m_canCharge = true;

			// �G�l�~�[����v���C���[�֌������x�N�g��
			m_chargeDiff = m_chargeTergetPosition - m_position;
			m_chargeDiff.y = 0.0f;
			m_chargeDiff.Normalize();
		}

		// �ړ����x�ɉ��Z
		Vector3 moveSpeed = m_chargeDiff * (MOVE_SPEED * ADD_SPEED);
		m_position += moveSpeed * m_chargeMove;

		// ���ړ��������v�Z
		m_sumPosition += moveSpeed;
		SetEnemyAnimationState(m_enAnimationState_Run);

		// �Փ˔���
		Action_ChargeHitWall();
	}
	else {
		// ��]�̂݃v���C���[��ǔ�������
		m_chargeDiff = m_playerManagement->GetPosition() - m_position;
		m_chargeDiff.Normalize();
		SetEnemyAnimationState(m_enAnimationState_Idle);
	}

	// ��]��������
	Rotation(m_chargeDiff);
}

void Enemy_Charge::Action_ChargeEnd()
{
	m_position = m_position;								// ���W���Œ�
	
	ReSetAddTimer(m_TimerState_UntilTheCharge);				// �^�C�}�[�����Z�b�g
	m_sumPosition = Vector3::Zero;							// �ړ����������Z�b�g
	m_canCharge = false;								// �t���O���~�낷

	SetEffectDrawFlag(m_EffectState_QuestionMark,false);	// �G�t�F�N�g�̃t���O���~�낷
	SetEffectDrawFlag(m_EffectState_ExclamationPoint, false);

	// �v���C���[������p���ɂ���Ƃ�
	if (GetTrackingPlayerFlag() == true) {
		Efect_FindPlayer();
		SetEnemyActionState(m_ActionState_Charge);
		return;
	}
	else {
		// ���Ȃ��Ƃ��͏����Ԃɖ߂�
		SetEnemyActionState(m_ActionState_Search_MissingPlayer);
	}
}

void Enemy_Charge::Action_ChargeHitWall()
{
	// �ǂɏՓ˂��锻��

	// �ǂɏՓ˂������ǂ���
	// �v���C���[�̕����֌������P�ʃx�N�g���ɃX�J���[����Z�������̂����Z���ēn��
	if (Enemy::WallAndHit(m_position + (m_chargeDiff * ADD_LENGTH.x)) == false) {
		// �Փ˂����Ƃ�
		m_chargeMove = 0.0f;									// ��Z���Ă���l��0�ɂ��ē����Ȃ��悤�ɂ���

		ReSetAddTimer(m_TimerState_UntilTheCharge);				// �^�C�}�[�����Z�b�g
		m_sumPosition = Vector3::Zero;							// �ړ����������Z�b�g
		m_canCharge = false;								// �t���O���~�낷

		SetEffectDrawFlag(m_EffectState_QuestionMark,false);	// !�̃G�t�F�N�g�̃t���O���~�낷
		SetEnemyActionState(m_ActionState_Dizzy);
		return;
	}

	// �Փ˂��Ă��Ȃ��Ƃ��͑��s����
	m_chargeMove = 1.0f;
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

	Action_ChargeStart(STOP_TIMER);
}

void Enemy_Charge::Update_OnChargeEnd()
{
	Action_ChargeEnd();
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
	Enemy::Action_GoLocationListenSound(m_gatherPosition);
}

void Enemy_Charge::Update_OnDizzy()
{
	Enemy::Action_HitFlashBullet();
}

void Enemy_Charge::UpDate_OnListen()
{
	Enemy::Action_GoLocationListenSound(m_itemPosition);
}

void Enemy_Charge::Update_OnCatch()
{
	Enemy::Action_CatchPlayer();
}

void Enemy_Charge::Render(RenderContext& rc)
{
	// �`��
	if (GetNotDrawFlag() == false&&
		GetActiveFlag() == false) {
		m_enemyRender.Draw(rc);
	}
}