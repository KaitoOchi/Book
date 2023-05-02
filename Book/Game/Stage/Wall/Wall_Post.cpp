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
	// 柱
	m_wallRender.Init("Assets/modelData/level_test/tkm/post.tkm", 0, 0, enModelUpAxisZ,true, true, 0);
	// 当たり判定を作成
	m_physicsBoxObj.Create(Vector3{ 60.0f,150.0f,50.0f }, m_position, m_rotation);
	Wall::Start();

	return true;
}

void Wall_Post::Render(RenderContext& rc) 
{
	Wall::Render(rc);
}