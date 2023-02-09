#pragma once
#include "../../k2EngineLow/physics/PhysicsGhostObject.h"


namespace nsBookEngine {

	/// <summary>
	/// コリジョンの当たり判定を行うクラス。
	/// </summary>
	class CollisionObject : public IGameObject
	{
	public:
		CollisionObject();
		~CollisionObject();
		bool Start();
		void Update();

		/// <summary>
		/// ボックス形状のゴーストオブジェクトを作成。
		/// </summary>
		/// <param name="pos">座標。</param>
		/// <param name="rot">回転。</param>
		/// <param name="size">拡大率。</param>
		void CreateBox(const Vector3& pos, const Quaternion& rot, const Vector3& size)
		{
			m_physicsGhostObject.CreateBox(pos, rot, size);
		}

		/// <summary>
		/// カプセル形状のゴーストオブジェクトを作成。
		/// </summary>
		/// <param name="pos">座標。</param>
		/// <param name="rot">回転。</param>
		/// <param name="radius">半径。</param>
		/// <param name="height">高さ。</param>
		void CreateCapsule(const Vector3& pos, const Quaternion& rot, float radius, float height)
		{
			m_physicsGhostObject.CreateCapsule(pos, rot, radius, height);
		}

		/// <summary>
		/// 球形状のゴーストオブジェクトを作成。
		/// </summary>
		/// <param name="pos">座標。</param>
		/// <param name="rot">回転。</param>
		/// <param name="radius">半径。</param>
		void CreateSphere(const Vector3& pos, const Quaternion& rot, float radius)
		{
			m_physicsGhostObject.CreateSphere(pos, rot, radius);
		}

		/// <summary>
		/// メッシュ形状のゴーストオブジェクトを作成。
		/// </summary>
		/// <param name="pos">座標。</param>
		/// <param name="rot">回転。</param>
		/// <param name="model">モデル。</param>
		/// <param name="matrix">ワールド行列。</param>
		void CreateMesh(const Vector3& pos, const Quaternion& rot, const Model& model, const Matrix& matrix)
		{
			m_physicsGhostObject.CreateMesh(pos, rot, model, matrix);
		}

		/// <summary>
		/// 自動削除の時間を設定。デフォルトで1F後に削除。
		/// </summary>
		/// <param name="timeLimit"></param>
		void SetTimeLimit(const float timeLimit)
		{
			m_timeLimit = timeLimit;
		}

		/// <summary>
		/// 名前を設定。
		/// </summary>
		/// <param name="name"></param>
		void SetName(const char*& name)
		{
			m_name = name;
		}

		/// <summary>
		/// 名前を取得。
		/// </summary>
		/// <returns></returns>
		const char* GetName()
		{
			return m_name;
		}

		/// <summary>
		/// 座標を設定。
		/// </summary>
		/// <param name="pos"></param>
		void SetPosition(const Vector3& pos)
		{
			m_physicsGhostObject.SetPosition(pos);
		}

		/// <summary>
		/// 回転を設定。
		/// </summary>
		/// <param name="rot"></param>
		void SetRotation(const Quaternion& rot)
		{
			m_physicsGhostObject.SetRotation(rot);
		}

		/// <summary>
		/// 行列を設定。
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
		/// 自動で削除するかどうか設定。
		/// </summary>
		/// <param name="isFlag"></param>
		void SetIsEnableAutoDelete(const bool isFlag)
		{
			m_isEnableAutoDelete = isFlag;
		}

		//todo 当たり判定
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
		/// 当たり判定が有効かどうか設定。
		/// </summary>
		/// <param name="isEnable"></param>
		void SetIsEnable(const bool isEnable)
		{
			m_isEnable = isEnable;
		}

		/// <summary>
		/// 当たり判定が有効かどうか取得。
		/// </summary>
		/// <returns></returns>
		const bool IsEnable()
		{
			return m_isEnable;
		}

	private:
		PhysicsGhostObject m_physicsGhostObject;		//ゴーストオブジェクト
		const char* m_name = nullptr;					//名前
		float m_time = 0.0f;							//タイマー
		float m_timeLimit = 0.0f;						//削除されるまでの時間
		bool m_isEnableAutoDelete = false;				//自動で削除するかどうか
		bool m_isEnable = true;							//表示中かどうか
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
		/// 名前が完全一致するコリジョンオブジェクトを検索。
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		CollisionObject* FindCollisionObject(const char* name)
		{
			for (auto collisionObject : m_collisionObjectVector)
			{
				//名前一致
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
				//条件一致した要素を削除
				if (*it == deleteCollisionObject) {
					//削除された要素の次を指すイテレータが返される
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

