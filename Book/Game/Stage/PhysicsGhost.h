#pragma once
#include "Ghost.h"

class PlayerManagement;
class TutorialUI;

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
	/// <summary>
	/// �v���C���[�Ƃ̏Փ˔���
	/// </summary>
	bool IsHitPlayer();

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

	/// <summary>
	/// �t���O��ݒ肷��
	/// </summary>
	/// <param name="flag">true�Ȃ�`��</param>
	void SetIsDraw(const bool flag)
	{
		m_isDraw = flag;
	}

public:
	PhysicsGhostObject m_physicsGhostObj;
private:
	EffectEmitter*		m_kirakiraEffect;		//�G�t�F�N�g
	PlayerManagement*	m_playerManagement;		//�v���C���[�}�l�W�����g
	TutorialUI*			m_turorialUI = nullptr;	//�Q�[��UI
	bool				m_isDraw = false;		// �G�t�F�N�g��\�����邩�ǂ���
};

