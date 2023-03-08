#pragma once

class Player;
class Mirror;

class Debug : public IGameObject
{
public:
	Debug();
	~Debug();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	ModelRender m_stageModelRender;
	ModelRender m_modelRender;
	ModelRender m_animModelRender;
	FontRender m_fontRender;

	CollisionObject* m_playerCollision = nullptr;

	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Run,
		enAnimationClip_Jump,
		enAnimationClip_Num,
	};

	AnimationClip			animationClips[enAnimationClip_Num];

	LevelRender* m_levelRender = nullptr;

	Player* m_player = nullptr;
	Mirror* m_mirror = nullptr;

	PointLight m_pointLight;
};