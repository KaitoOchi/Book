#pragma once
#include "GameManager.h"
#include "Title.h"

class Fade;

class Title_Setting : public IGameObject
{
public:
	Title_Setting();
	~Title_Setting();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

public:
	/// <summary>
	/// タイトルクラスのポインタを設定。
	/// </summary>
	void SetTitlePtr(Title* title)
	{
		m_title = title;
	}

private:
	/// <summary>
	/// 入力処理。
	/// </summary>
	void Input();

	/// <summary>
	/// カーソルの移動処理。
	/// </summary>
	void CursorMove();

	/// <summary>
	/// 入力による値の更新処理。
	/// </summary>
	void ValueUpdate(const bool vertical, const bool vol, const bool setFPS);

	/// <summary>
	/// コマンド処理。
	/// </summary>
	void Command(char command);

	/// <summary>
	/// 画像の更新処理。
	/// </summary>
	void SpriteUpdate();

	/// <summary>
	/// ステートの遷移処理。
	/// </summary>
	void StateChange();

private:
	/// <summary>
	/// データ配列にセーブデータを入れて保存する処理。
	/// </summary>
	void SetDataArray()
	{
		m_saveDataArray[0] = m_saveData.bgm * 100;
		m_saveDataArray[1] = m_saveData.sfx * 100;

		float frame = m_saveData.frameRate - 60.0f;
		for (int i = 0; i < 3; i++) {
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
	/// セーブデータをリセットする処理。
	/// </summary>
	void ResetSaveData()
	{
		//SaveData構造体にデフォルトの値を入れてセーブ
		GameManager::SaveData defaultSaveData;
		m_saveData = defaultSaveData;
		GameManager::GetInstance()->DataSave(m_saveData);

		//配列に初期化したセーブデータを入れる
		SetDataArray();

		//値に応じて音量と画像を設定
		GameManager::GetInstance()->SetVolume();
		SpriteUpdate();
		m_cursor_horizontal = 0;
		m_fpsCursorSpriteRender.SetPosition(Vector3(-120.0f, -240.0f, 0.0f));
		m_fpsCursorSpriteRender.Update();

		m_title->Sound(1);
	}

private:
	SpriteRender					m_settingSpriteRender;			//設定画像
	SpriteRender					m_cursorSpriteRender;			//カーソル画像
	SpriteRender					m_fpsCursorSpriteRender;		//FPSカーソル
	SpriteRender					m_catSpriteRender;				//猫画像
	std::array< SpriteRender, 2 >	m_gaugeSpriteRender;			//BGMのメーター
	std::array< SpriteRender, 3 >	m_settingTextSpriteRender;		//設定の説明画像
	std::array< SpriteRender, 3 >	m_buttonSpriteRender;			//ボタン画像
	std::vector<SpriteRender*>		m_sprites;						//SpriteRenderのベクター型
	FontRender						m_percentFontRender;			//パーセント文字
	Fade* m_fade = nullptr;
	Title* m_title = nullptr;
	GameManager::SaveData			m_saveData;						//セーブデータの構造体
	std::array< int, 3 >			m_saveDataArray;				//セーブデータの一時的な配列
	Vector3							m_cursorPos;					//カーソル座標
	char							m_commandList[16] = {"NNNNNNNNNNNNNN0"};	//コマンドリスト
	bool							m_isWaitState = false;			//ステートの遷移待ちかどうか
	bool							m_isWaitFadeOut = false;		//フェード中かどうか
	bool							m_isSetFPS = false;				//FPSをセットするか
	int								m_cursor_vertical = 0;			//縦カーソル
	int								m_cursor_horizontal = 0;		//横カーソル
	int								m_nextCursor = 1;				//次に移動するカーソル
	float							m_timer = 0.0f;					//時間
	float							m_alpha = 0.0f;					//透明度
	float							m_cursorTimer = 1.0f;			//カーソルタイマー
};