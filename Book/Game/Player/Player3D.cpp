#include "stdafx.h"
#include "Player3D.h"

#include "Player2D.h"
#include "GameCamera.h"
#include "PlayerManagement.h"
#include "FlashBom.h"
#include "SoundBom.h"
#include "Enemy.h"
#include "Game.h"
#include "GameManager.h"

namespace
{
	const Vector3	BOXSIZE{ 50.0f,120.0f,50.0f };						//・ｽ{・ｽb・ｽN・ｽX・ｽR・ｽ・ｽ・ｽC・ｽ_・ｽ[・ｽﾌ大き・ｽ・ｽ
	const float		SPEEDDOWN = 0.8;									//・ｽ・ｽ・ｽx・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ
	const float		PLAYERSTAMINA = 10.0f;								//・ｽv・ｽ・ｽ・ｽC・ｽ・ｽ・ｽ[・ｽﾌス・ｽ^・ｽ~・ｽi
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

	DeleteGO(m_soundBom);
	DeleteGO(m_flashBom);

}
bool Player3D::Start()
{
	m_characon = new CharacterController;
	Player::Start();
	
	//・ｽM・ｽ・ｽ・ｽe・ｽﾌ呼び出・ｽ・ｽ
	m_flashBom = NewGO<FlashBom>(0, "flashBom");
	//・ｽ・ｽ・ｽ・ｽ・ｽe・ｽﾌ呼び出・ｽ・ｽ
	m_soundBom = NewGO<SoundBom>(0, "soundBom");


	//3Dアニメーションの読み込み
	Player::Animation3D();
	
	//・ｽL・ｽ・ｽ・ｽ・ｽ・ｽR・ｽ・ｽ・ｽ・ｽR・ｽ・ｽ・ｽW・ｽ・ｽ・ｽ・ｽ・ｽﾌ作成
	m_characon->Init(BOXSIZE, m_position);
	m_collisionObject->CreateBox(
	    Vector3(m_position.x,m_position.y+70.0f,m_position.z),
		Quaternion::Identity,
		BOXSIZE
		);
	m_collisionObject->SetIsEnableAutoDelete(false);

	m_modelRender= new ModelRender;
	
	//・ｽ}・ｽl・ｽW・ｽ・ｽ・ｽ・ｽ・ｽg・ｽﾌ呼び出・ｽ・ｽ
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
	m_playerManagement->SetCharacon(m_characon);
	
	//・ｽ・ｽ・ｽf・ｽ・ｽ・ｽ・ｽﾇみ搾ｿｽ・ｽ・ｽ
	m_modelRender->Init("Assets/modelData/player/player.tkm", m_animationClips, m_enAnimationClip_Num, enModelUpAxisZ, true, true, 1, D3D12_CULL_MODE_BACK);
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
	//・ｽﾇに厄ｿｽ・ｽﾜゑｿｽ・ｽﾄゑｿｽ・ｽﾈゑｿｽ・ｽﾈゑｿｽ
	if (m_ghostHit)
	{
		//atn2・ｽﾅ３D・ｽﾌ会ｿｽ]・ｽ・ｽ・ｽ・ｽﾟゑｿｽ
		angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
		//・ｽv・ｽ・ｽ・ｽC・ｽ・ｽ・ｽ[・ｽﾌ擾ｿｽ・ｽ・ｽ・ｽ・ｽﾄび出・ｽ・ｽ
		Player::Update();
		

		//・ｽv・ｽ・ｽ・ｽC・ｽ・ｽ・ｽ[・ｽﾌ移難ｿｽ・ｽ・ｽp・ｽ・ｽ・ｽ・ｽ・ｽ・ｽB
		//・ｽL・ｽ・ｽ・ｽ・ｽ・ｽR・ｽ・ｽ・ｽﾅ搾ｿｽ・ｽW・ｽ・ｽﾚ難ｿｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽB
		m_characon->SetPosition(m_position);
		m_collisionObject->SetPosition(Vector3(m_position.x, m_position.y + 70.0f, m_position.z));
		m_position = m_characon->Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
		m_modelRender->SetPosition(m_position);
		m_modelRender->SetRotation(m_rotation);
		m_modelRender->Update();
		m_collisionObject->Update();
	}
	//・ｽﾇに厄ｿｽ・ｽﾜゑｿｽ・ｽﾄゑｿｽ・ｽ・ｽﾈゑｿｽ
	else
	{
		PlayerPush();
	}

}

void Player3D::PlayerPush()
{
	m_Player_Act = false;

	senkeiPos += g_gameTime->GetFrameDeltaTime() * 1.5f;
	m_position.Lerp(senkeiPos, GetPushPosition(), GetGhostPosition());
	m_modelRender->SetPosition(m_position);
	m_modelRender->Update();
	m_characon->SetPosition(m_position);
	m_characon->GetRigidBody()->SetPositionAndRotation(m_position, m_rotation);
	//・ｽv・ｽ・ｽ・ｽC・ｽ・ｽ・ｽ[・ｽ・ｽ・ｽ・ｽ・ｽ・ｽo・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽﾉ会ｿｽ]・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ
	m_pushRotPos = GetPushPosition()-GetGhostPosition();
	m_pushRot = atan2(-m_pushRotPos.x, -m_pushRotPos.z);
	m_rotation.SetRotationY(m_pushRot);
	m_modelRender->SetRotation(m_rotation);
	if (senkeiPos >= 1.0f)
	{
		senkeiPos = 0.0f;
		m_ghostHit = true;
		m_playerState = m_enPlayer_Down;
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
	//・ｽX・ｽe・ｽ[・ｽg・ｽ・ｽJ・ｽﾚゑｿｽ・ｽ・ｽB
	ProcessCommonStateTransition();
	Deactivate();
	
}
//・ｽL・ｽ・ｽ・ｽ・ｽ・ｽR・ｽ・ｽ・ｽﾌ作成・ｽﾖ撰ｿｽ
void Player3D::CreatCharcon()
{
	m_characon = new CharacterController;
	m_characon->Init(BOXSIZE, GetPosition());
}
void Player3D::Animation()
{
	//・ｽv・ｽ・ｽ・ｽC・ｽ・ｽ・ｽ[・ｽﾌス・ｽe・ｽ[・ｽg・ｽﾉゑｿｽ・ｽ・ｽﾄ変ゑｿｽ・ｽ・ｽ
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
		m_modelRender->SetAnimationSpeed(1.5f);
		m_modelRender->PlayAnimation(m_enAnimationClip_Jump, 0.8f);
		break;
	case Player::m_enPlayer_Jumpend:
		m_modelRender->PlayAnimation(m_enAnimationClip_Jumpend, 0.8f);
		break;
	case Player::m_enPlayer_Change:
		break;
	case Player::m_enPlayer3D_Throw:
		m_modelRender->PlayAnimation(m_enAnimationClip_Throw, 0.5f);
		break;
	case Player::m_enPlayer_Down:
		//・ｽv・ｽ・ｽ・ｽC・ｽ・ｽ・ｽ[・ｽ・ｽ・ｽ・ｽ・ｽ・ｽo・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽﾉ会ｿｽ]・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ
		m_pushRotPos = GetPushPosition() - GetGhostPosition();
		m_pushRot = atan2(-m_pushRotPos.x, -m_pushRotPos.z);
		m_rotation.SetRotationY(m_pushRot);
		m_modelRender->SetRotation(m_rotation);
		m_modelRender->SetAnimationSpeed(0.6f);
		m_modelRender->PlayAnimation(m_enAnimationClip_Down, 0.5f);
		break;
	case Player::m_enPlayer_Caught:
		m_modelRender->PlayAnimation(m_enAnimationClip_CaughtStart,0.5);
		break;
	case Player::m_enPlayer_Catching:
		m_modelRender->PlayAnimation(m_enAnimationClip_Caught, 0.5);
		break;
	default:
		break;
	}
}

void Player3D::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	(void)clipName;
	//・ｽL・ｽ[・ｽﾌ厄ｿｽ・ｽO・ｽ・ｽAttack_Start・ｽﾌ趣ｿｽ
	if (wcscmp(eventName, L"Attack_Start") == 0)
	{
		switch (m_enItemState)
		{
		//・ｽM・ｽ・ｽ・ｽe・ｽ鰍・ｽ・ｽ・ｽ
		case Player::m_enItem_Flash:
			if (m_flashBom->m_flashCount > 0&&
				m_flashBom->m_FlashState==m_flashBom->m_enFlash_No)
			{
				m_flashBom->m_FlashState = m_flashBom->m_enFlash_Start;
				m_flashBom->SetItemPosition(m_position);
				//SE・ｽ・ｽﾂらす
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(9);
				se->SetVolume(GameManager::GetInstance()->GetSFX());
				se->Play(false);
			}
			break;
		//・ｽ・ｽ・ｽ・ｽ・ｽe・ｽ鰍・ｽ・ｽ・ｽ
		case Player::m_enItem_SoundBom:
			if (m_soundBom->m_soundState == m_soundBom->m_enSoundState_End&&
				m_soundBom->GetSoundBomNumber()>0)
			{
				m_soundBom->Activate();
				m_soundBom->NotifySoundStart();
				m_soundBom->SetItemPosition(m_position);
			}
			break;
		default:
			break;
		}

		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(8);
		se->SetVolume(GameManager::GetInstance()->GetSFX());
		se->Play(false);
		
	}
	else if (wcscmp(eventName, L"Step") == 0)
	{
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(6);
		se->SetVolume(GameManager::GetInstance()->GetSFX());
		se->Play(false);
	}
	else if (wcscmp(eventName, L"Jump_end") == 0)
	{
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(7);
		se->SetVolume(GameManager::GetInstance()->GetSFX());
		se->Play(false);
	}

}




void Player3D::ProcessIdleStateTransition()
{
	//・ｽX・ｽe・ｽ[・ｽg・ｽ・ｽJ・ｽﾚゑｿｽ・ｽ・ｽB
	ProcessCommonStateTransition();
}
void Player3D::ProcessWalkStateTransition()
{
	//・ｽX・ｽe・ｽ[・ｽg・ｽ・ｽJ・ｽﾚゑｿｽ・ｽ・ｽB
	ProcessCommonStateTransition();
}
void Player3D::ProcessRunStateTransition()
{
	//・ｽX・ｽe・ｽ[・ｽg・ｽ・ｽJ・ｽﾚゑｿｽ・ｽ・ｽB
	ProcessCommonStateTransition();
}
void Player3D::ProcessJumpStateTransition()
{
	if (m_modelRender->IsPlayingAniamtion() == false)
	{
		m_playerState = m_enPlayer_Jumpend;

	}
}
void Player3D::ProcessJumpendStateTransition()
{

	if (m_modelRender->IsPlayingAniamtion() == false && m_characon->IsOnGround())
	{
		//・ｽX・ｽe・ｽ[・ｽg・ｽ・ｽJ・ｽﾚゑｿｽ・ｽ・ｽB
		ProcessCommonStateTransition();
	}

}
void Player3D::ProcessChangeStateTransition()
{

	//・ｽX・ｽe・ｽ[・ｽg・ｽ・ｽJ・ｽﾚゑｿｽ・ｽ・ｽB
	ProcessCommonStateTransition();
}

void Player3D::ProcessDownStateTransition()
{
	//・ｽ・ｽ・ｽx・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ
	m_moveSpeed.x = 0;
	m_moveSpeed.z = 0;
	auto laststar = m_game->GetEnemyList().size();
	if (m_modelRender->IsPlayingAniamtion() == false)
	{
		//・ｽX・ｽe・ｽ[・ｽg・ｽﾌ遷・ｽ・ｽ
		ProcessCommonStateTransition();
		m_Player_Act = true;
		m_modelRender->SetAnimationSpeed(1.0f);
	}
}
void Player3D::ProcessThrowStateTransition()
{
	//・ｽ・ｽ・ｽx・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ
	m_moveSpeed.x *= SPEEDDOWN;
	m_moveSpeed.z *= SPEEDDOWN;
	m_Player_Act = false;
	if (m_modelRender->IsPlayingAniamtion() == false)
	{
		//・ｽX・ｽe・ｽ[・ｽg・ｽ・ｽJ・ｽﾚゑｿｽ・ｽ・ｽB
		ProcessCommonStateTransition();
		m_Player_Act = true;
	}
}
void Player3D::ProcessStealStateTransition()
{

}

void Player3D::ProcessFoundStateTransition()
{

}

void Player3D::ProcessCaughtStateTransition()
{
	//・ｽ・ｽ・ｽx・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ
	m_moveSpeed.x *= 0.0f;
	m_moveSpeed.z *= 0.0f;
	m_Player_Act = false;
	if (m_modelRender->IsPlayingAniamtion() == false)
	{
		m_playerState = m_enPlayer_Catching;
	}
}

void Player3D::ProcessCatchingStateTransition()
{
	
	m_catchTime -= g_gameTime->GetFrameDeltaTime();
	if (m_catchTime < 0.0f)
	{
		m_catchTime = 2.0f;
	}
}

void Player3D::ProcessClearStateTransition()
{


}

void Player3D::ProcessGameOverStateTransition()
{


}
void Player3D::Render(RenderContext& rc)
{
	m_modelRender->Draw(rc);
}