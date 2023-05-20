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
	m_physicsGhostList = m_game->GetPhysicsGhostList();
	// ����̈ʒu
	m_treasurePos = m_treasure->GetPosition();

	// �摜��p�ӂ���
	// �}�b�v�摜�̐ݒ�
	m_SpriteRender.Init("Assets/sprite/UI/miniMap/base.DDS", 340, 340);
	m_SpriteRender.SetPosition(CENTER_POSITION);
	//m_SpriteRender.SetMulColor({ 1.0f, 1.0f, 1.0f, ALPHA });

	// �A�E�g���C���摜�̐ݒ�
	m_OutLineSpriteRender.Init("Assets/sprite/UI/miniMap/base_outLine.DDS", 362, 519);
	m_OutLineSpriteRender.SetPosition({ 640.0f, -210.0f, 0.0f });
	m_OutLineSpriteRender.Update();

	// �v���C���[�摜�̐ݒ�
	m_PlayerSpriteRender.Init("Assets/sprite/UI/miniMap/player.DDS", 20,40);
	m_PlayerSpriteRender.SetPosition(CENTER_POSITION);

	// �G�l�~�[�摜�̐ݒ�
	for (int i = 0; i < m_enemyList.size(); i++) {
		m_EnemySpriteRender[i].Init("Assets/sprite/UI/miniMap/map_2.DDS", 15, 15);
		m_isImage[i] = false;
	}

	//�ǉ摜�̐ݒ�
	for (int i = 0; i < m_physicsGhostList.size(); i++) {
		m_wallSpriteRender[i].Init("Assets/sprite/UI/miniMap/map_wall.DDS", 20.0f, 20.0f);
		m_enableWallSprite[i] = false;
	}

	// ����摜�̐ݒ�
	m_TreasureSpriteRender.Init("Assets/sprite/UI/miniMap/map_exit.DDS", 20.0f, 20.0f);
	m_TreasureSpriteRender.SetMulColor(Vector4(1.0f, 1.0f, 0.0f, 1.0f));

return true;
}

void MiniMap::Update()
{
	// �}�b�v���W�ɕϊ�
	DrawMap();

	// �����`��
	DrawMap_Treasure(m_treasurePos);

	m_PlayerSpriteRender.Update();
	m_SpriteRender.Update();
}

void MiniMap::DrawMap()
{
	// �v���C���[�̍��W
	Vector3 playerPos = m_playerManagement->GetPosition();
	Vector3 mapPos;
	Vector3 enemyPos;

	for (int i = 0; i < m_enemyList.size(); i++) {
		//�G���X�g������W���擾
		enemyPos =  m_enemyList[i]->GetPosition();

		// �}�b�v�ɕ\������͈͂ɓG��������
		if (WorldPositionConvertToMapPosition(playerPos, enemyPos, mapPos, false)) {

			Vector3 diff = enemyPos - playerPos;
			diff.y = 0.0f;
			float alpha = (pow(LIMITED_RANGE_IMAGE, 2.0f) - diff.LengthSq()) / 100000.0f;

			// spriteRender�ɍ��W��ݒ�
			m_EnemySpriteRender[i].SetPosition(mapPos);
			m_EnemySpriteRender[i].SetMulColor(Vector4(1.0f, 1.0f, 1.0f, alpha));
			m_EnemySpriteRender[i].Update();
			// �}�b�v�ɕ\������
			m_isImage[i] = true;
		}
		// �}�b�v�ɕ\������G�����Ȃ�������
		else {
			m_isImage[i] = false;
		}
	}

	for (int i = 0; i < m_physicsGhostList.size(); i++) {
		//�ǃ��X�g������W���擾
		enemyPos = m_physicsGhostList[i]->GetPosition();

		// �}�b�v�ɕ\������͈͂ɓG��������
		if (WorldPositionConvertToMapPosition(playerPos, enemyPos, mapPos, false)) {

			// spriteRender�ɍ��W��ݒ�
			m_wallSpriteRender[i].SetPosition(mapPos);
			m_wallSpriteRender[i].Update();
			// �}�b�v�ɕ\������
			m_enableWallSprite[i] = true;
		}
		// �}�b�v�ɕ\������G�����Ȃ�������
		else {
			m_enableWallSprite[i] = false;
		}
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

const bool MiniMap::WorldPositionConvertToMapPosition(Vector3 worldCenterPosition, Vector3 worldPosition, Vector3& mapPosition, const bool isTreasure)
{
	// Y���W�̓}�b�v�Ƃ͊֌W�Ȃ��̂�0.0f��ݒ�
	worldCenterPosition.y = 0.0f;
	worldPosition.y = 0.0f;

	// ���S�̍��W����\���������I�u�W�F�N�g�̍��W�֌������x�N�g�����v�Z
	Vector3 diff = worldPosition - worldCenterPosition;
	Vector3 diff2 = diff;

	if (!isTreasure) {

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

	if (isTreasure) {

		//���󂪂�������։�]������
		rot.SetRotationZ(atan2(mapPosition.y - CENTER_POSITION.y, mapPosition.x - CENTER_POSITION.x) + 0.5);
		m_TreasureSpriteRender.SetRotation(rot);

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
	//�~�j�}�b�v�摜�̕`��
	m_SpriteRender.Draw(rc);

	//�g�摜�̕`��
	m_OutLineSpriteRender.Draw(rc);

	//�v���C���[�摜�̕`��
	m_PlayerSpriteRender.Draw(rc);

	//����摜�̕`��
	m_TreasureSpriteRender.Draw(rc);

	//�G�摜�̕`��
	for (int i = 0; i < m_enemyList.size(); i++) {

		if (m_isImage[i] == true) {
			m_EnemySpriteRender[i].Draw(rc);
		}
	}

	//�ǉ摜�̕`��
	for (int i = 0; i < m_physicsGhostList.size(); i++) {

		if (m_enableWallSprite[i]) {
			m_EnemySpriteRender[i].Draw(rc);
		}
	}
}