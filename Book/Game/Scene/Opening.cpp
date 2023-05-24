#include "stdafx.h"
#include "Opening.h"

#include "Fade.h"
#include "GameManager.h"
#include "Game.h"

namespace
{
	const int		SCENE_MAX = 5;													//シーンの最大数
	const float		CIRCLE_MAX = 360.0f;											//円形ゲージの最大数
	const double	PI = 3.14159;													//円周率

	const bool		CAMERA_SET_POS[SCENE_MAX] = { true, false, true, true, true };	//カメラの座標変更をするか
	const float		SCENE_TIME[SCENE_MAX] = { 3.0f, 3.0f, 3.0f, 2.7f, 2.0f };		//シーン遷移するための時間

	const Vector3	CAMERA_POS[SCENE_MAX] = { { 50.0f, 20.0f, 100.0f },
										{ 170.0f, 50.0f, 30.0f },
										{ 100.0f, 50.0f, 50.0f },
										{ 220.0f, 5.0f, 0.0f },
										{ 0.0f, 50.0f, 200.0f } };			//カメラの座標
	const Vector3	CAMERA_TARGET[5] = { {0.0f, 20.0f, 0.0f},
											{ 0.0f, 50.0f, 30.0f },
											{ 0.0f, 50.0f, 0.0f },
											{ 200.0f, 5.0f, 0.0f },
											{ 0.0f, 75.0f, 0.0f } };			//カメラの注視点
	const Vector3	CAMERA_SPEED[5] = { {0.0f, 0.0f, 50.0f},
											{ 0.0f, 50.0f, 0.0f },
											{ 0.0f, 0.0f, 0.0f },
											{ 0.0f, 0.0f, -150.0f },
											{ 0.0f, 3.0f, 0.0f } };			//カメラ速度
	const Vector3	FILM_POS[4] = { { -750.0f, 0.0f, 0.0f },
									{ 750.0f, 0.0f, 0.0f },
									{ 0.0f, 420.0f, 0.0f },
									{ 0.0f, -420.0f, 0.0f } };			//フィルムの座標
}

Opening::Opening()
{

}

Opening::~Opening()
{
	//ゲームを開始
	NewGO<Game>(0, "game");
	DeleteGO(this);

	GameManager::GetInstance()->SetGameState(GameManager::enState_Game);
}

bool Opening::Start()
{
	m_animationClips[animationClip_Idle].Load("Assets/animData/player/idle.tka");
	m_animationClips[animationClip_Idle].SetLoopFlag(true);
	m_animationClips[animationClip_Walk].Load("Assets/animData/player/event/walk_start.tka");
	m_animationClips[animationClip_Walk].SetLoopFlag(false);
	m_animationClips[animationClip_Put].Load("Assets/animData/player/idle_act.tka");
	m_animationClips[animationClip_Put].SetLoopFlag(false);

	//プレイヤーモデルの設定
	m_playerModelRender.Init("Assets/modelData/player/player.tkm", m_animationClips, animationClip_Num, enModelUpAxisZ, true, true, 0, D3D12_CULL_MODE_BACK);

	//フィルム画像の設定
	m_filmSpriteRender[0].Init("Assets/sprite/UI/event/event_outline3.DDS", 200.0f, 960.0f, AlphaBlendMode_Trans);
	m_filmSpriteRender[1].Init("Assets/sprite/UI/event/event_outline3.DDS", 200.0f, 960.0f, AlphaBlendMode_Trans);
	m_filmSpriteRender[2].Init("Assets/sprite/UI/event/event_outline.DDS", 2000.0f, 80.0f, AlphaBlendMode_Trans, 4);
	m_filmSpriteRender[3].Init("Assets/sprite/UI/event/event_outline2.DDS", 2000.0f, 80.0f, AlphaBlendMode_Trans, 4);
	for (int i = 0; i < 4; i++) {
		m_filmSpriteRender[i].SetPosition(FILM_POS[i]);
		m_filmSpriteRender[i].Update();
	}

	//スキップ画像の設定
	m_skipSpriteRender[0].Init("Assets/sprite/UI/PressAndHoldGauge/skip_base.DDS", 157.0f, 178.0f);
	m_skipSpriteRender[1].Init("Assets/sprite/UI/PressAndHoldGauge/gauge.DDS", 157.0f, 178.0f, AlphaBlendMode_Trans, 5);
	for (int i = 0; i < 2; i++) {
		m_skipSpriteRender[i].SetPosition(Vector3(700.0f, -350.0f, 0.0f));
		m_skipSpriteRender[i].Update();
	}
	RenderingEngine::GetInstance()->GetSpriteCB().clipSize.y = (m_degree * PI) / 180.0f;

	//カメラの設定
	m_cameraPos = CAMERA_POS[0];
	m_cameraTarget = CAMERA_TARGET[0];
	g_camera3D->SetNear(10.0f);
	g_camera3D->SetPosition(m_cameraPos);
	g_camera3D->SetTarget(m_cameraTarget);
	g_camera3D->Update();

	//BGMの設定
	GameManager::GetInstance()->SetBGM(23);

	//フェードの設定
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();

	return true;
}

void Opening::Update()
{
	//フェードの待機時間
	if (m_isWaitFadeOut) {
		//フェードが終了したら
		if (!m_fade->IsFade()) {
			DeleteGO(this);
		}
	}
	else {
		//フェードアウトを始める
		if (m_cameraScene == SCENE_MAX - 1 &&
			m_timer > 1.0f) {
			m_isWaitFadeOut = true;
			m_fade->SetEnableTips(false);
			m_fade->StartFadeOut();
		}
	}

	Time();

	Input();
}

void Opening::Time()
{
	//時間を計測
	m_timer += g_gameTime->GetFrameDeltaTime();

	//フィルム用の時間を計測
	m_filmTimer += 0.0005;
	RenderingEngine::GetInstance()->GetSpriteCB().clipSize.z = m_filmTimer;
}

void Opening::Input()
{
	//Aボタンが押されたら
	if (g_pad[0]->IsPress(enButtonA)) {
		m_degree -= 120.0f * g_gameTime->GetFrameDeltaTime();

		//ゲージが最大になったらスキップ
		if (m_degree < 0.0f) {
			m_isWaitFadeOut = true;
			m_fade->SetEnableTips(false);
			m_fade->StartFadeOut();
		}
	}
	else {
		//押されてない時、徐々に減少
		m_degree += 60.0f * g_gameTime->GetFrameDeltaTime();
		m_degree = min(m_degree, CIRCLE_MAX);
	}
	RenderingEngine::GetInstance()->GetSpriteCB().clipSize.y = (m_degree * PI) / 180.0f;
}

void Opening::Camera()
{
	//カメラ速度を設定
	Vector3 cameraSpeed;

	cameraSpeed = CAMERA_SPEED[m_cameraScene] * g_gameTime->GetFrameDeltaTime() * 1.0f;
	m_cameraPos += cameraSpeed;
	m_cameraTarget += cameraSpeed;

	//カメラの遷移時間が経過したら
	if (m_timer > SCENE_TIME[m_cameraScene]) {
		m_cameraScene += 1;
		m_timer = 0.0f;

		//カメラの位置を設定
		m_cameraPos = CAMERA_POS[m_cameraScene];
		m_cameraTarget = CAMERA_TARGET[m_cameraScene];
	}

	//カメラが移動可能なシーンなら
	if (CAMERA_SET_POS[m_cameraScene]) {

		//カメラの移動
		if (m_cameraScene == 3) {
			g_camera3D->SetPosition(m_cameraPos);
			g_camera3D->SetTarget(m_cameraTarget);
		}
		else {
			g_camera3D->SetPosition(m_cameraPos);
			g_camera3D->SetTarget(m_cameraTarget);
		}
		g_camera3D->Update();
	}
}


void Opening::Render(RenderContext& rc)
{
	//フィルム画像の描画
	for (int i = 0; i < 4; i++) {
		m_filmSpriteRender[i].Draw(rc);
	}

	//ゲージ画像の描画
	if (m_degree < 360.0f) {
		m_skipSpriteRender[1].Draw(rc);
		m_skipSpriteRender[0].Draw(rc);
	}
}