#pragma once
#include "Enemy.h"
class Enemy_Search:public Enemy
{
public:
	Enemy_Search();
	~Enemy_Search();
	bool Start();
	void Update();
	void Rotaition();
	void LightRotaition();
	void Render(RenderContext& rc);

	void Update_OnSearch();				// 索敵
	void Update_OnCall();				// 周りの敵を呼ぶ
	void Update_OnCallEnd();			// 呼ぶ行動の終了時
	void Update_OnConfusion();			// 錯乱
		
private:
	//SpotLight m_spotLight;				//スポットライト
	Quaternion m_rot;					//回転
	bool m_rotState=true;				//回転の向きの切替
	float m_rotTime = 2.0f;
};