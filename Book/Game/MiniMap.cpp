#include "stdafx.h"
#include "MiniMap.h"

#include "Player.h"
#include "Player2D.h"
#include "Player3D.h"
#include "Enemy.h"

namespace
{
	Vector3 CENTER_POSITION = Vector3(0.0f, 0.0f, 0.0f);	// �}�b�v�̒��S
	float MAP_RADIUS = 240.0f;								// �}�b�v�̔��a
	float LIMITED_RANGE_IMAGE = 600.0f;						// �}�b�v�͈̔�
}

MiniMap::MiniMap()
{
}

MiniMap::~MiniMap()
{
}

bool MiniMap::Start()
{
	// �}�b�v�̍����摜

	// ���S�̉摜

	// �G�l�~�[��\���摜


	// �C���X�^���X��T��
}

void MiniMap::Update()
{
	// ���W���擾
	Vector3 playerPos = m_player->GetPosition();
	Vector3 enemyPos = m_enemy->GetPosition();

	Vector3 mapPos;

	// �}�b�v�ɕ\������͈͂ɓG��������
	if (WorldPositionConvertToMapPosition(playerPos, enemyPos, mapPos)) {
		// �}�b�v�ɕ\������
		m_isImage = true;
		// spriteRender�ɍ��W��ݒ�
		m_EnemySpriteRender.SetPosition(mapPos);
	}
	// �}�b�v�ɕ\������G�����Ȃ�������
	else {
		m_isImage = false;
	}
}

const bool MiniMap::WorldPositionConvertToMapPosition(Vector3 worldCenterPosition,Vector3 worldPosition,Vector3& mapPosition) 
{
	// Y���W�̓}�b�v�Ƃ͊֌W�Ȃ��̂�0.0f��ݒ�
	worldCenterPosition.y = 0.0f;
	worldPosition.y = 0.0f;

	// ���S���W����\���������I�u�W�F�N�g�̍��W�֌������x�N�g�����v�Z
	Vector3 diff = worldPosition - worldCenterPosition;

	// �v�Z�����x�N�g�������ȏ㗣��Ă�����
	if (diff.LengthSq() >= LIMITED_RANGE_IMAGE * LIMITED_RANGE_IMAGE) {
		// �͈͊O�ɑ��݂��Ă���̂Ń}�b�v�ɕ\�����Ȃ�
		return false;
	}

	/*�ȉ��R�s�y�B�G�̌��������f����Ă��Ȃ��炵���̂Ŕ��f�����邱��*/


	//�x�N�g���̒������擾����
	float length = diff.Length();

	//�J�����̑O�����x�N�g������B
	//�N�H�[�^�j�I���𐶐��B
	Vector3 forward = g_camera3D->GetForward();
	Quaternion rot;
	rot.SetRotationY(atan2(-forward.x, forward.z));

	//�x�N�g���ɃJ�����̉�]��K�p�B
	rot.Apply(diff);

	//�x�N�g���𐳋K���B
	diff.Normalize();

	//�}�b�v�̑傫��/���������ŁB
	//�x�N�g�����}�b�v���W�n�ɕϊ�����B
	diff *= length * MAP_RADIUS / LIMITED_RANGE_IMAGE;

	//�}�b�v�̒������W�Ə�L�x�N�g�������Z����B
	mapPosition = Vector3(CENTER_POSITION.x + diff.x, CENTER_POSITION.y + diff.z, 0.0f);
	return true;
}

void MiniMap::Render(RenderContext& rc)
{
	m_SpriteRender.Draw(rc);
	m_PlayerSpriteRender.Draw(rc);

	if (m_isImage == false) {
		return;
	}

	m_EnemySpriteRender.Draw(rc);
}