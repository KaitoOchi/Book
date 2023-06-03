#pragma once
#include "GameManager.h"

class Game;

/// <summary>
/// �|�[�Y�N���X�B
/// </summary>
class Pause : public IGameObject
{
public:
	enum PauseState
	{
		enState_FadeIn,
		enState_FadeOut,
		enState_Pause,
		enState_Game
	};

public:
	Pause();
	~Pause();
	bool Start();
	void Render(RenderContext& rc);

private:
	/// <summary>
	/// �|�[�Y���̃A�b�v�f�[�g�����B
	/// </summary>
	void PauseUpdate();

	/// <summary>
	/// �|�[�Y���̏����B
	/// </summary>
	void PauseScreen();

	/// <summary>
	/// �J�[�\���̈ړ������B
	/// </summary>
	void CursorMove();

	/// <summary>
	/// �t�F�[�h�C�������B
	/// </summary>
	void FadeIn();

	/// <summary>
	/// �t�F�[�h�A�E�g�����B
	/// </summary>
	void FadeOut();

	/// <summary>
	/// �X�v���C�g�̕ύX�����B
	/// </summary>
	void SetSprite();

	/// <summary>
	/// SE���o���B
	/// </summary>
	void PlaySE(const int num)
	{
		SoundSource* se = NewGO<SoundSource>(0, "sound");
		se->Init(num);
		se->Play(false);
		se->SetVolume(GameManager::GetInstance()->GetSFX());
	}

private:
	SpriteRender	m_backGroundSpriteRender;		//�w�i�摜
	SpriteRender	m_pauseSpriteRender;			//�|�[�Y���j���[�摜
	SpriteRender	m_cursorSpriteRender;			//�J�[�\���摜
	PauseState		m_pauseState = enState_Game;	//�|�[�Y�X�e�[�g
	Game*			m_game = nullptr;				//�Q�[���N���X
	Vector3			m_cursorPos;					//�J�[�\�����W
	bool			m_isPause = false;				//�|�[�Y�������ǂ���
	int				m_cursor = 0;					//�J�[�\��
	int				m_nextCursor = 0;				//���̃J�[�\��
	float			m_timer = 0.0f;					//�^�C�}�[
	float			m_cursorTimer = 1.0f;			//�J�[�\���^�C�}�[
};