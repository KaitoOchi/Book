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
	/// ステートの遷移中の処理。
	/// </summary>
	void StateChange();

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

	/// <summary>
	/// 効果音を鳴らせるかどうか。
	/// </summary>
	/// <param name="sound">鳴らしたい効果音の番号。</param>
	void IsCanPlaySound(const bool sound)
	{
		int title = m_titleState_tmp;
		title = min(max(m_titleState, 0), 4);

		if (m_titleState == title) {
			Sound(sound);
		}
	}

	/// <summary>
	/// 効果音を鳴らす処理。
	/// </summary>
	/// <param name="num">鳴らしたい効果音の番号。</param>
	void Sound(const int num)
	{
		/*

		//射撃音を出す
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(num);
		se->Play(false);
		se->SetVolume(GameManager::GetInstance()->GetSFX());

		*/
	}

private:
	SpriteRender			m_backGroundSpriteRender;		//背景画像
	SpriteRender			m_titleSpriteRender;			//タイトル画像
	SpriteRender			m_pressSpriteRender;			//press画像
	SpriteRender			m_menuSpriteRender;				//メニュー画像
	SpriteRender			m_guideSpriteRender;			//操作方法画像
	SpriteRender			m_settingSpriteRender;			//設定画像
	SpriteRender			m_gaugeSpriteRender[2];			//BGMのメーター
	SpriteRender			m_settingTextSpriteRender[3];	//設定の説明画像
	SpriteRender			m_cursorSpriteRender;			//カーソル画像
	SpriteRender			m_buttonSpriteRender[2];		//ボタン画像

	std::vector<SpriteRender*> m_sprites;					//SpriteRenderのベクター型

	FontRender				m_debugFontRender;

	Level2DRender*			m_level2DRender = nullptr;		//レベルレンダー
	Fade*					m_fade = nullptr;				//フェードクラス
	GameManager::SaveData	m_saveData;
	bool					m_isWaitState = false;			//ステートの遷移中かどうか
	bool					m_isWaitFadeOut = false;		//フェード状態かどうか
	int						m_titleState = 0;				//タイトルステート
	int						m_titleState_tmp = 0;			//タイトルステートの一時的変数
	int						m_cursor_vertical = 0;			//縦カーソル
	int						m_cursor_horizontal = 0;		//横カーソル
	int						m_saveDataArray[4];				//セーブデータの一時的な配列
	float					m_alpha = 0.0f;					//色のアルファ値
	float					m_timer = 0.0f;					//時間
	float					m_animTime = 1.0f;				//アニメーション時間

	Vector3 a;
};
