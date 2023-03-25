#pragma once
#include "Enemy.h"
class Enemy_Serch:public Enemy
{
public:
	Enemy_Serch();
	~Enemy_Serch();
	bool Start();
	void Update();
	void Rotaition();
	void LightRotaition();
	void Render(RenderContext& rc);
		
private:
	//SpotLight m_spotLight;				//スポットライト
	Quaternion m_rot;					//回転
	bool m_rotState=true;				//回転の向きの切替
	float m_rotTime = 2.0f;
};

