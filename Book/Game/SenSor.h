#pragma once
#include "Object.h"

class SenSor : public IGameObject, public Object
{
public:
	SenSor();
	~SenSor();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc);

private:
	/// <summary>
	/// ���������Ƃ��̏����B
	/// </summary>
	void Hit() override;

private:
};

