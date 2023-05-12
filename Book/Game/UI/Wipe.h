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
	D3D11_VIEWPORT viewPorts[2];
	ID3D11DeviceContext* d3dDeviceContext;

	ModelRender m_modelRender;
};