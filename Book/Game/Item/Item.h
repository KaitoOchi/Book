#pragma once

class Game;
class Player3D;
class PlayerManagement;
class Enemy;
class GameUI;

/// <summary>
/// アイテム基底クラス。
/// </summary>
class Item :public IGameObject
{
public:
	Item();
	virtual ~Item();
	virtual bool Start();
	void Update();

public:
	/// <summary>
	/// アイテムが当たったかどうか
	/// </summary>
	virtual void ItemHit() = 0;

	/// <summary>
	/// アイテムを使用した座標
	/// </summary>
	/// <param name="pos">使った時の座標</param>
	/// <returns></returns>
	const void SetItemPosition(const Vector3 pos)
	{
		m_position = pos;
	}

protected:
	/// <summary>
	/// ステートの遷移
	/// </summary>
	virtual void ManageState() = 0;

	/// <summary>
	/// アイテム範囲内のエネミーを取得。
	/// </summary>
	std::vector<Enemy*> GetHitEnemyList()
	{
		return m_HitEnemyList;
	}

protected:	
	Game*					m_game = nullptr;				//ゲーム
	PlayerManagement*		m_playerManagement = nullptr;	//プレイヤーマネジメント
	Player3D*				m_player3D = nullptr;			//プレイヤー
	GameUI*					m_gameUI = nullptr;				//ゲームUI
	std::vector< Enemy* >	m_HitEnemyList;					//当たったエネミーのリスト
	Vector3					m_position;						//アイテムを使った座標
	Vector3					EffecrRange;					//エフェクト範囲
};

