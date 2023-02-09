#pragma once
#include "../../k2EngineLow/physics/PhysicsGhostObject.h"


namespace nsBookEngine {

	/// <summary>
	/// �R���W�����̓����蔻����s���N���X�B
	/// </summary>
	class CollisionObject : public IGameObject
	{
	public:
		CollisionObject();
		~CollisionObject();
		bool Start();
		void Update();

		/// <summary>
		/// �{�b�N�X�`��̃S�[�X�g�I�u�W�F�N�g���쐬�B
		/// </summary>
		/// <param name="pos">���W�B</param>
		/// <param name="rot">��]�B</param>
		/// <param name="size">�g�嗦�B</param>
		void CreateBox(const Vector3& pos, const Quaternion& rot, const Vector3& size)
		{
			m_physicsGhostObject.CreateBox(pos, rot, size);
		}

		/// <summary>
		/// �J�v�Z���`��̃S�[�X�g�I�u�W�F�N�g���쐬�B
		/// </summary>
		/// <param name="pos">���W�B</param>
		/// <param name="rot">��]�B</param>
		/// <param name="radius">���a�B</param>
		/// <param name="height">�����B</param>
		void CreateCapsule(const Vector3& pos, const Quaternion& rot, float radius, float height)
		{
			m_physicsGhostObject.CreateCapsule(pos, rot, radius, height);
		}

		/// <summary>
		/// ���`��̃S�[�X�g�I�u�W�F�N�g���쐬�B
		/// </summary>
		/// <param name="pos">���W�B</param>
		/// <param name="rot">��]�B</param>
		/// <param name="radius">���a�B</param>
		void CreateSphere(const Vector3& pos, const Quaternion& rot, float radius)
		{
			m_physicsGhostObject.CreateSphere(pos, rot, radius);
		}

		/// <summary>
		/// ���b�V���`��̃S�[�X�g�I�u�W�F�N�g���쐬�B
		/// </summary>
		/// <param name="pos">���W�B</param>
		/// <param name="rot">��]�B</param>
		/// <param name="model">���f���B</param>
		/// <param name="matrix">���[���h�s��B</param>
		void CreateMesh(const Vector3& pos, const Quaternion& rot, const Model& model, const Matrix& matrix)
		{
			m_physicsGhostObject.CreateMesh(pos, rot, model, matrix);
		}

		/// <summary>
		/// �����폜�̎��Ԃ�ݒ�B�f�t�H���g��1F��ɍ폜�B
		/// </summary>
		/// <param name="timeLimit"></param>
		void SetTimeLimit(const float timeLimit)
		{
			m_timeLimit = timeLimit;
		}

		/// <summary>
		/// ���O��ݒ�B
		/// </summary>
		/// <param name="name"></param>
		void SetName(const char*& name)
		{
			m_name = name;
		}

		/// <summary>
		/// ���O���擾�B
		/// </summary>
		/// <returns></returns>
		const char* GetName()
		{
			return m_name;
		}

		/// <summary>
		/// ���W��ݒ�B
		/// </summary>
		/// <param name="pos"></param>
		void SetPosition(const Vector3& pos)
		{
			m_physicsGhostObject.SetPosition(pos);
		}

		/// <summary>
		/// ��]��ݒ�B
		/// </summary>
		/// <param name="rot"></param>
		void SetRotation(const Quaternion& rot)
		{
			m_physicsGhostObject.SetRotation(rot);
		}

		/// <summary>
		/// �s���ݒ�B
		/// </summary>
		/// <param name="matrix"></param>
		void SetWorldMatrix(const Matrix& matrix)
		{
			Vector3 position;
			position.x = matrix.m[3][0];
			position.y = matrix.m[3][1];
			position.z = matrix.m[3][2];
			SetPosition(position);
			Quaternion rotation;
			rotation.SetRotation(matrix);
			SetRotation(rotation);
		}

		/// <summary>
		/// �����ō폜���邩�ǂ����ݒ�B
		/// </summary>
		/// <param name="isFlag"></param>
		void SetIsEnableAutoDelete(const bool isFlag)
		{
			m_isEnableAutoDelete = isFlag;
		}

		//todo �����蔻��
		/*
		const bool IsHit(CollisionObject* collisionObject)
		{
			PhysicsWorld::GetInstance()->ContactTest(&collisionObject->GetbtCollisionObject(), [&](const btCollisionObject& contactObject) {
				if (m_physicsGhostObject.IsSelf(contactObject) == true) {
					return true;
				}
			});
			return false;
		}

		const bool IsHit(CharacterController& characterController)
		{
			PhysicsWorld::GetInstance()->ContactTest(characterController, [&](const btCollisionObject& contactObject) {
				if (m_physicsGhostObject.IsSelf(contactObject) == true) {
					return true;
				}
			});
			return false;
		}
		*/

		/// <summary>
		/// �����蔻�肪�L�����ǂ����ݒ�B
		/// </summary>
		/// <param name="isEnable"></param>
		void SetIsEnable(const bool isEnable)
		{
			m_isEnable = isEnable;
		}

		/// <summary>
		/// �����蔻�肪�L�����ǂ����擾�B
		/// </summary>
		/// <returns></returns>
		const bool IsEnable()
		{
			return m_isEnable;
		}

	private:
		PhysicsGhostObject m_physicsGhostObject;		//�S�[�X�g�I�u�W�F�N�g
		const char* m_name = nullptr;					//���O
		float m_time = 0.0f;							//�^�C�}�[
		float m_timeLimit = 0.0f;						//�폜�����܂ł̎���
		bool m_isEnableAutoDelete = false;				//�����ō폜���邩�ǂ���
		bool m_isEnable = true;							//�\�������ǂ���
	};


	class CollisionObjectManager
	{
	public:
		CollisionObjectManager();
		~CollisionObjectManager();

		void AddCollisionObject(CollisionObject* collisionObject)
		{
			m_collisionObjectVector.push_back(collisionObject);
		}

		/// <summary>
		/// ���O�����S��v����R���W�����I�u�W�F�N�g�������B
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		CollisionObject* FindCollisionObject(const char* name)
		{
			for (auto collisionObject : m_collisionObjectVector)
			{
				//���O��v
				if (strcmp(collisionObject->GetName(), name) == true) {

					if (collisionObject->IsEnable() == true) {
						return collisionObject;
					}
				}
			}
			return nullptr;
		}

		void RemoveCollisionObject(CollisionObject* deleteCollisionObject)
		{
			for (auto it = m_collisionObjectVector.begin(); it != m_collisionObjectVector.end();) {
				//������v�����v�f���폜
				if (*it == deleteCollisionObject) {
					//�폜���ꂽ�v�f�̎����w���C�e���[�^���Ԃ����
					it = m_collisionObjectVector.erase(it);
				}
				else {
					it++;
				}
			}
		}


	private:
		std::vector<CollisionObject*>		m_collisionObjectVector;
		std::vector<CollisionObject*>		m_findsCollisionObjectVector;
		std::vector<CollisionObject*>		m_findMatchForwardNameCollisionObjectVector;
	};
}

