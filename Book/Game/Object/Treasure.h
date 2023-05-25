#pragma once
#include "Object.h"
#include "Game.h"
#include <random>
class Gage;
class GameUI;
class Treasure :public Object 
{
public:
	Treasure();
	~Treasure()override;
	bool Start()override ;
	void Update()override;
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

private:
	EffectEmitter*			m_kirakiraEffect = nullptr;		//�G�t�F�N�g
	GameUI*					m_gameUI = nullptr;				//�Q�[��UI
	std::vector< Vector3 >	m_treasurePositions;			//����̍��W
};