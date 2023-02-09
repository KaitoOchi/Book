#include "../PreCompile.h"
#include "CollisionObject.h"


namespace nsBookEngine {

	CollisionObject::CollisionObject()
	{

	}

	CollisionObject::~CollisionObject()
	{
		if (g_collisionObjectManager) {
			g_collisionObjectManager->RemoveCollisionObject(this);
		}
	}

	bool CollisionObject::Start()
	{
		g_collisionObjectManager->AddCollisionObject(this);
		return true;
	}

	void CollisionObject::Update()
	{
		if (m_isEnableAutoDelete == false) {
			return;
		}

		if (m_time > m_timeLimit + 0.1f) {
			DeleteGO(this);
		}
		m_time += g_gameTime->GetFrameDeltaTime();
	}

	CollisionObjectManager::CollisionObjectManager()
	{

	}

	CollisionObjectManager::~CollisionObjectManager()
	{
		m_collisionObjectVector.clear();
	}
}