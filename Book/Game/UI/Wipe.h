#pragma once
#include "d3d11.h"

class Wipe : public IGameObject
{
public:
	Wipe();
	~Wipe();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	D3D12_VIEWPORT m_viewPorts[2];

	ModelRender m_modelRender;
};