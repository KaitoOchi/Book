#pragma once
#include "Object.h"
class Treasure :public Object 
{
public:
	Treasure();
	~Treasure();
	bool Start()override ;
	void Update()override;
	void Render(RenderContext& rc);
	void Hit()override;
	int m_gagecount=0;
private:

};