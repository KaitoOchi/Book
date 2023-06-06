#pragma once
#include "GameManager.h"
#include "level2DRender/Level2DRender.h"
#include "Fade.h"

/// <summary>
/// タイトルクラス。
/// </summary>
class Title :public IGameObject
{
public:
	Title();
	~Title();
	bool Start();
	void Update();
	void Render(RenderContext &rc);

public:
	/// <summary>
	/// アニメーション処理。
	/// </summary>
	/// <param name="time">時間。</param>
	void Animation(float& time, float& alpha);

	/// <summary>
	/// アクティブ状態を設定。
	/// </summary>
	void SetActive(const bool active)
	{
		m_active = active;

		if (active) {
			m_fade->StartFadeIn();
			m_titleState_tmp = 1;
			m_titleState = 1;
			m_cursor = 1;
			m_animTime = -0.11f;
			m_isWaitState = true;
		}
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
		//射撃音を出す
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(num);
		se->Play(false);
		se->SetVolume(GameManager::GetInstance()->GetSFX());
	}

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
	/// シーンの遷移処理。
	/// </summary>
	void SceneChange();

	/// <summary>
	/// 入力処理。
	/// </summary>
	void Input();

	/// <summary>
	/// 入力による値の更新処理。
	/// </summary>
	void ValueUpdate(bool vertical);

	/// <summary>
	/// カーソルの移動処理。
	/// </summary>
	void CursorMove();

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

private:
	enum EnAnimationClip {
		animationClip_Idle,
		animationClip_Put,
		animationClip_Num,
	};
	AnimationClip m_animationClips[animationClip_Num];

private:
	ModelRender						m_backGroundModelRender;		//背景モデル
	ModelRender						m_playerModelRender;			//プレイヤーモデル
	SpriteRender					m_titleSpriteRender;			//タイトル画像
	SpriteRender					m_pressSpriteRender;			//press画像
	SpriteRender					m_menuSpriteRender;				//メニュー画像
	SpriteRender					m_cursorSpriteRender;			//カーソル画像
	std::array< SpriteRender,2 >	m_buttonSpriteRender;			//ボタン画像
	std::vector< SpriteRender* >	m_sprites;						//SpriteRenderのベクター型
	Level2DRender*					m_level2DRender = nullptr;		//レベルレンダー
	Fade*							m_fade = nullptr;				//フェードクラス
	Vector3							m_cursorPos;					//カーソル座標
	bool							m_isWaitState = false;			//ステートの遷移中かどうか
	bool							m_isWaitFadeOut = false;		//フェード状態かどうか
	bool							m_active = true;				//アクティブ状態
	int								m_titleState = 0;				//タイトルステート
	int								m_titleState_tmp = 0;			//タイトルステートの一時的変数
	int								m_cursor = 0;					//縦カーソル
	int								m_nextCursor = 0;				//次に移動するカーソル
	float							m_alpha = 0.0f;					//色のアルファ値
	float							m_timer = 0.0f;					//時間
	float							m_animTime = 1.0f;				//アニメーション時間
	float							m_cursorTimer = 1.0f;			//カーソルタイマー
};
