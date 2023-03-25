#pragma once

class PlayerManagement;
class Enemy_Normal;
class Enemy_Serch;
class MiniMap:public IGameObject
{
public:
	MiniMap();
	~MiniMap();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void DrawMap(Vector3 pos,int num);		// マップに描画するかどうかの判定

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

	bool m_isImage[2];						// 表示するかどうかのフラグ。エネミーの数分用意する
};

