#pragma once
#include "physics/PhysicsGhostObject.h"

class Ghost:public IGameObject
{
public:
	Ghost();
	virtual ~Ghost();
	bool Start();
	void Update();

	/// <summary>
	/// �ǂ̃I�u�W�F�N�g���쐬���邩
	/// </summary>
	enum EnGhostObject
	{
		m_enGhostObje_physicsGhost,		//�S�[�X�g�I�u�W�F�N�g
		m_enGhostObje_boxCollider,		//�{�b�N�X�R���C�_�[
	};

public:
	void CreateGhostBox();
	/// <summary>
	/// ���W��ݒ�B
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}

	/// <summary>
	/// ���W���擾�B
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPosition()
	{
		return m_position;
	}

	/// <summary>
	/// ��]��ݒ�B
	/// </summary>
	/// <param name="rot"></param>
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}

	/// <summary>
	/// ��]���擾�B
	/// </summary>
	/// <returns></returns>
	const Quaternion& GetRotation()
	{
		return m_rotation;
	}

	/// <summary>
	/// �傫����ݒ�B
	/// </summary>
	/// <param name="rot"></param>
	void SetScale(const Vector3& scl)
	{
		m_scale = scl;
	}

	/// <summary>
	/// �傫�����擾�B
	/// </summary>
	/// <returns></returns>
	const Vector3& GetScale()
	{
		return m_scale;
	}
	

	/// <summary>
	/// �������W��ݒ�B
	/// </summary>
	/// <param name="pos"></param>
	void SetGhostPosition(const Vector3& ghostpos)
	{
		m_ghostPosition = ghostpos;
	}

	/// <summary>
	/// �������W���擾�B
	/// </summary>
	/// <returns></returns>
	const Vector3& GetGhostPosition()
	{
		return m_ghostPosition;
	}

	
protected:
	Vector3 m_position = Vector3::Zero;
	Vector3 m_ghostPosition = Vector3::Zero;
	Vector3 m_scale = Vector3::Zero;
	Quaternion m_rotation;
	float posXmax = 0;
	float posXmin = 1000000.0;
	float posYmax = 0;
	float posYmin = 1000000.0;
	float posZmax = 0;
	float posZmin = 1000000.0;
	float posX;
	float posY;
	float posZ;
	Vector3 m_boxSize = Vector3::Zero;
	ModelRender m_modelRender;
};

