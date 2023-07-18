#include "stdafx.h"
#include "Enemy.h"

#include "PlayerManagement.h"
#include "Gage.h"
#include "Game.h"
#include "GameManager.h"

#define SEACH_DECISION	100.0f * 100.0f						// �x�N�g�����쐬����͈�

namespace
{
	const Vector3	BOXSIZE = { 60.0f, 80.0f,60.0f };		// CharacterController�̃T�C�Y

	const float		LINEAR_COMPLETION = 1.0f;				// ���`�⊮

	const float		MOVE_SPEED = 3.0f;						// �ړ����x
	const float		ADD_SPEED = 1.3f;						// ��Z���x

	const float		MOVING_DISTANCE = 600.0f;				// �ːi����ړ�����

	const float		CALL_DISTANCE_MAX = 400.0f;				// �ĂԂ��Ƃ��ł���ő�l
	const float		CALL_DISTANCE_MIN = 190.0f;				// �ĂԂ��Ƃ��ł���ŏ��l

	const float		CHANGING_DISTANCE = 20.0f;				// �ړI�n��ύX���鋗��

	const float		LENGTH_LOWESTVALUE = 10.0f;				// �ړ����鋗���̌��x

	const float		CANMOVE_TIMER = 5.5f;					// �ēx�s���ł���܂ł̑ҋ@����
	const float		WAITING_TIMER = 3.0f;					// �p�X�ړ����̑ҋ@����
	const float		SEARCHPLAYER_TIMER = 7.0f;				// �v���C���[�������������̑ҋ@����
	const float		PLAYER_LOSTUPTO_TIMER = 10.0f;			// �v���C���[���������܂ł̎���

	const float		ADD_MOVE_MIN = 250.0f;					// �p�X�ړ�
	const float		ADD_MOVE_LONG = 400.0f;					// �p�X�ړ�

	const float		AI_RADIUS = 150.0f;						// AI�G�[�W�F���g�̔��a
	const float		AI_HIGH = 200.0f;						// AI�G�[�W�F���g�̍���
	const float		CALCULATIONNAVI_TIMER = 0.5f;			// �i�r���b�V�����ēx�v�Z����^�C�}�[

	const float		CATCH_DECISION = 60.0f;					// �v���C���[���m�ۂ������ƂɂȂ�͈�

	const Vector3	ADD_LENGTH = { 50.0f, 0.0f, 50.0f };	// �ːi���ɒǉ����钷��

	const float     VIGILANCETIME = 0.3f;					// �x���xUP����

	const float		ANGLE = 45.0f;							// �p�x
	const Vector3   LIGHTCOLOR(1.5f, 0.3f, 0.0f);			// �J���[
	const float		LIGHTRANGE = 600.0f;					// �͈�
	const float		LIGHTPOSITION = 80.0f;					// ���W
	const Vector3	LIGHT_DIRECTION = { 0.0f, 1.0f, 1.0f };

	const float		Z_UP_EFFECTPOSITON = 10.0f;				// �G�t�F�N�g�̕\�����W(m_player�ɉ��Z����l)

	const float		DOWN_VOLUME = 0.1f;						// ���ʂ�������l
}

Enemy::Enemy()
{
	//m_point = new Point;
}

Enemy::~Enemy()
{
	m_pointList.clear();
	m_pointList.shrink_to_fit();

	if (m_Effect != nullptr) {
		// �G�t�F�N�g�̒�~�ƍ폜
		m_Effect->Stop();
		DeleteGO(m_Effect);
	}
}

bool Enemy::Start()
{
	// �X�P�[����ݒ�
	SetScale({ 2.0f,2.0f,2.0f });

	// �s���p�^�[����������
	if (m_enemyType == TYPE_SEARCH) {
		m_ActionState = m_ActionState_Search;
	}
	else {
		m_ActionState = m_ActionState_Craw;
	}

	//�x���x���Ԃ���
	m_Vicount = VIGILANCETIME;

	// �L�����N�^�[�R���g���[���[������������
	m_characterController.Init(BOXSIZE, m_position);

	// �X�t�B�A�R���C�_�[��ݒ�
	m_sphereCollider.Create(20.0f);

	// �i�r���b�V�����\�z
	m_nvmMesh.Init("Assets/nvm/nvm1.tkn");

	// �C���X�^���X��T��
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
	m_gage = FindGO<Gage>("gage");
	m_game = FindGO<Game>("game");

	// �^�C�}�[�̃��Z�b�g
	for (int i = 0; i < 4; i++) {
		m_addTimer[i] = 0.0f;
	}

	// �G�t�F�N�g�𐶐�����t���O�̃��Z�b�g
	for (int i = 0; i < 3; i++) {
		m_efectDrawFlag[i] = false;
	}

	// ������쐬
	SpotLight_New(m_position, m_spotNum);

	//----------------------------------------------
	m_foward = Vector3::AxisZ;
	m_rotation.Apply(m_foward);
	//----------------------------------------------

	return true;
}

void Enemy::LoadAnimation()
{
	// �A�j���[�V�����̓ǂݍ���
	m_enAnimationClips[m_enAnimation_Idle].Load("Assets/animData/enemy/idle.tka");
	m_enAnimationClips[m_enAnimation_Idle].SetLoopFlag(true);

	m_enAnimationClips[m_enAnimation_Walk].Load("Assets/animData/enemy/walk_battle.tka");
	m_enAnimationClips[m_enAnimation_Walk].SetLoopFlag(true);

	m_enAnimationClips[m_enAnimation_Run].Load("Assets/animData/enemy/run_battle.tka");
	m_enAnimationClips[m_enAnimation_Run].SetLoopFlag(true);

	m_enAnimationClips[m_enAnimation_Attack].Load("Assets/animData/enemy/attack.tka");
	m_enAnimationClips[m_enAnimation_Attack].SetLoopFlag(false);

	m_enAnimationClips[m_enAnimation_Damage].Load("Assets/animData/enemy/damege.tka");
	m_enAnimationClips[m_enAnimation_Damage].SetLoopFlag(false);

	m_enAnimationClips[m_enAnimation_Dizzy].Load("Assets/animData/enemy/dizzy.tka");
	m_enAnimationClips[m_enAnimation_Dizzy].SetLoopFlag(true);

	m_enAnimationClips[m_enAnimation_Loss].Load("Assets/animData/enemy/search.tka");
	m_enAnimationClips[m_enAnimation_Loss].SetLoopFlag(true);

	m_enAnimationClips[m_enAnimation_Call].Load("Assets/animData/enemy/search_idle.tka");
	m_enAnimationClips[m_enAnimation_Call].SetLoopFlag(true);
}

void Enemy::PlayAnimation()
{
	// �s���p�^�[���ōĐ��A�j���[�V������ϓ�
	switch (m_enAnimationState)
	{
	case m_enAnimationState_Idle:
		m_enemyRender.PlayAnimation(m_enAnimation_Idle, LINEAR_COMPLETION);
		break;
	case m_enAnimationState_Walk:
		m_enemyRender.PlayAnimation(m_enAnimation_Walk, LINEAR_COMPLETION);
		break;
	case m_enAnimationState_Run:
		m_enemyRender.PlayAnimation(m_enAnimation_Run, LINEAR_COMPLETION);
		break;
	case m_enAnimationState_Attack:
		m_enemyRender.PlayAnimation(m_enAnimation_Attack, LINEAR_COMPLETION);
		break;
	case m_enAnimationState_Damage:
		m_enemyRender.PlayAnimation(m_enAnimation_Damage, LINEAR_COMPLETION);
		break;
	case m_enAnimationState_Dizzy:
		m_enemyRender.PlayAnimation(m_enAnimation_Dizzy, LINEAR_COMPLETION);
		break;
	case m_enAnimationState_Loss:
		m_enemyRender.PlayAnimation(m_enAnimation_Loss, LINEAR_COMPLETION);
		break;
	case m_enAnimationState_Call:
		m_enemyRender.PlayAnimation(m_enAnimation_Call, LINEAR_COMPLETION);
		break;
	}
}

void Enemy::Efect_Dizzy()
{
	if (m_efectDrawFlag[0] == false) {

		// ���̃G�t�F�N�g�𐶐�
		m_Effect = NewGO<EffectEmitter>(2);
		m_Effect->Init(2);
		// �G�t�F�N�g�̑傫�����w�肷��
		m_Effect->SetScale(Vector3::One * 1.0f);
		// �G�t�F�N�g�̍��W�̐ݒ�
		m_Effect->SetPosition(Vector3(m_position.x + 5.0f, 100.0f, m_position.z + Z_UP_EFFECTPOSITON));
		m_Effect->SetTime(g_gameTime->GetFrameDeltaTime() * 60.0f);
		m_Effect->Play();
		m_Effect->Update();

		m_efectDrawFlag[0] = true;
	}
}

void Enemy::Efect_FindPlayer()
{
	if (m_efectDrawFlag[1] == false) {
		// !�̃G�t�F�N�g�𐶐�
		m_Effect = NewGO<EffectEmitter>(3);
		m_Effect->Init(3);
		// �G�t�F�N�g�̑傫�����w�肷��
		m_Effect->SetScale(Vector3::One * 1.2f);
		// �G�t�F�N�g�̍��W�̐ݒ�
		m_Effect->SetPosition(Vector3(m_position.x + 5.0f, 100.0f, m_position.z + Z_UP_EFFECTPOSITON));
		m_Effect->SetTime(g_gameTime->GetFrameDeltaTime() * 60.0f);
		m_Effect->Play();
		m_Effect->Update();

		m_efectDrawFlag[1] = true;
	}
}

void Enemy::Efect_MissingPlayer()
{
	if (m_efectDrawFlag[2] == false) {
		// ?�̃G�t�F�N�g�𐶐�
		m_Effect = NewGO<EffectEmitter>(4);
		m_Effect->Init(4);
		// �G�t�F�N�g�̑傫�����w�肷��
		m_Effect->SetScale(Vector3::One * 1.5f);
		// �G�t�F�N�g�̍��W�̐ݒ�
		m_Effect->SetPosition(Vector3(m_position.x + 5.0f, 100.0f, m_position.z + Z_UP_EFFECTPOSITON));
		m_Effect->SetTime(g_gameTime->GetFrameDeltaTime() * 60.0f);
		m_Effect->Play();
		m_Effect->Update();

		m_efectDrawFlag[2] = true;
	}
}

void Enemy::Rotation(Vector3 rot)
{
	// ��]
	m_rotation.SetRotationYFromDirectionXZ(rot);
	m_enemyRender.SetRotation(m_rotation);

	//---------------------------------------------
	//�G�l�~�[�̑O�x�N�g�������߂�
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	//---------------------------------------------

}

void Enemy::SpecifyPath(int pathNumber)
{
	// �p�X���w��
	switch (pathNumber)
	{
		// �c
	case 0:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z - ADD_MOVE_LONG),2 });
		break;
		// ��
	case 1:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x + ADD_MOVE_LONG,m_position.y,m_position.z),2 });
		break;
		// �E���(�����`)
	case 2:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x - ADD_MOVE_MIN,m_position.y,m_position.z),2 });
		m_pointList.push_back({ Vector3(m_position.x - ADD_MOVE_MIN,m_position.y,m_position.z - ADD_MOVE_MIN),3 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z - ADD_MOVE_MIN),4 });
		break;
	}
}

void Enemy::CreateNavimesh(Vector3 pos)
{
	// �^�C�}�[�����Z
	m_NaviTimer += g_gameTime->GetFrameDeltaTime();

	// �G�l�~�[����v���C���[�֌������x�N�g��
	Vector3 diff = pos - m_position;

	diff.Normalize();
	Rotation(diff);

	// ��莞�Ԉȉ��̂Ƃ�return
	if (CALCULATIONNAVI_TIMER >= m_NaviTimer) {
		return;
	}

	// �i�r���b�V��
	bool isEnd;							// �t���O

	// �p�X����
	m_pathFiding.Execute(
		m_path,							// �\�z���ꂽ�p�X�̊i�[��
		m_nvmMesh,						// �i�r���b�V��
		m_position,						// �J�n���W
		pos,							// �ړ��ڕW���W
		PhysicsWorld::GetInstance(),	// �����G���W��
		AI_RADIUS,						// AI�G�[�W�F���g�̔��a
		AI_HIGH							// AI�G�[�W�F���g�̍���
	);

	// �p�X��𑖂��Ĉړ�����
	m_position = m_path.Move(
		m_position,						// ���W
		MOVE_SPEED * ADD_SPEED,			// �ړ����x�i�p�X�ړ��������߁j
		isEnd							// �ړ������Ƃ�true
	);
}

void Enemy::Action_SeachPlayer()
{
	// �������͉������Ȃ�
	if (m_ActionState == m_ActionState_Dizzy) {
		return;
	}

	// �ǐՂ��Ă���Œ�
	if (m_TrackingPlayerFlag == true) {
		// �Փ˔�����s��
		if (WallAndHit(m_playerManagement->GetPosition()) == false) {
			// �ǂɏՓ˂����Ƃ�
			m_TrackingPlayerFlag = false;
			return;
		}
	}

	// �X�|�b�g���C�g�̒��Ƀv���C���[������Ƃ�
	if (m_spotLight.IsHit(m_playerManagement->GetPosition()) == true) {
		// �Փ˔�����s��
		if (WallAndHit(m_playerManagement->GetPosition()) == false) {
			// �ǂɏՓ˂����Ƃ�
			m_TrackingPlayerFlag = false;
			return;
		}

		// �ǐՃt���O��true�ɂ���
		m_TrackingPlayerFlag = true;
		// �G�t�F�N�g�𐶐�
		Efect_FindPlayer();
		return;
	}
}

// �Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�Ǘp)
struct SweepResultWall :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;		// �Փ˃t���O

	virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		// �ǂƂԂ����Ă��Ȃ�������
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall) {
			// �Փ˂����͕̂ǂł͂Ȃ�
			return 0.0f;
		}

		// �ǂƂԂ�������
		// �t���O��true�ɂ���
		isHit = true;
		return 0.0f;
	}
};

bool Enemy::WallAndHit(Vector3 pos)
{
	btTransform start, end;

	start.setIdentity();
	end.setIdentity();

	// �n�_�̓G�l�~�[�̍��W
	start.setOrigin(btVector3(m_position.x,  25.0f, m_position.z));

	// �I�_�̓v���C���[�̍��W (�ːi���͎n�_�̏����O)
	end.setOrigin(btVector3(pos.x, 25.0f, pos.z));

	SweepResultWall callback;

	// �R���C�_�[���n�_����I�_�܂œ������āA
	// �Փ˂��邩�ǂ������ׂ�
	PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(),
		start, end, callback);

	// �ǂƏՓ˂���
	if (callback.isHit == true) {
		return false;
	}

	// �ǂƏՓ˂��Ă��Ȃ�
	// �v���C���[��������
	return true;
}

bool Enemy::Action_CatchPlayer()
{
	// �G�l�~�[����v���C���[�֌������x�N�g��
	Vector3 diff = m_playerManagement->GetPosition() - m_position;
	// �������v�Z����
	float length = diff.Length();

	// ���̒����̂Ƃ�
	if (length <= CATCH_DECISION) {

		// �v���C���[�̕���������
		diff.Normalize();
		Rotation(diff);
		// �߂܂���
		m_enAnimationState = m_enAnimationState_Attack;

		for (int i = 0; i < m_game->GetEnemyList().size(); i++) {
			// �X�e�[�g��ϓ�������
			m_game->GetEnemyList()[i]->m_ActionState = m_ActionState_CatchPlayer;
		}
		return true;
	}
	return false;
}

void Enemy::Action_MoveMissingPosition()
{
	// �v���C���[�𔭌������Ƃ�
	if (m_TrackingPlayerFlag == true) {
		// �ēx�ǐՂ���
		Efect_FindPlayer();
		m_NaviTimer = 0.0f;
		m_addTimer[3] = 0.0f;			// �^�C�}�[�����Z�b�g

		// ���G����^�C�v�Ȃ�
		if (m_enemyType == TYPE_SEARCH) {
			// ����̓G���Ă�
			m_ActionState = m_ActionState_Call_AroundEnemy;
			return;
		}
		else if (m_enemyType == TYPE_CHARGE) {
			// �ːi����
			m_ActionState = m_ActionState_Charge;
			return;
		}
		m_ActionState = m_ActionState_Tracking;
		return;
	}

	// �x�N�g�����쐬
	Vector3 diff = m_playerMissiongPosition - m_position;

	// �������v�Z
	float length = diff.Length();

	// �ړ�����
	diff.Normalize();

	// �ړ����x�ɉ��Z
	Vector3 moveSpeed = diff * (MOVE_SPEED * ADD_SPEED);
	m_position += moveSpeed * m_Chargemove;

	// ����A�j���[�V�������Đ�
	m_enAnimationState = m_enAnimationState_Run;

	// ���������ȉ���������
	if (length < LENGTH_LOWESTVALUE) {
		// �X�e�[�g���ڍs
		m_ActionState = m_ActionState_Search_MissingPlayer;
		return;
	}

	// ��]��������
	Rotation(moveSpeed);
}

void Enemy::Action_SearchMissingPlayer()
{
	// �G�t�F�N�g�𐶐�
	Efect_MissingPlayer();

	// �v���C���[�𔭌������Ƃ�
	if (m_TrackingPlayerFlag == true) {
		// �ēx�ǐՂ���
		Efect_FindPlayer();
		m_NaviTimer = 0.0f;
		m_addTimer[3] = 0.0f;			// �^�C�}�[�����Z�b�g

		// ���G����^�C�v�Ȃ�
		if (m_enemyType == TYPE_SEARCH) {
			// ����̓G���Ă�
			m_ActionState = m_ActionState_Call_AroundEnemy;
			return;
		}
		else if (m_enemyType == TYPE_CHARGE) {
			// �ːi����
			m_ActionState = m_ActionState_Charge;
			return;
		}
		m_ActionState = m_ActionState_Tracking;
		return;
	}

	// ���n�����[�V�������Đ�
	m_enAnimationState = m_enAnimationState_Loss;

	// ���[�V�������Đ�
	if (Action_StopMove(SEARCHPLAYER_TIMER, 3) == true) {

		m_efectDrawFlag[1] = false;		// �G�t�F�N�g�̕`��t���O
		m_efectDrawFlag[2] = false;

		m_addTimer[3] = 0.0f;			// �^�C�}�[�����Z�b�g
		m_sumPos = Vector3::Zero;		// �ړ����������Z�b�g

		// ���G����^�C�v�Ȃ�
		if (m_enemyType == TYPE_SEARCH) {
			// ���G��Ԃɖ߂�
			m_ActionState = m_ActionState_Search;
			return;
		}
		m_ActionState = m_ActionState_BackBasedOn;
	}
}

void Enemy::Action_HitFlashBullet()
{
	// �v���C���[��ǐՂ���t���O��true�ɂȂ�����
	if (m_TrackingPlayerFlag == true) {
		// false�ɂ���
		m_TrackingPlayerFlag = false;
	}

	// �v���C���[���m�ۂ����t���O��true�ɂȂ�����
	if (m_ChachPlayerFlag == true) {
		// false�ɂ���
		m_ChachPlayerFlag = false;
	}

	// �߂܂��̃A�j���[�V�������Đ�
	m_enAnimationState = m_enAnimationState_Dizzy;

	Efect_Dizzy();

	// �^�C�}�[��true�̂Ƃ�
	if (Action_StopMove(CANMOVE_TIMER,0) == true) {
		// �����t���O�����Z�b�g
		m_efectDrawFlag[0] = false;

		// �t���O���~�낷
		m_TrackingPlayerFlag = false;
		m_HitFlashBulletFlag = false;

		// �^�C�}�[�����Z�b�g
		m_addTimer[0] = 0.0f;

		// �G�t�F�N�g�𐶐�
		Efect_MissingPlayer();

		// �v���C���[��T��
		m_ActionState = m_ActionState_Search_MissingPlayer;
	}
	else {
		// �G�t�F�N�g�̐����t���O��false�ɂ��Ă���
		m_efectDrawFlag[2] = false;
	}
}

void Enemy::Action_GoLocationListenSound(Vector3 tergetPosition)
{
	// �v���C���[�𔭌������Ƃ�
	if (m_TrackingPlayerFlag == true) {

		// �ːi�^�C�v�̂Ƃ�
		if (m_enemyType == TYPE_CHARGE) {
			m_ActionState = m_ActionState_Charge;
			return;
		}
		else {
			m_ActionState = m_ActionState_Tracking;
			return;
		}
	}

	Efect_FindPlayer();

	// �G�l�~�[����A�C�e���֌������x�N�g�����쐬
	Vector3 diff = tergetPosition - m_position;
	float length = diff.Length();

	// ���������ȏ�̂Ƃ�
	if (length >= CALL_DISTANCE_MIN) {
		// �A�C�e���̍��W����ɂ��ăi�r���b�V�����쐬
		CreateNavimesh(tergetPosition);

		// �o�ߎ��Ԃ��v��
		m_addTimer[4] += g_gameTime->GetFrameDeltaTime();

		// ����A�j���[�V�������Đ�
		m_enAnimationState = m_enAnimationState_Run;
		// �G�t�F�N�g�̍Đ��t���O��false�ɂ��Ă���
		m_efectDrawFlag[2] = false;
	}
	else if (m_addTimer[4] > PLAYER_LOSTUPTO_TIMER) {
		// ��莞�Ԃ��o�߂����Ƃ�
		// �ړ��𒆒f���Č��������v���C���[��T��
		m_ActionState = m_ActionState_Search_MissingPlayer;
		m_HearedSoundBulletFlag = false;
		// �^�C�}�[�����Z�b�g
		m_addTimer[4] = 0.0f;
		m_efectDrawFlag[1] = false;
		return;
	}
	else {
		// ���������v���C���[��T��
		m_ActionState = m_ActionState_Search_MissingPlayer;
		m_HearedSoundBulletFlag = false;
		m_efectDrawFlag[1] = false;
		return;
	}
}

void Enemy::Action_CrawPath()
{
	// �v���C���[�𔭌������Ƃ�
	if (m_TrackingPlayerFlag == true) {
		// �ːi�^�C�v�̂Ƃ�
		if (m_enemyType == TYPE_CHARGE) {
			m_ActionState = m_ActionState_Charge;
			return;
		}
		// ����ȊO
		m_ActionState = m_ActionState_Tracking;
		return;
	}
	
	// �G�l�~�[����p�X�֌������x�N�g��
	Vector3 diff = m_point->s_position - m_position;

	// ���������̂Ƃ�
	if (diff.Length() <= CHANGING_DISTANCE) {

		// �p�X���Ō�̂Ƃ�
		if (m_point->s_number == m_pointList.size()) {
			// �ŏ��̒l��n��
			m_point = &m_pointList[0];
		}
		// �����łȂ��Ƃ�
		else {
			m_point = &m_pointList[m_point->s_number];
		}

		m_addTimer[1] = 0.0f;	// �^�C�}�[�����Z�b�g
	}

	// �G�l�~�[����v���C���[�֌������x�N�g��
	Vector3 moveSpeed = m_point->s_position - m_position;
	// ���K��
	moveSpeed.Normalize();
	// ��]��������
	moveSpeed *= MOVE_SPEED;
	Rotation(moveSpeed);

	// �^�C�}�[��true�̂Ƃ�
	if (Action_StopMove(WAITING_TIMER,1) == true) {
		// �����A�j���[�V�������Đ�
		m_enAnimationState = m_enAnimationState_Walk;
		// ���W�ɉ��Z
		m_position += moveSpeed;
	}
	else {
		// �ҋ@�A�j���[�V�������Đ�
		m_enAnimationState = m_enAnimationState_Idle;
	}
}

void Enemy::Action_TrackingPlayer()
{
	// �i�r���b�V�����쐬
	CreateNavimesh(m_playerManagement->GetPosition());

	if (m_ChachPlayerFlag == false) {
		// ����A�j���[�V�������Đ�
		m_enAnimationState = m_enAnimationState_Run;
	}

	// ����������
	if (m_TrackingPlayerFlag == false) {
		// �v���C���[�̍��W���Q�Ƃ���
		m_playerMissiongPosition = m_playerManagement->GetPosition();
		m_ActionState = m_ActionState_Move_MissingPositon;
	}
}

void Enemy::Action_ChargeStart(float time)
{
	// �ړ������̒��������ȏ�̂Ƃ�
	if (m_sumPos.Length() >= MOVING_DISTANCE) {
		// �ːi���I������
		m_ActionState = m_ActionState_ChargeEnd;

		// ���ړ����������Z�b�g
		m_sumPos = Vector3::Zero;
		return;
	}

	// �^�C�}�[��true�̂Ƃ�
	if (Action_StopMove(time,2) == true) {

		// ��x�������s����
		if (m_CalculatedFlag == false) {
			// ���W���Q��
			m_chargeTergetPosition = m_playerManagement->GetPosition();

			// ���x�����s���Ȃ��悤��true�ɂ���
			m_CalculatedFlag = true;

			// �G�l�~�[����v���C���[�֌������x�N�g��
			m_chargeDiff = m_chargeTergetPosition - m_position;
			m_chargeDiff.y = 0.0f;
			m_chargeDiff.Normalize();
		}

		// �ړ����x�ɉ��Z
		Vector3 moveSpeed = m_chargeDiff * (MOVE_SPEED * ADD_SPEED);
		m_position += moveSpeed * m_Chargemove;

		// ���ړ��������v�Z
		m_sumPos += moveSpeed;
		// ����A�j���[�V�������Đ�
		m_enAnimationState = m_enAnimationState_Run;

		// �Փ˔���
		Action_ChargeHitWall();
	}
	else {
		// ��]�̂݃v���C���[��ǔ�������
		m_chargeDiff = m_playerManagement->GetPosition() - m_position;
		m_chargeDiff.Normalize();

		// �ҋ@�A�j���[�V�������Đ�
		m_enAnimationState = m_enAnimationState_Idle;
	}

	// ��]��������
	Rotation(m_chargeDiff);
}

void Enemy::Action_ChargeEnd()
{
	m_position = m_position;		// ���W���Œ�

	m_addTimer[2] = 0.0f;			// �^�C�}�[�����Z�b�g
	m_sumPos = Vector3::Zero;		// �ړ����������Z�b�g
	m_CalculatedFlag = false;		// �t���O���~�낷

	m_efectDrawFlag[2] = false;		// !�̃G�t�F�N�g�̃t���O���~�낷
	m_efectDrawFlag[1] = false;

	// �v���C���[������p���ɂ���Ƃ�
	if (m_TrackingPlayerFlag == true) {
		Efect_FindPlayer();
		// �ēx�ːi����
		m_ActionState = m_ActionState_Charge;
		return;
	}
	else {
		// ���Ȃ��Ƃ��͏����Ԃɖ߂�
		m_ActionState = m_ActionState_Search_MissingPlayer;
	}
}

void Enemy::Action_ChargeHitWall()
{
	// �ǂɏՓ˂��锻��

	// �ǂɏՓ˂������ǂ���
	// �v���C���[�̕����֌������P�ʃx�N�g���ɃX�J���[����Z�������̂����Z���ēn��
	if (Enemy::WallAndHit(m_position + (m_chargeDiff * ADD_LENGTH.x)) == false) {
		// �Փ˂����Ƃ�
		m_Chargemove = 0.0f;			// ��Z���Ă���l��0�ɂ��ē����Ȃ��悤�ɂ���
		
		m_addTimer[2] = 0.0f;			// �^�C�}�[�����Z�b�g
		m_sumPos = Vector3::Zero;		// �ړ����������Z�b�g
		m_CalculatedFlag = false;		// �t���O���~�낷

		m_efectDrawFlag[2] = false;		// !�̃G�t�F�N�g�̃t���O���~�낷

		// �s����������Ԃɂ���
		m_ActionState = m_ActionState_Dizzy;
		return;
	}

	// �Փ˂��Ă��Ȃ��Ƃ��͑��s����
	m_Chargemove = 1.0f;
}

void Enemy::Action_CallAroundEnemy()
{
	// se��炷
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(17);
	se->SetVolume(GameManager::GetInstance()->GetSFX() * DOWN_VOLUME);	// ���ʂ�������
	se->Play(false);

	if (m_TrackingPlayerFlag == false) {
		// �t���O���~�낷
		m_efectDrawFlag[1] = false;
		m_ActionState = m_ActionState_Search_MissingPlayer;
		se->Stop();

		return;
	}

	Vector3 rot = m_playerManagement->GetPosition() - m_position;
	rot.Normalize();

	// �G�l�~�[�̃��X�g������
	for (int i = 0; i < m_game->GetEnemyList().size(); i++) {

		// �������������Ƃ��͂���ȍ~�͎��s���Ȃ�
		if (m_game->GetEnemyList()[i]->m_ActionState == m_ActionState_Dizzy) {
			return;
		}

		// �e�G�l�~�[����Y���G�l�~�[�֌������x�N�g��
		Vector3 diff = m_position - m_game->GetEnemyList()[i]->m_position;
		float length = diff.Length();

		// ���������ȓ��̂Ƃ� ���� �܂��Ă�ł��Ȃ��Ƃ�
		if (length > CALL_DISTANCE_MIN &&
			length < CALL_DISTANCE_MAX &&
			m_game->GetEnemyList()[i]->m_ActionState != m_ActionState_Called) {

			m_game->GetEnemyList()[i]->m_ActionState = m_ActionState_Called;		// �s���p�^�[����ύX����
			m_game->GetEnemyList()[i]->m_setPos = m_position - BOXSIZE;		// ���g�̍��W-�L�����R����ڕW�n�_�Ƃ��ēn��
		}
	}

	Rotation(rot);

	m_enAnimationState = m_enAnimationState_Call;
}

void Enemy::Action_MissingPlayer()
{
	m_addTimer[1] = 0.0f;	// �^�C�}�[�����Z�b�g
	m_addTimer[2] = 0.0f;	// �ːi����̈ڍs���ɂ�����̃^�C�}�[�����Z�b�g

	// ���߂̃p�X������
	// float�ɍő�l���i�[
	float NowTargetDiff = D3D12_FLOAT32_MAX;
	int NowTargetNum = -1;

	// �p�X�̌�����
	for (int i = 0; i < m_pointList.size(); i++) {

		m_point = &m_pointList[i];
		Vector3 diff = m_point->s_position - m_position;
		float length = diff.Length();

		// �ēx�v�Z�������������ݕۑ����Ă��钷�����Z���Ƃ�
		if (NowTargetDiff > length) {
			// �l���X�V����
			NowTargetNum = i;
			NowTargetDiff = length;
		}
	}

	// �ŒZ�̃p�X�̏���n��
	m_point = &m_pointList[NowTargetNum];

	// �i�r���b�V�����쐬
	CreateNavimesh(m_point->s_position);

	// ����A�j���[�V�������Đ�
	m_enAnimationState = m_enAnimationState_Run;


	// �G�l�~�[����p�X�֌������x�N�g��
	Vector3 diff = m_point->s_position - m_position;
	float length = diff.Length();

	// ���������̂Ƃ�
	if (length <= CHANGING_DISTANCE) {
		m_NaviTimer = 0.0f;
		m_ActionState = m_ActionState_Craw;
	}
}

bool Enemy::Action_StopMove(float time,int timerNumber)
{
	// �t���[�������Z
	m_addTimer[timerNumber] += g_gameTime->GetFrameDeltaTime();

	// �^�C�}�[�����ȏ�ɂȂ�����
	if (time <= m_addTimer[timerNumber]) {
		return true;
	}

	return false;
}

void Enemy::SpotLight_New(Vector3 position, int num)
{
	m_spotLight.SetSpotLight(
		num,
		position,
		LIGHTCOLOR,
		LIGHTRANGE,
		LIGHT_DIRECTION,
		ANGLE
	);
}

void Enemy::SpotLight_Serch(Quaternion lightrotaition, Vector3 lightpos)
{
	// ������Ԃ̎��͎��s���Ȃ�
	if (m_ActionState == m_ActionState_Dizzy) {
		return;
	}

	lightpos.y = LIGHTPOSITION;
	//Y���W��ݒ�
	Vector3 m_Yup = Vector3(0.0f, 1.0f, 0.0f);
	//�O���̍��W��ݒ�
	Vector3 m_front = Vector3(0.0f, -0.3f, 0.9f);
	lightrotaition.Apply(m_front);
	//��L�̍��W�̊O�ς��v�Z
	Vector3 m_vertical = Cross(m_Yup, m_front);
	Quaternion m_SitenRot;
	//�v�Z�����O�ς����ɉ�]������
	m_SitenRot.SetRotationDeg(m_vertical, ANGLE);
	//�N�H�[�^�j�I����K��
	m_SitenRot.Apply(m_front);
	m_spotLight.SetDirection(m_front);
	if (m_spotLight.IsHit(m_playerManagement->GetPosition()) == true)
	{
		VigilanceCount();

		// �J�E���g����t���O��false�̂Ƃ�
		if (m_CountFlag == false) {
			// �����񐔂�1�����Z
			GameManager::GetInstance()->AddSearchNum();
			// �t���O��true�ɂ��čēx�s��Ȃ��悤�ɂ���
			m_CountFlag = true;
		}
	}
	else {
		// �v���C���[���m�ۂ��Ă��Ȃ��Ƃ�
		if (m_ChachPlayerFlag == false) {
			// �v���C���[�����݂��Ȃ��Ƃ��̓t���O��߂�
			m_CountFlag = false;
		}
	}
	m_spotLight.SetPosition(lightpos);
	m_spotLight.Update();
}
void Enemy::VigilanceCount()
{
	m_Vicount -= g_gameTime->GetFrameDeltaTime();
	if (m_Vicount <= 0.0f)
	{
		//�X�e�[�g�̑J��
		m_gage->GageUp(1, true);
		m_Vicount = VIGILANCETIME;
	}
}