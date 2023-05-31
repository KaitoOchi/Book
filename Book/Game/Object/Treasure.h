#pragma once
#include "Object.h"
#include "Game.h"
#include <random>
class Gage;
class GameUI;
class Player2D;
class Treasure :public Object 
{
public:
	Treasure();
	~Treasure() override;
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc);

public:
	/// <summary>
	/// ������W�̃��X�g�Ƀv�b�V������B
	/// </summary>
	void SetTreasureList(const Vector3& pos)
	{
		m_treasurePositions.push_back(pos);
	}

	/// <summary>
	/// �����_���ɂ���̈ʒu��ݒ�B
	/// </summary>
	const Vector3& SetTreasurePosition()
	{
		//����̍��W�������_���Ő�������
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_int_distribution<int>dist(0, 2);

		m_position = m_treasurePositions[dist(mt)];

		return m_position;
	}

	/// <summary>
	/// ����̈ʒu���擾
	/// </summary>
	const Vector3 GetPosition()
	{
		return m_position;
	}
	void Hit()override;
	/// <summary>
	/// ����ɐG��Ă��邩�擾
	/// </summary>
	/// <returns>true�Ȃ�G��Ă���</returns>
	const bool GetHitState()
	{
		return m_hitState;
	}

	/// <summary>
	/// �G�t�F�N�g���擾�B
	/// </summary>
	EffectEmitter* GetEffect()
	{
		if (m_kirakiraEffect != nullptr) {
			return m_kirakiraEffect;
		}
		return nullptr;
	}

private:
	EffectEmitter*			m_kirakiraEffect = nullptr;		//�G�t�F�N�g
	GameUI*					m_gameUI = nullptr;				//�Q�[��UI
	std::vector< Vector3 >	m_treasurePositions;			//����̍��W
	bool					m_hitState = false;				//true�Ȃ炨��ɐG��Ă���
	Player2D*				m_player2d = nullptr;
};