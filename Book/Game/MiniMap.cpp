#include "stdafx.h"
#include "MiniMap.h"

#include "PlayerManagement.h"
#include "Enemy_Normal.h"

namespace
{
	const Vector3 CENTER_POSITION = Vector3(635.0f, -290.0f, 0.0f);		// �}�b�v�̒��S
	const float MAP_RADIUS = 150.0f;									// �}�b�v�̔��a
	const float LIMITED_RANGE_IMAGE = 600.0f;							// �}�b�v�͈̔�
	const float ALPHA = 0.75f;
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
	m_SpriteRender.Init("Assets/sprite/UI/miniMap/base.DDS", 340, 340);
	m_SpriteRender.SetPosition(CENTER_POSITION);
	//m_SpriteRender.SetMulColor({ 1.0f, 1.0f, 1.0f, ALPHA });

	// ����̉摜
	m_OutLineSpriteRender.Init("Assets/sprite/UI/miniMap/base_outLine.DDS", 362, 519);
	m_OutLineSpriteRender.SetPosition({ 635.0f, -200.0f, 0.0f });
	m_OutLineSpriteRender.Update();

	// ���S�̉摜
	m_PlayerSpriteRender.Init("Assets/sprite/UI/miniMap/player.DDS", 20,40);
	m_PlayerSpriteRender.SetPosition(CENTER_POSITION);

	// �G�l�~�[��\���摜
	m_EnemySpriteRender.Init("Assets/sprite/UI/miniMap/map_2.DDS", 15, 15);

	// �C���X�^���X��T��
	m_playerManagement = FindGO<PlayerManagement>("playerManagement");
	m_enemyNormal = FindGO<Enemy_Normal>("enemyNormal");

	return true;
}

void MiniMap::Update()
{
	DrawMap();

	m_EnemySpriteRender.Update();
	m_PlayerSpriteRender.Update();
	m_SpriteRender.Update();
}

void MiniMap::DrawMap()
{
	// ���W���擾
	Vector3 playerPos = m_playerManagement->GetPosition();
	Vector3 enemy_NormalPos = m_enemyNormal->GetPosition();

	Vector3 mapPos;

	// �}�b�v�ɕ\������͈͂ɓG��������
	if (WorldPositionConvertToMapPosition(playerPos, enemy_NormalPos, mapPos)) {
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

	/*�ȉ��R�s�y�B
	�G�̌��������f����Ă��Ȃ��炵���̂Ŕ��f�����邱�ƁB*/


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
	// �`��
	m_SpriteRender.Draw(rc);
	m_OutLineSpriteRender.Draw(rc);
	m_PlayerSpriteRender.Draw(rc);

	// �G�l�~�[���߂��ɂ��Ȃ��Ƃ�
	if (m_isImage == false) {
		// �����ŏ������I��
		return;
	}

	m_EnemySpriteRender.Draw(rc);
}