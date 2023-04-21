#pragma once
#include "GameManager.h"
#include "level2DRender/Level2DRender.h"

class Fade;

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
	/// スプライトの初期化処理。
	/// </summary>
	void InitSprite();

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
		m_saveDataArray[0] = m_saveData.bgm * 100;
		m_saveDataArray[1] = m_saveData.sfx * 100;

		float frame = m_saveData.frameRate - 60.0f;
		for (int i = 0; i < 2; i++) {
			frame -= 30.0f;

			if (frame < 0) {
				m_saveDataArray[2] = i;
				break;
			}
		}
	}

	/// <summary>
	/// セーブデータにデータ配列を入れて保存する処理。
	/// </summary>
	void SetSaveData()
	{
		m_saveData.bgm = (float)m_saveDataArray[0] / 100.0f;
		m_saveData.sfx = (float)m_saveDataArray[1] / 100.0f;
		m_saveData.frameRate = 60 + (m_saveDataArray[2] * 30);
		GameManager::GetInstance()->DataSave(m_saveData);
	}

private:
	SpriteRender	m_backGroundSpriteRender;		//背景画像
	SpriteRender	m_titleSpriteRender;			//タイトル画像
	SpriteRender	m_pressSpriteRender;			//press画像
	SpriteRender	m_menuSpriteRender[3];			//メニュー画像
	SpriteRender	m_guideSpriteRender;			//操作方法画像

	SpriteRender	m_bgmSpriteRender;			//BGMのメーター
	SpriteRender	m_sfxSpriteRender;			//SFXのメーター
	FontRender		m_frameFontRender;				//フレームレートの表示

	SpriteRender	m_cursorSpriteRender;		//カーソル画像

	FontRender		m_settingFontRender;

	FontRender		m_debugFontRender;

	Level2DRender* m_level2DRender = nullptr;

	GameManager::SaveData m_saveData;

	Fade* m_fade = nullptr;

	bool m_isWaitFadeOut = false;

	int				m_titleState = 0;			//タイトルステート
	int				m_cursor_vertical = 0;		//縦カーソル
	int				m_cursor_horizontal = 0;	//横カーソル

	int m_saveDataArray[4];		//セーブデータの一時的な配列

	float m_alpha = 0.0f;		//色のアルファ値
	float m_timer = 0.0f;		//時間
};
