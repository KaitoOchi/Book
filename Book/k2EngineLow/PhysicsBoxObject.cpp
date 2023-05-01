/*!
* @brief	�ÓI�I�u�W�F�N�g�B
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
		
		//���b�V������Â��ׂĂ���
		for (const auto& mesh : meshParts)
		{
			for (const auto& vertex : mesh.vertexBuffer)
			{
				auto pos = vertex.pos;
				//���f���̉��̒��������߂�
				if (posXmax < pos.x)
				{
					posXmax = pos.x;
				}
				if (posXmin > pos.x)
				{
					posXmin = pos.x;
				}
				//���f���̍��������߂�
				if (posYmax < pos.y)
				{
					posYmax = pos.y;
				}
				if (posYmin > pos.y)
				{
					posYmin = pos.y;
				}
				//���f���̉��s�����߂�
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
		//�T�C�Y���|����
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