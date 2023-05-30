#include "stdafx.h"
#include "Enemy_Charge.h"

#include "GameManager.h"
#include "PlayerManagement.h"
#include "Gage.h"

namespace 
{
	const float		LINEAR_COMPLETION = 0.2f;		// ���`�⊮�̃t���[����
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
	Animation();

	// ���f���̓ǂݍ���
	m_enemyRender.Init("Assets/modelData/enemy/enemy_charge.tkm", m_enAnimationClips, m_enAnimation_Num, enModelUpAxisZ, true, true, 2);

	Enemy::Start();

	m_enemyRender.SetScale(m_scale);
	m_enemyRender.SetPosition(m_position);
	m_enemyRender.SetRotation(m_rotation);

	// �p�X�ړ�
	m_point = &m_pointList[0];

	return true;
}

void Enemy_Charge::Update()
{
	//�s���ł��邩���ׂ�
	if (m_activeFlag == true)
	{
		Vector3 move{0.0f,-3000.0f,0.0f};
		m_characterController.Execute(move, 1.0f);
		m_spotLight.SetPosition(move);
		m_spotLight.Update();
		return;
	}

	// �`�悵�Ȃ��t���O��true�̂Ƃ�
	if (m_NotDrawFlag == true) {
		if (m_Effect != nullptr) {
			m_Effect->Stop();
			DeleteGO(m_Effect);
		}
		return;
	}

	// �v���C���[��߂܂����Ƃ�
	if (m_ActState == CATCH) {
		m_enAnimationState = IDLE;
		return;
	}

	if (m_HearedSoundBulletFlag == true && m_HitFlashBulletFlag == true) {
		m_HearedSoundBulletFlag = false;
	}

	// �M���e�ɓ�������
	if (m_HitFlashBulletFlag == true) {
		m_ActState = CONFUSION;
	}
	// �����e���g�p����
	if (m_HearedSoundBulletFlag == true) {
		m_ActState = LISTEN;
	}

	switch (m_ActState) {
		// ����
	case CRAW:
		Update_OnCraw();
		break;
		// �ːi
	case CHARGE:
		Update_OnCharge();
		break;
		// �ːi�I��
	case CHARGEEND:
		Update_OnChargeEnd();
		break;
		// �v���C���[��T��
	case MISSING_SEARCHPLAYER:
		Update_OnSearchMissingPlayer();
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
	case EVENT:
		Update_OnEvent();
		break;
	}

	Enemy::PlayAnimation();		// �A�j���[�V����

	// �X�V
	m_enemyRender.SetPosition(m_position);
	m_characterController.SetPosition(m_position);

	// �L�����N�^�[�R���g���[���[�����f���̈ʒu�Ɠ���
	Vector3 move = Vector3::Zero;
	m_position = m_characterController.Execute(move, g_gameTime->GetFrameDeltaTime());

	// �X�|�b�g���C�g
	Enemy::SpotLight_Serch(m_rotation, m_position);
	// ����p
	Enemy::Act_SeachPlayer();
	m_enemyRender.Update();
}

void Enemy_Charge::Update_OnCraw()
{
	// �v���C���[��߂܂����Ƃ�
	if (Act_CatchPlayer() == true) {
		m_ActState = CATCH;
	}

	Enemy::Act_Craw();					// ����s��
}

void Enemy_Charge::Update_OnCharge()
{
	// �v���C���[��߂܂����Ƃ�
	if (Enemy::Act_CatchPlayer() == true) {
		m_ActState = CATCH;
	}

	Enemy::Act_Charge(STOP_TIMER);		// �ːi�U��
										// �֐����ŏ����Ԃɖ߂鏈�����L�q
}

void Enemy_Charge::Update_OnChargeEnd()
{
	Enemy::Act_ChargeEnd();		// �ːi����߂�
}

void Enemy_Charge::Update_OnSearchMissingPlayer()
{
	// �v���C���[��T��
	Enemy::Act_SearchMissingPlayer();
}

void Enemy_Charge::Update_OnBackBasedOn()
{
	// �ːi�ˏ���ւ̐؂�ւ�
	Enemy::Act_Loss();					// �ǐՍs������̐؂�ւ�
}

void Enemy_Charge::Update_OnCalled()
{
	Enemy::Act_GoLocationListenSound(m_setPos);
}

void Enemy_Charge::Update_OnConfusion()
{
	Enemy::Act_HitFlashBullet();
}

void Enemy_Charge::UpDate_OnListen()
{
	// �����e���g�����Ƃ�
	Enemy::Act_GoLocationListenSound(m_itemPos);
}

void Enemy_Charge::Update_OnCatch()
{
	Enemy::Act_CatchPlayer();
}

void Enemy_Charge::Render(RenderContext& rc)
{
	if (m_NotDrawFlag == false&&
		m_activeFlag == false) {
		m_enemyRender.Draw(rc);
	}
}