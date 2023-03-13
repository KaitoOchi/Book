#include "stdafx.h"
#include "Enemy.h"

#include "Player2D.h"
#include "Player3D.h"

#define FIELDOFVIEW Math::PI / 180.0f) * 120.0f // �G�l�~�[�̎���p(�����l120)

namespace
{
	const float		 LINEARCOMPLETION = 0.5f;						// ���`�⊮�̃t���[����
	const float		 CATCHDECISION = 10.0f;					// �v���C���[���m�ۂ������ƂɂȂ�͈�
	const float		 SCALESIZE = 1.3f;						// SetScale�̃T�C�Y
	const Vector3	 BOXSIZE = { 75.0f, 90.0f,60.0f };	// CharacterController�̃T�C�Y
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

	// �v���C���[�̃C���X�^���X��T��
	m_player2D = FindGO<Player2D>("player2d");
	m_player3D = FindGO<Player3D>("player3d");

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
	// �v���C���[�����������Ƃ�
	if (MissigPlayerFlag) {
		// ����Ɉڍs����
		m_enEnemyActState = m_enEnemyActState_Craw;

		// ���߂̃p�X��T��
		// �p�X�ɖ߂����Ƃ���false�ɂ���
	}

	// �v���C���[�𔭌������Ƃ�
	if (FoundPlayerFlag) {
		// �ǐՂɈڍs����
		m_enEnemyActState = m_enEnemyActState_Tracking;

		// �v���C���[�Ɍ������o�H���쐬
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
	Vector3 diff =  /*player�̍��W*/ -m_position;

	// �v���C���[�ɂ�����x�߂��Ƃ�
	if (diff.LengthSq() <= 700.0 * 700.0f) {
		// �G�l�~�[����v���C���[�֌������x�N�g���𐳋K��
		diff.Normalize();
		// �G�l�~�[�̐��ʃx�N�g���A�G�l�~�[����v���C���[�֌������x�N�g���̓��ς��v�Z
		float cos = m_forward.Dot(diff);
		// ���ς���p�x���v�Z
		float angle = acosf(cos);
		// �p�x������p��苷���Ƃ�
		if (angle <= (FIELDOFVIEW) {
			// �v���C���[�𔭌�
			flag = true;
		}
	}

	return flag;
}

bool Enemy::CatchPlayer()
{
	// �v���C���[���m�ۂ��鏈��
	// �G�l�~�[����v���C���[�֌������x�N�g�����v�Z����
	//float diff = player->position - m_position;

	//// �x�N�g�������ȉ��̂Ƃ�
	//if (diff <= CATCHDECISION) {
	//	// �߂܂����t���O��true�ɂ���
	//	CatchPlayerFlag = true;

	//	// �߂܂������Ƃ�`����
	//	m_fontRender.SetText(L"�߂܂���");
	//	m_fontRender.SetPosition({ 0.0f, 0.0f, 0.0f });
	//}
}

void Enemy::HitFlashBullet()
{
	// �M���e�����������Ƃ��̏���
}

void Enemy::Animation()
{
	// �A�j���[�V�����̃X�e�[�g
	switch (m_enEnemyAnimationState) {
		// �ҋ@
	case Enemy::m_enEnemyAnimationState_Idle:
		m_modelRender.PlayAnimation(m_enAnimationClip_Idle, LINEARCOMPLETION);
		break;
		// ����
	case Enemy::m_enEnemyAnimationState_Walk:
		m_modelRender.PlayAnimation(m_enAnimationClip_Walk, LINEARCOMPLETION);
		break;
		// ����
	case Enemy::m_enEnemyAnimationState_Run:
		m_modelRender.PlayAnimation(m_enAnimationClip_Run, LINEARCOMPLETION);
		break;
		// �U��
	case Enemy::m_enEnemyAnimationState_Attack:
		m_modelRender.PlayAnimation(m_enAnimationClip_Attack, LINEARCOMPLETION);
		break;
		// ��e
	case Enemy::m_enEnemyAnimationState_Damege:
		m_modelRender.PlayAnimation(m_enAnimationClip_Damege, LINEARCOMPLETION);
		break;
	}
}

void Enemy::Render(RenderContext& rc)
{
	// �`��
	m_modelRender.Draw(rc);
}