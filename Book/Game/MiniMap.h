#pragma once

class PlayerManagement;
class Enemy_Normal;
class Enemy_Serch;
class Enemy_Charge;
class MiniMap:public IGameObject
{
public:
	MiniMap();
	~MiniMap();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// マップに描画するかどうかの判定
	/// </summary>
	/// <param name="pos">変換するエネミーの座標</param>
	/// <param name="num">配列番号</param>
	void DrawMap(Vector3 enemyPos,int num);		

private:
	/// <summary>
	/// ワールド座標系からマップ座標系に変換
	/// </summary>
	/// <param name="worldcenterPosition">マップの中心とするオブジェクトのワールド座標</param>
	/// <param name="worldPosition">マップに表示したいオブジェクトのワールド座標</param>
	/// <param name="mapPosirion">変換した後のマップ座標</param>
	/// <returns></returns>
	const bool WorldPositionConvertToMapPosition(
		Vector3 worldcenterPosition,
		Vector3 worldPosition,
		Vector3& mapPosirion
	);

	SpriteRender m_SpriteRender;			// スプライトレンダー。ミニマップのベース
	SpriteRender m_OutLineSpriteRender;		// スプライトレンダー。ミニマップの装飾部分
	SpriteRender m_PlayerSpriteRender;		// スプライトレンダー。プレイヤー
	SpriteRender m_EnemySpriteRender[2];	// スプライトレンダー。エネミー

	PlayerManagement* m_playerManagement = nullptr;
	Enemy_Normal* m_enemyNormal = nullptr;
	Enemy_Serch* m_enemySerch = nullptr;
	Enemy_Charge* m_enemyCharge = nullptr;

	bool m_isImage[5];						// 表示するかどうかのフラグ。エネミーの数分用意する
};

