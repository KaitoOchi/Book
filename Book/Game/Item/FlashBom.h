#pragma once
#include "Item.h"
class Game;

class FlashBom:public Item
{
public:
	FlashBom();
	~FlashBom()override;
	bool Start()override;
	void Update();
	void ItemHit()override;
private:
	SphereCollider m_sphereCollider;					//スフィアコライダー
	RigidBody m_rigidBody;								//動態
};

