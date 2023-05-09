#include "stdafx.h"
#include "Enemy_Search.h"

#include "GameManager.h"
namespace
{
	const float KEEPTIME = 2.0f;
	
	const float ANGLE = 45.0f;
	const float RODADD = 0.01f;
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

	m_enemyRender.Init("Assets/modelData/enemy/enemy_search.tkm", m_enAnimationClips, m_enAnimation_Num, enModelUpAxisZ, true, true, 2);

	Enemy::Start();

	m_enemyRender.SetPosition(m_position);
	m_enemyRender.SetRotation(m_rotation);
	m_enemyRender.SetScale(m_scale);

	m_ActState = SEARCH;		// �s���p�^�[����ݒ�
	m_enAnimationState = IDLE;	// �A�j���[�V�����X�e�[�g��ݒ�

	return true;
}
void Enemy_Search::Update()
{
	switch (m_ActState) {
	case SEARCH:
		Update_OnSearch();
		break;
	case CALL:
		Update_OnCall();
		break;
	case CALLEND:
		Update_OnCallEnd();
		break;
	case CONFUSION:
		Update_OnConfusion();
		break;
		// �f�t�H���g�ɖ߂�
	case DEFAULT:
		m_ActState = CRAW;
		break;
	case NOOP:
		return;
		break;
	}

	// �M���e�ɓ�������
	if (m_HitFlashBulletFlag == true) {
		m_ActState = CONFUSION;
	}
	// �����e���g�p����
	if (m_HitSoundBulletFlag == true) {
		m_ActState = LISTEN;
	}

	Enemy::SpotLight_Serch(m_rot, m_position);
	// ����p
	Enemy::Act_SeachPlayer();

	// �X�V
	m_enemyRender.SetRotation(m_rot);
	m_enemyRender.Update();
}

void Enemy_Search::Update_OnSearch()
{
	// ���G
	Rotaition();

	// ����p���Ƀv���C���[�����݂���Ƃ�
	if (m_TrakingPlayerFlag == true) {
		m_ActState = CALL;
	}
}

void Enemy_Search::Update_OnCall()
{
	// ����̓G���Ă�
	Enemy::Act_Call();

	// ����p���Ƀv���C���[�����݂��Ȃ��Ƃ�
	if (m_TrakingPlayerFlag == false) {
		m_ActState = CALLEND;
	}
}

void Enemy_Search::Update_OnCallEnd()
{
	Rotaition();
	Enemy::Act_CallEnd();

	// �G�l�~�[�����ɖ߂�
	if (Enemy::Act_CallEnd() == true) {
		m_ActState = SEARCH;
	}
}

void Enemy_Search::Update_OnConfusion()
{
	// ����
	Enemy::Act_HitFlashBullet();

	// �d���������Ă���Ƃ�
	if (Enemy::GetHitFlushBullet() == false) {
		m_ActState = SEARCH;
	}
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
		m_rot.AddRotationY(RODADD);
		break;
	case false:
		m_rot.AddRotationY(-RODADD);
		break;
	default:
		break;
	}
	
	
}
void Enemy_Search::Render(RenderContext& rc)
{
	if (m_ActState != NOOP) {
		m_enemyRender.Draw(rc);
	}
}