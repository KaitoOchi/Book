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
	/// お宝座標のリストにプッシュする。
	/// </summary>
	void SetTreasureList(const Vector3& pos)
	{
		m_treasurePositions.push_back(pos);
	}

	/// <summary>
	/// ランダムにお宝の位置を設定。
	/// </summary>
	void SetTreasurePosition()
	{
		//お宝の座標をランダムで生成する
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