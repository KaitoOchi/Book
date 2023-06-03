#pragma once

namespace
{
	const int	TIPS_MAX = 15;		//�q���g�摜�̐�
	const float CIRCLE_ROADING_ROT = -180.0f;		//���[�f�B���O�摜�̉�]��
}

/// <summary>
/// �t�F�[�h�N���X�B
/// </summary>
class Fade : public IGameObject
{
public:
	Fade();
	~Fade();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// �t�F�[�h�C�������B
	/// </summary>
	void StartFadeIn()
	{
		m_state = enState_FadeIn;
	}
	
	/// <summary>
	/// �t�F�[�h�A�E�g�����B
	/// </summary>
	void StartFadeOut()
	{
		m_spriteNum = rand() % TIPS_MAX;
		m_state = enState_FadeOut;
	}
	
	/// <summary>
	/// �t�F�[�h�����ǂ����B
	/// </summary>
	/// <returns></returns>
	const bool IsFade() const
	{
		return m_state != enState_Idle;
	}

	/// <summary>
	/// �q���g��\�����邩�ǂ����B
	/// </summary>
	void SetEnableTips(const bool tips)
	{
		for (int i = 0; i < TIPS_MAX; i++) {
			m_tipsSpriteRender[i].SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
			m_tipsSpriteRender[i].Update();
		}
		m_enableTips = tips;
	}

private:
	/// <summary>
	/// �摜�̍X�V�����B
	/// </summary>
	void SpriteUpdate();

private:
	enum EnState {
		enState_FadeIn,			//�t�F�[�h�C����
		enState_FadeOut,		//�t�F�[�h�A�E�g��
		enState_Idle,			//�ҋ@��
	};

	SpriteRender	m_nowLoadingSpriteRender;		//���[�f�B���O�����摜
	SpriteRender	m_loadingSpriteRender;			//���[�f�B���O�摜
	SpriteRender	m_tipsSpriteRender[TIPS_MAX];	//�q���g�摜
	SpriteRender	m_spriteRender;					//�w�i�摜
	EnState			m_state = enState_Idle;			//���
	bool			m_enableTips = false;			//tips��\�������邩�ǂ���
	int				m_spriteNum = 0;				//�\��������摜�̔ԍ�
	float			m_alpha = 2.0f;					//�����x
	float			m_rotTimer = 0.0f;				//��]��
};

