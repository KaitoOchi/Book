#include "BookEnginePreCompile.h"
#include "SpriteRender.h"

namespace nsBookEngine {

	void SpriteRender::Init(const char* filePath, const float w, const float h, AlphaBlendMode alpha, const SpriteClipMode clipMode)
	{
		SpriteInitData initData;
		///DDSファイルのファイルパスを指定する
		initData.m_ddsFilePath[0] = filePath;
		//Sprite表示用のシェーダーのファイルパスを指定する
		initData.m_fxFilePath = "Assets/shader/sprite.fx";
		//スプライトの幅と高さを指定する
		initData.m_width = static_cast<UINT>(w);
		initData.m_height = static_cast<UINT>(h);
		initData.m_alphaBlendMode = alpha;

		ClipMode(initData, clipMode);

		///Sprite初期化オブジェクトを使用して、Spriteを初期化する
		m_sprite.Init(initData);
	}

	void SpriteRender::ClipMode(SpriteInitData& initData, const SpriteClipMode clipMode)
	{
		switch (clipMode)
		{
		//2Dゲージのクリップ
		case enMode_2DGauge:
			initData.m_vsEntryPointFunc = "VSMain1";
			initData.m_psEntryPoinFunc = "PSClip";
			RenderingEngine::GetInstance()->GetSpriteCB().clipMode = clipMode;
			initData.m_expandConstantBuffer = &RenderingEngine::GetInstance()->GetSpriteCB();
			initData.m_expandConstantBufferSize = sizeof(RenderingEngine::GetInstance()->GetSpriteCB());
			break;

		//BGMのクリップ
		case enMode_SettingBGM:
			initData.m_vsEntryPointFunc = "VSMain2";
			initData.m_psEntryPoinFunc = "PSClip";
			RenderingEngine::GetInstance()->GetSpriteCB().clipMode = clipMode;
			initData.m_expandConstantBuffer = &RenderingEngine::GetInstance()->GetSpriteCB();
			initData.m_expandConstantBufferSize = sizeof(RenderingEngine::GetInstance()->GetSpriteCB());
			break;

		//SFXのクリップ
		case enMode_SettingSFX:
			initData.m_vsEntryPointFunc = "VSMain3";
			initData.m_psEntryPoinFunc = "PSClip";
			RenderingEngine::GetInstance()->GetSpriteCB().clipMode = clipMode;
			initData.m_expandConstantBuffer = &RenderingEngine::GetInstance()->GetSpriteCB();
			initData.m_expandConstantBufferSize = sizeof(RenderingEngine::GetInstance()->GetSpriteCB());
			break;

		//UVスクロール
		case enMode_UVScroll:
			initData.m_psEntryPoinFunc = "PSMainUVScroll";
			initData.m_expandConstantBuffer = &RenderingEngine::GetInstance()->GetSpriteCB();
			initData.m_expandConstantBufferSize = sizeof(RenderingEngine::GetInstance()->GetSpriteCB());
			break;

		//円形ゲージ
		case enMode_CircleGauge:
			initData.m_psEntryPoinFunc = "PSCircleGauge";
			RenderingEngine::GetInstance()->GetSpriteCB().clipMode = clipMode;
			initData.m_expandConstantBuffer = &RenderingEngine::GetInstance()->GetSpriteCB();
			initData.m_expandConstantBufferSize = sizeof(RenderingEngine::GetInstance()->GetSpriteCB());
			break;
		}
	}

	void SpriteRender::Draw(RenderContext& rc)
	{
		RenderingEngine::GetInstance()->AddRenderObject(this);
	}
}