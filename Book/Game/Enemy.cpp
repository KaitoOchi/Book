#include "stdafx.h"
#include "Enemy.h"

#include "PlayerManagement.h"

#define FIELDOF_VIEW Math::PI / 180.0f) * 120.0f				// �G�l�~�[�̎���p(�����l120)

namespace
{
	const float		MOVE_SPEED = 3.0f;						// �ړ����x
	const float		CHANGING_DISTANCE = 20.0f;				// �ړI�n��ύX���鋗��
	const float		CANMOVE_TIMER = 10.0f;					// �ēx�s���ł���܂ł̃^�C�}�[
	const float		STOP_TIMER = 12.0f;						// �p�X�ړ����̑ҋ@����
	const float		CATCH_DECISION = 20.0f;					// �v���C���[���m�ۂ������ƂɂȂ�͈�
	const float		ACCESS_DECISION = 40.0f;				// �v���C���[�ɋ߂Â��͈�
	const float		SCALESIZE = 1.3f;						// SetScale�̃T�C�Y
	const Vector3	BOXSIZE = { 75.0f, 90.0f,60.0f };		// CharacterController�̃T�C�Y
}

Enemy::Enemy()
{
}

Enemy::~Enemy()
{

}

bool Enemy::Start()
{
	// �L�����N�^�[�R���g���[���[������������
	m_characterController.Init(BOXSIZE, m_position);

	// �C���X�^���X��T��
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");

	return true;
}

bool Enemy::SeachPlayer()
{
	// ����p�̏���
	// true�̂Ƃ��v���C���[�𔭌����Ă���

	// �G�l�~�[����v���C���[�֌������x�N�g��
	Vector3 diff = m_playerManagement->GetPosition() - m_position;

	// �v���C���[�ɂ�����x�߂��Ƃ�
	if (diff.LengthSq() <= 700.0 * 700.0f) {
		// �G�l�~�[����v���C���[�֌������x�N�g���𐳋K��
		diff.Normalize();
		// �G�l�~�[�̐��ʃx�N�g���A�G�l�~�[����v���C���[�֌������x�N�g���̓��ς��v�Z
		float cos = m_forward.Dot(diff);
		// ���ς���p�x���v�Z
		float angle = acosf(cos);
		// �p�x������p��苷���Ƃ�
		if (angle <= (FIELDOF_VIEW) {
			// �v���C���[�𔭌�
			return true;
		}
	}

	return false;
}

bool Enemy::CatchPlayer()
{
	// �v���C���[���m�ۂ��鏈��
	// true�̂Ƃ��v���C���[���m�ۂ��Ă���
	bool flag = false;

	// �G�l�~�[����v���C���[�֌������x�N�g�����v�Z����
	Vector3 diff = m_playerManagement->GetPosition() - m_position;
	// �x�N�g���̒��������߂�
	float length = diff.Length();

	// �x�N�g�������ȉ��̂Ƃ�
	if (length <= CATCH_DECISION) {
		// �߂܂��鏈�����s��
		// �߂܂����t���O��true�ɂ���
		m_fontRender.SetText(L"�߂܂���");
		m_fontRender.SetPosition({ 500.0f, 200.0f, 0.0f });
		return true;
	}

	return false;
}

bool Enemy::HitFlashBullet()
{
	// �M���e�����������Ƃ�
	// true�Ȃ瓖������
	if (HitFlashBulletFlag == true) {
		// �ړ����d��
		m_position = m_position;

		return true;
	}

	return false;
}

void Enemy::Act_Craw()
{
	// �p�X�ړ�
	
	// �ڕW�Ƃ���|�C���g�̍��W����A���݂̍��W���������x�N�g��
	Vector3 diff = m_point->s_position - m_position;

	// ���������ȓ��Ȃ�ړI�n�Ƃ���|�C���g��ύX����
	if (diff.Length() <= CHANGING_DISTANCE) {

		// ���݂̖ړI�n�̃|�C���g���z��̍Ō�̂Ƃ�
		if (m_point->s_number == m_pointList.size()) {
			// ��ԍŏ��̃|�C���g��ړI�n�Ƃ���
			m_point = &m_pointList[0];
		}
		// �����łȂ��Ƃ�
		else {
			m_point = &m_pointList[m_point->s_number];
		}
	}

	// �ڕW�Ƃ���|�C���g�̍��W����A���݂̍��W���������x�N�g��
	Vector3 moveSpeed = m_point->s_position - m_position;
	// ���K��
	moveSpeed.Normalize();
	// �x�N�g���ɃX�J���[����Z
	moveSpeed *= MOVE_SPEED;
	// ���W�ɉ��Z����
	m_position += moveSpeed;

	Enemy::SeachPlayer();
}

void Enemy::Act_Tracking()
{
	// �i�r���b�V���ł̈ړ�


	Enemy::CatchPlayer();
}

void Enemy::Act_Access()
{
	// �G�l�~�[����v���C���[�֌������x�N�g��
	Vector3 diff = m_playerManagement->GetPosition() - m_position;
	// �x�N�g���̒���
	float length = diff.Length();

	// �x�N�g�������ȉ��̂Ƃ�
	if (length <= ACCESS_DECISION) {
		// �x�N�g���𐳋K��
		diff.Normalize();
		m_position += diff * MOVE_SPEED;
	}
}

void Enemy::Act_Confuion()
{
	// �M���e������������̍d������
	
	// �o�ߎ��Ԃ����Z
	float time = 0.0f;
	time += g_gameTime->GetFrameDeltaTime();

	// ���Z���ꂽ���Ԃ����ȏ�ɂȂ����Ƃ�
	if (CANMOVE_TIMER <= time) {
		return;
	}
}