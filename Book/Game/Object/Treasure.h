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
	/// お宝座標のリストにプッシュする。
	/// </summary>
	void SetTreasureList(const Vector3& pos)
	{
		m_treasurePositions.push_back(pos);
	}

	/// <summary>
	/// ランダムにお宝の位置を設定。
	/// </summary>
	const Vector3& SetTreasurePosition()
	{
		//お宝の座標をランダムで生成する
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_int_distribution<int>dist(0, 2);

		m_position = m_treasurePositions[dist(mt)];

		return m_position;
	}

	/// <summary>
	/// お宝の位置を取得
	/// </summary>
	const Vector3 GetPosition()
	{
		return m_position;
	}
	void Hit()override;

private:
	EffectEmitter*			m_kirakiraEffect = nullptr;		//エフェクト
	GameUI*					m_gameUI = nullptr;				//ゲームUI
	std::vector< Vector3 >	m_treasurePositions;			//お宝の座標
};