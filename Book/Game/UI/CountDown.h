#pragma once
#include "GameManager.h"

/// <summary>
/// �J�E���g�_�E���N���X�B
/// </summary>
class CountDown : public IGameObject
{
public:
	CountDown();
	~CountDown();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	/// <summary>
	/// �J�E���g�I�����̏����B
	/// </summary>
	void CountEnd();

private:
	/// <summary>
	/// SE��炷�����B
	/// </summary>
	/// <param name="num">�炷SE�̔ԍ��B</param>
	void PlaySE(const int num)
	{
		//SE���Đ�
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(num);
		se->Play(false);
		se->SetVolume(GameManager::GetInstance()->GetSFX());
	}

private:
	std::array< SpriteRender, 4 >	m_countDownSpriteRender;		//�J�E���g�_�E���摜
	int								m_count = 3;					//�J�E���g
	float							m_timer = 2.0f;					//�^�C�}�[
};