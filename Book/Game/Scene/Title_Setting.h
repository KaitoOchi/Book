#pragma once
#include "GameManager.h"

class Fade;
class Title;

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
	/// 入力による値の更新処理。
	/// </summary>
	void ValueUpdate(bool vertical);

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
	SpriteRender					m_settingSpriteRender;			//設定画像
	SpriteRender					m_cursorSpriteRender;			//カーソル画像
	std::array< SpriteRender, 2 >	m_gaugeSpriteRender;			//BGMのメーター
	std::array< SpriteRender, 3 >	m_settingTextSpriteRender;		//設定の説明画像
	std::array< SpriteRender, 2 >	m_buttonSpriteRender;			//ボタン画像

	std::vector<SpriteRender*>		m_sprites;						//SpriteRenderのベクター型

	Fade* m_fade = nullptr;
	Title* m_title = nullptr;

	GameManager::SaveData			m_saveData;						//セーブデータの構造体
	std::array< int, 4 >			m_saveDataArray;				//セーブデータの一時的な配列

	bool							m_isWaitState = false;			//ステートの遷移待ちかどうか
	bool							m_isWaitFadeOut = false;		//フェード中かどうか
	int								m_cursor_vertical = 0;			//縦カーソル
	int								m_cursor_horizontal = 0;		//横カーソル
	float							m_timer = 0.0f;					//時間
	float							m_alpha = 0.0f;					//透明度
};