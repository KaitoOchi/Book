#include "stdafx.h"
#include "Enemy.h"

#include "PlayerManagement.h"
#include "Gage.h"
#include "Game.h"
#include "GameManager.h"

namespace
{
	const Vector3	BOXSIZE = { 60.0f, 80.0f,60.0f };		// CharacterController�̃T�C�Y
	const Vector3	SCALE = { 2.0f,2.0f,2.0f };				// �X�P�[��
	const float		LINEAR_COMPLETION = 1.0f;				// ���`�⊮
	const float		COLLIDER_SIZE = 20.0f;					// �X�t�B�A�R���C�_�[�̃T�C�Y
	const float		MOVE_SPEED = 3.0f;						// �ړ����x
	const float		ADD_SPEED = 1.3f;						// ��Z���x
	const float		CALL_DISTANCE_MAX = 400.0f;				// �ĂԂ��Ƃ��ł���ő�l
	const float		CALL_DISTANCE_MIN = 190.0f;				// �ĂԂ��Ƃ��ł���ŏ��l
	const float		CHANGING_DISTANCE = 20.0f;				// �ړI�n��ύX���鋗��
	const float		LENGTH_LOWESTVALUE = 10.0f;				// �ړ����鋗���̌��x
	const float		TIMER_CANMOVE = 5.5f;					// �ēx�s���ł���܂ł̑ҋ@����
	const float		TIMER_WAITING = 3.0f;					// �p�X�ړ����̑ҋ@����
	const float		TIMER_SEARCHPLAYER = 7.0f;				// �v���C���[�������������̑ҋ@����
	const float		TIMER_PLAYER_LOSTUPTO = 10.0f;			// �v���C���[���������܂ł̎���
	const float		ADD_MOVE_MIN = 250.0f;					// �p�X�ړ�
	const float		ADD_MOVE_LONG = 400.0f;					// �p�X�ړ�
	const float		AI_RADIUS = 150.0f;						// AI�G�[�W�F���g�̔��a
	const float		AI_HIGH = 200.0f;						// AI�G�[�W�F���g�̍���
	const float		CALCULATIONNAVI_TIMER = 0.5f;			// �i�r���b�V�����ēx�v�Z����^�C�}�[
	const float		CATCH_DECISION = 60.0f;					// �v���C���[���m�ۂ������ƂɂȂ�͈�
	const float		SEACH_DECISION = 100.0f * 100.0f;		// ���G�͈�
	const float     VIGILANCETIME = 0.3f;					// �x���xUP����
	const Vector3	LIGHT_DIRECTION = { 0.0f, 1.0f, 1.0f };	// �X�|�b�g���C�g�̕���
	const Vector3   LIGHTCOLOR(1.5f, 0.3f, 0.0f);			// �X�|�b�g���C�g�̃J���[
	const float		ANGLE = 45.0f;							// �X�|�b�g���C�g�̊p�x
	const float		LIGHTRANGE = 600.0f;					// �X�|�b�g���C�g�͈̔�
	const float		LIGHTPOSITION = 80.0f;					// �X�|�b�g���C�g�̍��W
	const float		EFFECT_Z = 10.0f;						// �G�t�F�N�g��Z���W���Z�l
	const float		EFFECT_X = 5.0f;						// �G�t�F�N�g��X���W���Z�l
	const float		EFFECT_Y = 100.0f;						// �G�t�F�N�g��Y���W
	const float		EFFECT_SIZE_STAR = 1.0f;				// �G�t�F�N�g�̃T�C�Y
	const float		EFFECT_SIZE_EXCLAMATION = 1.2f;			// �G�t�F�N�g�̃T�C�Y
	const float		EFFECT_SIZE_QUESTION = 1.5f;			// �G�t�F�N�g�̃T�C�Y
}

Enemy::Enemy()
{
	// �^�C�}�[�̃��Z�b�g
	for (int i = 0; i < m_TimerState_Num; i++) {
		m_addTimer[i] = 0.0f;
	}

	// �G�t�F�N�g�𐶐�����t���O�̃��Z�b�g
	for (int i = 0; i < m_EffectState_Num; i++) {
		m_existsEfectDraw[i] = false;
	}

	//�x���x���Ԃ���
	m_addVicount = VIGILANCETIME;
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
	SetScale(SCALE);

	// �s���p�^�[����������
	if (m_enemyType == TYPE_SEARCH) {
		m_enActionState = m_ActionState_Search;
	}
	else {
		m_enActionState = m_ActionState_Craw;
	}

	// �L�����N�^�[�R���g���[���[������������
	m_characterController.Init(BOXSIZE, m_position);

	// �X�t�B�A�R���C�_�[��ݒ�
	m_sphereCollider.Create(COLLIDER_SIZE);

	// �i�r���b�V�����\�z
	m_nvmMesh.Init("Assets/nvm/nvm1.tkn");

	// �C���X�^���X��T��
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
	m_gage = FindGO<Gage>("gage");
	m_game = FindGO<Game>("game");

	// ������쐬
	SpotLight_New(m_position, m_spotNum);

	//----------------------------------------------
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
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
		m_enemyModelRender.PlayAnimation(m_enAnimation_Idle, LINEAR_COMPLETION);
		break;
	case m_enAnimationState_Walk:
		m_enemyModelRender.PlayAnimation(m_enAnimation_Walk, LINEAR_COMPLETION);
		break;
	case m_enAnimationState_Run:
		m_enemyModelRender.PlayAnimation(m_enAnimation_Run, LINEAR_COMPLETION);
		break;
	case m_enAnimationState_Attack:
		m_enemyModelRender.PlayAnimation(m_enAnimation_Attack, LINEAR_COMPLETION);
		break;
	case m_enAnimationState_Damage:
		m_enemyModelRender.PlayAnimation(m_enAnimation_Damage, LINEAR_COMPLETION);
		break;
	case m_enAnimationState_Dizzy:
		m_enemyModelRender.PlayAnimation(m_enAnimation_Dizzy, LINEAR_COMPLETION);
		break;
	case m_enAnimationState_Loss:
		m_enemyModelRender.PlayAnimation(m_enAnimation_Loss, LINEAR_COMPLETION);
		break;
	case m_enAnimationState_Call:
		m_enemyModelRender.PlayAnimation(m_enAnimation_Call, LINEAR_COMPLETION);
		break;
	}
}

void Enemy::Efect_Dizzy()
{
	if (m_existsEfectDraw[m_EffectState_Star] == false) {
		// ���̃G�t�F�N�g�𐶐�
		m_Effect = NewGO<EffectEmitter>(2);
		m_Effect->Init(2);
		// �G�t�F�N�g�̑傫�����w�肷��
		m_Effect->SetScale(Vector3::One * EFFECT_SIZE_STAR);
		// �G�t�F�N�g�̍��W�̐ݒ�
		m_Effect->SetPosition(Vector3(m_position.x + EFFECT_X, EFFECT_Y, m_position.z + EFFECT_Z));
		m_Effect->SetTime(g_gameTime->GetFrameDeltaTime() * 60.0f);
		m_Effect->Play();
		m_Effect->Update();

		m_existsEfectDraw[m_EffectState_Star] = true;
	}
}

void Enemy::Efect_FindPlayer()
{
	if (m_existsEfectDraw[m_EffectState_ExclamationPoint] == false) {
		// !�̃G�t�F�N�g�𐶐�
		m_Effect = NewGO<EffectEmitter>(3);
		m_Effect->Init(3);
		// �G�t�F�N�g�̑傫�����w�肷��
		m_Effect->SetScale(Vector3::One * EFFECT_SIZE_EXCLAMATION);
		// �G�t�F�N�g�̍��W�̐ݒ�
		m_Effect->SetPosition(Vector3(m_position.x + EFFECT_X, EFFECT_Y, m_position.z + EFFECT_Z));
		m_Effect->SetTime(g_gameTime->GetFrameDeltaTime() * 60.0f);
		m_Effect->Play();
		m_Effect->Update();

		m_existsEfectDraw[m_EffectState_ExclamationPoint] = true;
	}
}

void Enemy::Efect_MissingPlayer()
{
	if (m_existsEfectDraw[m_EffectState_QuestionMark] == false) {
		// ?�̃G�t�F�N�g�𐶐�
		m_Effect = NewGO<EffectEmitter>(4);
		m_Effect->Init(4);
		// �G�t�F�N�g�̑傫�����w�肷��
		m_Effect->SetScale(Vector3::One * EFFECT_SIZE_QUESTION);
		// �G�t�F�N�g�̍��W�̐ݒ�
		m_Effect->SetPosition(Vector3(m_position.x + EFFECT_X, EFFECT_Y, m_position.z + EFFECT_Z));
		m_Effect->SetTime(g_gameTime->GetFrameDeltaTime() * 60.0f);
		m_Effect->Play();
		m_Effect->Update();

		m_existsEfectDraw[m_EffectState_QuestionMark] = true;
	}
}

void Enemy::Rotation(const Vector3& rot)
{
	// ��]
	m_rotation.SetRotationYFromDirectionXZ(rot);
	m_enemyModelRender.SetRotation(m_rotation);

	//---------------------------------------------
	//�G�l�~�[�̑O�x�N�g�������߂�
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	//---------------------------------------------
}

void Enemy::SpecifyPath(int pathNumber)
{
	// �p�X���w��
	switch (pathNumber){
	case m_PathState_VerticalMovement:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z - ADD_MOVE_LONG),2 });
		break;
	case m_PathState_MoveSideways:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x + ADD_MOVE_LONG,m_position.y,m_position.z),2 });
		break;
	case m_PathState_ClockwiseRotation:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x - ADD_MOVE_MIN,m_position.y,m_position.z),2 });
		m_pointList.push_back({ Vector3(m_position.x - ADD_MOVE_MIN,m_position.y,m_position.z - ADD_MOVE_MIN),3 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z - ADD_MOVE_MIN),4 });
		break;
	}
}

void Enemy::CreateNavimesh(const Vector3& pos)
{
	m_naviTimer += g_gameTime->GetFrameDeltaTime();

	// �ڕW�n�_�֌������x�N�g��
	Vector3 diff = pos - m_position;
	diff.Normalize();

	// ��]
	Rotation(diff);

	// ��莞�Ԉȉ��̂Ƃ�return
	if (CALCULATIONNAVI_TIMER >= m_naviTimer) {
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
	if (m_enActionState == m_ActionState_Dizzy) {
		return;
	}

	// �ǐՂ��Ă���Œ�
	if (m_isTrackingPlayer == true) {
		// �Փ˔�����s��
		if (WallAndHit(m_playerManagement->GetPosition()) == false) {
			m_isTrackingPlayer = false;
			return;
		}
	}

	// �X�|�b�g���C�g�̒��Ƀv���C���[������Ƃ�
	if (m_spotLight.IsHit(m_playerManagement->GetPosition()) == true) {
		// �Փ˔�����s��
		if (WallAndHit(m_playerManagement->GetPosition()) == false) {
			m_isTrackingPlayer = false;
			return;
		}

		m_isTrackingPlayer = true;
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

bool Enemy::WallAndHit(const Vector3& pos)
{
	btTransform start, end;

	start.setIdentity();
	end.setIdentity();

	// �n�_�̓G�l�~�[�̍��W
	// Y���͏�����ɔz�u����
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

	// �v���C���[��߂܂����钷���̂Ƃ�
	if (length <= CATCH_DECISION) {

		// �v���C���[�̕���������
		diff.Normalize();
		Rotation(diff);
		// �߂܂���
		m_enAnimationState = m_enAnimationState_Attack;

		for (int i = 0; i < m_game->GetEnemyList().size(); i++) {
			// �X�e�[�g��ϓ�������
			m_game->GetEnemyList()[i]->m_enActionState = m_ActionState_CatchPlayer;
		}
		return true;
	}
	return false;
}

void Enemy::Action_MoveMissingPosition()
{
	// �v���C���[�𔭌������Ƃ�
	if (m_isTrackingPlayer == true) {
		// �ēx�ǐՂ���
		Efect_FindPlayer();
		m_naviTimer = 0.0f;
		m_addTimer[m_TimerState_MissingPlayer] = 0.0f;			// �^�C�}�[�����Z�b�g

		if (m_enemyType == TYPE_SEARCH) {
			m_enActionState = m_ActionState_Call_AroundEnemy;
			return;
		}
		else if (m_enemyType == TYPE_CHARGE) {
			m_enActionState = m_ActionState_Charge;
			return;
		}
		m_enActionState = m_ActionState_Tracking;
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
	m_position += moveSpeed;

	m_enAnimationState = m_enAnimationState_Run;

	// ���������ȉ���������
	if (length < LENGTH_LOWESTVALUE) {
		// �X�e�[�g���ڍs
		m_enActionState = m_ActionState_Search_MissingPlayer;
		return;
	}

	// ��]��������
	Rotation(moveSpeed);
}

void Enemy::Action_SearchMissingPlayer()
{
	Efect_MissingPlayer();

	if (m_isTrackingPlayer == true) {
		// �ēx�ǐՂ���
		Efect_FindPlayer();
		m_naviTimer = 0.0f;
		m_addTimer[m_TimerState_MissingPlayer] = 0.0f;			// �^�C�}�[�����Z�b�g

		if (m_enemyType == TYPE_SEARCH) {
			m_enActionState = m_ActionState_Call_AroundEnemy;
			return;
		}
		else if (m_enemyType == TYPE_CHARGE) {
			m_enActionState = m_ActionState_Charge;
			return;
		}
		m_enActionState = m_ActionState_Tracking;
		return;
	}

	m_enAnimationState = m_enAnimationState_Loss;

	// ���[�V�������Đ�
	if (Action_StopMove(TIMER_SEARCHPLAYER, m_TimerState_MissingPlayer) == true) {

		m_existsEfectDraw[m_EffectState_ExclamationPoint] = false;	// �G�t�F�N�g�̕`��t���O
		m_existsEfectDraw[m_EffectState_QuestionMark] = false;

		m_addTimer[m_TimerState_MissingPlayer] = 0.0f;				// �^�C�}�[�����Z�b�g
		m_sumPosition = Vector3::Zero;								// �ړ����������Z�b�g

		if (m_enemyType == TYPE_SEARCH) {
			// ���G��Ԃɖ߂�
			m_enActionState = m_ActionState_Search;
			return;
		}
		m_enActionState = m_ActionState_BackBasedOn;
	}
}

void Enemy::Action_HitFlashBullet()
{
	if (m_isTrackingPlayer == true) {
		// �M���e���������Ă���Ƃ��̓v���C���[��ǂ������Ȃ�
		m_isTrackingPlayer = false;
	}

	if (m_isChachPlayer == true) {
		// �M���e���������Ă���Ƃ��̓v���C���[��߂܂��Ȃ�
		m_isChachPlayer = false;
	}

	m_enAnimationState = m_enAnimationState_Dizzy;
	Efect_Dizzy();

	if (Action_StopMove(TIMER_CANMOVE, m_TimerState_HitByaFlashbang) == true) {
		// �G�t�F�N�g�̐����t���O��false�ɂ��Ă���
		m_existsEfectDraw[m_EffectState_Star] = false;

		// �t���O���~�낷
		m_isTrackingPlayer = false;
		m_hitFlashBullet = false;

		// �^�C�}�[�����Z�b�g
		m_addTimer[m_TimerState_HitByaFlashbang] = 0.0f;

		Efect_MissingPlayer();
		m_enActionState = m_ActionState_Search_MissingPlayer;
	}
	else {
		// �G�t�F�N�g�̐����t���O��false�ɂ��Ă���
		m_existsEfectDraw[m_EffectState_QuestionMark] = false;
	}
}

void Enemy::Action_GoLocationListenSound(const Vector3& tergetPosition)
{
	if (m_isTrackingPlayer == true) {
		if (m_enemyType == TYPE_CHARGE) {
			m_enActionState = m_ActionState_Charge;
			return;
		}
		else {
			m_enActionState = m_ActionState_Tracking;
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
		m_addTimer[m_TimerState_HitByaSoundbang] += g_gameTime->GetFrameDeltaTime();
		m_enAnimationState = m_enAnimationState_Run;

		// �G�t�F�N�g�̍Đ��t���O��false�ɂ��Ă���
		m_existsEfectDraw[m_EffectState_QuestionMark] = false;
	}
	else if (m_addTimer[m_TimerState_HitByaSoundbang] > TIMER_PLAYER_LOSTUPTO) {
		// ��莞�Ԃ��o�߂����Ƃ�
		m_enActionState = m_ActionState_Search_MissingPlayer;
		m_hearedSoundBullet = false;
		// �^�C�}�[�����Z�b�g
		m_addTimer[m_TimerState_HitByaSoundbang] = 0.0f;
		m_existsEfectDraw[m_EffectState_ExclamationPoint] = false;
		return;
	}
	else {
		m_enActionState = m_ActionState_Search_MissingPlayer;
		// �t���O�����Z�b�g
		m_hearedSoundBullet = false;
		m_existsEfectDraw[m_EffectState_ExclamationPoint] = false;
		return;
	}
}

void Enemy::Action_CrawPath()
{
	if (m_isTrackingPlayer == true) {
		if (m_enemyType == TYPE_CHARGE) {
			m_enActionState = m_ActionState_Charge;
			return;
		}
		m_enActionState = m_ActionState_Tracking;
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

		m_addTimer[m_TimerState_StayOnThePath] = 0.0f;	// �^�C�}�[�����Z�b�g
	}

	// �G�l�~�[����v���C���[�֌������x�N�g��
	Vector3 moveSpeed = m_point->s_position - m_position;
	// ���K��
	moveSpeed.Normalize();
	// ��]��������
	moveSpeed *= MOVE_SPEED;
	Rotation(moveSpeed);

	if (Action_StopMove(TIMER_WAITING, m_TimerState_StayOnThePath) == true) {
		m_enAnimationState = m_enAnimationState_Walk;
		m_position += moveSpeed;
	}
	else {
		m_enAnimationState = m_enAnimationState_Idle;
	}
}

void Enemy::Action_TrackingPlayer()
{
	// �i�r���b�V�����쐬
	CreateNavimesh(m_playerManagement->GetPosition());

	if (m_isChachPlayer == false) {
		m_enAnimationState = m_enAnimationState_Run;
	}

	// ����������
	if (m_isTrackingPlayer == false) {
		// �v���C���[�̍��W���Q�Ƃ���
		m_playerMissiongPosition = m_playerManagement->GetPosition();
		m_enActionState = m_ActionState_Move_MissingPositon;
	}
}

void Enemy::Action_MissingPlayer()
{
	m_addTimer[m_TimerState_StayOnThePath] = 0.0f;	// �^�C�}�[�����Z�b�g
	m_addTimer[m_TimerState_UntilTheCharge] = 0.0f;	// �ːi����̈ڍs���ɂ�����̃^�C�}�[�����Z�b�g

	// ���߂̃p�X������
	// float�ɍő�l���i�[
	float NowTargetDiff = D3D12_FLOAT32_MAX;
	int NowTargetNum = -1;

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

	CreateNavimesh(m_point->s_position);
	m_enAnimationState = m_enAnimationState_Run;

	// �G�l�~�[����p�X�֌������x�N�g��
	Vector3 diff = m_point->s_position - m_position;
	float length = diff.Length();

	// ���������̂Ƃ�
	if (length <= CHANGING_DISTANCE) {
		m_naviTimer = 0.0f;
		m_enActionState = m_ActionState_Craw;
	}
}

bool Enemy::Action_StopMove(float time,int timerNumber)
{
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
	if (m_enActionState == m_ActionState_Dizzy) {
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

		if (m_shouldCount == false) {
			// �����񐔂�1�����Z
			GameManager::GetInstance()->AddSearchNum();
			// �t���O��true�ɂ��čēx�s��Ȃ��悤�ɂ���
			m_shouldCount = true;
		}
	}
	else {
		if (m_isChachPlayer == false) {
			// �v���C���[�����݂��Ȃ��Ƃ��̓t���O��߂�
			m_shouldCount = false;
		}
	}
	m_spotLight.SetPosition(lightpos);
	m_spotLight.Update();
}

void Enemy::VigilanceCount()
{
	m_addVicount -= g_gameTime->GetFrameDeltaTime();
	if (m_addVicount <= 0.0f)
	{
		//�X�e�[�g�̑J��
		m_gage->GageUp(1, true);
		m_addVicount = VIGILANCETIME;
	}
}