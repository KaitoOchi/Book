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
	m_wallRender.Init("Assets/modelData/level_test/tkm/post.tkm", 0, 0, enModelUpAxisZ, 1, 1);
	// “–‚½‚è”»’è‚ğì¬
	m_physicsBoxObj.Create(Vector3{ 60.0f,150.0f,50.0f }, m_position, m_rotation);
	Wall::Start();

	return true;
}

void Wall_Post::Render(RenderContext& rc) 
{
	Wall::Render(rc);
}