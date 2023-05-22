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

	m_enemyRender.Init("Assets/modelData/enemy/enemy_search.tkm", m_enAnimationClips, m_enAnimation_Num, enModelUpAxisZ, true, true, 2);

	Enemy::Start();

	m_enemyRender.SetPosition(m_position);
	m_enemyRender.SetRotation(m_rotation);
	m_enemyRender.SetScale(m_scale);

	return true;
}
void Enemy_Search::Update()
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

	// �M���e�ɓ�������
	if (m_HitFlashBulletFlag == true) {
		m_ActState = CONFUSION;
	}

	switch (m_ActState) {
	case CRAW:
	case SEARCH:
		Update_OnSearch();
		break;
	case CALL:
		Update_OnCall();
		break;
	case MISSING_SEARCHPLAYER:
		Update_OnMissingPlayer();
		break;
	case CONFUSION:
		Update_OnConfusion();
		break;
	}

	Enemy::PlayAnimation();

	Enemy::SpotLight_Serch(m_rotation, m_position);
	// ����p
	Enemy::Act_SeachPlayer();

	// �X�V
	m_enemyRender.SetRotation(m_rotation);
	m_enemyRender.Update();
}

void Enemy_Search::Update_OnSearch()
{
	// ���G
	Rotaition();

	// ����p��Ƀv���C���[�����݂���Ƃ�
	if (m_TrackingPlayerFlag == true) {
		// フラグを降ろす
		m_efectDrawFlag[2] = false;
		m_ActState = CALL;
	}
}

void Enemy_Search::Update_OnCall()
{
	// ����̓G��Ă�
	Enemy::Act_Call();
}

void Enemy_Search::Update_OnConfusion()
{
	// ����
	Enemy::Act_HitFlashBullet();
}

void Enemy_Search::Update_OnMissingPlayer()
{
	// プレイヤーを見失った時
	Enemy::Act_SearchMissingPlayer();
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
	
	m_enAnimationState = IDLE;	// �A�j���[�V�����X�e�[�g��ݒ�
}
void Enemy_Search::Render(RenderContext& rc)
{
	if (m_NotDrawFlag == false&&
		m_activeFlag==false) {
		m_enemyRender.Draw(rc);
	}
}