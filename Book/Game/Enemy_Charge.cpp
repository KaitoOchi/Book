#include "stdafx.h"
#include "Enemy_Charge.h"

namespace 
{
	const float		LINEAR_COMPLETION = 0.2f;		// ���`�⊮�̃t���[����
	const float		STOP_TIMER = 1.0f;				// ���ߎ���
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

	// ���f���̓ǂݍ���
	m_ChargeModelRender.Init("Assets/modelData/enemy/enemy.tkm", m_animationClips, m_enAnimationClip_Num, enModelUpAxisZ);
	m_ChargeModelRender.SetScale(m_scale);
	m_ChargeModelRender.SetPosition(m_position);
	m_ChargeModelRender.SetRotation(m_rotation);

	Enemy::Start();

	// �����`�Ɉړ�
	m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
	m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z + 300.0f),2 });
	m_pointList.push_back({ Vector3(m_position.x + 500.0f ,m_position.y,m_position.z + 300.0f),3 });
	m_pointList.push_back({ Vector3(m_position.x + 500.0f,m_position.y,m_position.z),4 });

	m_point = &m_pointList[0];

	// ������쐬
	Enemy::SpotLight_New(m_position);

	return true;
}

void Enemy_Charge::Update()
{
	Act();			// �s���p�^�[��
	Animation();	// �A�j���[�V����

	// �X�V
	m_ChargeModelRender.SetScale(m_scale);
	m_ChargeModelRender.SetPosition(m_position);
	m_characterController.SetPosition(m_position);

	// �L�����N�^�[�R���g���[���[�����f���̈ʒu�Ɠ���
	Vector3 move = Vector3::Zero;
	m_position = m_characterController.Execute(move, g_gameTime->GetFrameDeltaTime());

	m_ChargeModelRender.Update();
}

void Enemy_Charge::Act()
{
	Enemy::Act_HitFlashBullet();	// �M���e�ɓ��������Ƃ��̏���

	// �X�|�b�g���C�g
	Enemy::SpotLight_Serch(m_rotation, m_position);

	// �v���C���[�𔭌������Ƃ�
	if (Enemy::Act_SeachPlayer() == true) {
		Enemy::Act_Access();		// �ːi�U��

		if (Enemy::Act_SeachPlayer() == false) {
			Enemy::Act_Craw();		// ����
		}
	}
	else {
		Enemy::Act_Craw();			// ����
	}
}

void Enemy_Charge::Pass(int PassState)
{
	switch (PassState)
	{
		// �c
	case Line:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z - 500.0f),2 });
		break;
		// ��
	case Horizontal:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x + 500.0f,m_position.y,m_position.z),2 });
		break;
		// �E���
	case RightRotation:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x + 500.0f,m_position.y,m_position.z),2 });
		m_pointList.push_back({ Vector3(m_position.x + 500.0f,m_position.y,m_position.z - 500.0f),3 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z - 500.0f),4 });
		break;
		// �����
	case LeftRotation:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x - 500.0f,m_position.y,m_position.z),2 });
		m_pointList.push_back({ Vector3(m_position.x - 500.0f,m_position.y,m_position.z - 500.0f),3 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z - 500.0f),4 });
		break;
		// (����)���p
	case RightAngle:
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z),1 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z - 500.0f),2 });
		m_pointList.push_back({ Vector3(m_position.x - 500.0f,m_position.y,m_position.z - 500.0f),3 });
		m_pointList.push_back({ Vector3(m_position.x,m_position.y,m_position.z + 500.0f),4 });
		break;
	}
}

void Enemy_Charge::Animation()
{
	// �A�j���[�V�����̃X�e�[�g
	switch (m_enEnemyAnimationState) {
		// �ҋ@
	case Enemy::m_enEnemyAnimationState_Idle:
		m_ChargeModelRender.PlayAnimation(m_enAnimationClip_Idle, LINEAR_COMPLETION);
		break;
		// ����
	case Enemy::m_enEnemyAnimationState_Walk:
		m_ChargeModelRender.PlayAnimation(m_enAnimationClip_Walk, LINEAR_COMPLETION);
		break;
		// ����
	case Enemy::m_enEnemyAnimationState_Run:
		m_ChargeModelRender.PlayAnimation(m_enAnimationClip_Run, LINEAR_COMPLETION);
		break;
		// �U��
	case Enemy::m_enEnemyAnimationState_Attack:
		m_ChargeModelRender.PlayAnimation(m_enAnimationClip_Attack, LINEAR_COMPLETION);
		break;
		// ��e
	case Enemy::m_enEnemyAnimationState_Damege:
		m_ChargeModelRender.PlayAnimation(m_enAnimationClip_Damege, LINEAR_COMPLETION);
		break;
	}
}

void Enemy_Charge::Render(RenderContext& rc)
{
	// �`��
	m_ChargeModelRender.Draw(rc);
}