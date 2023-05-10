#include "stdafx.h"
#include "Enemy_Clear.h"

#include "GameManager.h"
namespace
{
	const float		LINEAR_COMPLETION = 0.2f;		// ���`�⊮�̃t���[����
}

Enemy_Clear::Enemy_Clear()
{

}
Enemy_Clear::~Enemy_Clear()
{
	m_clearFlag = true;
}
bool Enemy_Clear::Start()
{
	// �A�j���[�V�����̓ǂݍ���
	Animation();

	// ���f���̓ǂݍ���
	m_enemyRender.Init("Assets/modelData/enemy/enemy_clear.tkm", m_enAnimationClips, m_enAnimation_Num, enModelUpAxisZ, true, true, 2);

	Enemy::Start();

	m_enemyRender.SetScale(m_scale);
	m_enemyRender.SetPosition(m_position);
	m_enemyRender.SetRotation(m_rotation);

	// �p�X�ړ�
	m_point = &m_pointList[0];

	return true;
}
void Enemy_Clear::Update()
{
	// �`�悵�Ȃ��t���O��true�̂Ƃ�
	if (m_NotDrawFlag == true) {
		return;
	}
	// �f�t�H���g�ɖ߂��t���O��true�̂Ƃ�
	if (m_ChangeDefaultFlag == true) {
		m_ActState = CRAW;
	}

	// �M���e�ɓ�������
	if (m_HitFlashBulletFlag == true) {
		m_ActState = CONFUSION;
	}
	// �����e���g�p����
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
		// �����e���g�p�����Ƃ�
	case LISTEN:
		UpDate_OnListen();
		break;
		// �ߊl
	case CATCH:
		Update_OnCatch();
		break;
	}

	Enemy::PlayAnimation();		// �A�j���[�V����
	ClearChange();
	m_enemyRender.SetPosition(m_position);
	m_characterController.SetPosition(m_position);

	// �L�����N�^�[�R���g���[���[�����f���̈ʒu�Ɠ���
	Vector3 move = Vector3::Zero;
	m_position = m_characterController.Execute(move, g_gameTime->GetFrameDeltaTime());

	Enemy::SpotLight_Serch(m_rotation, m_position);
	// ����p
	Enemy::Act_SeachPlayer();

	m_enemyRender.Update();	// �X�V
}
void Enemy_Clear::Update_OnCraw()
{
	if (m_ChachPlayerFlag == true) {
		m_enAnimationState = IDLE;
		return;
	}

	Enemy::Act_Craw();				// ����s��

	// ����p�Ƀv���C���[������Ƃ�
	if (m_TrakingPlayerFlag == true) {
		m_ActState = TRACKING;
	}
}

void Enemy_Clear::Update_OnTracking()
{
	Enemy::Act_Tracking();			// �ǐՍs��

	// ����p�Ƀv���C���[�����Ȃ��Ƃ�
	if (m_TrakingPlayerFlag == false) {
		Enemy::Act_MissingPlayer();
	}

	// �v���C���[��߂܂����Ƃ�
	if (Act_CatchPlayer() == true) {
		m_ActState = CATCH;
	}
}

void Enemy_Clear::Update_OnCalled()
{
	Enemy::Act_Called();

	// ����p�Ƀv���C���[������Ƃ�
	if (m_TrakingPlayerFlag == true) {
		m_ActState = TRACKING;
	}
}

void Enemy_Clear::Update_OnBackBasedOn()
{
	Enemy::Act_Loss();					// �ǐՍs������̐؂�ւ�
	m_ActState = CRAW;
}

void Enemy_Clear::Update_OnConfusion()
{
	// �M���e�ɓ��������Ƃ�

	Enemy::Act_HitFlashBullet();

	// �d���������Ă���Ƃ�
	if (m_HitFlashBulletFlag == false) {
		m_ActState = BACKBASEDON;
	}
}

void Enemy_Clear::UpDate_OnListen()
{
	// �����e���g�����Ƃ�

	Enemy::Act_HitSoundBullet();

	// ���ʂ��I�������Ƃ�
	if (Enemy::GetHitSoundBullet() == false) {
		m_ActState = BACKBASEDON;
	}
}

void Enemy_Clear::Update_OnCatch()
{

	Enemy::Act_CatchPlayer();
	m_ActState = CRAW;
}

void Enemy_Clear::ClearChange()

{
	if (m_SetActState != m_ActState&&m_clearChangeTime>=0.0f)
	{
		m_clearChangeTime -= g_gameTime->GetFrameDeltaTime();
		return;
	}
	//���񒆂Ȃ瓧�����ɂ���
	if (m_ActState == CRAW)
	{
		m_clearFlag = true;
		m_SetActState = CRAW;
		m_clearChangeTime = 0.0f;
	}
	//�ǐՒ��Ȃ瓧����������
	else if (m_ActState == TRACKING)
	{
		m_clearFlag = false;
		m_SetActState = TRACKING;
		m_clearChangeTime = 1.0f;
	
	}
}

void Enemy_Clear::Render(RenderContext& rc)
{
	//����������
	if (m_clearFlag == false)
	{
		if (m_NotDrawFlag == false) {
			m_enemyRender.Draw(rc);
		}
	}
	
}