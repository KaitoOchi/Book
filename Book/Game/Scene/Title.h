#pragma once
#include "GameManager.h"
#include "level2DRender/Level2DRender.h"
#include "Fade.h"

/// <summary>
/// �^�C�g���N���X�B
/// </summary>
class Title :public IGameObject
{
public:
	Title();
	~Title();
	bool Start();
	void Update();
	void Render(RenderContext &rc);

public:
	/// <summary>
	/// �A�j���[�V���������B
	/// </summary>
	/// <param name="time">���ԁB</param>
	void Animation(float& time, float& alpha);

	/// <summary>
	/// �A�N�e�B�u��Ԃ�ݒ�B
	/// </summary>
	void SetActive(const bool active)
	{
		m_active = active;

		if (active) {
			m_fade->StartFadeIn();
			m_titleState_tmp = 1;
			m_titleState = 1;
			m_cursor = 1;
			m_animTime = -0.11f;
			m_isWaitState = true;
		}
	}

	/// <summary>
	/// ���ʉ���点�邩�ǂ����B
	/// </summary>
	/// <param name="sound">�炵�������ʉ��̔ԍ��B</param>
	void IsCanPlaySound(const bool sound)
	{
		int title = m_titleState_tmp;
		title = min(max(m_titleState, 0), 4);

		if (m_titleState == title) {
			Sound(sound);
		}
	}

	/// <summary>
	/// ���ʉ���炷�����B
	/// </summary>
	/// <param name="num">�炵�������ʉ��̔ԍ��B</param>
	void Sound(const int num)
	{
		//�ˌ������o��
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(num);
		se->Play(false);
		se->SetVolume(GameManager::GetInstance()->GetSFX());
	}

private:
	/// <summary>
	/// �X�v���C�g�̏����������B
	/// </summary>
	void InitSprite();

	/// <summary>
	/// �X�e�[�g�̑J�ڒ��̏����B
	/// </summary>
	void StateChange();

	/// <summary>
	/// �V�[���̑J�ڏ����B
	/// </summary>
	void SceneChange();

	/// <summary>
	/// ���͏����B
	/// </summary>
	void Input();

	/// <summary>
	/// ���͂ɂ��l�̍X�V�����B
	/// </summary>
	void ValueUpdate(bool vertical);

	/// <summary>
	/// �J�[�\���̈ړ������B
	/// </summary>
	void CursorMove();

	/// <summary>
	/// �X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ManageState();

	/// <summary>
	/// �^�C�g����ʂ̏����B
	/// </summary>
	void TitleScreen();

	/// <summary>
	/// ���j���[��ʂ̏����B
	/// </summary>
	void MenuScreen();

private:
	enum EnAnimationClip {
		animationClip_Idle,
		animationClip_Put,
		animationClip_Num,
	};
	AnimationClip m_animationClips[animationClip_Num];

private:
	ModelRender						m_backGroundModelRender;		//�w�i���f��
	ModelRender						m_playerModelRender;			//�v���C���[���f��
	SpriteRender					m_titleSpriteRender;			//�^�C�g���摜
	SpriteRender					m_pressSpriteRender;			//press�摜
	SpriteRender					m_menuSpriteRender;				//���j���[�摜
	SpriteRender					m_cursorSpriteRender;			//�J�[�\���摜
	std::array< SpriteRender,2 >	m_buttonSpriteRender;			//�{�^���摜
	std::vector< SpriteRender* >	m_sprites;						//SpriteRender�̃x�N�^�[�^
	Level2DRender*					m_level2DRender = nullptr;		//���x�������_�[
	Fade*							m_fade = nullptr;				//�t�F�[�h�N���X
	Vector3							m_cursorPos;					//�J�[�\�����W
	bool							m_isWaitState = false;			//�X�e�[�g�̑J�ڒ����ǂ���
	bool							m_isWaitFadeOut = false;		//�t�F�[�h��Ԃ��ǂ���
	bool							m_active = true;				//�A�N�e�B�u���
	int								m_titleState = 0;				//�^�C�g���X�e�[�g
	int								m_titleState_tmp = 0;			//�^�C�g���X�e�[�g�̈ꎞ�I�ϐ�
	int								m_cursor = 0;					//�c�J�[�\��
	int								m_nextCursor = 0;				//���Ɉړ�����J�[�\��
	float							m_alpha = 0.0f;					//�F�̃A���t�@�l
	float							m_timer = 0.0f;					//����
	float							m_animTime = 1.0f;				//�A�j���[�V��������
	float							m_cursorTimer = 1.0f;			//�J�[�\���^�C�}�[
};
