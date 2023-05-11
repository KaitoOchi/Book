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
	/// アニメーション処理。
	/// </summary>
	void Animation();

	/// <summary>
	/// カメラの処理。
	/// </summary>
	void Camera();

private:
	enum EnAnimationClip {
		animationClip_Idle,
		animationClip_Put,
		animationClip_Num,
	};
	AnimationClip m_animationClips[animationClip_Num];

	enum EnEnemyAnimClip {
		enemyAnimClip_Run,
		enemyAnimClip_Num,
	};
	AnimationClip m_enemyAnimClips[enemyAnimClip_Num];

private:
	ModelRender		m_playerModelRender;
	ModelRender		m_enemyModelRender[3];
	ModelRender		m_volumeLightModelRender;
	Fade*			m_fade = nullptr;
	Vector3			m_tresurePos;			//宝の座標
	Vector3			m_cameraPos;			//カメラ座標
	Vector3			m_cameraTarget;			//カメラ注視点
	bool			m_isWaitFadeOut;		//フェードアウト中
	int				m_cameraScene = 0;		//カメラシーン
	float			m_timer = 0.0f;			//タイマー
};

