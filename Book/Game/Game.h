#pragma once


class Player;

class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	ModelRender m_stageModelRender;
	ModelRender m_modelRender;
	ModelRender m_animModelRender;
	FontRender m_fontRender;
	PhysicsStaticObject m_demobg;

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

	PointLight m_pointLight;
};

