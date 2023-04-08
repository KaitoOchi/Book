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
	/// �}�b�v�ɕ`�悷�邩�ǂ����̔���
	/// </summary>
	/// <param name="pos">�ϊ�����G�l�~�[�̍��W</param>
	/// <param name="num">�z��ԍ�</param>
	void DrawMap(Vector3 enemyPos,int num);		

private:
	/// <summary>
	/// ���[���h���W�n����}�b�v���W�n�ɕϊ�
	/// </summary>
	/// <param name="worldcenterPosition">�}�b�v�̒��S�Ƃ���I�u�W�F�N�g�̃��[���h���W</param>
	/// <param name="worldPosition">�}�b�v�ɕ\���������I�u�W�F�N�g�̃��[���h���W</param>
	/// <param name="mapPosirion">�ϊ�������̃}�b�v���W</param>
	/// <returns></returns>
	const bool WorldPositionConvertToMapPosition(
		Vector3 worldcenterPosition,
		Vector3 worldPosition,
		Vector3& mapPosirion
	);

	SpriteRender m_SpriteRender;			// �X�v���C�g�����_�[�B�~�j�}�b�v�̃x�[�X
	SpriteRender m_OutLineSpriteRender;		// �X�v���C�g�����_�[�B�~�j�}�b�v�̑�������
	SpriteRender m_PlayerSpriteRender;		// �X�v���C�g�����_�[�B�v���C���[
	SpriteRender m_EnemySpriteRender[2];	// �X�v���C�g�����_�[�B�G�l�~�[

	PlayerManagement* m_playerManagement = nullptr;
	Enemy_Normal* m_enemyNormal = nullptr;
	Enemy_Serch* m_enemySerch = nullptr;
	Enemy_Charge* m_enemyCharge = nullptr;

	bool m_isImage[5];						// �\�����邩�ǂ����̃t���O�B�G�l�~�[�̐����p�ӂ���
};

