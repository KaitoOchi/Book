#pragma once


namespace nsBookEngine {

	// �\�t�g�V���h�E�ŗ��p����J���[�o�b�t�@�̃t�H�[�}�b�g�B
	// �����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g�B
	struct RenderTargetFormat {
		DXGI_FORMAT colorBufferFormat;	// �J���[�o�b�t�@�̃t�H�[�}�b�g�B
		DXGI_FORMAT depthBufferFormat;	// �[�x�o�b�t�@�̃t�H�[�}�b�g�B
	};

	// ���C�������_�����O�^�[�Q�b�g�̃t�H�[�}�b�g�B
	const RenderTargetFormat g_mainRenderTargetFormat = {
		DXGI_FORMAT_R16G16B16A16_FLOAT,
		DXGI_FORMAT_UNKNOWN
	};
}