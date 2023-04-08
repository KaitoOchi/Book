#include "stdafx.h"
#include "MiniMap.h"

#include "PlayerManagement.h"
#include "Enemy_Normal.h"
#include "Enemy_Serch.h"
#include "Enemy_Charge.h"

namespace
{
	const Vector3	CENTER_POSITION = Vector3(635.0f, -290.0f, 0.0f);		// �}�b�v�̒��S
	const float		MAP_RADIUS = 150.0f;									// �}�b�v�̔��a
	const float		LIMITED_RANGE_IMAGE = 500.0f;							// �}�b�v�͈̔�
	const float		ALPHA = 0.75f;											// ���l
	const int		ENEMY_NUM = 3;											// �G�l�~�[�̑���
}

MiniMap::MiniMap()
{
}

MiniMap::~MiniMap()
{
}

bool MiniMap::Start()
{
	// �x�[�X
	m_SpriteRender.Init("Assets/sprite/UI/miniMap/base.DDS", 340, 340);
	m_SpriteRender.SetPosition(CENTER_POSITION);
	//m_SpriteRender.SetMulColor({ 1.0f, 1.0f, 1.0f, ALPHA });

	// �x�[�X�̑���
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
	m_enemyNormal = FindGO<Enemy_Normal>("enemyNormal");
	m_enemySerch = FindGO<Enemy_Serch>("enemySerch");
	m_enemyCharge = FindGO<Enemy_Charge>("enemyCharge");

	return true;
}

void MiniMap::Update()
{
	// �}�b�v���W�ɕϊ�
	DrawMap(m_enemyNormal->GetPosition(),0);
	DrawMap(m_enemySerch->GetPosition(), 1);
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
	// ���W���擾
	Vector3 playerPos = m_playerManagement->GetPosition();

	Vector3 mapPos;

	// �}�b�v�ɕ\������͈͂ɓG��������
	if (WorldPositionConvertToMapPosition(playerPos, enemyPos, mapPos)) {

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

	for (int i = 0; i < ENEMY_NUM; i++) {
		// �G�l�~�[���߂��ɂ��Ȃ��Ƃ�
		if (m_isImage[i] == true) {
			// �߂��ɂ���Ƃ��͕`�悷��
			m_EnemySpriteRender[i].Draw(rc);
		}
	}
}