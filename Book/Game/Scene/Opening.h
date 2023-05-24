#pragma once

class Fade;
#include "nature/SkyCube.h"

class Opening : public IGameObject
{
public:
	Opening();
	~Opening();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	/// <summary>
	/// モデルの初期化。
	/// </summary>
	void InitModel();

	/// <summary>
	/// 画像の初期化。
	/// </summary>
	void InitSprite();

	/// <summary>
	/// 計測処理。
	/// </summary>
	void Time();

	/// <summary>
	/// 入力処理。
	/// </summary>
	void Input();

	/// <summary>
	/// アニメーション処理。
	/// </summary>
	void Animation();

	/// <summary>
	/// カメラ処理。
	/// </summary>
	void Camera();

	/// <summary>
	/// アニメーションイベント。
	/// </summary>
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

private:
	enum EnAnimationClip {
		animationClip_Idle,
		animationClip_Walk,
		animationClip_Put,
		animationClip_Num,
	};
	AnimationClip m_animationClips[animationClip_Num];

private:
	ModelRender						m_playerModelRender;			//プレイヤーモデル
	ModelRender						m_wallModelRender;				//壁モデル
	ModelRender						m_backGroundModelRender;		//床モデル

	std::array< SpriteRender, 2 >	m_skipSpriteRender;				//スキップ画像
	std::array< SpriteRender, 4 >	m_filmSpriteRender;				//フィルム画像

	Fade*							m_fade = nullptr;
	SkyCube* m_skyCube = nullptr;

	Vector3							m_cameraPos;					//カメラ座標
	Vector3							m_cameraTarget;					//カメラ注視点
		
	bool							m_isWaitFadeOut = false;		//フェードアウトかどうか
	int								m_cameraScene = 0;				//カメラシーン
	float							m_timer = 0.0f;					//タイマー
	float							m_filmTimer = 0.0f;
	float							m_degree = 360.0f;				//ゲージの角度
};