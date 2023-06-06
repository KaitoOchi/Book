#pragma once

namespace
{
	const int	TIPS_MAX = 15;		//ヒント画像の数
	const float CIRCLE_ROADING_ROT = -180.0f;		//ローディング画像の回転量
}

/// <summary>
/// フェードクラス。
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
	/// フェードイン処理。
	/// </summary>
	void StartFadeIn()
	{
		m_state = enState_FadeIn;
	}
	
	/// <summary>
	/// フェードアウト処理。
	/// </summary>
	void StartFadeOut()
	{
		m_spriteNum = rand() % TIPS_MAX;
		m_state = enState_FadeOut;
	}
	
	/// <summary>
	/// フェード中かどうか。
	/// </summary>
	/// <returns></returns>
	const bool IsFade() const
	{
		return m_state != enState_Idle;
	}

	/// <summary>
	/// ヒントを表示するかどうか。
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
	/// 画像の更新処理。
	/// </summary>
	void SpriteUpdate();

private:
	enum EnState {
		enState_FadeIn,			//フェードイン中
		enState_FadeOut,		//フェードアウト中
		enState_Idle,			//待機中
	};

	SpriteRender	m_nowLoadingSpriteRender;		//ローディング文字画像
	SpriteRender	m_loadingSpriteRender;			//ローディング画像
	SpriteRender	m_tipsSpriteRender[TIPS_MAX];	//ヒント画像
	SpriteRender	m_spriteRender;					//背景画像
	EnState			m_state = enState_Idle;			//状態
	bool			m_enableTips = false;			//tipsを表示させるかどうか
	int				m_spriteNum = 0;				//表示させる画像の番号
	float			m_alpha = 2.0f;					//透明度
	float			m_rotTimer = 0.0f;				//回転量
};

