#include "stdafx.h"
#include "MiniMap.h"

#include "PlayerManagement.h"
#include "Enemy_Normal.h"
#include "Enemy_Search.h"
#include "Enemy_Charge.h"
#include "Game.h"
#include "Enemy.h"
#include "Treasure.h"

namespace
{
	const Vector3	CENTER_POSITION = Vector3(635.0f, -290.0f, 0.0f);		// �}�b�v�̒��S
	const float		MAP_RADIUS = 140.0f;									// �}�b�v�̔��a
	const float		LIMITED_RANGE_IMAGE = 650.0f;							// �}�b�v�͈̔�
	const float		ALPHA = 0.75f;											// ���l
}

MiniMap::MiniMap()
{
}

MiniMap::~MiniMap()
{
}

bool MiniMap::Start()
{
	// �C���X�^���X��T��
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
	m_game = FindGO<Game>("game");
	m_treasure = FindGO<Treasure>("treaSure");
	// �G�l�~�[�̃��X�g�������Ă���
	m_enemyList = m_game->GetEnemyList();
	// ����̈ʒu
	m_treasurePos = m_treasure->GetPosition();

	// �摜��p�ӂ���
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
	for (int i = 0; i < m_enemyList.size(); i++) {
		m_EnemySpriteRender[i].Init("Assets/sprite/UI/miniMap/map_2.DDS", 15, 15);
		// �t���O�����������Ă���
		m_isImage[i] = false;
	}

	// ����
	m_TreasureSpriteRender.Init("Assets/sprite/UI/miniMap/map_treasure.DDS", 25,25);
	// �}�b�v��̐F�����F�ɐݒ�B�G�l�~�[��菭���傫���`�悷��
	m_TreasureSpriteRender.SetMulColor({ 5.0f,3.0f,0.0f,1.0f });

return true;
}

void MiniMap::Update()
{
	for (int i = 0; i < m_enemyList.size(); i++) {
		// �}�b�v���W�ɕϊ�
		DrawMap(m_enemyList[i]->GetPosition(), i);
		// �X�V
		m_EnemySpriteRender[i].Update();
	}

	// �����`��
	DrawMap_Treasure(m_treasurePos);

	m_PlayerSpriteRender.Update();
	m_SpriteRender.Update();
}

void MiniMap::DrawMap(Vector3 enemyPos, int num)
{
	// �v���C���[�̍��W
	Vector3 playerPos = m_playerManagement->GetPosition();

	Vector3 mapPos;

	// �}�b�v�ɕ\������͈͂ɓG��������
	if (WorldPositionConvertToMapPosition(playerPos, enemyPos, mapPos, false)) {

		// spriteRender�ɍ��W��ݒ�
		m_EnemySpriteRender[num].SetPosition(mapPos);
		// �}�b�v�ɕ\������
		m_isImage[num] = true;
	}
	// �}�b�v�ɕ\������G�����Ȃ�������
	else {
		m_isImage[num] = false;
	}
}

void MiniMap::DrawMap_Treasure(Vector3 TreasurePos)
{
	// �v���C���[�̍��W
	Vector3 playerPos = m_playerManagement->GetPosition();
	Vector3 mapPos;

	//������W���}�b�v��̍��W�ɕϊ�����
	WorldPositionConvertToMapPosition(playerPos, TreasurePos, mapPos, true);

	m_TreasureSpriteRender.SetPosition(mapPos);
	m_TreasureSpriteRender.Update();
}

const bool MiniMap::WorldPositionConvertToMapPosition(Vector3 worldCenterPosition, Vector3 worldPosition, Vector3& mapPosition, const bool isTresure)
{
	// Y���W�̓}�b�v�Ƃ͊֌W�Ȃ��̂�0.0f��ݒ�
	worldCenterPosition.y = 0.0f;
	worldPosition.y = 0.0f;

	// ���S�̍��W����\���������I�u�W�F�N�g�̍��W�֌������x�N�g�����v�Z
	Vector3 diff = worldPosition - worldCenterPosition;
	Vector3 diff2 = diff;

	if (!isTresure) {

		// 	�v�Z�����x�N�g�������ȏ㗣��Ă�����
		if (diff.LengthSq() >= LIMITED_RANGE_IMAGE * LIMITED_RANGE_IMAGE) {

			// �͈͊O�ɑ��݂��Ă���̂Ń}�b�v�ɕ\�����Ȃ�
			return false;
		}
	}

	// �x�N�g���̒������擾����
	float length = diff.Length();

	// �J�����̑O�����x�N�g������A
	// Y�������̉�]�N�H�[�^�j�I���𐶐�����
	Vector3 forward = g_camera3D->GetForward();
	Quaternion rot;
	rot.SetRotationY(atan2(-forward.x, forward.z));

	// �x�N�g���ɃJ�����̉�]��K�p
	rot.Apply(diff);

	// �x�N�g���𐳋K��
	diff.Normalize();

	// �}�b�v�̑傫��/�͈͂ŁA
	// �x�N�g�����}�b�v���W�n�ɕϊ�����
	diff *= length * MAP_RADIUS / LIMITED_RANGE_IMAGE;

	// �}�b�v�̒������W�Ə�L�x�N�g�������Z����
	mapPosition = Vector3(CENTER_POSITION.x + diff.x, CENTER_POSITION.y + diff.z, 0.0f);

	if (isTresure) {
		// 	�v�Z�����x�N�g�������ȏ㗣��Ă�����
		if (diff2.LengthSq() >= LIMITED_RANGE_IMAGE * LIMITED_RANGE_IMAGE) {
			//������W�̕��������߂�
			diff2 =	Vector3(mapPosition.x - CENTER_POSITION.x, mapPosition.y - CENTER_POSITION.y, 0.0f);
			length = diff2.Length();
			//�}�b�v��Ɏʂ����W�����߂�
			mapPosition.x = CENTER_POSITION.x + (diff2.x / length) * MAP_RADIUS;
			mapPosition.y = CENTER_POSITION.y + (diff2.y / length) * MAP_RADIUS;

			// �͈͊O�ɑ��݂��Ă���
			return false;
		}
	}
	return true;
}

void MiniMap::Render(RenderContext& rc)
{
	// �`��
	m_SpriteRender.Draw(rc);
	m_OutLineSpriteRender.Draw(rc);
	m_PlayerSpriteRender.Draw(rc);

	m_TreasureSpriteRender.Draw(rc);

	for (int i = 0; i < m_enemyList.size(); i++) {
		// �͈͓��̂Ƃ�
		if (m_isImage[i] == true) {
			// �`�悷��
			m_EnemySpriteRender[i].Draw(rc);
		}
	}
}