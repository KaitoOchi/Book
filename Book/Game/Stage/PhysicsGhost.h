#pragma once
#include "Ghost.h"
class PlayerManagement;
class PhysicsGhost:public Ghost
{
public:
	PhysicsGhost();
	~PhysicsGhost()override;
	bool Start();
	void Update();
	void EffectDraw();
	
	EffectEmitter* GetEffect()
	{
		return m_kirakiraEffect;
	}

public:
	PhysicsGhostObject m_physicsGhostObj;
private:
	EffectEmitter* m_kirakiraEffect;
	PlayerManagement* m_playerManagement;
};

