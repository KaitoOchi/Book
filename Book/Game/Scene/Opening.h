#pragma once

class Fade;

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
	/// 計測処理。
	/// </summary>
	void Time();

	/// <summary>
	/// 入力処理。
	/// </summary>
	void Input();

	/// <summary>
	/// カメラ処理。
	/// </summary>
	void Camera();

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

	std::array< SpriteRender, 2 >	m_skipSpriteRender;				//スキップ画像
	std::array< SpriteRender, 4 >	m_filmSpriteRender;				//フィルム画像

	Fade*							m_fade = nullptr;

	Vector3							m_cameraPos;					//カメラ座標
	Vector3							m_cameraTarget;					//カメラ注視点
		
	bool							m_isWaitFadeOut = false;		//フェードアウトかどうか
	int								m_cameraScene = 0;				//カメラシーン
	float							m_timer = 0.0f;					//タイマー
	float							m_filmTimer = 0.0f;
	float							m_degree = 0.0f;				//ゲージの角度
};