#pragma once
#include "Player.h"
class Player3D :public Player
{
public:
	Player3D();
	~Player3D();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	CharacterController m_3Dcharacon;//�L�����R��
	ModelRender m_model3DRender;//3D���f��

};

