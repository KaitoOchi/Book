#include "stdafx.h"
#include "Wall_Post.h"

Wall_Post::Wall_Post()
{

}

Wall_Post::~Wall_Post()
{

}

bool Wall_Post::Start()
{
	// ’Œ
	m_wallRender.Init("Assets/modelData/level_test/tkm/post.tkm", 0, 0, enModelUpAxisZ,true, true, 0, D3D12_CULL_MODE_BACK, true);
	// “–‚½‚è”»’è‚ðì¬
	m_physicsBoxObj.Create(Vector3{ 40.0f,150.0f,40.0f }, m_position, m_rotation);
	Wall::Start();
	m_physicsBoxObj.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Wall);

	return true;
}

void Wall_Post::Render(RenderContext& rc) 
{
	Wall::Render(rc);
}