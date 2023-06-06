#pragma once
#include "Ghost.h"

class PlayerManagement;

/// <summary>
/// �Ǘp�̓����蔻��N���X�B
/// </summary>
class PhysicsGhost:public Ghost
{
public:
	PhysicsGhost();
	~PhysicsGhost()override;
	bool Start();
	void Update();
	void EffectDraw();

public:
	/// <summary>
	/// �G�t�F�N�g�̎擾�B
	/// </summary>
	/// <returns></returns>
	EffectEmitter* GetEffect()
	{
		if (m_kirakiraEffect != nullptr) {
			return m_kirakiraEffect;
		}
		return nullptr;
	}

public:
	PhysicsGhostObject m_physicsGhostObj;
private:
	EffectEmitter*		m_kirakiraEffect;		//�G�t�F�N�g
	PlayerManagement*	m_playerManagement;		//�v���C���[�}�l�W�����g
};

