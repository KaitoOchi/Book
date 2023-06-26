#include "stdafx.h"
#include "Enemy_Search.h"

#include "GameManager.h"
namespace
{
	const float KEEPTIME = 2.0f;
	
	const float ANGLE = 45.0f;
	const float RODADD = 0.01f;

	const float SEARCHPLAYER_TIMER = 7.0f;
}
Enemy_Search::Enemy_Search()
{

}
Enemy_Search::~Enemy_Search()
{
}
bool Enemy_Search::Start()
{
	// �A�j���[�V�����̓ǂݍ���
	Animation();

	// ���f���̓ǂݍ���
	m_enemyRender.Init("Assets/modelData/enemy/enemy_search.tkm", m_enAnimationClips, m_enAnimation_Num, enModelUpAxisZ, true, true, 2);

	Enemy::Start();

	m_enemyRender.SetPosition(m_position);
	m_enemyRender.SetRotation(m_rotation);
	m_enemyRender.SetScale(m_scale);

	return true;
}
void Enemy_Search::Update()
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

	// �M���e�ɓ��������Ƃ�
	if (GetHitFlashBulletFlag() == true) {
		SetEnemyActionState(m_ActionState_Dizzy);
	}

	switch (GetEnemyActionState()) {
	case m_ActionState_Craw:
	case m_ActionState_Search:
		// ���G
		Update_OnSearch();
		break;
	case m_ActionState_Call_AroundEnemy:
		// ����̃G�l�~�[���Ă�
		Update_OnCallAroundEnemy();
		break;
	case m_ActionState_Search_MissingPlayer:
		// ���������v���C���[��T��
		Update_OnMissingPlayer();
		break;
	case m_ActionState_Dizzy:
		// �M���e���g�p���ꂽ�Ƃ�
		Update_OnDizzy();
		break;
	}

	Enemy::PlayAnimation();							// �A�j���[�V����

	Enemy::SpotLight_Serch(m_rotation, m_position);	// �X�|�b�g���C�g
	Enemy::Action_SeachPlayer();					// ���G

	m_characterController.SetPosition(m_position);

	m_enemyRender.SetPosition(m_position);
	m_enemyRender.SetRotation(m_rotation);
	m_enemyRender.Update();
}

void Enemy_Search::Update_OnSearch()
{
	Rotaition();

	if (GetTrackingPlayerFlag() == true) {
		SetEffectDrawFlag(false, 2);
		SetEnemyActionState(m_ActionState_Call_AroundEnemy);
	}
}

void Enemy_Search::Update_OnCallAroundEnemy()
{
	// ����ɂ���G�l�~�[���Ă�
	Enemy::Action_CallAroundEnemy();
}

void Enemy_Search::Update_OnDizzy()
{
	// �M���e�ɓ��������Ƃ�
	Enemy::Action_HitFlashBullet();
}

void Enemy_Search::Update_OnMissingPlayer()
{
	// �v���C���[������������
	Enemy::Action_SearchMissingPlayer();
}

void Enemy_Search::Rotaition()
{
	m_rotTime -= g_gameTime->GetFrameDeltaTime();
	if (m_rotTime < 0.0f)
	{
		m_rotTime = KEEPTIME;
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
	switch (m_rotState)
	{
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
	if (GetNotDrawFlag() == false &&
		GetActiveFlag() == false) {
		m_enemyRender.Draw(rc);
	}
}