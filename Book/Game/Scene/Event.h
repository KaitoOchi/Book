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

private:
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

	Fade*			m_fade = nullptr;

	Vector3			m_cameraPos;			//カメラ座標
	Vector3			m_cameraTarget;			//カメラ注視点

	int				m_cameraScene = 0;		//カメラシーン
	float			m_timer = 0.0f;			//タイマー
};

