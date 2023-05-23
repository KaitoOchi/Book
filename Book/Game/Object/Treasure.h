#pragma once
#include "Object.h"
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
	void SetTreasurePosition()
	{
		//����̍��W�������_���Ő�������
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_int_distribution<int>dist(0, 2);

		m_position = m_treasurePositions[dist(mt)];
	}

	void Hit()override;

private:
	Gage*					m_gage = nullptr;
	GameUI*					m_gameUI = nullptr;
	std::vector< Vector3 >	m_treasurePositions;
	float					m_gaugeCount = 0;
	
};