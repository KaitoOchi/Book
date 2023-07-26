#include "BookEnginePreCompile.h"
#include "SpriteRender.h"

namespace nsBookEngine {

	void SpriteRender::Init(const char* filePath, const float w, const float h, AlphaBlendMode alpha, const SpriteClipMode clipMode)
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

		ClipMode(initData, clipMode);

		///Sprite�������I�u�W�F�N�g���g�p���āASprite������������
		m_sprite.Init(initData);
	}

	void SpriteRender::ClipMode(SpriteInitData& initData, const SpriteClipMode clipMode)
	{
		switch (clipMode)
		{
		//2D�Q�[�W�̃N���b�v
		case enMode_2DGauge:
			initData.m_vsEntryPointFunc = "VSMain1";
			initData.m_psEntryPoinFunc = "PSClip";
			RenderingEngine::GetInstance()->GetSpriteCB().clipMode = clipMode;
			initData.m_expandConstantBuffer = &RenderingEngine::GetInstance()->GetSpriteCB();
			initData.m_expandConstantBufferSize = sizeof(RenderingEngine::GetInstance()->GetSpriteCB());
			break;

		//BGM�̃N���b�v
		case enMode_SettingBGM:
			initData.m_vsEntryPointFunc = "VSMain2";
			initData.m_psEntryPoinFunc = "PSClip";
			RenderingEngine::GetInstance()->GetSpriteCB().clipMode = clipMode;
			initData.m_expandConstantBuffer = &RenderingEngine::GetInstance()->GetSpriteCB();
			initData.m_expandConstantBufferSize = sizeof(RenderingEngine::GetInstance()->GetSpriteCB());
			break;

		//SFX�̃N���b�v
		case enMode_SettingSFX:
			initData.m_vsEntryPointFunc = "VSMain3";
			initData.m_psEntryPoinFunc = "PSClip";
			RenderingEngine::GetInstance()->GetSpriteCB().clipMode = clipMode;
			initData.m_expandConstantBuffer = &RenderingEngine::GetInstance()->GetSpriteCB();
			initData.m_expandConstantBufferSize = sizeof(RenderingEngine::GetInstance()->GetSpriteCB());
			break;

		//UV�X�N���[��
		case enMode_UVScroll:
			initData.m_psEntryPoinFunc = "PSMainUVScroll";
			initData.m_expandConstantBuffer = &RenderingEngine::GetInstance()->GetSpriteCB();
			initData.m_expandConstantBufferSize = sizeof(RenderingEngine::GetInstance()->GetSpriteCB());
			break;

		//�~�`�Q�[�W
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