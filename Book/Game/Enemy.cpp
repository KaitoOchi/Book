#include "stdafx.h"
#include "Enemy.h"

#include "Player.h"

#define CATCHDECISION 10.0f	// �m�ۂ�������ɂȂ�͈�

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
	m_modelRender.SetPosition({ 0.0f,0.0f,0.0f });

	// �A�j���[�V�����̓ǂݍ���
	//m_animationClips[enEnemyAnimationClip_Idle].Load("Assets/animData/enemy/idle.tka");
	//m_animationClips[enEnemyAnimationClip_Idle].SetLoopFlag(true);
	//m_animationClips[enEnemyAnimationClip_Walk].Load("Assets/animData/enemy/walk.tka");
	//m_animationClips[enEnemyAnimationClip_Walk].SetLoopFlag(true);
	//m_animationClips[enEnemyAnimationClip_Run].Load("Assets/animData/enemy/run.tka");
	//m_animationClips[enEnemyAnimationClip_Run].SetLoopFlag(true);
	//m_animationClips[enEnemyAnimationClip_Attack].Load("Assets/animData/enemy/attack.tka");
	//m_animationClips[enEnemyAnimationClip_Attack].SetLoopFlag(false);
	//m_animationClips[enEnemyAnimationClip_Damege].Load("Assets/animData/enemy/damege.tka");
	//m_animationClips[enEnemyAnimationClip_Damege].SetLoopFlag(false);

	return true;
}

void Enemy::Update()
{
	// �X�V
	//m_modelRender.PlayAnimation(enEnemyAnimationClip_Idle);
	m_modelRender.Update();
}

void Enemy::MoveState()
{
	// �G�l�~�[�̍s������
}

void Enemy::HeadToDestination()
{
	// �ړI�n�Ɍ���������
	// �v���C���[�����������Ƃ�
	if (MissigPlayerFlag) {
		// ���߂̃p�X��T��
		// �p�X�ɖ߂����Ƃ���false�ɂ���
	}
	// �v���C���[�𔭌������Ƃ�
	if (FoundPlayerFlag) {
		// �v���C���[�Ɍ������o�H���쐬
	}	
}

void Enemy::Seach()
{
	// ����p�̏���
}

void Enemy::CatchPlayer()
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

void Enemy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}