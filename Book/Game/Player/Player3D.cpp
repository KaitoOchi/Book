#include "stdafx.h"
#include "Player3D.h"
#include"Player2D.h"
#include "GameCamera.h"
#include "PlayerManagement.h"
#include "FlashBom.h"
#include "SoundBom.h"
namespace
{
	const Vector3 BOXSIZE{ 50.0f,120.0f,50.0f };//�{�b�N�X�R���C�_�[�̑傫��
}
Player3D::Player3D()
{

}
Player3D::~Player3D()
{
	if (IsActive())
	{
		delete(m_characon);
		delete(m_modelRender);		
	}
	DeleteGO(m_collisionObject);

}
bool Player3D::Start()
{
	m_characon = new CharacterController;
	Player::Start();
	
	//�M���e�̌Ăяo��
	m_flashBom = FindGO<FlashBom>("flashBom");
	//�����e�̌Ăяo��
	m_soundBom = FindGO<SoundBom>("soundBom");

	//3D�A�j���[�V�����̓ǂݍ���
	Player::Animation3D();
	
	//�L�����R����R���W�����̍쐬
	m_characon->Init(BOXSIZE, m_position);
	m_collisionObject->CreateBox(
	    Vector3(m_position.x,m_position.y+70.0f,m_position.z),
		Quaternion::Identity,
		BOXSIZE
		);
	m_collisionObject->SetIsEnableAutoDelete(false);

	m_modelRender= new ModelRender;
	
	//�}�l�W�����g�̌Ăяo��
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
	m_playerManagement->SetCharacon(m_characon);
	
	//���f����ǂݍ���
	m_modelRender->Init("Assets/modelData/player/player.tkm", m_animationClips, m_enAnimationClip_Num, enModelUpAxisZ, true, true, D3D12_CULL_MODE_NONE);
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(Quaternion::Identity);
	m_modelRender->SetScale(Vector3::One);
	m_modelRender->Update();

	m_modelRender->AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName)
		{
			OnAnimationEvent(clipName, eventName);
		});


	m_playerManagement->SetCharacon(m_characon);
	return true;
}

void Player3D::Update()
{
	if (GetCharacon() == nullptr)
	{
		return;
	}
	//�ǂɖ��܂��Ă��Ȃ��Ȃ�
	if (m_ghostHit)
	{
		//atn2�łRD�̉�]�����߂�
		angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
		Player::Update();
		//�A�C�e���𓊂���
		if (g_pad[0]->IsTrigger(enButtonRB1) && m_playerState != m_enAnimationClip_Jump)
		{
			Throw();
		}
		//�v���C���[�̈ړ����p������B
		//�L�����R���ō��W���ړ�������B
		m_characon->SetPosition(m_position);
		m_collisionObject->SetPosition(Vector3(m_position.x, m_position.y + 70.0f, m_position.z));
		m_position = m_characon->Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
		m_modelRender->SetPosition(m_position);
		m_modelRender->SetRotation(m_rotation);
		m_modelRender->Update();
		m_collisionObject->Update();
	}
	//�ǂɖ��܂��Ă���Ȃ�
	else
	{
		senkeiPos += g_gameTime->GetFrameDeltaTime()*1.5f;
		m_position.Lerp(senkeiPos,GetPushPosition(), GetGhostPosition());
		m_modelRender->SetPosition(m_position);
		m_modelRender->Update();
		m_characon->SetPosition(m_position);
		m_characon->GetRigidBody()->SetPositionAndRotation(m_position, m_rotation);
		//�v���C���[�������o�������ɉ�]������
		m_pushRotPos = GetPushPosition() - GetGhostPosition();
		m_pushRot=atan2(-m_pushRotPos.x, m_pushRotPos.z);
		m_rotation.SetRotationY(m_pushRot);
		m_modelRender->SetRotation(m_rotation);
		if (senkeiPos >= 1.0f)
		{
			senkeiPos = 0.0f;
			m_ghostHit = true;
		}
	}

}
void Player3D::Throw()
{
	m_playerState = m_enPlayer3D_Throw;
}
void Player3D::PlayerChang()
{
	delete(m_characon);
	m_characon = nullptr;
	//�X�e�[�g��J�ڂ���B
	ProcessCommonStateTransition();
	Deactivate();
	
}
//�L�����R���̍쐬�֐�
void Player3D::CreatCharcon()
{
	m_characon = new CharacterController;
	m_characon->Init(BOXSIZE, GetPosition());
}
void Player3D::Animation()
{
	//�v���C���[�̃X�e�[�g�ɂ���ĕς���
	switch (m_playerState)
	{
	case Player::m_enPlayer_Idle:
		m_modelRender->PlayAnimation(m_enAnimationClip_Idle, 0.5f);
		break;
	case Player::m_enPlayer_walk:
		m_modelRender->PlayAnimation(m_enAnimationClip_Walk, 0.1f);
		break;
	case Player::m_enPlayer_Run:
		m_modelRender->PlayAnimation(m_enAnimationClip_Run, 0.1f);
		break;
	case Player::m_enPlayer_Jump:
		m_modelRender->PlayAnimation(m_enAnimationClip_Jump, 0.8f);
		break;
	case Player::m_enPlayer_Jumpend:
		break;
	case Player::m_enPlayer_Change:
		break;
	case Player::m_enPlayer3D_Throw:
		m_modelRender->PlayAnimation(m_enAnimationClip_Throw, 0.5f);

		break;
	default:
		break;
	}
}

void Player3D::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	(void)clipName;
	//�L�[�̖��O��Attack_Start�̎�
	if (wcscmp(eventName, L"Attack_Start") == 0)
	{
		switch (m_enItemState)
		{
		case Player::m_enItem_Flash:
			m_flashBom->Activate();
			break;
		case Player::m_enItem_SoundBom:
			m_soundBom->Activate();
			break;
		default:
			break;
		}
		
	}

}
void Player3D::Render(RenderContext& rc)
{
	m_modelRender->Draw(rc);
}