#include "stdafx.h"
#include "Wall.h"

namespace
{
	const int GAP_SUM = 1;		// 隙間ありの種類
	const int PAINT_SUM = 1;	// 絵画ありの種類
}

Wall::Wall()
{

}
Wall::~Wall()
{

}
bool Wall::Start()
{
	//SetWallModel(ModelState);

	m_wallRender.Init("Assets/modelData/level_test/box.tkm");
	m_wallRender.SetPosition(m_position);
	m_wallRender.SetScale(m_scale);
	m_wallRender.SetRotation(m_rotation);
	
	m_physicsBoxObj.CreateFromModel(m_wallRender.GetModel(), m_wallRender.GetModel().GetWorldMatrix(),m_scale);
	m_physicsBoxObj.Create(m_position, m_rotation);

	m_physicsStaticObj.CreateFromModel(m_wallRender.GetModel(), m_wallRender.GetModel().GetWorldMatrix());
	m_physicsStaticObj.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Wall);	// コリジョン属性を設定
	m_physicsStaticObj.GetRigidBody()->SetPositionAndRotation(m_position, m_rotation);

	m_wallRender.Update();

	return true;
}

void Wall::SetModel(int num)
{
	// 変数を初期化
	int hoge = 0;

	// 壁のモデルを読み込む
	switch (num)
	{
	case NORMAL:
		m_wallRender.Init("Assets/modelData/level_test/box.tkm");
		break;
	case WITH_PANINTING:
		// ランダムに値を返す
		hoge = rand() % PAINT_SUM;
		SetModel_withPainting(hoge);
		break;
	case WITH_GAP:
		// ランダムに値を返す
		hoge = rand() % GAP_SUM;
		SetModel_withGap(hoge);
		break;
	case START:
		m_wallRender.Init("Assets/modelData/level_test/box.tkm");
		break;
	case GOAL:
		m_wallRender.Init("Assets/modelData/level_test/box.tkm");
		break;
	}
}

void Wall::SetModel_withPainting(int num)
{
	// モデル用意してないのでまだ。
}

void Wall::SetModel_withGap(int num)
{
	switch (num) {
	case 0:
		m_wallRender.Init("Assets/modelData/level_test/box.tkm");
		break;
	case 1:
		m_wallRender.Init("Assets/modelData/level_test/box.tkm");
		break;
	case 2:
		m_wallRender.Init("Assets/modelData/level_test/box.tkm");
		break;
	}
}

void Wall::Update()
{
	m_wallRender.SetPosition(m_position);
	m_wallRender.SetScale(m_scale);
	m_wallRender.SetRotation(m_rotation);

	m_wallRender.Update();
}
void Wall::Render(RenderContext& rc)
{
	m_wallRender.Draw(rc);
}