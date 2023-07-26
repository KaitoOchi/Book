#include "BookEnginePreCompile.h"
#include "nature/SkyCube.h"

namespace nsBookEngine {

	namespace
	{
		const float SKYCUBE_ROTATION_X = -90.0f;	//�X�J�C�L���[�u�̉�]�B
	}

	SkyCube::SkyCube()
	{

	}

	SkyCube::~SkyCube()
	{

	}

	bool SkyCube::Start()
	{
		Quaternion rot;

		//�e�N�X�`���̏�����
		if (m_number == 0) {
			m_texture.InitFromDDSFile(L"Assets/modelData/preset/skyCubeMapNight_Toon.dds");
			rot.AddRotationX(Math::DegToRad(SKYCUBE_ROTATION_X));
		}
		else if (m_number == 1) {
			m_texture.InitFromDDSFile(L"Assets/modelData/preset/skyCubeMapNight.dds");
		}

		//���f���̏�����
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

		//���f���̐ݒ�
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