#include "stdafx.h"
#include "MiniMap.h"

#include "PlayerManagement.h"
#include "Enemy_Normal.h"
#include "Enemy_Search.h"
#include "Enemy_Charge.h"

namespace
{
	const Vector3	CENTER_POSITION = Vector3(635.0f, -290.0f, 0.0f);		// 繝槭ャ繝励・荳ｭ蠢・
	const float		MAP_RADIUS = 140.0f;									// 繝槭ャ繝励・蜊雁ｾ・
	const float		LIMITED_RANGE_IMAGE = 500.0f;							// 繝槭ャ繝励・遽・峇
	const float		ALPHA = 0.75f;											// ﾎｱ蛟､
}

MiniMap::MiniMap()
{
}

MiniMap::~MiniMap()
{
}

bool MiniMap::Start()
{
	// マップ画像
	m_SpriteRender.Init("Assets/sprite/UI/miniMap/base.DDS", 340, 340);
	m_SpriteRender.SetPosition(CENTER_POSITION);
	//m_SpriteRender.SetMulColor({ 1.0f, 1.0f, 1.0f, ALPHA });

	// アウトライン
	m_OutLineSpriteRender.Init("Assets/sprite/UI/miniMap/base_outLine.DDS", 362, 519);
	m_OutLineSpriteRender.SetPosition({ 640.0f, -210.0f, 0.0f });
	m_OutLineSpriteRender.Update();

	// プレイヤー
	m_PlayerSpriteRender.Init("Assets/sprite/UI/miniMap/player.DDS", 20,40);
	m_PlayerSpriteRender.SetPosition(CENTER_POSITION);

	// エネミー
	for (int i = 0; i < ENEMY_NUM; i++) {
		m_EnemySpriteRender[i].Init("Assets/sprite/UI/miniMap/map_2.DDS", 15, 15);
	}

	// インスタンスを探す
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");

	//m_enemyNormal = FindGO<Enemy_Normal>("enemyNormal");
	m_enemySearch = FindGO<Enemy_Search>("enemySearch");
	m_enemyCharge = FindGO<Enemy_Charge>("enemyCharge");

	return true;
}

void MiniMap::Update()
{
	// マップ座標に変換
	//DrawMap(m_enemyNormal->GetPosition(),0);
	DrawMap(m_enemySearch->GetPosition(), 1);
	DrawMap(m_enemyCharge->GetPosition(), 2);

	// 更新
	for (int i = 0; i < ENEMY_NUM; i++) {
		m_EnemySpriteRender[i].Update();
	}

	m_PlayerSpriteRender.Update();
	m_SpriteRender.Update();
}

void MiniMap::DrawMap(Vector3 enemyPos, int num)
{
	// プレイヤーの座標
	Vector3 playerPos = m_playerManagement->GetPosition();

	Vector3 mapPos;

	// マップに表示する範囲に敵がいたら
	if (WorldPositionConvertToMapPosition(playerPos, enemyPos, mapPos)) {

		// spriteRender縺ｫ蠎ｧ讓吶ｒ險ｭ螳・
		m_EnemySpriteRender[num].SetPosition(mapPos);
		// 繝槭ャ繝励↓陦ｨ遉ｺ縺吶ｋ
		m_isImage[num] = true;
	}
	// 繝槭ャ繝励↓陦ｨ遉ｺ縺吶ｋ謨ｵ縺後＞縺ｪ縺九▲縺溘ｉ
	else {
		m_isImage[num] = false;
	}
}

const bool MiniMap::WorldPositionConvertToMapPosition(Vector3 worldCenterPosition,Vector3 worldPosition,Vector3& mapPosition) 
{
	// Y蠎ｧ讓吶・繝槭ャ繝励→縺ｯ髢｢菫ゅ↑縺・・縺ｧ0.0f繧定ｨｭ螳・
	worldCenterPosition.y = 0.0f;
	worldPosition.y = 0.0f;

	// 荳ｭ蠢・ｺｧ讓吶°繧芽｡ｨ遉ｺ縺励◆縺・が繝悶ず繧ｧ繧ｯ繝医・蠎ｧ讓吶∈蜷代°縺・・繧ｯ繝医Ν繧定ｨ育ｮ・
	Vector3 diff = worldPosition - worldCenterPosition;

	// 險育ｮ励＠縺溘・繧ｯ繝医Ν縺御ｸ螳壻ｻ･荳企屬繧後※縺・◆繧・
	if (diff.LengthSq() >= LIMITED_RANGE_IMAGE * LIMITED_RANGE_IMAGE) {
		// 遽・峇螟悶↓蟄伜惠縺励※縺・ｋ縺ｮ縺ｧ繝槭ャ繝励↓陦ｨ遉ｺ縺励↑縺・
		return false;
	}

	

	//繝吶け繝医Ν縺ｮ髟ｷ縺輔ｒ蜿門ｾ励☆繧・
	float length = diff.Length();

	//繧ｫ繝｡繝ｩ縺ｮ蜑肴婿蜷代・繧ｯ繝医Ν縺九ｉ縲・
	//繧ｯ繧ｩ繝ｼ繧ｿ繝九が繝ｳ繧堤函謌舌・
	Vector3 forward = g_camera3D->GetForward();
	Quaternion rot;
	rot.SetRotationY(atan2(-forward.x, forward.z));

	//繝吶け繝医Ν縺ｫ繧ｫ繝｡繝ｩ縺ｮ蝗櫁ｻ｢繧帝←逕ｨ縲・
	rot.Apply(diff);

	//繝吶け繝医Ν繧呈ｭ｣隕丞喧縲・
	diff.Normalize();

	//繝槭ャ繝励・螟ｧ縺阪＆/霍晞屬蛻ｶ髯舌〒縲・
	//繝吶け繝医Ν繧偵・繝・・蠎ｧ讓咏ｳｻ縺ｫ螟画鋤縺吶ｋ縲・
	diff *= length * MAP_RADIUS / LIMITED_RANGE_IMAGE;

	//繝槭ャ繝励・荳ｭ螟ｮ蠎ｧ讓吶→荳願ｨ倥・繧ｯ繝医Ν繧貞刈邂励☆繧九・
	mapPosition = Vector3(CENTER_POSITION.x + diff.x, CENTER_POSITION.y + diff.z, 0.0f);
	return true;
}

void MiniMap::Render(RenderContext& rc)
{
	// 描画
	m_SpriteRender.Draw(rc);
	m_OutLineSpriteRender.Draw(rc);
	m_PlayerSpriteRender.Draw(rc);

	for (int i = 0; i < ENEMY_NUM; i++) {
		// 範囲内のとき
		if (m_isImage[i] == true) {
			// 描画する
			m_EnemySpriteRender[i].Draw(rc);
		}
	}
}