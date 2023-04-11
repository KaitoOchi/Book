#include "stdafx.h"
#include "Gost.h"
Gost::Gost()
{

}
Gost::~Gost()
{

}
bool Gost::Start()
{
	//モデルを読み込みモデルの大きさを求める
	m_modelRender.Init("Assets/modelData/level/box.tkm");
	m_modelRender.Update();
	const auto& tkmFile = m_modelRender.GetModel().GetTkmFile();
	const auto& meshParts = tkmFile.GetMeshParts();

	//メッシュを一つづつ調べていく
	for (const auto& mesh : meshParts)
	{
		for (const auto& vertex : mesh.vertexBuffer)
		{
			auto pos = vertex.pos;
			//モデルの横の長さを求める
			if (posXmax < pos.x)
			{
				posXmax = pos.x;
			}
			if (posXmin > pos.x)
			{
				posXmin = pos.x;
			}
			//モデルの高さを求める
			if (posYmax < pos.y)
			{
				posYmax = pos.y;
			}
			if (posYmin > pos.y)
			{
				posYmin = pos.y;
			}
			//モデルの奥行を求める
			if (posZmax < pos.z)
			{
				posZmax = pos.z;
			}
			if (posZmin > pos.z)
			{
				posZmin = pos.z;
			}
		}
	}
	posX = posXmax - posXmin;
	posY = posYmax - posYmin;
	posZ = posZmax - posZmin;
	//サイズを掛ける
	m_boxSize.x = posX * m_scale.x;
	m_boxSize.y = posY * m_scale.y;
	m_boxSize.z = posZ * m_scale.z;
	m_physicsGhostObj.CreateBox(
		m_position,
		m_rotation,
		m_boxSize
	);
	m_physicsGhostObj.GetbtCollisionObject().setUserIndex(enCollisionAttr_Wall);
	return true;
}
void Gost::Update()
{
	m_modelRender.Update();
}