#include "BookEnginePreCompile.h"
#include "SpriteRender.h"

namespace nsBookEngine {

	void SpriteRender::Init(const char* filePath, const float w, const float h, AlphaBlendMode alpha, const int clipMode)
	{
		SpriteInitData initData;
		///DDS�t�@�C���̃t�@�C���p�X���w�肷��
		initData.m_ddsFilePath[0] = filePath;
		//Sprite�\���p�̃V�F�[�_�[�̃t�@�C���p�X���w�肷��
		initData.m_fxFilePath = "Assets/shader/sprite.fx";
		//�X�v���C�g�̕��ƍ������w�肷��
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

		///Sprite�������I�u�W�F�N�g���g�p���āASprite������������
		m_sprite.Init(initData);
	}

	void SpriteRender::Draw(RenderContext& rc)
	{
		RenderingEngine::GetInstance()->AddRenderObject(this);
	}
}