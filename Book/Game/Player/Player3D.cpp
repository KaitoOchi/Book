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
	const Vector3	BOXSIZE{ 50.0f,120.0f,50.0f };						//ボックスコライダーの大きさ
	const float		SPEED_DOWN = 0.8;									//速度の減少量
	const float		PLAYER_STAMINA = 10.0f;								//プレイヤーの体力
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
	
	//閃光弾を生成
	m_flashBom = NewGO<FlashBom>(0, "flashBom");
	//音爆弾を生成
	m_soundBom = NewGO<SoundBom>(0, "soundBom");


	//3Dアニメーションの読み込み
	Player::Animation3D();
	
	//キャラコンの作成
	m_characon->Init(BOXSIZE, m_position);
	m_collisionObject->CreateBox(
	    Vector3(m_position.x,m_position.y+70.0f,m_position.z),
		Quaternion::Identity,
		BOXSIZE
		);
	m_collisionObject->SetIsEnableAutoDelete(false);

	m_modelRender= new ModelRender;
	
	//PlayerManagementを呼び出す
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
	m_playerManagement->SetCharacon(m_characon);
	
	//モデルの設定
	m_modelRender->Init("Assets/modelData/player/player.tkm", m_animationClips, m_enAnimationClip_Num, enModelUpAxisZ, true, true, ModelRender::enOutlineMode_3DPlayer, D3D12_CULL_MODE_BACK);
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
	//壁に当たっていないなら
	if (m_ghostHit)
	{
		Player::Update();
		

		//キャラコンを使用して
		//プレイヤーの座標とモデルの座標を更新する
		m_characon->SetPosition(m_position);
		m_collisionObject->SetPosition(Vector3(m_position.x, m_position.y + 70.0f, m_position.z));
		m_position = m_characon->Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

		//atn2を使って角度を求める
		Rotation();
		//float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
		//SetAngle(angle);

		m_modelRender->SetPosition(m_position);
		m_modelRender->SetRotation(m_rotation);
		m_modelRender->Update();
		m_collisionObject->Update();
	}
	//壁に埋まっているなら
	else
	{
		PlayerPush();
	}

}

void Player3D::PlayerPush()
{
	m_Player_Act = false;
	//線形補完量を求める
	m_senkeiPos += g_gameTime->GetFrameDeltaTime() * 1.5f;
	//プレイヤーを線形補完を利用して移動させる
	m_position.Lerp(m_senkeiPos, GetPushPosition(), GetGhostPosition());
	m_modelRender->SetPosition(m_position);
	m_modelRender->Update();
	m_characon->SetPosition(m_position);
	m_characon->GetRigidBody()->SetPositionAndRotation(m_position, m_rotation);
	//押し出された方向に回転させる
	m_pushRotPos = GetPushPosition()-GetGhostPosition();
	m_pushRot = atan2(-m_pushRotPos.x, -m_pushRotPos.z);
	m_rotation.SetRotationY(m_pushRot);
	m_modelRender->SetRotation(m_rotation);
	//線形補完量が最大なら
	if (m_senkeiPos >= 1.0f)
	{

		m_senkeiPos = 0.0f;
		m_ghostHit = true;
		//プレイヤーを気絶させる
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
	//ステートの遷移
	ProcessCommonStateTransition();
	Deactivate();
	
}
//キャラコンの生成
void Player3D::CreatCharcon()
{
	m_characon = new CharacterController;
	m_characon->Init(BOXSIZE, GetPosition());
}
void Player3D::Animation()
{
	//プレイヤーのステートごとのアニメーションを再生する
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
		//押し出された方向に回転させる
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
	//イベントなら
	if (wcscmp(eventName, L"Attack_Start") == 0)
	{
		switch (m_enItemState)
		{
		//フラッシュをアクティブにする
		case Player::m_enItem_Flash:
			if (m_flashBom->m_flashCount > 0&&
				m_flashBom->m_FlashState==m_flashBom->m_enFlash_No)
			{
				m_flashBom->m_FlashState = m_flashBom->m_enFlash_Start;
				m_flashBom->SetItemPosition(m_position);
				//SE鳴らす
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(9);
				se->SetVolume(GameManager::GetInstance()->GetSFX());
				se->Play(false);
			}
			break;
		//音爆弾を鳴らす
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
	//ステートの遷移
	ProcessCommonStateTransition();
}
void Player3D::ProcessWalkStateTransition()
{
	//ステートの遷移
	ProcessCommonStateTransition();
}
void Player3D::ProcessRunStateTransition()
{
	//ステートの遷移
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
		//ステートの遷移
		ProcessCommonStateTransition();
	}

}
void Player3D::ProcessChangeStateTransition()
{

	//ステートの遷移
	ProcessCommonStateTransition();
}

void Player3D::ProcessDownStateTransition()
{
	
	//移動速度を無くす
	m_moveSpeed.x = 0;
	m_moveSpeed.z = 0;
	auto laststar = m_game->GetEnemyList().size();
	if (m_modelRender->IsPlayingAniamtion() == false)
	{
		//ステートの遷移
		ProcessCommonStateTransition();
		m_Player_Act = true;
		m_modelRender->SetAnimationSpeed(1.0f);
	}
}
void Player3D::ProcessThrowStateTransition()
{
	//移動速度を無くす
	m_moveSpeed.x *= SPEED_DOWN;
	m_moveSpeed.z *= SPEED_DOWN;
	m_Player_Act = false;
	if (m_modelRender->IsPlayingAniamtion() == false)
	{
		//ステートの遷移
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
	//移動速度を無くす
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