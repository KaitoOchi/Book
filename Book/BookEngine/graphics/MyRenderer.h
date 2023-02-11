#pragma once


namespace nsBookEngine {

	// ソフトシャドウで利用するカラーバッファのフォーマット。
	// レンダリングターゲットのフォーマット。
	struct RenderTargetFormat {
		DXGI_FORMAT colorBufferFormat;	// カラーバッファのフォーマット。
		DXGI_FORMAT depthBufferFormat;	// 深度バッファのフォーマット。
	};

	// メインレンダリングターゲットのフォーマット。
	const RenderTargetFormat g_mainRenderTargetFormat = {
		DXGI_FORMAT_R16G16B16A16_FLOAT,
		DXGI_FORMAT_UNKNOWN
	};
}