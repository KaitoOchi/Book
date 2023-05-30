#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#include "PlayerManagement.h"
#include "Game.h"
#include "Enemy.h"
#include "Treasure.h"
#include "GameCamera.h"
#include "GameUI.h"
#include "Stage/Wall/Wall.h"
namespace
{
	const float		WALK = 30.0f;										//歩き時の乗算量
	const float		RUN = 60.0f;										//走り時の乗算量
	const float		JUMPVOLUM = 200.0f;									//ジャンプ量
	const float		GRAVITY = 400.0f;									//重力
	const float		SPEEDDOWN = 0.8;									//速度減速率
	const float		PLAYERSTAMINA = 10.0f;								//プレイヤーのスタミナ
	const float		STAMINAHEAL = 2.0f;									//スタミナの回復
	const float		STAMINASTOPHEAL = 3.0f;
	const float		STAMINADOWN = 2.0f;									//スタミナの減少
	const float		STAMINA_BASE_POSITION = 60.0f;						//スタミナベース画像の座標
	const float		STAMINA_GAGE_POSITION = 0.0f;						//スタミナゲージ画像の座標
	const float		STAMINA_COOL_TIME = 1.0f;							//スタミナが回復するまでの時間
	const float 	EFFECTSIZE = 0.5f;
}

Player::Player()
{

}

Player::~Player()
{
}

bool Player::Start()
{

	
	m_stamina = PLAYERSTAMINA;
	
	m_gamecamera = FindGO<GameCamera>("gameCamera");
	m_playerManagement=FindGO<PlayerManagement>("playerManagement");
	m_collisionObject = NewGO<CollisionObject>(0);
	m_game = FindGO<Game>("game");
	//お宝の呼び出し
	m_treasure = FindGO<Treasure>("treaSure");
	m_gameUI = FindGO<GameUI>("gameUI");
	m_gameUI->StaminaGage(m_stamina,m_position);
	//クールタイムを初期化
	m_staminaCoolTime = STAMINA_COOL_TIME;
	return true;
}
void Player::Animation3D()
{
	//アニメーションを読み込む
	m_animationClips[m_enAnimationClip_Idle].Load("Assets/animData/player/idle.tka");
	m_animationClips[m_enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[m_enAnimationClip_Walk].Load("Assets/animData/player/walk_key.tka");
	m_animationClips[m_enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[m_enAnimationClip_Run].Load("Assets/animData/player/run_key.tka");
	m_animationClips[m_enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[m_enAnimationClip_Jump].Load("Assets/animData/player/jump_start.tka");
	m_animationClips[m_enAnimationClip_Jump].SetLoopFlag(false);
	m_animationClips[m_enAnimationClip_Jumpend].Load("Assets/animData/player/jump_end.tka");
	m_animationClips[m_enAnimationClip_Jumpend].SetLoopFlag(false);
	m_animationClips[m_enAnimationClip_Down].Load("Assets/animData/player/confusion.tka");
	m_animationClips[m_enAnimationClip_Down].SetLoopFlag(false);
	m_animationClips[m_enAnimationClip_DownEnd].Load("Assets/animData/player/idle_act.tka");
	m_animationClips[m_enAnimationClip_DownEnd].SetLoopFlag(false);
	m_animationClips[m_enAnimationClip_Throw].Load("Assets/animData/player/attack.tka");
	m_animationClips[m_enAnimationClip_Throw].SetLoopFlag(false);
	m_animationClips[m_enAnimationClip_CaughtStart].Load("Assets/animData/player/sleep_start.tka");
	m_animationClips[m_enAnimationClip_CaughtStart].SetLoopFlag(false);
	m_animationClips[m_enAnimationClip_Caught].Load("Assets/animData/player/sleep.tka");
	m_animationClips[m_enAnimationClip_Caught].SetLoopFlag(true);
}
void Player::Animation2D()
{
	m_player2D[0].InitFromDDSFile(L"Assets/animData/player_2D/idle/idle_1.DDS");
	m_player2D[1].InitFromDDSFile(L"Assets/animData/player_2D/idle/idle_2.DDS");
	m_player2D[2].InitFromDDSFile(L"Assets/animData/player_2D/idle/idle_3.DDS");
	m_player2D[3].InitFromDDSFile(L"Assets/animData/player_2D/walk/walk_1.DDS");
	m_player2D[4].InitFromDDSFile(L"Assets/animData/player_2D/walk/walk_2.DDS");
	m_player2D[5].InitFromDDSFile(L"Assets/animData/player_2D/walk/walk_3.DDS");
	m_player2D[6].InitFromDDSFile(L"Assets/animData/player_2D/walk/walk_4.DDS");
	m_player2D[7].InitFromDDSFile(L"Assets/animData/player_2D/walk/walk_5.DDS");
	m_player2D[8].InitFromDDSFile(L"Assets/animData/player_2D/walk/walk_6.DDS");
	m_player2D[9].InitFromDDSFile(L"Assets/animData/player_2D/jump/jump_1.DDS");
	m_player2D[10].InitFromDDSFile(L"Assets/animData/player_2D/jump/jump_2.DDS");
	m_player2D[11].InitFromDDSFile(L"Assets/animData/player_2D/jump/jump_3.DDS");
	m_player2D[12].InitFromDDSFile(L"Assets/animData/player_2D/jump/jump_4.DDS");
	m_player2D[13].InitFromDDSFile(L"Assets/animData/player_2D/jump/jump_5.DDS");
}
void Player::Update()
{
	m_treasure = FindGO<Treasure>("treaSure");
	//行動できるなら
	if (m_Player_Act&&
		m_playerManagement->GetGameState()
		) {
		
		Rotation();
		Move();
		ItemChange();
		//2Dならお宝の近くに居てもジャンプさせる
		//スタミナがない時はジャンプできない
		if (m_playerManagement->m_enMananagementState == m_playerManagement->m_enPlayer_2DChanging&&
			m_runState != false)
		{
			Jump();
		}
		else if (m_Player_Act && m_playerManagement->GetGameState())
		{
			//お宝のコリジョンがない時は普通にジャンプさせる
			if (m_treasure->GetCollision() == nullptr ||
				m_collisionObject->IsHit(m_treasure->GetCollision()) == false&&
				m_runState != false)
			{
				Jump();
			}
		}
		if (g_pad[0]->IsTrigger(enButtonRB1)&& 
			m_Player_Act&&
			m_playerState!=m_enPlayer_Jump&&
			m_runState==true)
		{
			Throw();
		}
		m_Player_Act = true;
	}

	//空中にいる場合
	if(m_characon->IsOnGround() == false)
	{
		//重力を発生させる
		m_moveSpeed.y -= GRAVITY * g_gameTime->GetFrameDeltaTime();
	}

	PlayerCatch();
	Animation();
	ManageState();
	if (!m_treasure->GetHitState()&&
		!m_gameUI->GetCircleDrawState())
	{
		m_gameUI->StaminaGage(m_stamina, m_position);
	}
	
	for (int i=0; i < m_game->GetWallList().size(); i++)
	{
		m_game->GetWallList()[i]->SetWallRenderPosition(m_position);
	}

}

void Player::Move()
{
	m_Lstic.x = 0.0f;
	m_Lstic.z = 0.0f;
	//速度を初期化
	m_moveSpeed.x *= SPEEDDOWN;
	m_moveSpeed.z *= SPEEDDOWN;
	//左ステックの情報を取得
	m_Lstic.x = g_pad[0]->GetLStickXF();
	m_Lstic.y = g_pad[0]->GetLStickYF();
	//カメラの前方向と、右方向の取得
	Vector3 cameraFoward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	
	//XZ平面での前方方向と右方向を取得
	cameraFoward.y = 0.0f;
	cameraFoward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();
	if (m_runState == true)
	{
		//もしAボタンが押されているなら
		if (g_pad[0]->IsPress(enButtonA) &&
			m_characon->IsOnGround() == true)
		{
			if (m_Lstic.x != 0 || m_Lstic.y != 0)
				PlayerRun();
		}
		else
		{
			//クールタイムがなくなるまで回復しない
			m_staminaCoolTime -= g_gameTime->GetFrameDeltaTime();
			if (m_staminaCoolTime < 0.0f)
			{
				//プレイヤーが動いていないなら
				if (m_moveSpeed.x >= 0.0f &&
					m_moveSpeed.y >= 0.0f &&
					m_moveSpeed.z >= 0.0f)
				{
					//スタミナの回復量を1.5にする
					m_stamina += STAMINASTOPHEAL * g_gameTime->GetFrameDeltaTime();
					m_stamina = min(m_stamina, PLAYERSTAMINA);
				}
				else
				{
					//スタミナの回復量を1にする
					m_stamina += STAMINAHEAL * g_gameTime->GetFrameDeltaTime();
					m_stamina = min(m_stamina, PLAYERSTAMINA);
				}

			}
			//左ステックと歩く速度を乗算させる
			m_moveSpeed += cameraFoward * m_Lstic.y * WALK;
			m_moveSpeed += cameraRight * m_Lstic.x * WALK;
		}
	}
	else
	{
		if (m_stamina == 0.0f)
		{
			TireEffect();
		}
		
		//スタミナがない場合行動できないようにする
		m_moveSpeed *=0.9;
		//スタミナの回復する
		m_stamina += STAMINASTOPHEAL * g_gameTime->GetFrameDeltaTime();
		if (!m_treasure->GetHitState())
		{
			m_stamina = min(m_stamina, PLAYERSTAMINA);
		}
		if (m_treasure == nullptr)
		{
			m_stamina = min(m_stamina, PLAYERSTAMINA);
		}
		
		//スタミナが全快したなら
		if (m_stamina == PLAYERSTAMINA)
		{
			//走れるようにする
			m_runState = true;
			m_tireEffect->Stop();
		}
	}
	
	
}

void Player::PlayerRun()
{
	m_stamina -= STAMINADOWN * g_gameTime->GetFrameDeltaTime();
	m_stamina = max(m_stamina, 0.0f);
	if (m_stamina <= 0.0f)
	{
		m_runState = false;
	}
	//カメラの前方向と、右方向の取得
	Vector3 cameraFoward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	//XZ平面での前方方向と右方向を取得
	cameraFoward.y = 0.0f;
	cameraFoward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();
	//ダッシュをさせる
	//左ステックと走る速度を乗算する
	m_moveSpeed += cameraFoward * m_Lstic.y * RUN;
	m_moveSpeed += cameraRight * m_Lstic.x * RUN;
	m_staminaCoolTime = STAMINA_COOL_TIME;
}

void Player::TireEffect()
{
	m_tireEffect = NewGO<EffectEmitter>(0);
	m_tireEffect->Init(7);

	Vector3 effectPos = m_position;
	Vector3 effectfoward = m_forward;
	


	Vector3 cameraFoward = g_camera3D->GetForward();
	Vector3 pos =m_position-m_gamecamera->GetPosition();
	Vector3 move = m_moveSpeed;
	move.y = 0.0f;
	
	Quaternion rot = Quaternion::Identity;
	move *= -1.0f;

	move.Normalize();
	rot.SetRotationYFromDirectionXZ(move);
	
	effectPos +=move * 10.0f;
	effectPos.y = 120.0f;

	m_tireEffect->SetPosition(effectPos);
	m_tireEffect->SetScale(Vector3::One * EFFECTSIZE);

	m_tireEffect->SetRotation(rot);
	m_tireEffect->Play();
	m_tireEffect->Update();
}


void Player::Jump()
{
	if (GetCharacon() == nullptr)
	{
		return;
	}
	//もし地面についているなら
	if (m_characon->IsOnGround() == true)
	{
		//重力を無くす
		m_moveSpeed.y = 0.0f;
		//もしBボタンが押されたら
		if (g_pad[0]->IsTrigger(enButtonB))
		{
			//ジャンプをする
			m_moveSpeed.y = JUMPVOLUM;
			m_Player_Act = false;

		}
	}
	
}
void Player::Rotation()
{
	//もし少しも動いていないなら
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f) {
		return;
	}

	
	//SetRotationを使用する
	m_rotation.SetRotationY(-angle);
}

void Player::ItemChange()
{
	if (g_pad[0]->IsTrigger(enButtonRight))
	{
		//閃光弾を持っている
		m_enItemState = m_enItem_SoundBom;
	}
	if (g_pad[0]->IsTrigger(enButtonLeft))
	{
		//音爆弾を持っている
		
		m_enItemState = m_enItem_Flash;
	}
}

void Player::Throw()
{
}

void Player::GhostHit()
{
	float NowTargetDiff = D3D12_FLOAT32_MAX;
	for (const auto& ghostposition : m_ghostpositions)
	{
		Vector3 diff = ghostposition - GetPosition();
		float lenght = diff.Length();
		if (NowTargetDiff > lenght)
		{
			NowTargetDiff = lenght;
			SetGhostPosition(ghostposition);
		}
	}

	
}

void Player::PlayerCatch()
{
	for (int i = 0; i < m_game->GetEnemyList().size(); i++)
	{
		if (m_game->GetEnemyList()[i]->m_ActState == m_game->GetEnemyList()[i]->CATCH)
		{
			m_playerState = m_enPlayer_Caught;
			m_gamecamera->SetCameraPositio(m_position);
		}
	}
}

void Player::ProcessCommonStateTransition()
{

	for (int i = 0; i < m_game->GetEnemyList().size(); i++)
	{
		if (m_game->GetEnemyList()[i]->m_ActState == m_game->GetEnemyList()[i]->CATCH && m_playerCaught)
		{
			m_playerState = m_enPlayer_Caught;
			m_playerCaught = false;
			m_gamecamera->SetCameraPositio(m_position);
			return;
		}
	}
	if (GetCharacon() == nullptr)
	{
		return;
	}
	if (m_characon->IsOnGround() == false)
	{
		//ジャンプ中にする
		m_playerState = m_enPlayer_Jump;
	}
	else if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		if (g_pad[0]->IsPress(enButtonA)&&
			m_runState == true)
		{
			//ダッシュ中にする
			m_playerState = m_enPlayer_Run;
		}
		else
		{
			//歩き中にする
			m_playerState = m_enPlayer_walk;
		}
	}
	else
	{
		//待機中にする
		m_playerState = m_enPlayer_Idle;
	}

}



void Player::ProcessIdleStateTransition()
{
}
void Player::ProcessWalkStateTransition()
{
}
void Player::ProcessRunStateTransition()
{
}
void Player::ProcessJumpStateTransition()
{
}
void Player::ProcessJumpendStateTransition()
{
}
void Player::ProcessChangeStateTransition()
{
}
void Player::ProcessCatchingStateTransition()
{

}
void Player::ProcessDownStateTransition()
{
}
void Player::ProcessThrowStateTransition()
{
}
void Player::ProcessCaughtStateTransition()
{
}

void Player::ProcessClearStateTransition()
{
}

void Player::ProcessGameOverStateTransition()
{
}

void Player::ManageState()
{
	switch (m_playerState)
	{
		//待機ステートの時
	case m_enPlayer_Idle:
		//待機ステートのステート遷移処理。
		ProcessIdleStateTransition();
		break;
		//歩きステートの時
	case m_enPlayer_walk:
		//歩きステートのステート遷移処理。
		ProcessWalkStateTransition();
		break;
		//ダッシュステートの時
	case m_enPlayer_Run:
		//ダッシュステートのステート遷移処理。
		ProcessRunStateTransition();
		break;
		//ジャンプステートの時
	case m_enPlayer_Jump:
		//ジャンプステートのステート遷移処理。
		ProcessJumpStateTransition();
		break;
		//ジャンプ終わりステートの時
	case m_enPlayer_Jumpend:
		//ジャンプ終わりのステート遷移処理。
		ProcessJumpendStateTransition();
		break;
		//切替ステートの時
	case m_enPlayer_Change:
		//切替ステートのステート遷移処理。
		ProcessChangeStateTransition();
		break;
	     //投げるとき
	case m_enPlayer3D_Throw:
		ProcessThrowStateTransition();
		break;
		//気絶しているとき
	case m_enPlayer_Down:
		ProcessDownStateTransition();
		break;
	case m_enPlayer_Caught:
		if (m_tireEffect == nullptr)
		{
			ProcessCaughtStateTransition();
			break;
		}
		if (m_tireEffect->IsPlay() == true)
		{
			m_tireEffect->Stop();
		}
		ProcessCaughtStateTransition();
		break;
	case m_enPlayer_Catching:
		ProcessCatchingStateTransition();
		break;
	default:
		break;
	}
}

void Player::Animation()
{
}

void Player::Render(RenderContext& rc)
{
	
}

