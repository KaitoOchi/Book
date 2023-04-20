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
	SphereCollider m_sphereCollider;					//�X�t�B�A�R���C�_�[
	RigidBody m_rigidBody;								//����
};

