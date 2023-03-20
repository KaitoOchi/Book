#include "stdafx.h"
#include "Enemy.h"

#include "PlayerManagement.h"

#define FIELDOF_VIEW Math::PI / 180.0f) * 120.0f			// �G�l�~�[�̎���p(����:120)

namespace
{
	const float		MOVE_SPEED = 3.0f;						// �ړ����x
	const float		CHANGING_DISTANCE = 20.0f;				// �ړI�n��ύX���鋗��
	const float		CALCULATIONNAVI_TIMER = 1.0f;			// �i�r���b�V����ēx�v�Z����^�C�}�[
	const float		CANMOVE_TIMER = 10.0f;					// �ēx�s���ł���܂ł̃^�C�}�[
	const float		WAITING_TIMER = 3.0f;					// �p�X�ړ����̑ҋ@����
	const float		AI_RADIUS = 50.0f;						// AI�G�[�W�F���g�̔��a
	const float		AI_HIGH = 200.0f;						// AI�G�[�W�F���g�̍���
	const float		CATCH_DECISION = 20.0f;					// �v���C���[��m�ۂ������ƂɂȂ�͈�
	const float		ACCESS_DECISION = 40.0f;				// �v���C���[�ɋ߂Â��͈�
	const float		SCALESIZE = 1.3f;						// SetScale�̃T�C�Y
	const Vector3	BOXSIZE = { 75.0f, 90.0f,60.0f };		// CharacterController�̃T�C�Y
	const float		ANGLE = 45.0f;							//��]�p�x
	const Vector3   LIGHTCOLOR(100.0f, 1.0f, 1.0f);			//���C�g�̃J���[
	const float		LIGHTRANGE = 300.0f;						//���C�g�̉e���͈�
	const float		LIGHTPOSITION = 40.0f;						//���C�g�̃|�W�V����
}
Enemy::Enemy()
{
}

Enemy::~Enemy()
{

}

bool Enemy::Start()
{
	// �L�����N�^�[�R���g���[���[�����������
	m_characterController.Init(BOXSIZE, m_position);

	// �i�r���b�V����\�z
	m_nvmMesh.Init("Assets/nvm/nvm1.tkn");

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
		// �G�l�~�[�̐��ʃx�N�g���A�G�l�~�[����v���C���[�֌������x�N�g���̓�ς�v�Z
		float cos = m_forward.Dot(diff);
		// ��ς���p�x��v�Z
		float angle = acosf(cos);
		// �p�x������p��苷���Ƃ�
		if (angle <= (FIELDOF_VIEW) {
			// �v���C���[�𔭌�
			m_rotation.SetRotationY(angle);
			return true;
		}
	}

	return false;
}

bool Enemy::CatchPlayer()
{
	// �v���C���[��m�ۂ��鏈��
	// true�̂Ƃ��v���C���[��m�ۂ��Ă���
	bool flag = false;

	// �G�l�~�[����v���C���[�֌������x�N�g����v�Z����
	Vector3 diff = m_playerManagement->GetPosition() - m_position;
	// �x�N�g���̒�������߂�
	float length = diff.Length();

	// �x�N�g�������ȉ��̂Ƃ�
	if (length <= CATCH_DECISION) {
		// �߂܂��鏈����s��
		// �U���A�j���[�V������Đ�
		m_enEnemyAnimationState = m_enEnemyAnimationState_Attack;

		return true;
	}

	return false;
}

void Enemy::HitFlashBullet()
{
	// �M���e�����������Ƃ�
	// true�Ȃ瓖������
	if (HitFlashBulletFlag == true) {
		// ��e�A�j���[�V������Đ�
		m_enEnemyAnimationState = m_enEnemyAnimationState_Damege;

		// �^�C�}�[��true�̂Ƃ�
		if (Act_Stop(CANMOVE_TIMER) == true) {
			HitFlashBulletFlag = false;		// �t���O��~�낷
			addTimer = 0.0f;				// ���Z�p�^�C�}�[����Z�b�g

		}
		// �����łȂ��Ƃ�
		else {
			// �ҋ@�A�j���[�V������Đ�
			m_enEnemyAnimationState = m_enEnemyAnimationState_Idle;
		}
	}
}

void Enemy::Act_Craw()
{
	// �p�X�ړ�
	
	// �ڕW�Ƃ���|�C���g�̍��W����A���݂̍��W��������x�N�g��
	Vector3 diff = m_point->s_position - m_position;

	// ���������ȓ�Ȃ�ړI�n�Ƃ���|�C���g��ύX����
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

		addTimer = 0.0f;	// ���Z�p�^�C�}�[����Z�b�g
	}

	// �ڕW�Ƃ���|�C���g�̍��W����A���݂̍��W��������x�N�g��
	Vector3 moveSpeed = m_point->s_position - m_position;
	// ���K��
	moveSpeed.Normalize();
	// �x�N�g���ɃX�J���[���Z
	moveSpeed *= MOVE_SPEED;

	// �^�C�}�[��true�̂Ƃ�
	if (Act_Stop(WAITING_TIMER) == true) {
		// �ҋ@�A�j���[�V������Đ�
		m_enEnemyAnimationState = m_enEnemyAnimationState_Walk;
		// ���W�ɉ��Z����
		m_position += moveSpeed;
	}
	// �����łȂ��Ƃ�
	else {
		// ����A�j���[�V������Đ�
		m_enEnemyAnimationState = m_enEnemyAnimationState_Idle;
	}
}

void Enemy::Act_Tracking()
{
	// �o�ߎ��Ԃ���Z
	NaviTimer += g_gameTime->GetFrameDeltaTime();

	// ��莞�Ԉȉ��̂Ƃ�return
	if (CALCULATIONNAVI_TIMER >= NaviTimer) {
		return;
	}

	// �i�r���b�V���ł̈ړ�
	// �v���C���[�̍��W��l������
	m_playerPos = m_playerManagement->GetPosition();

	bool isEnd;							// �p�X�ړ����I�������ۂ̃t���O

	// �p�X����
	m_pathFiding.Execute(
		m_path,							// �\�z���ꂽ�p�X�̊i�[��
		m_nvmMesh,						// �i�r���b�V��
		m_position,						// �J�n���W
		m_playerPos,					// �ڕW�n�_
		PhysicsWorld::GetInstance(),	// �����G���W��
		AI_RADIUS,						// AI�G�[�W�F���g�̔��a
		AI_HIGH							// AI�G�[�W�F���g�̍���
	);

	// �p�X���ړ�����
	m_position = m_path.Move(
		m_position,						// �ړ���������W
		MOVE_SPEED,						// �ړ����x
		isEnd							// �I������true��i�[����t���O
	);

	// �߂܂����Ƃ�
	if (CatchPlayer() == true) {
		// �ҋ@�A�j���[�V������Đ�
		m_enEnemyAnimationState = m_enEnemyAnimationState_Walk;

		m_fontRender.SetText(L"�߂܂���");
		m_fontRender.SetPosition({ 500.0f, 200.0f, 0.0f });
	}
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
		// �G�l�~�[�̍��W�ɉ��Z
		m_position += diff * MOVE_SPEED;
		// ����A�j���[�V������Đ�
		m_enEnemyAnimationState = m_enEnemyAnimationState_Idle;
	}

	// �߂܂����Ƃ�
	if (CatchPlayer() == true) {
		// �ҋ@�A�j���[�V������Đ�
		m_enEnemyAnimationState = m_enEnemyAnimationState_Walk;

		m_fontRender.SetText(L"�߂܂���");
		m_fontRender.SetPosition({ 500.0f, 200.0f, 0.0f });
	}
}

bool Enemy::Act_Stop(float time)
{
	// �o�ߎ��Ԃ���Z
	addTimer += g_gameTime->GetFrameDeltaTime();

	// ���Z���ꂽ���Ԃ����ȏ�ɂȂ����Ƃ�
	if (time <= addTimer) {
		return true;
	}

	return false;
}

void Enemy::SpotLight_New(Vector3 position)
{
	m_spotLight.SetPosition(position);
	m_spotLight.SetColor(LIGHTCOLOR);
	m_spotLight.SetRange(LIGHTRANGE);
	m_spotLight.SetAngle(ANGLE);
	Vector3 forward = Vector3::AxisY;
	//ライトの方向設定
	m_spotLight.SetDirection(forward);
	m_spotLight.Update();
}
void Enemy::SpotLight_Serch(Quaternion lightrotaition, Vector3 lightpos)
{
	lightpos.y = LIGHTPOSITION;
	//Y軸
	Vector3 m_Yup = Vector3(0.0f, 1.0f, 0.0f);
	//プレイヤーの正面
	Vector3 m_front = Vector3(0.0f, 0.0f, 1.0f);
	lightrotaition.Apply(m_front);
	//その二つの垂直なベクトル
	Vector3 m_vertical = Cross(m_Yup, m_front);
	Quaternion m_SitenRot;
	//その垂直なベクトルを元にクォータニオンを作る
	m_SitenRot.SetRotationDeg(m_vertical, ANGLE);
	//ベクトルにクォータニオンを加算する
	m_SitenRot.Apply(m_front);
	m_spotLight.SetDirection(m_front);

	if (m_spotLight.IsHit(m_playerManagement->GetPosition()) == true)
	{
		//ステートの遷移
		int a = 0;
	}
	m_spotLight.SetPosition(lightpos);
	m_spotLight.Update();
}