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
	// 柱モデルの設定
	m_wallRender.Init("Assets/modelData/level_test/tkm/post.tkm", 0, 0, enModelUpAxisZ,true, true, ModelRender::enOutlineMode_None, D3D12_CULL_MODE_BACK, true);
	
	// 当たり判定を作成
	m_physicsBoxObj.Create(Vector3{ 90.0f,150.0f,90.0f }, m_position, m_rotation);
	m_physicsBoxObj.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Wall);

	Wall::Start();

	return true;
}

void Wall_Post::Render(RenderContext& rc) 
{
	Wall::Render(rc);
}