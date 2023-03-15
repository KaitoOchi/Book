#pragma once
#include "Object.h"

class Sensor : public Object
{
public:
	Sensor();
	~Sensor();
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

