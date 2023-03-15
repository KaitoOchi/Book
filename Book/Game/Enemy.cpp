#include "stdafx.h"
#include "Enemy.h"

#include "PlayerManagement.h"

#define FIELDOF_VIEW Math::PI / 180.0f) * 120.0f				// �G�l�~�[�̎���p(�����l120)

namespace
{
	const float		MOVE_SPEED = 8.0f;						// �ړ����x
	const float		CANMOVE_TIMER = 10.0f;					// �ēx�s���ł���܂ł̃^�C�}�[
	const float		CATCH_DECISION = 20.0f;					// �v���C���[���m�ۂ������ƂɂȂ�͈�
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

	return start;
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
		return true;
	}

	return false;
}

void Enemy::HitFlashBullet()
{
	// �M���e�����������Ƃ�
	if (HitFlashBulletFlag == true) {
		// �ړ����d��
		m_position = m_position;

		HitAfterFlashBullet();
	}
}

void Enemy::HitAfterFlashBullet()
{
	// �M���e������������̍d������
	// �o�ߎ��Ԃ����Z
	float time = +g_gameTime->GetFrameDeltaTime();

	// ���Z���ꂽ���Ԃ����ȏ�ɂȂ����Ƃ�
	if (CANMOVE_TIMER <= time) {
		// ���Z����o�ߎ��Ԃ����Z�b�g
		time = 0;
		return;
	}
}

//void Enemy::Act()
//{
//	// �s���p�^�[��
//	switch (m_enEnemyActState) {
//		// ����
//	case Enemy::m_enEnemyActState_Craw:
//		break;
//		// �ǐ�
//	case Enemy::m_enEnemyActState_Tracking:
//		break;
//	}
//}