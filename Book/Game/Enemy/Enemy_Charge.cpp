#include "stdafx.h"
#include "Enemy_Charge.h"

#include "GameManager.h"

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
	// �`�悵�Ȃ��t���O��true�̂Ƃ�
	if (m_NotDrawFlag == true) {
		return;
	}
	// �f�t�H���g�ɖ߂��t���O��true�̂Ƃ�
	if(m_ChangeDefaultFlag == true) {
		m_ActState = CRAW;
		m_ChangeDefaultFlag = false;
	}


	if (m_HitSoundBulletFlag == true && m_HitFlashBulletFlag == true) {
		m_HitSoundBulletFlag = false;
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
		// �ːi
	case CHARGE:
		Update_OnCharge();
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
	// ����

	// �v���C���[��߂܂����Ƃ�
	if (m_ActState == CATCH) {
		m_enAnimationState = IDLE;
		return;
	}

	Enemy::Act_Craw();					// ����s��

	// ����p�Ƀv���C���[������Ƃ�
	if (m_TrakingPlayerFlag == true) {
		m_ActState = CHARGE;
	}
}

void Enemy_Charge::Update_OnCharge()
{
	Enemy::Act_Charge_HitWall();		// �ǂƂ̏Փ˔���
	Enemy::Act_Charge(STOP_TIMER);		// �ːi�U��
										// �֐����ŏ����Ԃɖ߂鏈�����L�q

	// �v���C���[��߂܂����Ƃ�
	if (Enemy::Act_CatchPlayer() == true) {
		m_ActState = CATCH;
	}
}

void Enemy_Charge::Update_OnBackBasedOn()
{
	// �ːi�ˏ���ւ̐؂�ւ�
	Enemy::Act_Loss();					// �ǐՍs������̐؂�ւ�
	m_ActState = CRAW;
}

void Enemy_Charge::Update_OnCalled()
{
	Enemy::Act_Called();

	// ����p�Ƀv���C���[������Ƃ�
	if (m_TrakingPlayerFlag == true) {
		m_ActState = CHARGE;
	}
}

void Enemy_Charge::Update_OnConfusion()
{
	// �M���e�ɓ��������Ƃ�

	Enemy::Act_HitFlashBullet();

	// �M���e�ɓ������Ă��Ȃ��Ƃ�
	if (Enemy::GetHitFlushBullet() == false) {
		m_ActState = BACKBASEDON;
	}
}

void Enemy_Charge::UpDate_OnListen()
{
	// �����e���g�����Ƃ�

	// �M���e��H����Ă���Ƃ��͎��s���Ȃ�
	if (m_HitFlashBulletFlag == true) {
		m_HitFlashBulletFlag = false;
		return;
	}


	// ���ʂ��I�������Ƃ�
	if (m_HitSoundBulletFlag == true) {
		m_ActState = BACKBASEDON;
	}
	// ���ʂ������������Ƃ�
	else {
		m_ActState = CRAW;
	}
}

void Enemy_Charge::Update_OnCatch()
{

	Enemy::Act_CatchPlayer();

	m_ActState = CRAW;
}

void Enemy_Charge::Render(RenderContext& rc)
{
	if (m_NotDrawFlag == false) {
		m_enemyRender.Draw(rc);
	}
}