#include "BookEnginePreCompile.h"
#include "SpriteRender.h"

namespace nsBookEngine {

	void SpriteRender::Init(const char* filePath, const float w, const float h, AlphaBlendMode alpha, const int clipMode)
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

		if (clipMode == 1) {
			initData.m_vsEntryPointFunc = "VSMain1";
			RenderingEngine::GetInstance()->GetSpriteCB().clipMode = clipMode;
			initData.m_expandConstantBuffer = &RenderingEngine::GetInstance()->GetSpriteCB();
			initData.m_expandConstantBufferSize = sizeof(RenderingEngine::GetInstance()->GetSpriteCB());
		}
		else if (clipMode == 2) {
			initData.m_vsEntryPointFunc = "VSMain2";
			RenderingEngine::GetInstance()->GetSpriteCB().clipMode = clipMode;
			initData.m_expandConstantBuffer = &RenderingEngine::GetInstance()->GetSpriteCB();
			initData.m_expandConstantBufferSize = sizeof(RenderingEngine::GetInstance()->GetSpriteCB());
		}
		else if (clipMode == 3) {
			initData.m_vsEntryPointFunc = "VSMain3";
			RenderingEngine::GetInstance()->GetSpriteCB().clipMode = clipMode;
			initData.m_expandConstantBuffer = &RenderingEngine::GetInstance()->GetSpriteCB();
			initData.m_expandConstantBufferSize = sizeof(RenderingEngine::GetInstance()->GetSpriteCB());
		}

		///Sprite初期化オブジェクトを使用して、Spriteを初期化する
		m_sprite.Init(initData);
	}

	void SpriteRender::Draw(RenderContext& rc)
	{
		RenderingEngine::GetInstance()->AddRenderObject(this);
	}
}