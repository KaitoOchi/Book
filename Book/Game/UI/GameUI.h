#pragma once

class PlayerManagement;
class Game;
class Player3D;
class Treasure;

/// <summary>
/// �Q�[��UI�N���X�B
/// </summary>
class GameUI : public IGameObject	
{
public:
	GameUI();
	~GameUI();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

public:
	/// <summary>
	/// �^�C�����擾�B
	/// </summary>
	const float GetTime()
	{
		return m_timer;
	}

	/// <summary>
	/// �M���e�̐�
	/// </summary>
	/// <param name="flash">���݂̑M���e�̐�</param>
	const void SetFlashSlot(const int flash)
	{
		m_flashNumber = flash;
	}

	/// <summary>
	/// �����e�̐�
	/// </summary>
	/// <param name="flash">���݂̉����e�̐�</param>
	/// <returns></returns>
	const void SetSoundSlot(const int sound)
	{
		m_soundNumber = sound;
	}

	/// <summary>
	/// �~�`�Q�[�W�𑝂₹�邩�ǂ���
	/// </summary>
	/// <param name="state"></param>
	void SetCircleState(bool state)
	{
		m_circleState = state;
	}

	/// <summary>
	/// �~�`�Q�[�W���ő剻�ǂ����̔���
	/// </summary>
	/// <returns></returns>
	const bool GetCircleMAXState()
	{
		return m_circleMaxState;
	}

	/// <summary>
	/// �~�`�Q�[�W��`�悷�邩�ǂ���
	/// </summary>
	/// <param name="state">true�Ȃ�`��</param>
	/// <returns></returns>
	const void SetCircleDrawState(const bool state)
	{
		m_circleDrawState = state;
	}

	/// <summary>
	/// �~�`�Q�[�W�̊p�x���擾
	/// </summary>
	/// <returns></returns>
	const float GetDegree()
	{
		return m_degree;
	}

	/// <summary>
	/// �~�`�Q�[�W�̕`���Ԃ��擾�B
	/// </summary>
	const bool GetCircleDrawState()
	{
		return m_circleDrawState;
	}

	/// <summary>
	/// �Q�[�W�X�e�[�g�̐ݒ�B
	/// </summary>
	/// <param name="state"></param>
	void SetChangeGaugeState(const bool state)
	{
		m_changeGaugeState = state;
	}

	/// <summary>
	/// �X�^�~�i�Q�[�W�̕`���Ԃ�ݒ�B
	/// </summary>
	void SetStaminaDrawState(const bool state)
	{
		m_staminaDrawState = state;
	}

	/// <summary>
	/// �X�^�~�i�Q�[�W�̕ύX
	/// </summary>
	void StaminaGage(const float stamina, const Vector3& pos);

private:
	/// <summary>
	/// ���Ԍv�������B
	/// </summary>
	void Time();

	/// <summary>
	/// �Q�[�W�̕ύX�����B
	/// </summary>
	void ChangeGage();

	/// <summary>
	/// ���Ԃ̈ړ������B
	/// </summary>
	void TimeMove(const int m, const int s);

	/// <summary>
	/// �A�C�e���̏������̏����B
	/// </summary>
	void ItemSlot();

	/// <summary>
	/// �A�C�e���̊g�又���B
	/// </summary>
	void ItemScaleUp();

	/// <summary>
	/// �~�`�Q�[�W�̕ύX�B
	/// </summary>
	void CircleChange();

private:
	SpriteRender		m_itemFlashRender;				//�M���e�̉摜
	SpriteRender		m_itemSoundRender;				//�����e�̉摜
	SpriteRender		m_itemBaseRender;				//�A�C�e���̔w�i�摜
	SpriteRender		m_gageFrameSpriteRender;		//�Q�[�W�̘g�摜
	SpriteRender		m_gageSpriteRender;				//�Q�[�W�摜
	SpriteRender		m_staminaBaseRender;			//�X�^�~�i�Q�[�W�x�[�X�摜
	SpriteRender		m_staminaGageRender;			//�X�^�~�i�Q�[�W�摜
	SpriteRender		m_circleBaseSpriteRender;		//�~�`�Q�[�W�̃x�[�X�摜
	SpriteRender		m_circleSpriteRender;			//�~�`�Q�[�W�̃Q�[�W�摜
	SpriteRender		m_guideItemSpriteRender;		//�A�C�e���g�p�K�C�h�̃e�L�X�g�摜
	FontRender			m_timeFontRender;				//�^�C������
	FontRender			m_itemFlashNumber;				//�M���e�̐�
	FontRender			m_itemSoundNumber;				//�����e�̐�
	PlayerManagement*	m_playerManagement;
	Game*				m_game;
	Player3D*			m_player3D;
	Treasure*			m_treasure;
	Vector2				m_circlePos;					//�~�`�Q�[�W�̍��W
	Vector2				m_staminaPos;					//�X�^�~�i�Q�[�W�̍��W
	Vector3				m_stamianGageScale = Vector3::One;	//�X�^�~�i�Q�[�W�̃X�P�[��
	bool				m_falshState = true;			//�M���e�̃X�e�[�g
	bool				m_soundState = false;			//�����e�̃X�e�[�g
	bool				m_staminaDrawState = true;		//�X�^�~�i�Q�[�W�̕\�����
	bool				m_changeGaugeState = true;		//�~�`�Q�[�W�̕\�����
	bool				m_isTimerEnable = true;			//�^�C�}�[�̕\�����
	bool				m_circleState = false;			//true�Ȃ瑝��false�Ȃ猸��
	bool				m_circleMaxState = false;		//MAX���ǂ���
	bool				m_circleDrawState = false;		//�~�`�Q�[�W��`�悷�邩�ǂ���
	int					m_flashNumber = 0;				//�M���e�̐�
	int					m_soundNumber = 0;				//�����e�̐�
	int					m_vigilanceGage = 1;			//�x���x
	float				m_flashScale = 1.0f;			//�M���e�̉摜�̑傫��
	float				m_soundScale = 1.0f;			//�M���e�̉摜�̑傫��
	float				m_timer = 0.0f;					//�^�C�}�[
	float				m_vigilanceTime = 0.0f;			//�x���x�p�̃^�C�}�[
	float				m_gage = 0.0f;					//MP�Q�[�W
	float				m_gaugeTimer = 0.0f;			//�Q�[�W�p�̃^�C�}�[
	float				m_timePosition = 0.0f;			//�^�C���̍��W
	float				m_staminaDegree = 360.0f;		//�X�^�~�i�Q�[�W�̊p�x
	float				m_degree = 360.0f;				//�Q�[�W�̊p�x
};