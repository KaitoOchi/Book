#include "BookEnginePreCompile.h"
#include "nature/SkyCube.h"

namespace nsBookEngine {

	SkyCube::SkyCube()
	{

	}

	SkyCube::~SkyCube()
	{

	}

	bool SkyCube::Start()
	{
		Quaternion rot;

		if (m_number == 0) {
			//テクスチャの初期化
			m_texture.InitFromDDSFile(L"Assets/modelData/preset/skyCubeMapNight_Toon.dds");
			rot.AddRotationX(Math::DegToRad(-90.0f));
		}
		else if (m_number == 1) {
			//テクスチャの初期化
			m_texture.InitFromDDSFile(L"Assets/modelData/preset/skyCubeMapNight.dds");
		}

		//モデルの初期化
		ModelInitData initData;
		initData.m_tkmFilePath = "Assets/modelData/preset/sky.tkm";
		initData.m_fxFilePath = "Assets/shader/SkyCubeMap.fx";
		initData.m_vsEntryPointFunc = "VSMain";
		initData.m_vsSkinEntryPointFunc = "VSMain";
		initData.m_psEntryPointFunc = "PSMain";

		initData.m_expandShaderResoruceView[0] = &m_texture;
		initData.m_expandConstantBuffer = &m_luminance;
		initData.m_expandConstantBufferSize = sizeof(m_luminance);
		initData.m_colorBufferFormat[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;

		//モデルの設定
		m_modelRender.InitModelData(initData);
		m_modelRender.SetPosition(m_position);
		m_modelRender.SetRotation(rot);
		m_modelRender.SetScale(m_scale);
		m_modelRender.Update();

		return true;
	}

	void SkyCube::Render(RenderContext& rc)
	{
		m_modelRender.Draw(rc);
	}
}