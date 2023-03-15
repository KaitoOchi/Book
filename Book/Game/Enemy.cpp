#include "stdafx.h"
#include "Enemy.h"

#include "PlayerManagement.h"

#define FIELDOF_VIEW Math::PI / 180.0f) * 120.0f				// �G�l�~�[�̎���p(�����l120)

namespace
{
	const float		MOVE_SPEED = 8.0f;						// �ړ����x
	const float		CANMOVE_TIMER = 10.0f;					// �ēx�s���ł���܂ł̃^�C�}�[
	const float		LINEAR_COMPLETION = 0.5f;				// ���`�⊮�̃t���[����
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
	// ���f���̓ǂݍ���
	m_modelRender.Init("Assets/modelData/enemy/enemy.tkm");
	m_modelRender.SetScale({ 1.3f, 1.3f, 1.3f });

	// �A�j���[�V�����̓ǂݍ���
	m_animationClips[m_enAnimationClip_Idle].Load("Assets/animData/enemy/idle.tka");
	m_animationClips[m_enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[m_enAnimationClip_Walk].Load("Assets/animData/enemy/walk.tka");
	m_animationClips[m_enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[m_enAnimationClip_Run].Load("Assets/animData/enemy/run.tka");
	m_animationClips[m_enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[m_enAnimationClip_Attack].Load("Assets/animData/enemy/attack1.tka");
	m_animationClips[m_enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClips[m_enAnimationClip_Damege].Load("Assets/animData/enemy/damege.tka");
	m_animationClips[m_enAnimationClip_Damege].SetLoopFlag(false);

	// �L�����N�^�[�R���g���[���[������������
	m_characterController.Init(BOXSIZE, m_position);

	// �C���X�^���X��T��
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");

	return true;
}

void Enemy::Update()
{
	// �X�V
	//HeadToDestination();
	m_modelRender.Update();
}

void Enemy::HeadToDestination()
{
	// �M���e�����������Ƃ�
	if (HitFlashBullet() == true) {
		// �����艺�̏��������s���Ȃ�
		return;
	}

	// �v���C���[�����������Ƃ�
	if (MissigPlayerFlag == true) {
		// ����Ɉڍs����
		m_enEnemyActState = m_enEnemyActState_Craw;

		// ���߂̃p�X��T��
		// �p�X�ɖ߂����Ƃ���false�ɂ���
	}

	// �v���C���[�𔭌������Ƃ�
	if (SeachPlayer() == true) {
		// �ǐՂɈڍs����
		m_enEnemyActState = m_enEnemyActState_Tracking;

		// �v���C���[�Ɍ������o�H���쐬


		// �v���C���[���m�ۂ����Ƃ�
		if (CatchPlayer() == true) {
			// �߂܂������Ƃ�`����
			m_fontRender.SetText(L"�߂܂���");
			m_fontRender.SetPosition({ 0.0f, 0.0f, 0.0f });
		}
	}

	// �ړI�n�Ɍ���������
	Act();
}

void Enemy::Act()
{
	// �s���p�^�[��
	switch (m_enEnemyActState) {
		// ����
	case Enemy::m_enEnemyActState_Craw:
		break;
		// �ǐ�
	case Enemy::m_enEnemyActState_Tracking:
		break;
	}
}

bool Enemy::SeachPlayer()
{
	// ����p�̏���
	// true�̂Ƃ��v���C���[�𔭌����Ă���
	bool flag = false;

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
			flag = true;
		}
	}

	return flag;
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
		flag = true;
	}

	return flag;
}

bool Enemy::HitFlashBullet()
{
	// �M���e�����������Ƃ��̏���
	// �M���e���q�b�g�����Ƃ�true
	bool flag = false;

	// �M���e�����������Ƃ�
	if (HitFlashBulletFlag == true) {
		// ��e
		m_enEnemyAnimationState = m_enEnemyAnimationState_Damege;

		// �ړ����d��
		m_position = m_position;

		HitAfterFlashBullet();
	}

	return flag;
}

void Enemy::HitAfterFlashBullet()
{
	// �M���e������������̍d������
	// �o�ߎ��Ԃ����Z
	float time =+ g_gameTime->GetFrameDeltaTime();

	// ���Z���ꂽ���Ԃ����ȏ�ɂȂ����Ƃ�
	if (CANMOVE_TIMER <= time) {
		// ���Z����o�ߎ��Ԃ����Z�b�g
		time = 0;
		return;
	}
}

void Enemy::Animation()
{
	// �A�j���[�V�����̃X�e�[�g
	switch (m_enEnemyAnimationState) {
		// �ҋ@
	case Enemy::m_enEnemyAnimationState_Idle:
		m_modelRender.PlayAnimation(m_enAnimationClip_Idle, LINEAR_COMPLETION);
		break;
		// ����
	case Enemy::m_enEnemyAnimationState_Walk:
		m_modelRender.PlayAnimation(m_enAnimationClip_Walk, LINEAR_COMPLETION);
		break;
		// ����
	case Enemy::m_enEnemyAnimationState_Run:
		m_modelRender.PlayAnimation(m_enAnimationClip_Run, LINEAR_COMPLETION);
		break;
		// �U��
	case Enemy::m_enEnemyAnimationState_Attack:
		m_modelRender.PlayAnimation(m_enAnimationClip_Attack, LINEAR_COMPLETION);
		break;
		// ��e
	case Enemy::m_enEnemyAnimationState_Damege:
		m_modelRender.PlayAnimation(m_enAnimationClip_Damege, LINEAR_COMPLETION);
		break;
	}
}

void Enemy::Render(RenderContext& rc)
{
	// �`��
	m_modelRender.Draw(rc);
}