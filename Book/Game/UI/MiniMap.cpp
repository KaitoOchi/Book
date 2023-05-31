#include "stdafx.h"
#include "MiniMap.h"

#include "PlayerManagement.h"
#include "Enemy_Normal.h"
#include "Enemy_Search.h"
#include "Enemy_Charge.h"
#include "Game.h"
#include "Enemy.h"

namespace
{
	const Vector3	CENTER_POSITION = Vector3(635.0f, -290.0f, 0.0f);		// �}�b�v�̒��S
	const float		MAP_RADIUS = 140.0f;									// �}�b�v�̔��a
	const float		LIMITED_RANGE_IMAGE = 650.0f;							// �}�b�v�͈̔�
}

MiniMap::MiniMap()
{
	m_enemyList.reserve(ENEMY_NUM);
	m_physicsGhostList.reserve(WALL_NUM);
}

MiniMap::~MiniMap()
{
	m_enemyList.clear();
	m_enemyList.shrink_to_fit();
	m_physicsGhostList.clear();
	m_physicsGhostList.shrink_to_fit();
}

bool MiniMap::Start()
{
	// �C���X�^���X��T��
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
	Game* game = FindGO<Game>("game");

	// �G�l�~�[�̃��X�g�������Ă���
	m_enemyList = game->GetEnemyList();
	m_physicsGhostList = game->GetPhysicsGhostList();

	// �w�i�摜�̐ݒ�
	m_spriteRender.Init("Assets/sprite/UI/miniMap/base.DDS", 340, 340);
	m_spriteRender.SetPosition(CENTER_POSITION);
	m_spriteRender.Update();

	// �����摜�̐ݒ�
	m_outlineSpriteRender.Init("Assets/sprite/UI/miniMap/base_outLine.DDS", 362, 519);
	m_outlineSpriteRender.SetPosition({ 640.0f, -210.0f, 0.0f });
	m_outlineSpriteRender.Update();

	// �v���C���[�摜�̐ݒ�
	m_playerSpriteRender.Init("Assets/sprite/UI/miniMap/player.DDS", 20,40);
	m_playerSpriteRender.SetPosition(CENTER_POSITION);
	m_playerSpriteRender.Update();

	// �G�l�~�[�摜�̐ݒ�
	for (int i = 0; i < m_enemyList.size(); i++) {
		m_enemySpriteRender[i].Init("Assets/sprite/UI/miniMap/map_enemy.DDS", 15, 15);
		m_enableEnemySprites[i] = false;
	}

	//�ǉ摜�̐ݒ�
	for (int i = 0; i < m_physicsGhostList.size(); i++) {
		m_wallSpriteRender[i].Init("Assets/sprite/UI/miniMap/map_wall.DDS", 20.0f, 20.0f);
		m_enableWallSprites[i] = false;
	}

	// ����摜�̐ݒ�
	m_treasureSpriteRender.Init("Assets/sprite/UI/miniMap/map_exit.DDS", 32.0f, 32.0f);

	return true;
}

void MiniMap::Update()
{
	// �v���C���[�̍��W���擾
	m_playerPos = m_playerManagement->GetPosition();

	// �}�b�v���W�ɕϊ�
	DrawMap_Actor();
}

void MiniMap::DrawMap_Actor()
{
	Vector3 mapPos;
	Vector3 enemyPos;
	float alpha = 0.0f;
	m_isTreasure = false;

	//�G���}�b�v�ɕ`��
	for (int i = 0; i < m_enemyList.size(); i++) {

		if (m_enemyList[i]->GetActiveFlag()) {
			continue;
		}

		enemyPos =  m_enemyList[i]->GetPosition();
		m_enableEnemySprites[i] = DrawMap(enemyPos, alpha);
		//�G�摜�̐ݒ�
		m_enemySpriteRender[i].SetPosition(m_mapPos);
		m_enemySpriteRender[i].SetMulColor(Vector4(1.0f, 1.0f, 1.0f, alpha));
		m_enemySpriteRender[i].Update();
	}

	//�ǂ��}�b�v�ɕ`��
	for (int i = 0; i < m_physicsGhostList.size(); i++) {

		enemyPos = m_physicsGhostList[i]->GetPosition();
		m_enableWallSprites[i] = DrawMap(enemyPos, alpha);
		//�ǉ摜�̐ݒ�
		m_wallSpriteRender[i].SetPosition(m_mapPos);
		m_wallSpriteRender[i].SetMulColor(Vector4(1.0f, 1.0f, 1.0f, alpha));
		m_wallSpriteRender[i].Update();
	}

	//������}�b�v�ɕ`��
	m_isTreasure = true;
	DrawMap(m_treasurePos, alpha);
	//����摜�̐ݒ�
	m_treasureSpriteRender.SetPosition(m_mapPos);
	m_treasureSpriteRender.Update();
}

bool MiniMap::DrawMap(const Vector3& enemyPos, float& alpha)
{
	// �}�b�v�ɕ\������͈͂ɓG��������
	if (WorldPositionConvertToMapPosition(m_playerPos, enemyPos)) {

		Vector3 diff = enemyPos - m_playerPos;
		diff.y = 0.0f;
		alpha = (pow(LIMITED_RANGE_IMAGE, 2.0f) - diff.LengthSq()) / 100000.0f;

		return true;
	}
	// �}�b�v�ɕ\������G�����Ȃ�������
	else {
		return false;
	}
}

const bool MiniMap::WorldPositionConvertToMapPosition(const Vector3& worldCenterPosition, const Vector3& worldPosition)
{
	// ���S�̍��W����\���������I�u�W�F�N�g�̍��W�֌������x�N�g�����v�Z
	Vector3 diff = worldPosition - worldCenterPosition;
	diff.y = 0.0f;
	Vector3 diff2 = diff;

	if (!m_isTreasure) {

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
	m_mapPos = Vector3(CENTER_POSITION.x + diff.x, CENTER_POSITION.y + diff.z, 0.0f);

	if (m_isTreasure) {

		//���󂪂�������։�]������
		rot.SetRotationZ(atan2(m_mapPos.y - CENTER_POSITION.y, m_mapPos.x - CENTER_POSITION.x) - 1.5f);
		m_treasureSpriteRender.SetRotation(rot);

		// 	�v�Z�����x�N�g�������ȏ㗣��Ă�����
		if (diff2.LengthSq() >= LIMITED_RANGE_IMAGE * LIMITED_RANGE_IMAGE) {
			//������W�̕��������߂�
			diff2 =	Vector3(m_mapPos.x - CENTER_POSITION.x, m_mapPos.y - CENTER_POSITION.y, 0.0f);
			length = diff2.Length();

			//�}�b�v��Ɏʂ����W�����߂�
			m_mapPos.x = CENTER_POSITION.x + (diff2.x / length) * MAP_RADIUS;
			m_mapPos.y = CENTER_POSITION.y + (diff2.y / length) * MAP_RADIUS;

			// �͈͊O�ɑ��݂��Ă���
			return false;
		}
	}
	return true;
}

void MiniMap::Render(RenderContext& rc)
{
	//�w�i�摜�̕`��
	m_spriteRender.Draw(rc);

	//�g�摜�̕`��
	m_outlineSpriteRender.Draw(rc);

	//�G�摜�̕`��
	for (int i = 0; i < m_enemyList.size(); i++)
	{
		if (m_enableEnemySprites[i] == true) {
			m_enemySpriteRender[i].Draw(rc);
		}
	}

	//�ǉ摜�̕`��
	for (int i = 0; i < m_physicsGhostList.size(); i++)
	{
		if (m_enableWallSprites[i]) {
			m_wallSpriteRender[i].Draw(rc);
		}
	}

	//����摜�̕`��
	m_treasureSpriteRender.Draw(rc);

	//�v���C���[�摜�̕`��
	m_playerSpriteRender.Draw(rc);
}