#include "stdafx.h"
#include "Ghost.h"

namespace
{
	const Vector3 LENGTH_MIN = Vector3(1000000.0f, 1000000.0f, 1000000.0f);
}

Ghost::Ghost()
{

}

Ghost::~Ghost()
{

}

bool Ghost::Start()
{
	//モデルを読み込みモデルの大きさを求める
	m_modelRender.Init("Assets/modelData/level_test/tkm/box.tkm");
	m_modelRender.Update();

	return true;
}

Vector3& Ghost::CreateGhostBox()
{
	const auto& tkmFile = m_modelRender.GetModel().GetTkmFile();
	const auto& meshParts = tkmFile.GetMeshParts();

	Vector3		lengthMax;
	Vector3		lengthMin = LENGTH_MIN;

	//メッシュを一つづつ調べていく
	for (const auto& mesh : meshParts)
	{
		for (const auto& vertex : mesh.vertexBuffer)
		{
			auto pos = vertex.pos;
			//モデルの横の長さを求める
			if (lengthMax.x < pos.x)
			{
				lengthMax.x = pos.x;
			}
			if (lengthMin.x > pos.x)
			{
				lengthMin.x = pos.x;
			}
			//モデルの高さを求める
			if (lengthMax.y < pos.y)
			{
				lengthMax.y = pos.y;
			}
			if (lengthMin.y > pos.y)
			{
				lengthMin.y = pos.y;
			}
			//モデルの奥行を求める
			if (lengthMax.z < pos.z)
			{
				lengthMax.z = pos.z;
			}
			if (lengthMin.z > pos.z)
			{
				lengthMin.z = pos.z;
			}
		}
	}

	Vector3 length = lengthMax - lengthMin;
	Vector3 boxSize;

	//サイズを掛ける
	boxSize.x = length.x * m_scale.x;
	boxSize.y = length.y * m_scale.y * 2.0f;
	boxSize.z = length.z * m_scale.z;

	return boxSize;
}
void Ghost::Update()
{
	m_modelRender.Update();
}