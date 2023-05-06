#pragma once
#include "Object.h"

class Gage;

class SenSor : public Object
{
public:
	SenSor();
	~SenSor()override;
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

