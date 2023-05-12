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
		m_tresurePos = pos;
	}

private:
	/// <summary>
	/// 時間処理。
	/// </summary>
	void Time();

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
	ModelRender		m_playerModelRender;		//プレイヤーモデル
	ModelRender		m_tresureModelRender;		//お宝モデル
	ModelRender		m_enemyModelRender[3];		//エネミーモデル
	ModelRender		m_volumeLightModelRender;	//ボリュームライトモデル
	SpriteRender	m_filmSpriteRender[4];		//フィルム画像
	SoundSource*	m_alert = nullptr;
	Fade*			m_fade = nullptr;
	Vector3			m_tresurePos;				//宝の座標
	Vector3			m_cameraPos;				//カメラ座標
	Vector3			m_cameraTarget;				//カメラ注視点
	bool			m_isWaitFadeOut;			//フェードアウト中
	int				m_cameraScene = 0;			//カメラシーン
	float			m_timer = 0.0f;				//タイマー
	float			m_filmTimer = 0.0f;			//フィルム用タイマー
};

