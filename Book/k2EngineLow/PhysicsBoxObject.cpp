/*!
* @brief	静的オブジェクト。
*/


#include "k2EngineLowPreCompile.h"
#include "PhysicsBoxObject.h"
namespace nsK2EngineLow {
	PhysicsBoxObject::PhysicsBoxObject()
	{
	}
	PhysicsBoxObject::~PhysicsBoxObject()
	{
	}
	void PhysicsBoxObject::CreateFromModel(const Model& model, const Matrix& worldMatrix, const Vector3 scale)
	{
		const auto& tkmFile = model.GetTkmFile();
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
		m_boxSize.x = posX * scale.x;
		m_boxSize.y = posY * scale.y;
		m_boxSize.z = posZ * scale.z;
		m_boxCollider.Create(m_boxSize);
		
	}
	void PhysicsBoxObject::Create(const Vector3 size, const Vector3 pos, const Quaternion rot)
	{
		
		rbInfo.collider = &m_boxCollider;
		rbInfo.mass = 0.0f;
		m_rigidBody.Init(rbInfo);
		m_rigidBody.SetPositionAndRotation(pos, rot);
	}
}