#pragma once

class PlayerManagement;
class Enemy_Normal;
class Enemy_Search;
class Enemy_Charge;
class Game;
class Enemy;
class Treasure;
class PhysicsGhost;

using namespace std;
namespace
{
	const int ENEMY_NUM = 50;			// 表示するかどうかのフラグ用。多めに用意してます
	const int WALL_NUM = 10;
}

class MiniMap:public IGameObject
{
public:
	MiniMap();
	~MiniMap();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

public:
	/// <summary>
	/// 永続表示する座標を設定。
	/// </summary>
	/// <param name="pos">座標。</param>
	/// <param name="color">trueなら色を変更。</param>
	void SetTreasurePos(const Vector3& pos, bool color)
	{
		m_treasurePos = pos;

		if (color) {
			m_TreasureSpriteRender.SetMulColor({ 1.0f,0.0f,0.0f,1.0f });
		}
	}

private:
	/// <summary>
	/// 描画処理。
	/// </summary>
	bool DrawMap(const Vector3& mapPos, float& alpha);

	/// <summary>
	/// 敵を描画する処理。
	/// </summary>
	void DrawMap_Enemy();		

	/// <summary>
	/// ワールド座標系からマップ座標系に変換。
	/// </summary>
	/// <param name="worldcenterPosition">マップの中心とするオブジェクトのワールド座標</param>
	/// <param name="worldPosition">マップに表示したいオブジェクトのワールド座標</param>
	/// <returns></returns>
	const bool WorldPositionConvertToMapPosition(
		Vector3 worldcenterPosition,
		Vector3 worldPosition
	);

private:
	SpriteRender						m_SpriteRender;							// スプライトレンダー。ミニマップのベース
	SpriteRender						m_OutLineSpriteRender;						// スプライトレンダー。ミニマップの装飾部分
	SpriteRender						m_PlayerSpriteRender;						// スプライトレンダー。プレイヤー
	SpriteRender						m_TreasureSpriteRender;					// スプライトレンダー。お宝
	array< SpriteRender, ENEMY_NUM >	m_EnemySpriteRender;	// スプライトレンダー。エネミー
	array< SpriteRender, WALL_NUM >		m_wallSpriteRender;//スプライトレンダー。壁

	PlayerManagement*					m_playerManagement = nullptr;
	Game*								m_game = nullptr;

	vector<Enemy*>						m_enemyList;						// エネミーのリスト
	vector<PhysicsGhost*>				m_physicsGhostList;	//壁のリスト

	Vector3								m_treasurePos;			// お宝の位置
	Vector3								m_playerPos;							//マップ上のプレイヤーの座標
	Vector3								m_mapPos;								//計算したマップに乗せる対象の座標

	std::array< bool, ENEMY_NUM >m_isImage;						// 表示するかどうかのフラグ。エネミーの数分用意する
	std::array< bool, WALL_NUM > m_enableWallSprite;

	bool m_isTreasure = false;	//お宝かどうか
};

