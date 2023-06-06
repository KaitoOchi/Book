#pragma once
#include "PlayerManagement.h"
#include "Game.h";

class Gage;
class Player3D;

/// <summary>
/// �I�u�W�F�N�g�̊��N���X�B
/// </summary>
class Object : public IGameObject
{
public:
	Object();
	virtual ~Object() = 0;
	virtual bool Start() = 0;
	virtual void Update() = 0;

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
	/// �\����Ԃ�ݒ�B
	/// </summary>
	void SetActive(const bool active)
	{
		m_isActive = active;
	}

	/// <summary>
	/// �\����Ԃ��擾�B
	/// </summary>
	bool GetActive()
	{
		return m_isActive;
	}

    /// <summary>
    /// �����蔻����擾�B
    /// </summary>
    /// <returns></returns>
    CollisionObject* GetCollision()
	{
		return m_collisionObject;
	}

	enum EnHitState {
		m_enObject_null,		//�������Ă���
		m_enObject_hit,			//�������Ă��Ȃ�
	};
	EnHitState m_HitState = m_enObject_null;

protected:

	/// <summary>
	/// �����蔻��̏����B
	/// </summary>
	void Collision();

	/// <summary>
	/// ���������Ƃ��̏����B
	/// </summary>
	virtual void Hit() = 0;

	ModelRender			m_modelRender;					//���f���B
	CollisionObject*	m_collisionObject = nullptr;	//�R���W�����B
	PlayerManagement*	m_player = nullptr;				//�v���C���[�}�l�W�����g�B
	Game*				m_game = nullptr;				//�Q�[���B
	Player3D*			m_player3d = nullptr;			//�v���C���[�B
	Vector3				m_position;						//���W�B
	Quaternion			m_rotation;						//��]�B
	Vector3				m_scale = Vector3::One;			//�g�嗦�B
	bool				m_isActive = true;				//�A�N�e�B�u���ǂ����B
};

