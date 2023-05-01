#pragma once
namespace nsK2EngineLow {
	/// <summary>
	/// 静的物理オブジェクト
	/// </summary>
		class PhysicsBoxObject : public Noncopyable {
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		PhysicsBoxObject();
		/// <summary>
		/// デストラクタ
		/// </summary>
		~PhysicsBoxObject();
		/// <summary>
		/// モデルからの静的オブジェクトの作成。
		/// </summary>
		/// <param name="model">モデル</param>
		/// <param name="worldMatrix">ワールド行列</param>
		/// <param name="restitution">反発力</param>
		void CreateFromModel(const Model& model, const Matrix& worldMatrix,const Vector3 scale);
		/// <summary>
		/// 摩擦力を設定する。
		/// </summary>
		/// <param name="friction">摩擦力。10が最大値。</param>
		void SetFriction(float friction)
		{
			m_rigidBody.SetFriction(friction);
		}
		btCollisionObject* GetbtCollisionObject()
		{
			return m_rigidBody.GetBody();
		}
		/// <summary>
		/// 剛体を取得
		/// </summary>
		RigidBody* GetRigidBody() {
			return &m_rigidBody;
		}
		/// <summary>
		/// 剛体を破棄。
		/// </summary>
		void Release()
		{
			m_rigidBody.Release();
		}
	/// <summary>
	///colliderの作成
	///  <param name="pos">座標</param>
	/// <param name="rot">回転</param>
	/// </summary>
		void Create(const Vector3 size, const Vector3 pos, const Quaternion rot);
	private:
		typedef std::vector<Vector3>		VertexBuffer;		//頂点バッファ。
		typedef std::vector<unsigned int>	IndexBuffer;		//インデックスバッファ。
		typedef std::unique_ptr<VertexBuffer>	VertexBufferPtr;
		typedef std::unique_ptr<IndexBuffer>	IndexBufferPtr;
		std::vector<VertexBufferPtr>					m_vertexBufferArray;		//頂点バッファの配列。
		std::vector<IndexBufferPtr>						m_indexBufferArray;		//インデックスバッファの配列。
		std::unique_ptr<btBvhTriangleMeshShape>			m_meshShape;				//メッシュ形状。
		std::unique_ptr<btTriangleIndexVertexArray>		m_stridingMeshInterface;
		BoxCollider m_boxCollider;		//メッシュコライダー。
		RigidBody m_rigidBody;				//剛体。
		PhysicsStaticObject m_physicsStaticobj;
		float posXmax=0;
		float posXmin = 1000000.0;
		float posYmax = 0;
		float posYmin = 1000000.0;
		float posZmax = 0;
		float posZmin = 1000000.0;
		float posX;
		float posY;
		float posZ;
		Vector3 m_boxSize = Vector3::Zero;
		RigidBodyInitData rbInfo;
	

	};
}