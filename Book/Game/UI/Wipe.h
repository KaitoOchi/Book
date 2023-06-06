#pragma once

class Wall;
class BackGround;

namespace
{
	const Vector3	WIPE_POS_MAX = { 8.0f, 662.0f, 0.0f };			//���C�v�̍ő���W
	const Vector3	WIPE_POS_MIN = { -270.0f, 662.0f, 0.0f };		//���C�v�̍ŏ����W
	const Vector3	OUTLINE_POS_MAX = { -671.5f, -280.0f, 0.0f };	//�֊s�摜�̍ő���W
	const Vector3	OUTLINE_POS_MIN = { -950.0f, -280.0f, 0.0f };	//�֊s�摜�̍ŏ����W
	const int		ENEMY_NUM_WIPE = 4;								//�G�̐�
}

/// <summary>
/// ���C�v�N���X�B
/// </summary>
class Wipe : public IGameObject
{
public:
	Wipe();
	~Wipe();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

public:
	/// <summary>
	/// �G�̒n�_�����Z�b�g
	/// </summary>
	void Reset()
	{
		//�G�̍��W�����Z�b�g
		for (int i = 0; i < 3; i++) {
			m_enemy[i].moveSpeed[0] = m_bezierPos[0];
			m_enemy[i].moveSpeed[1] = m_bezierPos[0];
			m_enemy[i].moveSpeed[2] = m_bezierPos[1];
		}
		
		//���C�v���W��������
		m_wipePos = WIPE_POS_MIN;
		RenderingEngine::GetInstance()->GetWipeViewPort().TopLeftX = m_wipePos.x;
		
		//�g�g�ݍ��W��������
		m_outlinePos = OUTLINE_POS_MIN;
		
		//���C�v��\����Ԃɂ���
		m_isWipe = true;

		//�^�C�}�[�̃��Z�b�g
		m_timer = 0.0f;
		m_outlineTimer = 0.0f;
	}

	/// <summary>
	/// ���C�v�̏I�������B
	/// </summary>
	void WipeEnd()
	{
		m_timer = 0.0f;
		m_isWipe = false;
	}

private:
	/// <summary>
	/// ���x�������_�[�����B
	/// </summary>
	void LevelDesign();

	/// <summary>
	/// �G�̈ړ������B
	/// </summary>
	void EnemyMove();

	/// <summary>
	/// �֊s���摜�̏����B
	/// </summary>
	void WipeOutline();

private:
	//�G�̍\����
	struct EnemyStruct
	{
		ModelRender modelRender;	//���f��
		Vector3		moveSpeed[3];	//�ړ����x
	};

private:
	SpriteRender		m_outlineSpriteRender;		//���C�v�̗֊s�摜
	SpriteRender		m_warningSpriteRender;		//�x���摜
	LevelRender			m_levelRender;				//���x�������_�[
	PointLight			m_pointLight;				//�|�C���g���C�g
	AnimationClip*		m_enemyAnim = nullptr;		//�G�̃A�j���[�V����
	BackGround*			m_backGround = nullptr;		//�n��
	std::vector<Wall*>	m_stage;					//�X�e�[�W
	EnemyStruct			m_enemy[ENEMY_NUM_WIPE];	//�G�̍\����
	Vector3				m_bezierPos[3];				//�G�̈ړ����W
	Vector3				m_outlinePos;				//�֊s���̍��W
	Vector3				m_wipePos;					//���C�v�̍��W
	bool				m_isWipe = false;			//���C�v��\�����邩�ǂ���
	float				m_timer = 0.0f;				//�^�C�}�[
	float				m_outlineTimer = 0.0f;		//���C�v�p�^�C�}�[
	float				m_alpha = 0.0f;				//����
};