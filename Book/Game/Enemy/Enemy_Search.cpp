#include "stdafx.h"
#include "Enemy_Search.h"

#include "GameManager.h"
#include "Game.h"

namespace
{
	const Vector3	BOXSIZE = { 60.0f, 80.0f,60.0f };		// CharacterController�̃T�C�Y
	const float		KEEPTIME = 2.0f;						// �^�C�}�[�����Z�b�g����萔
	const float		ANGLE = 45.0f;							// �p�x
	const float		RODADD = 0.01f;							// �ǉ�����p�x
	const float		DOWN_VOLUME = 0.1f;						// SE�̉��ʌ����l
	const float		CALL_DISTANCE_MAX = 400.0f;				// �ĂԂ��Ƃ��ł���ő�l
	const float		CALL_DISTANCE_MIN = 190.0f;				// �ĂԂ��Ƃ��ł���ŏ��l
}

Enemy_Search::Enemy_Search()
{
}

Enemy_Search::~Enemy_Search()
{
}

bool Enemy_Search::Start()
{
	Enemy::LoadAnimation();

	// ���f���̓ǂݍ���
	m_enemyModelRender.Init("Assets/modelData/enemy/enemy_search.tkm", m_enAnimationClips, m_enAnimation_Num, enModelUpAxisZ, true, true, ModelRender::enOutlineMode_Enemy);

	Enemy::Start();

	m_enemyModelRender.SetPosition(m_position);
	m_enemyModelRender.SetRotation(m_rotation);
	m_enemyModelRender.SetScale(m_scale);

	return true;
}

void Enemy_Search::Action_CallAroundEnemy()
{
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(17);
	se->SetVolume(GameManager::GetInstance()->GetSFX() * DOWN_VOLUME);	// ���ʂ�������
	se->Play(false);

	if (GetTrackingPlayerFlag() == false) {
		// �t���O���~�낷
		SetEffectDrawFlag(false, m_EffectState_ExclamationPoint);
		SetEnemyActionState(m_ActionState_Search_MissingPlayer);
		se->Stop();
		return;
	}

	Vector3 rot = m_playerManagement->GetPosition() - m_position;
	rot.Normalize();

	// �G�l�~�[�̃��X�g������
	for (int i = 0; i < m_game->GetEnemyList().size(); i++) {

		// �������������Ƃ��͂���ȍ~�͎��s���Ȃ�
		if (m_game->GetEnemyList()[i]->GetEnemyActionState() == m_ActionState_Dizzy) {
			return;
		}

		// �e�G�l�~�[����Y���G�l�~�[�֌������x�N�g��
		Vector3 diff = m_position - m_game->GetEnemyList()[i]->GetPosition();
		float length = diff.Length();

		// ���������ȓ��̂Ƃ� ���� �܂��Ă�ł��Ȃ��Ƃ�
		if (length > CALL_DISTANCE_MIN &&
			length < CALL_DISTANCE_MAX &&
			m_game->GetEnemyList()[i]->GetEnemyActionState() != m_ActionState_Called) {

			// ���g�̍��W-�L�����R�����̍��W��ڕW�n�_�ɂ���
			Vector3 position = m_position - BOXSIZE;

			m_game->GetEnemyList()[i]->SetEnemyActionState(m_ActionState_Called);		// �s���p�^�[����ύX����
			m_game->GetEnemyList()[i]->SetGatherPosition(position);						// �ڕW�n�_��n��
		}
	}

	Rotation(rot);
	SetEnemyAnimationState(m_enAnimationState_Call);
}

void Enemy_Search::Update()
{
	// �s���ł��邩���ׂ�
	if (GetActiveFlag() == true) {
		// �C�x���g���̏���
		Vector3 move = m_position;
		move.y -= 30000.0f;
		m_characterController.Execute(move, 1.0f);
		m_spotLight.SetPosition(move);
		m_spotLight.Update();
		return;
	}

	// �`�悵�Ȃ���ԂȂ�
	if (GetNotDrawFlag() == true) {
		// �G�t�F�N�g���폜����
		if (m_Effect != nullptr) {
			m_Effect->Stop();
			DeleteGO(m_Effect);
		}
		return;
	}

	// �M���e�ɓ��������Ƃ�
	if (GetHitFlashBulletFlag() == true) {
		// ������Ԃւ̈ڍs��D�悷��
		SetEnemyActionState(m_ActionState_Dizzy);
	}

	// �s���p�^�[��
	switch (GetEnemyActionState()) {
	case m_ActionState_Craw:
	case m_ActionState_Search:
		Update_OnSearch();
		break;
	case m_ActionState_Call_AroundEnemy:
		Update_OnCallAroundEnemy();
		break;
	case m_ActionState_Search_MissingPlayer:
		Update_OnMissingPlayer();
		break;
	case m_ActionState_Dizzy:
		Update_OnDizzy();
		break;
	}

	Enemy::PlayAnimation();
	Enemy::SpotLight_Serch(m_rotation, m_position);
	Enemy::Action_SeachPlayer();

	m_characterController.SetPosition(m_position);

	m_enemyModelRender.SetPosition(m_position);
	m_enemyModelRender.SetRotation(m_rotation);
	m_enemyModelRender.Update();
}

void Enemy_Search::Update_OnSearch()
{
	Rotaition();

	// �v���C���[������p���ɑ��݂���Ƃ�
	if (GetTrackingPlayerFlag() == true) {
		SetEffectDrawFlag(false, m_EffectState_QuestionMark);
		SetEnemyActionState(m_ActionState_Call_AroundEnemy);
	}
}

void Enemy_Search::Update_OnCallAroundEnemy()
{
	Action_CallAroundEnemy();
}

void Enemy_Search::Update_OnDizzy()
{
	Enemy::Action_HitFlashBullet();
}

void Enemy_Search::Update_OnMissingPlayer()
{
	Enemy::Action_SearchMissingPlayer();
}

void Enemy_Search::Rotaition()
{
	m_rotTime -= g_gameTime->GetFrameDeltaTime();

	if (m_rotTime < 0.0f) {
		m_rotTime = KEEPTIME;

		// ��]������؂�ւ���
		switch (m_rotState)
		{
		case true:
			m_rotState = false;
			break;
		case false:
			m_rotState = true;
			break;
		default:
			break;
		}
	}

	// ��]����
	switch (m_rotState) {
	case true:
		m_rotation.AddRotationY(RODADD);
		break;
	case false:
		m_rotation.AddRotationY(-RODADD);
		break;
	default:
		break;
	}
	
	SetEnemyAnimationState(m_enAnimationState_Idle);
}

void Enemy_Search::Render(RenderContext& rc)
{
	// �`��
	if (GetNotDrawFlag() == false &&
		GetActiveFlag() == false) {
		m_enemyModelRender.Draw(rc);
	}
}