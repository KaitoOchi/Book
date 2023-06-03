#pragma once
#include "GameManager.h"

/// <summary>
/// カウントダウンクラス。
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
	/// カウント終了時の処理。
	/// </summary>
	void CountEnd();

private:
	/// <summary>
	/// SEを鳴らす処理。
	/// </summary>
	/// <param name="num">鳴らすSEの番号。</param>
	void PlaySE(const int num)
	{
		//SEを再生
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(num);
		se->Play(false);
		se->SetVolume(GameManager::GetInstance()->GetSFX());
	}

private:
	std::array< SpriteRender, 4 >	m_countDownSpriteRender;		//カウントダウン画像
	int								m_count = 3;					//カウント
	float							m_timer = 2.0f;					//タイマー
};