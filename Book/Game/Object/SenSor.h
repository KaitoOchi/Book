#pragma once
#include "Object.h"

class Gage;

class Sensor : public Object
{
public:
	Sensor();
	~Sensor()override;
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc);

private:
	/// <summary>
	/// 当たり判定の処理。
	/// </summary>
	void Hit() override;

private:
	Gage* m_gage = nullptr;
};

