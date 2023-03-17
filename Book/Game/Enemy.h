#pragma once

class PlayerManagement;
class Enemy :public IGameObject
{
public:
	Enemy();
	~Enemy();

	bool Start();

	bool SeachPlayer();					// �v���C���[�𔭌����鏈��
	bool CatchPlayer();					// �v���C���[���m�ۂ��鏈��
	void HitFlashBullet();				// �M���e�����������Ƃ��̏���
	void Act_Craw();					// ����s��
	void Act_Tracking();				// �ǐՍs��
	void Act_Access();					// �ڋߍs��
	void Act_Stop(float time);			// �s����~
	void SpotLight_Serch();				//�X�|�b�g���C�g���G

	// �G�l�~�[�̃A�j���[�V�����X�e�[�g
	enum EnEnemyAnimationState
	{
		m_enEnemyAnimationState_Idle,	// �ҋ@
		m_enEnemyAnimationState_Walk,	// ����
		m_enEnemyAnimationState_Run,	// ����
		m_enEnemyAnimationState_Attack,	// �U��
		m_enEnemyAnimationState_Damege,	// ��e
	};
	// �A�j���[�V�����X�e�[�g
	EnEnemyAnimationState m_enEnemyAnimationState = m_enEnemyAnimationState_Idle;

	// �G�l�~�[�̍s���X�e�[�g
	enum EnEnemyActState
	{
		m_enEnemyActState_Craw,			// ����
		m_enEnemyActState_Tracking,		// �ǐ�
		m_enEnemyActState_Waiting,		// �ҋ@
		m_enEnemyActState_Confusion		// ����
	};
	// �s���X�e�[�g
	EnEnemyActState m_enEnemyActState = m_enEnemyActState_Craw;

	/// <summary>
	/// ���W��ݒ肷��
	/// </summary>
	void SetPosition(Vector3 pos) {
		m_position = pos;
	}

	/// <summary>
	/// ��]��ݒ肷��
	/// </summary>
	void SetRotation(Quaternion rot) {
		m_rotation = rot;
	}

	/// <summary>
	/// �X�P�[����ݒ肷��
	/// </summary>
	void SetScale(Vector3 sca) {
		m_scale = sca;
	}

	/// <summary>
	/// ��e�t���O�ݒ�p�B�K�v�Ȃ���Ώ���
	/// </summary>
	/// <param name="">��e�������ǂ����ǂ������肷��Btrue�Ȃ��e�����Ɣ���</param>
	void SetHitFlashBullet(bool b) {
		b = HitFlashBulletFlag;
	};

	/// <summary>
	/// ���W���擾����
	/// </summary>
	const Vector3& GetPosition() const {
		return m_position;
	}

	/// <summary>
	/// �X�P�[�����擾����
	/// </summary>
	const Vector3& GetScale() const {
		return m_scale;
	}

	/// <summary>
	/// ��]���擾����
	/// </summary>
	const Quaternion& GetRotation() const {
		return m_rotation;
	}

protected:

	// �p�X�ړ��p�̃|�C���g�\����
	struct Point {
		Vector3 s_position;					// �|�C���g�̍��W
		int s_number;						// �|�C���g�̔ԍ�
	};

	std::vector<Point> m_pointList;			// �|�C���g�\���̂̔z��
	Point* m_point = nullptr;				// �|�C���g�\���̂̃|�C���^�A���݂̖ړI�n�ɂȂ�

	PlayerManagement* m_playerManagement = nullptr;

	CharacterController m_characterController;
	FontRender m_fontRender;				// �t�H���g�����_�[

	Vector3 m_position = Vector3::Zero;		// �G�l�~�[�̍��W
	Vector3 m_forward = Vector3::AxisZ;		// �G�l�~�[�̑O����
	Vector3 m_scale = Vector3::One;			// �X�P�[��
	Quaternion m_rotation;					// ��]

	bool HitFlashBulletFlag = false;		// �M���e�������������ǂ���
	float addTimer = 0.0f;					// ���Z����^�C�}�[
	ModelRender m_enemyRender;
	SpotLight m_spotLight;				//�X�|�b�g���C�g
};