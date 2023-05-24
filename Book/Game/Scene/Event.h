#pragma once

class Fade;

class Event : public IGameObject
{
public:
	Event();
	~Event();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

public:
	/// <summary>
	/// 宝の座標を設定。
	/// </summary>
	void SetTresurePosition(const Vector3& pos)
	{
		m_treasurePos = pos;
	}

private:
	/// <summary>
	/// 時間処理。
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
	/// カメラの処理。
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
		animationClip_HeadUp,
		animationClip_HeadStop,
		animationClip_HeadDown,
		animationClip_RunAway,
		animationClip_Num,
	};
	AnimationClip m_animationClips[animationClip_Num];

	enum EnEnemyAnimClip {
		enemyAnimClip_Run,
		enemyAnimClip_Num,
	};
	AnimationClip m_enemyAnimClips[enemyAnimClip_Num];

private:
	ModelRender						m_playerModelRender;				//プレイヤーモデル
	ModelRender						m_treasureModelRender;				//お宝モデル
	ModelRender						m_volumeLightModelRender;			//ボリュームライトモデル
	std::array< ModelRender, 3 >	m_enemyModelRender;					//エネミーモデル
	std::array< SpriteRender, 4 >	m_filmSpriteRender;					//フィルム画像
	std::array< SpriteRender, 2 >	m_skipSpriteRender;					//スキップ画像
	SoundSource*					m_alert = nullptr;
	Fade*							m_fade = nullptr;
	Vector3							m_treasurePos;						//宝の座標
	Vector3							m_cameraPos;						//カメラ座標
	Vector3							m_cameraTarget;						//カメラ注視点
	bool							m_isWaitFadeOut;					//フェードアウト中
	int								m_cameraScene = 0;					//カメラシーン
	float							m_timer = 0.0f;						//タイマー
	float							m_filmTimer = 0.0f;					//フィルム用タイマー
	float							m_degree = 360.0f;					//ゲージの角度
};

