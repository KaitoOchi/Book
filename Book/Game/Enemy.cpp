#include "stdafx.h"
#include "Enemy.h"

#include "PlayerManagement.h"

#define FIELDOF_VIEW Math::PI / 180.0f) * 75.0f				// �G�l�~�[�̎���p(����:120)
#define SEACH_DECISION 200.0f * 200.0f						// �x�N�g����쐬����͈�

namespace
{
	const float		MOVE_SPEED = 3.0f;						// �ړ����x
	const float		MOVING_DISTANCE = 200.0f;				// �ړ�����
	const float		CHANGING_DISTANCE = 20.0f;				// �ړI�n��ύX���鋗��
	const float		CALCULATIONNAVI_TIMER = 1.0f;			// �i�r���b�V����ēx�v�Z����^�C�}�[
	const float		CANMOVE_TIMER = 10.0f;					// �ēx�s���ł���܂ł̃^�C�}�[
	const float		WAITING_TIMER = 3.0f;					// �p�X�ړ����̑ҋ@����
	const float		AI_RADIUS = 50.0f;						// AI�G�[�W�F���g�̔��a
	const float		AI_HIGH = 200.0f;						// AI�G�[�W�F���g�̍���
	const float		CATCH_DECISION = 60.0f;					// �v���C���[��m�ۂ������ƂɂȂ�͈�
	const float		ACT_LIMIT = 100.0f;						// �v���C���[�ɋ߂Â���͈�
	const float		SCALESIZE = 1.3f;						// SetScale�̃T�C�Y
	const Vector3	BOXSIZE = { 50.0f, 80.0f,50.0f };		// CharacterController�̃T�C�Y
	const float		ANGLE = 45.0f;							//??]?p?x
	const Vector3   LIGHTCOLOR(100.0f, 1.0f, 1.0f);			//???C?g??J???[
	const float		LIGHTRANGE = 300.0f;						//???C?g??e?????
	const float		LIGHTPOSITION = 40.0f;						//???C?g??|?W?V????
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
	// �X�t�B�A�R���C�_�[�������
	m_sphereCollider.Create(1.0f);

	// �i�r���b�V����\�z
	m_nvmMesh.Init("Assets/nvm/nvm1.tkn");

	// �C���X�^���X��T��
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");

	return true;
}

void Enemy::Rotation(Vector3 rot)
{
	// ��]����
	m_rotation.SetRotationYFromDirectionXZ(rot);
	m_enemyRender.SetRotation(m_rotation);
}

bool Enemy::Act_SeachPlayer()
{
	// ����p�̏���
	// true�̂Ƃ��v���C���[�𔭌����Ă���

	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

	m_playerPos = m_playerManagement->GetPosition();

	// �G�l�~�[����v���C���[�֌������x�N�g��
	Vector3 diff = m_playerPos - m_position;

	// �v���C���[�ɂ�����x�߂��Ƃ�
	if (FindPlayerFlag == false) {
		if (diff.LengthSq() <= SEACH_DECISION) {
			// �G�l�~�[����v���C���[�֌������x�N�g���𐳋K��
			diff.Normalize();
			// �G�l�~�[�̐��ʃx�N�g���A�G�l�~�[����v���C���[�֌������x�N�g���̓�ς�v�Z
			float cos = m_forward.Dot(diff);
			// ��ς���p�x��v�Z
			float angle = acosf(cos);
			// �p�x������p��苷���Ƃ�
			if (angle <= (FIELDOF_VIEW) {
				// �ǂƂ̏Փ˔����s��
				
				//// �ǂƂ̏Փ˔��肪true�̂Ƃ�
				//else if (WallAndHit(m_playerPos) == true) {
				//	FindPlayerFlag = true;
				//}

				return WallAndHit(m_playerPos);
			}
		}
	}
	//// �v���C���[�𔭌����Ă���Ƃ�
	//else if (FindPlayerFlag == true) {
	//	// �G�l�~�[����v���C���[�֌������x�N�g���𐳋K��
	//	diff.Normalize();
	//	// �G�l�~�[�̐��ʃx�N�g���A�G�l�~�[����v���C���[�֌������x�N�g���̓�ς�v�Z
	//	float cos = m_forward.Dot(diff);
	//	// ��ς���p�x��v�Z
	//	float angle = acosf(cos);
	//	// �p�x������p��苷���Ƃ�
	//	if (angle <= (FIELDOF_VIEW) {
	//		// �ǂƂ̏Փ˔����s��
	//		// �ǂƂ̏Փ˔��肪false�̂Ƃ�
	//		if (WallAndHit(m_playerPos) == false) {
	//			ChangeFlag = true;
	//			FindPlayerFlag = false;
	//		}

	//		return WallAndHit(m_playerPos);
	//	}
	//}

	return false;
}

// �Փ˂����ۂɌĂ΂��֐��I�u�W�F�N�g(�Ǘp)
struct SweepResultWall :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;		// �Փ˃t���O

	virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		// �ǂƏՓ˂��Ă��Ȃ��Ƃ�
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall) {
			// �Փ˂����͕̂ǂł͂Ȃ�
			return 0.0f;
		}

		// �ǂƏՓ˂����Ƃ�
		// �t���O��true�ɂ���
		isHit = true;
		return 0.0f;
	}
};

bool Enemy::WallAndHit(Vector3 pos)
{
	// ��Q����l����������p�̏���
	// �v���C���[�𔭌������Ƃ�true��Ԃ�

	btTransform start, end;

	start.setIdentity();
	end.setIdentity();

	// �n�_�̓G�l�~�[�̍��W
	start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
	// �I�_�̓v���C���[�̍��W
	end.setOrigin(btVector3(pos.x, pos.y + 70.0f, pos.z));

	SweepResultWall callback;

	// �R���C�_�[��n�_����I�_�܂œ������āA
	// �Փ˂��邩�ǂ����𒲂ׂ�
	PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);

	// �ǂƏՓ˂���
	if (callback.isHit == true) {
		// �v���C���[�͌������Ă��Ȃ��̂�false
		return false;
	}

	// �ǂƏՓ˂��Ă��Ȃ��̂�true
	return true;
}

bool Enemy::Act_CatchPlayer()
{
	// �v���C���[��m�ۂ��鏈��
	// true�̂Ƃ��v���C���[��m�ۂ��Ă���

	// �G�l�~�[����v���C���[�֌������x�N�g����v�Z����
	Vector3 diff = m_playerManagement->GetPosition() - m_position;
	// �x�N�g���̒�������߂�
	float length = diff.Length();

	// �x�N�g�������ȉ��̂Ƃ�
	if (length <= CATCH_DECISION) {
		// �߂܂��鏈����s��
		return true;
	}

	return false;
}

void Enemy::Act_HitFlashBullet()
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
	Rotation(moveSpeed);

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

	//// �ڕW�Ƃ���|�C���g�̍��W����A���݂̍��W��������x�N�g��
	Vector3 moveSpeed = m_playerPos - m_position;

	float angle = atan2(-moveSpeed.x, moveSpeed.z);
	Quaternion rot = Quaternion::Identity;
	rot.SetRotationY(-angle);

	// ��]�������
	m_enemyRender.SetRotation(rot);

	// ��s�A�j���[�V������Đ�
	m_enEnemyAnimationState = m_enEnemyAnimationState_Walk;
}

void Enemy::Pass(int PassState)
{
	switch (PassState)
	{
		// �c
	case LINE_VERTICAL:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z - 500.0f),2 });
		break;
		// ��
	case LINE_HORIZONTAL:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x + 500.0f,m_position.y,m_position.z),2 });
		break;
		// �E���(�����`)
	case SQUARE_RIGHT:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x - 500.0f,m_position.y,m_position.z),2 });
		m_pointList.push_back({ Vector3(m_position.x - 500.0f,m_position.y,m_position.z - 500.0f),3 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z - 500.0f),4 });
		break;
		// �����(�����`)
	case SQUARE_LEFT:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x + 500.0f,m_position.y,m_position.z),2 });
		m_pointList.push_back({ Vector3(m_position.x + 500.0f,m_position.y,m_position.z - 500.0f),3 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z - 500.0f),4 });
		break;
		// (�E��)���p
	case ANGLE_RIGHT:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z - 500.0f),2 });
		m_pointList.push_back({ Vector3(m_position.x - 500.0f,m_position.y,m_position.z - 500.0f),3 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z + 500.0f),4 });
		break;
		// (����)���p
	case ANGLE_LEFT:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z - 500.0f),2 });
		m_pointList.push_back({ Vector3(m_position.x + 500.0f,m_position.y,m_position.z - 500.0f),3 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z + 500.0f),4 });
		break;
		// �E���(�����`)
	case RECTANGLE_RIGHT:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z + 300.0f),2 });
		m_pointList.push_back({ Vector3(m_position.x - 500.0f ,m_position.y,m_position.z + 300.0f),3 });
		m_pointList.push_back({ Vector3(m_position.x - 500.0f,m_position.y,m_position.z),4 });
		break;
		// �����(�����`)
	case RECTANGLE_LEFT:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z + 300.0f),2 });
		m_pointList.push_back({ Vector3(m_position.x + 500.0f ,m_position.y,m_position.z + 300.0f),3 });
		m_pointList.push_back({ Vector3(m_position.x + 500.0f,m_position.y,m_position.z),4 });
	}
}

void Enemy::Act_Access()
{
	// �G�l�~�[����v���C���[�֌������x�N�g��
	Vector3 diff = m_playerManagement->GetPosition() - m_position;
	// �x�N�g���̒���
	float length = diff.Length();

	Vector3 rot = m_playerManagement->GetPosition() - m_position;
	rot.Normalize();
	Rotation(rot);

	// �x�N�g�������ȉ��̂Ƃ�
	if (length <= SEACH_DECISION) {
		// �x�N�g���𐳋K��
		diff.Normalize();
		// �G�l�~�[�̍��W�ɉ��Z
		Vector3 moveSpeed = diff * MOVE_SPEED;
		m_position += moveSpeed;

		// ����A�j���[�V������Đ�
		m_enEnemyAnimationState = m_enEnemyAnimationState_Walk;
	}
}

void Enemy::Act_Charge(float time)
{
	// ��]
	Vector3 rot = m_playerManagement->GetPosition() - m_position;
	rot.Normalize();
	Rotation(rot);

	// �ҋ@�A�j���[�V������Đ�
	m_enEnemyAnimationState = m_enEnemyAnimationState_Idle;

	// ��~���鎞�Ԃ�߂����Ƃ�
	if (Act_Stop(time) == true) {

		// ��x�������W��Q�Ƃ���
		if (CalculatedFlag == false) {

			// ���W��n��
			playerPos = m_playerManagement->GetPosition();
			enemyPos = m_position;

			// ������s��Ȃ��悤�Ƀt���O�𗧂Ă�
			CalculatedFlag = true;
		}

		// �G�l�~�[����v���C���[�֌��������W
		Vector3 diff = playerPos - enemyPos;
		diff.Normalize();
		// ��]
		Rotation(diff);

		// �G�l�~�[�̍��W�ɉ��Z
		Vector3 moveSpeed = diff * MOVE_SPEED;
		m_position += moveSpeed;
		// �ړ��ʂ���Z
		sumPos += moveSpeed;

		// ����A�j���[�V������Đ�
		m_enEnemyAnimationState = m_enEnemyAnimationState_Walk;

		// �ړ����������̂Ƃ����Z��I������
		if (sumPos.Length() > MOVING_DISTANCE) {
			
			m_position = m_position;	// ���W��Œ�

			addTimer = 0.0f;			// �^�C�}�[����Z�b�g
			sumPos = Vector3::Zero;		// ���ړ���������Z�b�g
			CalculatedFlag = false;		// �t���O��~�낷

			// �v���C���[������p��ɑ��݂���Ƃ�
			if (Act_SeachPlayer() == true) {
				return;
			}

			m_ActState = BACKBASEDON;
		}
	}
}

void Enemy::Act_Loss()
{
	// �v���C���[����������Ƃ��̃p�X�ւ̐؂�ւ����̏���
	// �v���C���[��Ō�Ɍ����ӏ��̏�����܂ňړ�����ƌ���

	addTimer = 0.0f;	// ���Z�p�^�C�}�[����Z�b�g

	// ���̑Ώۂ̋���
	// float�̍ő�l��i�[
	float NowTargetDiff = D3D12_FLOAT32_MAX;
	int NowTargetNum = -1;

	// ��ԋ߂��p�X��T��
	for (int i = 0; i < m_pointList.size(); i++) {

		m_point = &m_pointList[i];
		Vector3 diff = m_point->s_position - m_position;
		float length = diff.Length();

		// �������r
		// �V�����v�Z�����x�N�g���̕����Z���ꍇ�A�l�����ւ���
		if (NowTargetDiff > length) {
			NowTargetNum = i;
			NowTargetDiff = length;
		}
	}

	// �ŒZ�̃p�X�l��i�[����
	m_point = &m_pointList[NowTargetNum];

	// �ڕW�Ƃ���|�C���g�̍��W����A���݂̍��W��������x�N�g��
	Vector3 moveSpeed = m_point->s_position - m_position;
	// ���K��
	moveSpeed.Normalize();
	// �x�N�g���ɃX�J���[���Z
	moveSpeed *= MOVE_SPEED;
	// ��]�������
	Rotation(moveSpeed);

	return;
}

void Enemy::Act_Limit()
{
	// ���̋����ɂ͋߂Â��Ȃ�
	// �G�l�~�[����v���C���[�֌������x�N�g��
	Vector3 diff = m_playerManagement->GetPosition() - m_position;

	// ���������ȉ��̂Ƃ�
	if ( diff.Length() <= ACT_LIMIT) {
		// �����Ȃ��悤�ɂ���
		m_position = m_position;
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
	//���C�g�̕����ݒ�
	m_spotLight.SetDirection(forward);
	m_spotLight.Update();
}
void Enemy::SpotLight_Serch(Quaternion lightrotaition, Vector3 lightpos)
{
	lightpos.y = LIGHTPOSITION;
	//Y��
	Vector3 m_Yup = Vector3(0.0f, 1.0f, 0.0f);
	//�v���C���[�̐���
	Vector3 m_front = Vector3(0.0f, 0.0f, 1.0f);
	lightrotaition.Apply(m_front);
	//���̓�̐����ȃx�N�g��
	Vector3 m_vertical = Cross(m_Yup, m_front);
	Quaternion m_SitenRot;
	//���̐����ȃx�N�g������ɃN�H�[�^�j�I������
	m_SitenRot.SetRotationDeg(m_vertical, ANGLE);
	//�x�N�g���ɃN�H�[�^�j�I������Z����
	m_SitenRot.Apply(m_front);
	m_spotLight.SetDirection(m_front);

	if (m_spotLight.IsHit(m_playerManagement->GetPosition()) == true)
	{
		//�X�e�[�g�̑J��
		int a = 0;
	}
	m_spotLight.SetPosition(lightpos);
	m_spotLight.Update();
}