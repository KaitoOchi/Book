#pragma once
#include "GameManager.h"
#include "level2DRender/Level2DRender.h"


class Title :public IGameObject
{
public:
	Title();
	~Title();
	bool Start();
	void Update();
	void Render(RenderContext &rc);

private:
	/// <summary>
	/// 入力処理。
	/// </summary>
	void Input();

	/// <summary>
	/// 入力による値の更新処理。
	/// </summary>
	void ValueUpdate(bool vertical);

	/// <summary>
	/// アニメーション処理。
	/// </summary>
	void Animation();

	/// <summary>
	/// ステートの遷移処理。
	/// </summary>
	void ManageState();

	/// <summary>
	/// タイトル画面の処理。
	/// </summary>
	void TitleScreen();

	/// <summary>
	/// メニュー画面の処理。
	/// </summary>
	void MenuScreen();

	/// <summary>
	/// ゲームスタート時の処理。
	/// </summary>
	void StartScreen();

	/// <summary>
	/// 設定画面の処理。
	/// </summary>
	void SettingScreen();

	/// <summary>
	/// データ配列にセーブデータを入れて保存する処理。
	/// </summary>
	void SetDataArray()
	{
		m_saveDataArray[0] = m_saveData.bgm;
		m_saveDataArray[1] = m_saveData.sfx;
	}

	/// <summary>
	/// セーブデータにデータ配列を入れて保存する処理。
	/// </summary>
	void SetSaveData()
	{
		m_saveData.bgm = m_saveDataArray[0];
		m_saveData.sfx = m_saveDataArray[1];
		GameManager::GetInstance()->DataSave(m_saveData);
	}

private:
	SpriteRender	m_backGroundSpriteRender;
	SpriteRender	m_titleSpriteRender;
	SpriteRender	m_pressSpriteRender;
	SpriteRender	m_bgmSpriteRender[10];
	SpriteRender	m_sfxSpriteRender[10];
	FontRender		m_menuFontRender;
	FontRender		m_settingFontRender;
	FontRender		m_cursorFontRender;

	FontRender		m_debugFontRender;

	Level2DRender m_level2DRender;

	GameManager::SaveData m_saveData;

	int				m_titleState = 0;			//タイトルステート
	int				m_cursor_vertical = 0;		//縦カーソル
	int				m_cursor_horizontal = 0;	//横カーソル

	float m_alpha = 0.0f;		//色のアルファ値
	float m_timer = 0.0f;		//時間
	float m_saveDataArray[4];
};
