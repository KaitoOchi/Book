#pragma once
#include "Object.h"
class Gage;
class Treasure :public Object 
{
public:
	Treasure();
	~Treasure()override;
	bool Start()override ;
	void Update()override;
	void Render(RenderContext& rc);
	void Hit()override;
	float m_gagecount=0;

	

private:
	int						m_treasures = 0;
	int						m_randTreasure = 0;
	Gage*					m_gage = nullptr;
	
};