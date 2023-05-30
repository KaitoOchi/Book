#include "stdafx.h"
#include "Title.h"

#include "GameManager.h"
#include "Fade.h"
#include "Title_Setting.h"
#include "Title_Guide.h"
#include "Opening.h"

namespace
{
	const int		CURSOR_VERTICAL_MAX = 3;							//メニュー画面の縦カーソル最大値

	const Vector3	CURSOR_POS_MENU[4] = { { 0.0f,  0.0f, 0.0f },
										{ -725.0f,  80.0f, 0.0f},
										{ -725.0f,  -120.0f, 0.0f},
										{ -725.0f,  -270.0f, 0.0f} };	//メニュー画面のカーソル座標
}

Title::Title()
{
	m_sprites.reserve(8);
}

Title::~Title()
{
	for (auto& sprites : m_sprites)
	{
		m_sprites.pop_back();
	}
	m_sprites.clear();
	m_sprites.shrink_to_fit();
}

bool Title::Start()
{
	RenderingEngine::GetInstance()->SetDirectionLight(Vector3(-1.5f, -1, 1), Vector3(3.0f,3.0f,0.8f));
	RenderingEngine::GetInstance()->SetAmbient(0.0f);

	InitSprite();

	//カメラの設定
	g_camera3D->SetPosition({ 0.0f, 0.0f, -600.0f });
	g_camera3D->SetTarget({ 0.0f, 00.0f, 0.0f });
	g_camera3D->Update();

	//フェードの処理
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();

	//BGMを鳴らす
	GameManager::GetInstance()->SetBGM(20);

	return true;
}

void Title::InitSprite()
{
	//背景を設定
	m_backGroundModelRender.Init("Assets/modelData/title/title_model.tkm", 0, 0, enModelUpAxisZ, false, false, 0, D3D12_CULL_MODE_FRONT);
	m_backGroundModelRender.SetPosition(Vector3(0.0f, 0.0f, 335.0f));
	m_backGroundModelRender.Update();

	//プレイヤーモデルを設定
	m_animationClips[animationClip_Idle].Load("Assets/animData/player/idle.tka");
	m_animationClips[animationClip_Idle].SetLoopFlag(true);
	m_animationClips[animationClip_Put].Load("Assets/animData/player/idle_act.tka");
	m_animationClips[animationClip_Put].SetLoopFlag(false);

	Quaternion rot;
	rot.AddRotationY(Math::DegToRad(-120.0f));

	m_playerModelRender.Init("Assets/modelData/player/player.tkm", m_animationClips, animationClip_Num, enModelUpAxisZ);
	m_playerModelRender.SetPosition(Vector3(100.0f, -100.0f, -380.0f));
	m_playerModelRender.SetRotation(rot);
	m_playerModelRender.Update();


	m_level2DRender = new Level2DRender;
	//レベルのデータを使用してタイトル画像を読み込む。
	m_level2DRender->Init("Assets/level2D/titleLevel.casl", [&](Level2DObjectData& objData) {
		//名前が一致していたら。
		if (objData.EqualObjectName("title") == true) {
			//タイトルを設定
			m_titleSpriteRender.Init(objData.ddsFilePath, objData.width, objData.height);
			m_titleSpriteRender.SetPosition(objData.position);
			m_titleSpriteRender.SetScale(objData.scale);
			m_titleSpriteRender.Update();
			m_sprites.push_back(&m_titleSpriteRender);
			return true;
		}
		else if (objData.EqualObjectName("button") == true) {
			//ボタンを設定
			m_pressSpriteRender.Init(objData.ddsFilePath, objData.width, objData.height);
			m_pressSpriteRender.SetPosition(objData.position);
			m_pressSpriteRender.SetScale(objData.scale);
			m_sprites.push_back(&m_pressSpriteRender);
			return true;
		}
		return false;
	});

	m_menuSpriteRender.Init("Assets/sprite/UI/title/title_2_all.DDS", 1920.0f, 1080.0f);
	m_sprites.push_back(&m_menuSpriteRender);

	//カーソル画像の設定
	m_cursorSpriteRender.Init("Assets/sprite/UI/button/tryangle.DDS", 131.0f, 135.0f);
	m_cursorSpriteRender.SetPosition(CURSOR_POS_MENU[1]);
	m_cursorSpriteRender.Update();
	m_sprites.push_back(&m_cursorSpriteRender);

	//ボタン画像の設定
	m_buttonSpriteRender[0].Init("Assets/sprite/UI/button/text_Abutton.DDS", 287.0f, 152.0f);
	m_buttonSpriteRender[1].Init("Assets/sprite/UI/button/text_crosskey.DDS", 198.0f, 133.0f);
	for (int i = 0; i < 2; i++) {
		m_buttonSpriteRender[i].SetPosition(Vector3(-825.0f, -375.0f - (i * 50.0f), 0.0f));
		m_buttonSpriteRender[i].SetPivot(Vector2(0.0f, 0.5f));
		m_buttonSpriteRender[i].SetScale(Vector3(0.75f, 0.75f, 0.0f));
		m_buttonSpriteRender[1].SetScale(Vector3(1.0f, 1.0f, 0.0f));
		m_buttonSpriteRender[i].Update();
		m_sprites.push_back(&m_buttonSpriteRender[i]);
	}
}

void Title::Update()
{
	if (!m_active) {
		return;
	}

	//ステートの遷移処理
	ManageState();

	if (m_isWaitState) {
		//ステートの遷移中の処理。
		StateChange();

		m_playerModelRender.SetAnimationSpeed(0.9);
		m_playerModelRender.PlayAnimation(animationClip_Put, 0.5f);
		m_playerModelRender.Update();
		return;

	}

	m_playerModelRender.PlayAnimation(animationClip_Idle, 0.5f);
	m_playerModelRender.Update();

	//入力処理
	Input();

	//アニメーション処理
	Animation(m_timer, m_alpha);
}

void Title::StateChange()
{
	m_animTime -= g_gameTime->GetFrameDeltaTime();

	//フェードアウト中の処理
	if (m_isWaitFadeOut) {
		//フェードアウトが終了したら
		if (!m_fade->IsFade()) {
			SceneChange();
		}
		return;
	}
	else {
		//画像のアニメーションが終了したら
		if (m_animTime < 0.0f) {

			m_cursorSpriteRender.SetPosition(CURSOR_POS_MENU[1]);
			m_cursorSpriteRender.Update();
			m_cursorTimer = 1.0f;

			//メニュー画面以降なら
			if (m_titleState_tmp > 1 || m_titleState > 2) {
				//フェードアウトする
				m_isWaitFadeOut = true;
				m_fade->StartFadeOut();
				return;
			}
			else {
				m_titleState = m_titleState_tmp;
			}
		}

		//ステート遷移を終了する
		if (m_animTime < -1.0f) {
			m_isWaitState = false;
			m_animTime = 1.0f;
		}
	}

	//画像の透明度を変更
	for (auto& sprites : m_sprites)
	{
		sprites->SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(m_animTime)));
		sprites->Update();
	}
}

void Title::SceneChange()
{
	//ゲームスタートなら
	if (m_titleState_tmp == 2) {
		//環境光を初期化する
		RenderingEngine::GetInstance()->SetDirectionLight(Vector3(1, -1, 1), Vector3(0.2f, 0.2f, 0.2f));
		RenderingEngine::GetInstance()->SetAmbient(0.1f);

		NewGO<Opening>(0, "opening");
		DeleteGO(this);
	}
	//ガイド画面なら
	else if (m_titleState_tmp == 3) {
		Title_Guide* guide = NewGO<Title_Guide>(0, "title_guide");
		guide->SetTitlePtr(this);
	}
	//設定画面なら
	else if (m_titleState_tmp == 4) {
		Title_Setting* setting = NewGO<Title_Setting>(0, "title_setting");
		setting->SetTitlePtr(this);
	}

	SetActive(false);
	m_isWaitFadeOut = false;
	m_isWaitState = false;

}

void Title::Input()
{
	//カーソルの移動中は、入力を受け付けない
	if (m_cursorTimer < 1.0f) {
		CursorMove();
		return;
	}

	//Aボタンが押されたら
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		//画像のアニメーションを行う
		m_isWaitState = true;

		//可能ならSEを再生
		IsCanPlaySound(true);

		//タイトルとメニュー画面なら
		if (m_titleState_tmp <= 1) {
			//ヒント画像を非表示
			m_fade->SetEnableTips(false);
			m_titleState_tmp = m_cursor + 1;
			m_cursor = 1;
			ValueUpdate(true);

			//ゲームスタートなら
			if (m_titleState_tmp == 2) {
				//ヒント画像を表示する{
				m_fade->SetEnableTips(true);
				//BGMを削除する
				GameManager::GetInstance()->DeleteBGM();
			}
		}
	}
	//Bボタンが押されたら
	else if (g_pad[0]->IsTrigger(enButtonB))
	{
		if (m_titleState_tmp == 0) {
			return;
		}
		//メニュー画面以降なら
		else if (m_titleState_tmp >= 2) {
			m_titleState_tmp = 1;
			m_cursor = 1;
			m_isWaitState = true;
		}
		else {
			m_titleState_tmp--;
			m_cursor = 0;
			m_isWaitState = true;
		}

		IsCanPlaySound(false);
	}

	if (m_titleState != 0) {
		//上ボタンが押されたら
		if (g_pad[0]->IsTrigger(enButtonUp)) {
			m_cursor--;
			ValueUpdate(true);
			m_nextCursor = 1;
		}
		//下ボタンが押されたら
		else if (g_pad[0]->IsTrigger(enButtonDown)) {
			m_cursor++;
			ValueUpdate(true);
			m_nextCursor = -1;
		}
	}
}

void Title::ValueUpdate(bool vertical)
{
	int cursor_v = m_cursor;

	//範囲外にはみ出さないようにする
	m_cursor = min(max(m_cursor, 1), CURSOR_VERTICAL_MAX);

	//メニュー画面なら
	if (m_titleState_tmp == 1) {

		//音を鳴らす
		if (m_cursor == cursor_v)
		{
			Sound(2);
			m_cursorTimer = 0.0f;
		}
	}
}

void Title::CursorMove()
{
	m_cursorTimer += g_gameTime->GetFrameDeltaTime() * 3.0f;

	// -t^2 + 2t
	float rate = ((pow(m_cursorTimer, 2.0f) * -1.0f) + (2.0f * m_cursorTimer));
	rate = min(rate, 1.0f);

	//カーソルを移動
	m_cursorPos.Lerp(rate, CURSOR_POS_MENU[m_cursor + m_nextCursor], CURSOR_POS_MENU[m_cursor]);
	m_cursorSpriteRender.SetPosition(m_cursorPos);
	m_cursorSpriteRender.Update();
}

void Title::Animation(float& time, float& alpha)
{
	//時間の処理
	time += g_gameTime->GetFrameDeltaTime();
	if (time > 1.0f)
		time = -0.5f;

	// -t^2 + 2t
	alpha = fabsf(-pow(time, 2.0f) + (2 * time));
	alpha = min(alpha, 1.0f);

	if (m_titleState == 1 || m_titleState == 4) {
		alpha *= 3.0f;
		alpha = max(alpha, 1.0f);
	}

	//透明度を変更
	m_cursorSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, alpha));
}

void Title::ManageState()
{
	switch (m_titleState)
	{
	//タイトル画面なら
	case 0:
		TitleScreen();
		break;

	//メニュー画面なら
	case 1:
		MenuScreen();
		break;
	}
}

void Title::TitleScreen()
{
	m_titleSpriteRender.SetPosition(Vector3(-340.0f, 250.0f, 0.0f));
	m_titleSpriteRender.SetScale(Vector3(1.0f, 1.0f, 0.0f));
	m_titleSpriteRender.Update();

	//透明度を変更
	m_pressSpriteRender.SetMulColor(Vector4(m_alpha, m_alpha, m_alpha, m_alpha));
	m_pressSpriteRender.Update();
}

void Title::MenuScreen()
{
	m_titleSpriteRender.SetPosition(Vector3(-675.0f, 400.0f, 0.0f));
	m_titleSpriteRender.SetScale(Vector3(0.25f, 0.25f, 0.0f));
	m_titleSpriteRender.Update();
}

void Title::Render(RenderContext &rc)
{
	m_backGroundModelRender.Draw(rc);

	if (!m_active) {
		return;
	}

	switch (m_titleState)
	{
	//タイトル画面なら
	case 0:
		m_pressSpriteRender.Draw(rc);
		m_titleSpriteRender.Draw(rc);
		m_playerModelRender.Draw(rc);
		break;

	//メニュー画面なら
	case 1:
		m_menuSpriteRender.Draw(rc);
		m_titleSpriteRender.Draw(rc);
		m_playerModelRender.Draw(rc);
		m_cursorSpriteRender.Draw(rc);
		break;

	//ゲームスタート画面なら
	case 2:
		break;
	}

	//ボタン画像の描画
	for (int i = 0; i < 2; i++) {
		m_buttonSpriteRender[i].Draw(rc);
	}
}