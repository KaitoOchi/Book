#include "stdafx.h"
#include "Title.h"

#include "GameManager.h"
#include "Fade.h"
#include "Game.h"

namespace
{
	const int CURSOR_VERTICAL_MAX[5] = { 0, 3, 2, 0, 3 };		//各ステートの縦カーソル最大値
	const int CURSOR_HORIZONTAL_MAX[4] = { 0, 100, 100, 2};		//各設定の横カーソル最大値
}

Title::Title()
{

}

Title::~Title()
{
	for (auto& sprites : m_sprites)
	{
		m_sprites.pop_back();
	}
}

bool Title::Start()
{
	InitSprite();

	//カメラの設定
	g_camera3D->SetPosition({ 0.0f, 0.0f, -600.0f });
	g_camera3D->SetTarget({ 0.0f, 00.0f, 0.0f });
	g_camera3D->Update();

	//セーブデータのロード
	m_saveData = GameManager::GetInstance()->DataLoad();
	SetDataArray();

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
	m_playerModelRender.SetPosition(Vector3(100.0f, -100.0f, -400.0f));
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

	//ガイド背景画面の設定
	m_guideBackSpriteRender.Init("Assets/sprite/UI/guide/guide_all2.DDS", 1536.0f, 891.0f);
	m_guideBackSpriteRender.SetPosition(Vector3(-50.0f, 35.0f, 0.0f));
	m_guideBackSpriteRender.Update();
	m_sprites.push_back(&m_guideBackSpriteRender);

	//ガイド画面の設定
	m_guideSpriteRender[0].Init("Assets/sprite/UI/guide/guide_playguide.DDS", 1403.0f, 637.0f);
	m_guideSpriteRender[1].Init("Assets/sprite/UI/guide/rule.DDS", 1141.0f, 541.0f);

	for (int i = 0; i < 2; i++) {
		m_guideSpriteRender[i].SetPosition(Vector3(0.0f, -10.0f, 0.0f));
		m_guideSpriteRender[i].Update();
		m_sprites.push_back(&m_guideSpriteRender[i]);
	}



	//設定画面の設定
	m_settingSpriteRender.Init("Assets/sprite/UI/title/setting_all.DDS", 1920.0f, 1080.0f);
	m_sprites.push_back(&m_settingSpriteRender);

	//設定テキストの設定
	m_settingTextSpriteRender[0].Init("Assets/sprite/UI/setting/BGM_text.DDS", 380.0f, 28.0f);
	m_settingTextSpriteRender[1].Init("Assets/sprite/UI/setting/SE_text.DDS", 227.0f, 28.0f);
	m_settingTextSpriteRender[2].Init("Assets/sprite/UI/setting/FPS_text.DDS", 457.0f, 27.0f);

	for (int i = 0; i < 3; i++) {
		m_settingTextSpriteRender[i].SetPosition(Vector3(0.0f, -400.0f, 0.0f));
		m_settingTextSpriteRender[i].Update();
		m_sprites.push_back(&m_settingTextSpriteRender[i]);
	}

	m_gaugeSpriteRender[0].SetPosition(Vector3(-210.2, 166.4f, 0.0f));
	m_gaugeSpriteRender[1].SetPosition(Vector3(-211.3, -36.6f, 0.0f));

	//BGMとSFX音量の画像を設定
	for (int i = 0; i < 2; i++) {
		m_gaugeSpriteRender[i].Init("Assets/sprite/UI/setting/gauge.DDS", 750.0f, 67.0f, AlphaBlendMode_Trans, 2 + i);
		m_gaugeSpriteRender[i].SetPivot(Vector2(0.0f, 0.5f));
		m_gaugeSpriteRender[i].Update();
		m_sprites.push_back(&m_gaugeSpriteRender[i]);
	}

	//カーソル画像の設定
	m_cursorSpriteRender.Init("Assets/sprite/UI/button/tryangle.DDS", 131.0f, 135.0f);
	m_sprites.push_back(&m_cursorSpriteRender);

	//ボタン画像の設定
	m_buttonSpriteRender[0].Init("Assets/sprite/UI/button/text_Abutton.DDS", 287.0f, 152.0f);
	m_buttonSpriteRender[1].Init("Assets/sprite/UI/button/text_Bbutton.DDS", 287.0f, 152.0f);
	m_buttonSpriteRender[2].Init("Assets/sprite/UI/button/text_crosskey.DDS", 198.0f, 133.0f);

	for (int i = 0; i < 3; i++) {
		m_buttonSpritePos[i] = { -825.0f, -375.0f - (i * 50.0f), 0.0f };
		m_buttonSpriteRender[i].SetPivot(Vector2(0.0f, 0.5f));
		m_buttonSpriteRender[i].SetScale(Vector3(0.75f, 0.75f, 0.0f));
		m_buttonSpriteRender[2].SetScale(Vector3(1.0f, 1.0f, 0.0f));
		m_buttonSpriteRender[i].Update();
		m_sprites.push_back(&m_buttonSpriteRender[i]);
	}
}

void Title::Update()
{
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
	Animation();
}

void Title::StateChange()
{
	//一時的な変数の値を入れる
	if (m_animTime < 0.1f && m_animTime > -0.1f) {

		//メニュー画面以降なら
		if (m_titleState_tmp > 1 || m_titleState > 2) {

			//フェードアウトの待機時間
			if (m_isWaitFadeOut) {
				//フェードアウトし終えたら
				if (!m_fade->IsFade()) {
					//フェードインの処理
					m_fade->StartFadeIn();
					m_isWaitFadeOut = false;
					m_titleState = m_titleState_tmp;
					m_animTime = -0.11f;
					EnableButtonSprite();
				}
			}
			else {
				m_isWaitFadeOut = true;
				m_fade->StartFadeOut();
			}
			return;
		}
		else {
			m_titleState = m_titleState_tmp;
			EnableButtonSprite();
		}
	}
	//ステート遷移を終了する
	else if (m_animTime < -1.0f) {

		m_isWaitState = false;
		m_animTime = 1.0f;
	}

	m_animTime -= g_gameTime->GetFrameDeltaTime();

	//画像の透明度を変更
	for (auto& sprites : m_sprites)
	{
		sprites->SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(m_animTime)));
		sprites->Update();
	}
}

void Title::Input()
{
	//Aボタンが押されたら
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		//タイトルとメニュー画面なら
		if (m_titleState_tmp <= 1) {

			m_titleState_tmp = m_cursor_vertical + 1;
			m_cursor_vertical = 1;
			ValueUpdate(true);

			//画像のアニメーションを行う
			m_isWaitState = true;

			IsCanPlaySound(true);
		}
		//説明画面なら
		else if (m_titleState_tmp == 3) {

			if (m_cursor_horizontal != 0) {
				m_titleState_tmp = 1;
				m_cursor_vertical = 1;
				m_isWaitState = true;
			}
			else {
				m_cursor_horizontal++;
			}
			IsCanPlaySound(true);
		}
	}
	//Bボタンが押されたら
	else if (g_pad[0]->IsTrigger(enButtonB))
	{
		if (m_titleState_tmp == 0) {
			return;
		}

		//説明画面なら
		if (m_titleState_tmp == 3) {

			if (m_cursor_horizontal == 1) {
				m_cursor_horizontal = 0;
			}
			else {
				m_titleState_tmp = 1;
				m_cursor_vertical = 1;
				m_isWaitState = true;
			}
		}
		//メニュー画面以降なら
		else if (m_titleState_tmp >= 2) {
			m_titleState_tmp = 1;
			m_cursor_vertical = 1;
			m_isWaitState = true;
		}
		else {
			m_titleState_tmp--;
			m_cursor_vertical = 0;
			m_isWaitState = true;
		}

		IsCanPlaySound(false);
	}

	//上ボタンが押されたら
	if (g_pad[0]->IsTrigger(enButtonUp)) {
		m_cursor_vertical--;
		ValueUpdate(true);
	}
	//下ボタンが押されたら
	else if (g_pad[0]->IsTrigger(enButtonDown)) {
		m_cursor_vertical++;
		ValueUpdate(true);
	}

	//設定画面なら
	if (m_titleState_tmp == 4) {

		//BGM、SFX設定なら
		if (m_cursor_vertical == 1 || m_cursor_vertical == 2) {
			//左ボタンが押されたら
			if (g_pad[0]->IsPress(enButtonLeft)) {
				m_cursor_horizontal--;
				ValueUpdate(false);
			}
			//右ボタンが押されたら
			else if (g_pad[0]->IsPress(enButtonRight)) {
				m_cursor_horizontal++;
				ValueUpdate(false);
			}
		}
		else {
			//左ボタンが押されたら
			if (g_pad[0]->IsTrigger(enButtonLeft)) {
				m_cursor_horizontal--;
				ValueUpdate(false);
			}
			//右ボタンが押されたら
			else if (g_pad[0]->IsTrigger(enButtonRight)) {
				m_cursor_horizontal++;
				ValueUpdate(false);
			}
		}
	}
}

void Title::ValueUpdate(bool vertical)
{
	int cursor_v = m_cursor_vertical;
	int cursor_h = m_cursor_horizontal;

	//範囲外にはみ出さないようにする
	m_cursor_vertical = min(max(m_cursor_vertical, 1), CURSOR_VERTICAL_MAX[m_titleState_tmp]);
	m_cursor_horizontal = min(max(m_cursor_horizontal, 0), CURSOR_HORIZONTAL_MAX[m_cursor_vertical]);

	//メニュー画面なら
	if (m_titleState_tmp == 1) {

		//音を鳴らす
		if (m_cursor_vertical == cursor_v &&
			m_cursor_horizontal == cursor_h)
		{
			Sound(2);
		}
	}
	//設定画面なら
	else if (m_titleState_tmp == 4) {

		//音を鳴らす
		if (m_cursor_vertical == cursor_v &&
			m_cursor_horizontal == cursor_h)
		{
			Sound(2);
		}

		//今保持している設定の値に移動する
		if (vertical) {
			m_cursor_horizontal = m_saveDataArray[m_cursor_vertical - 1];
		}
		//配列に値を保存する
		else {
			m_saveDataArray[m_cursor_vertical - 1] = m_cursor_horizontal;
			//保存する
			SetSaveData();
			GameManager::GetInstance()->SetVolume();

			Sound(2);
		}
	}
}

void Title::Animation()
{
	//時間の処理
	m_timer += g_gameTime->GetFrameDeltaTime();
	if (m_timer > 1.0f)
		m_timer = -0.5f;

	// -t^2 + 2t
	m_alpha = fabsf(-pow(m_timer, 2.0f) + (2 * m_timer));
	m_alpha = min(m_alpha, 1.0f);

	if (m_titleState_tmp == 1 || m_titleState_tmp == 4) {
		m_alpha *= 3.0f;
		m_alpha = max(m_alpha, 1.0f);
	}

	//透明度を変更
	m_cursorSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, m_alpha));
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

	//ゲームスタートなら
	case 2:
		StartScreen();
		break;

	//操作方法画面なら
	case 3:
		HowToScreen();
		break;

	//設定画面なら
	case 4:
		SettingScreen();
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

	if (!m_isWaitState || m_animTime < 0.0f) {
		m_cursorSpriteRender.SetPosition(Vector3(-725.0f,  475.0f + (m_cursor_vertical * -235.0f), 0.0f));
		m_cursorSpriteRender.Update();
	}
}

void Title::StartScreen()
{
	//フェードの待機時間
	if (m_isWaitFadeOut) {
		//フェードし終えたら
		if (!m_fade->IsFade()) {

			//BGMが削除されたら
			if (GameManager::GetInstance()->IsDeleteBGM()) {
				//ゲーム画面へ遷移
				NewGO<Game>(0, "game");
				DeleteGO(this);
			}
		}
	}
	else {
		m_isWaitFadeOut = true;
		m_fade->StartFadeOut();

		//BGMを削除する
		GameManager::GetInstance()->DeleteBGM();
	}
}

void Title::HowToScreen()
{

}

void Title::SettingScreen()
{
	if (!m_isWaitState || m_animTime < 0.0f) {
		//FPSの設定
		if (m_cursor_vertical == 3) {
			m_cursorSpriteRender.SetPosition(Vector3(-200.0f + (m_cursor_horizontal * 275.0f), -240.0f, 0.0f));
		}
		//音量の設定
		else {
			m_cursorSpriteRender.SetPosition(Vector3(-600.0f, 370.0f + (-205.0f * m_cursor_vertical), 0.0f));
		}
		m_cursorSpriteRender.Update();
	}

	RenderingEngine::GetInstance()->GetSpriteCB().clipSize.x = 590.0f + (m_saveDataArray[0] * 7.5f);
	RenderingEngine::GetInstance()->GetSpriteCB().clipSize.y = 590.0f + (m_saveDataArray[1] * 7.5f);
	//BGMとSFX音量のゲージを変更
	for (int i = 0; i < 2; i++) {
		m_gaugeSpriteRender[i].Update();
	}
}

void Title::EnableButtonSprite()
{
	//ボタンの表示状態を設定
	switch (m_titleState)
	{
	case 0:
		for (int i = 0; i < 3; i++) {
			m_enableButtonSprite[i] = false;
		}
		break;

	case 1:
		m_enableButtonSprite[0] = true;
		m_enableButtonSprite[1] = false;
		m_enableButtonSprite[2] = true;
		break;

	case 2:
		break;

	case 3:
		m_enableButtonSprite[0] = true;
		m_enableButtonSprite[1] = true;
		m_enableButtonSprite[2] = false;
		break;

	case 4:
		m_enableButtonSprite[0] = false;
		m_enableButtonSprite[1] = true;
		m_enableButtonSprite[2] = true;
		break;
	}

	int num = 0;

	//ボタンの座標を設定
	for (int i = 0; i < 3; i++) {

		if (m_enableButtonSprite[i] == true) {
			m_buttonSpriteRender[i].SetPosition(m_buttonSpritePos[num]);
			num++;
		}
	}
}

void Title::Render(RenderContext &rc)
{
	m_backGroundModelRender.Draw(rc);

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

	//操作方法画面なら
	case 3:
		m_guideBackSpriteRender.Draw(rc);
		m_guideSpriteRender[m_cursor_horizontal].Draw(rc);
		break;

	//設定画面なら
	case 4:
		for (int i = 0; i < 2; i++) {
			m_gaugeSpriteRender[i].Draw(rc);
		}

		m_settingSpriteRender.Draw(rc);
		m_settingTextSpriteRender[m_cursor_vertical -1].Draw(rc);
		m_cursorSpriteRender.Draw(rc);
		break;
	}

	//ボタン画像の描画
	for (int i = 0; i < 3; i++) {
		if (m_enableButtonSprite[i] == true) {
			m_buttonSpriteRender[i].Draw(rc);
		}
	}
}