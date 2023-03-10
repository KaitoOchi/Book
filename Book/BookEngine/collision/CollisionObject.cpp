#include "BookEnginePreCompile.h"
#include "CollisionObject.h"


namespace nsBookEngine {

	CollisionObject::CollisionObject()
	{
	}

	CollisionObject::~CollisionObject()
	{
		if (g_bookEngine->GetCollisionObjectManager()) {
			g_bookEngine->GetCollisionObjectManager()->RemoveCollisionObject(this);
		}
	}

	bool CollisionObject::Start()
	{
		g_bookEngine->GetCollisionObjectManager()->AddCollisionObject(this);
		return true;
	}

	void CollisionObject::Update()
	{
		if (m_isEnableAutoDelete == false)
		{
			return;
		}


		if (m_time > m_timeLimit + 0000.1f)
		{
			DeleteGO(this);
		}
		m_time += g_gameTime->GetFrameDeltaTime();
	}


	CollisionObjectManager::CollisionObjectManager()
	{
		int a = 1;
	}

	CollisionObjectManager::~CollisionObjectManager()
	{
		int a = 1;
		m_collisionObjectVector.clear();
		m_findsCollisionObjectVector.clear();
		m_findMatchForwardNameCollisionObjectVector.clear();

	}
}