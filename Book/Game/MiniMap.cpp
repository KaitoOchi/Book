#include "stdafx.h"
#include "MiniMap.h"

#include "PlayerManagement.h"
#include "Enemy_Normal.h"
#include "Enemy_Search.h"
#include "Enemy_Charge.h"

namespace
{
	const Vector3	CENTER_POSITION = Vector3(635.0f, -290.0f, 0.0f);		// マップ�E中忁E
	const float		MAP_RADIUS = 140.0f;									// マップ�E半征E
	const float		LIMITED_RANGE_IMAGE = 500.0f;							// マップ�E篁E��
	const float		ALPHA = 0.75f;											// α値
}

MiniMap::MiniMap()
{
}

MiniMap::~MiniMap()
{
}

bool MiniMap::Start()
{
	// �}�b�v�摜
	m_SpriteRender.Init("Assets/sprite/UI/miniMap/base.DDS", 340, 340);
	m_SpriteRender.SetPosition(CENTER_POSITION);
	//m_SpriteRender.SetMulColor({ 1.0f, 1.0f, 1.0f, ALPHA });

	// �A�E�g���C��
	m_OutLineSpriteRender.Init("Assets/sprite/UI/miniMap/base_outLine.DDS", 362, 519);
	m_OutLineSpriteRender.SetPosition({ 640.0f, -210.0f, 0.0f });
	m_OutLineSpriteRender.Update();

	// �v���C���[
	m_PlayerSpriteRender.Init("Assets/sprite/UI/miniMap/player.DDS", 20,40);
	m_PlayerSpriteRender.SetPosition(CENTER_POSITION);

	// �G�l�~�[
	for (int i = 0; i < ENEMY_NUM; i++) {
		m_EnemySpriteRender[i].Init("Assets/sprite/UI/miniMap/map_2.DDS", 15, 15);
	}

	// �C���X�^���X��T��
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");

	//m_enemyNormal = FindGO<Enemy_Normal>("enemyNormal");
	m_enemySearch = FindGO<Enemy_Search>("enemySearch");
	m_enemyCharge = FindGO<Enemy_Charge>("enemyCharge");

	return true;
}

void MiniMap::Update()
{
	// �}�b�v���W�ɕϊ�
	//DrawMap(m_enemyNormal->GetPosition(),0);
	DrawMap(m_enemySearch->GetPosition(), 1);
	DrawMap(m_enemyCharge->GetPosition(), 2);

	// �X�V
	for (int i = 0; i < ENEMY_NUM; i++) {
		m_EnemySpriteRender[i].Update();
	}

	m_PlayerSpriteRender.Update();
	m_SpriteRender.Update();
}

void MiniMap::DrawMap(Vector3 enemyPos, int num)
{
	// �v���C���[�̍��W
	Vector3 playerPos = m_playerManagement->GetPosition();

	Vector3 mapPos;

	// �}�b�v�ɕ\������͈͂ɓG��������
	if (WorldPositionConvertToMapPosition(playerPos, enemyPos, mapPos)) {

		// spriteRenderに座標を設宁E
		m_EnemySpriteRender[num].SetPosition(mapPos);
		// マップに表示する
		m_isImage[num] = true;
	}
	// マップに表示する敵がいなかったら
	else {
		m_isImage[num] = false;
	}
}

const bool MiniMap::WorldPositionConvertToMapPosition(Vector3 worldCenterPosition,Vector3 worldPosition,Vector3& mapPosition) 
{
	// Y座標�Eマップとは関係なぁE�Eで0.0fを設宁E
	worldCenterPosition.y = 0.0f;
	worldPosition.y = 0.0f;

	// 中忁E��標から表示したぁE��ブジェクト�E座標へ向かぁE�Eクトルを計箁E
	Vector3 diff = worldPosition - worldCenterPosition;

	// 計算した�Eクトルが一定以上離れてぁE��めE
	if (diff.LengthSq() >= LIMITED_RANGE_IMAGE * LIMITED_RANGE_IMAGE) {
		// 篁E��外に存在してぁE��のでマップに表示しなぁE
		return false;
	}

	

	//ベクトルの長さを取得すめE
	float length = diff.Length();

	//カメラの前方向�Eクトルから、E
	//クォータニオンを生成、E
	Vector3 forward = g_camera3D->GetForward();
	Quaternion rot;
	rot.SetRotationY(atan2(-forward.x, forward.z));

	//ベクトルにカメラの回転を適用、E
	rot.Apply(diff);

	//ベクトルを正規化、E
	diff.Normalize();

	//マップ�E大きさ/距離制限で、E
	//ベクトルを�EチE�E座標系に変換する、E
	diff *= length * MAP_RADIUS / LIMITED_RANGE_IMAGE;

	//マップ�E中央座標と上記�Eクトルを加算する、E
	mapPosition = Vector3(CENTER_POSITION.x + diff.x, CENTER_POSITION.y + diff.z, 0.0f);
	return true;
}

void MiniMap::Render(RenderContext& rc)
{
	// �`��
	m_SpriteRender.Draw(rc);
	m_OutLineSpriteRender.Draw(rc);
	m_PlayerSpriteRender.Draw(rc);

	for (int i = 0; i < ENEMY_NUM; i++) {
		// �͈͓��̂Ƃ�
		if (m_isImage[i] == true) {
			// �`�悷��
			m_EnemySpriteRender[i].Draw(rc);
		}
	}
}