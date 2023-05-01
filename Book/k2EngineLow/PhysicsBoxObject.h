#pragma once
namespace nsK2EngineLow {
	/// <summary>
	/// �ÓI�����I�u�W�F�N�g
	/// </summary>
		class PhysicsBoxObject : public Noncopyable {
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		PhysicsBoxObject();
		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~PhysicsBoxObject();
		/// <summary>
		/// ���f������̐ÓI�I�u�W�F�N�g�̍쐬�B
		/// </summary>
		/// <param name="model">���f��</param>
		/// <param name="worldMatrix">���[���h�s��</param>
		/// <param name="restitution">������</param>
		void CreateFromModel(const Model& model, const Matrix& worldMatrix,const Vector3 scale);
		/// <summary>
		/// ���C�͂�ݒ肷��B
		/// </summary>
		/// <param name="friction">���C�́B10���ő�l�B</param>
		void SetFriction(float friction)
		{
			m_rigidBody.SetFriction(friction);
		}
		btCollisionObject* GetbtCollisionObject()
		{
			return m_rigidBody.GetBody();
		}
		/// <summary>
		/// ���̂��擾
		/// </summary>
		RigidBody* GetRigidBody() {
			return &m_rigidBody;
		}
		/// <summary>
		/// ���̂�j���B
		/// </summary>
		void Release()
		{
			m_rigidBody.Release();
		}
	/// <summary>
	///collider�̍쐬
	///  <param name="pos">���W</param>
	/// <param name="rot">��]</param>
	/// </summary>
		void Create(const Vector3 size, const Vector3 pos, const Quaternion rot);
	private:
		typedef std::vector<Vector3>		VertexBuffer;		//���_�o�b�t�@�B
		typedef std::vector<unsigned int>	IndexBuffer;		//�C���f�b�N�X�o�b�t�@�B
		typedef std::unique_ptr<VertexBuffer>	VertexBufferPtr;
		typedef std::unique_ptr<IndexBuffer>	IndexBufferPtr;
		std::vector<VertexBufferPtr>					m_vertexBufferArray;		//���_�o�b�t�@�̔z��B
		std::vector<IndexBufferPtr>						m_indexBufferArray;		//�C���f�b�N�X�o�b�t�@�̔z��B
		std::unique_ptr<btBvhTriangleMeshShape>			m_meshShape;				//���b�V���`��B
		std::unique_ptr<btTriangleIndexVertexArray>		m_stridingMeshInterface;
		BoxCollider m_boxCollider;		//���b�V���R���C�_�[�B
		RigidBody m_rigidBody;				//���́B
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