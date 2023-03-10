#include "BookEnginePreCompile.h"
#include "Bloom.h"

namespace nsBookEngine {

	Bloom::Bloom()
	{

	}

	Bloom::~Bloom()
	{

	}

	void Bloom::Init(RenderTarget& mainRenderTarget)
	{
		m_luminanceRenderTarget.Create(
			mainRenderTarget.GetWidth(),
			mainRenderTarget.GetHeight(),
			1,
			1,
			mainRenderTarget.GetColorBufferFormat(),
			DXGI_FORMAT_UNKNOWN
		);

		//�P�x���o�p�̃X�v���C�g��������
		SpriteInitData spriteInitData;

		spriteInitData.m_fxFilePath = "Assets/shader/postEffect/bloom.fx";
		spriteInitData.m_vsEntryPointFunc = "VSMain";
		spriteInitData.m_psEntryPoinFunc = "PSLuminance";
		spriteInitData.m_expandConstantBuffer = &m_luminanceCB;
		spriteInitData.m_expandConstantBufferSize = sizeof(m_luminanceCB);

		// �X�v���C�g�̕��ƍ�����luminnceRenderTarget�Ɠ���
		spriteInitData.m_width = mainRenderTarget.GetWidth();
		spriteInitData.m_height = mainRenderTarget.GetHeight();
		// �e�N�X�`���̓��C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@�[
		spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();

		// �`�����ރ����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g���w�肷��
		spriteInitData.m_colorBufferFormat[0] = mainRenderTarget.GetColorBufferFormat();

		m_luminanceSprite.Init(spriteInitData);


		//�K�E�V�A���u���[��������
		GaussianBlur gaussianBlur[4];

		gaussianBlur[0].Init(&m_luminanceRenderTarget.GetRenderTargetTexture());
		gaussianBlur[1].Init(&gaussianBlur[0].GetBokeTexture());
		gaussianBlur[2].Init(&gaussianBlur[1].GetBokeTexture());
		gaussianBlur[3].Init(&gaussianBlur[2].GetBokeTexture());
	}
}