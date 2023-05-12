#include "stdafx.h"
#include "Wipe.h"

#include "d3d11.h"


Wipe::Wipe()
{

}

Wipe::~Wipe()
{

}

bool Wipe::Start()
{
    //�ʏ��ʂ̕`��
    viewPorts[0].Width = FRAME_BUFFER_W;   //��ʂ̉��T�C�Y
    viewPorts[0].Height = FRAME_BUFFER_H;   //��ʂ̏c�T�C�Y
    viewPorts[0].TopLeftX = 0;   //��ʍ����x���W
    viewPorts[0].TopLeftY = 0;   //��ʍ����y���W
    viewPorts[0].MinDepth = 0.0f;   //�[�x�l�̍ŏ��l
    viewPorts[0].MaxDepth = 1.0f;   //�[�x�l�̍ő�l

    //���C�v��ʂ̕`��
    viewPorts[1].Width = FRAME_BUFFER_W / 4;   //��ʂ̉��T�C�Y
    viewPorts[1].Height = FRAME_BUFFER_H / 4;   //��ʂ̏c�T�C�Y
    viewPorts[1].TopLeftX = 0;   //��ʍ����x���W
    viewPorts[1].TopLeftY = FRAME_BUFFER_H / 2;   //��ʍ����y���W
    viewPorts[1].MinDepth = 0.0f;   //�[�x�l�̍ŏ��l
    viewPorts[1].MaxDepth = 1.0f;   //�[�x�l�̍ő�l


    m_modelRender.Init("Assets/modelData/enemy/enemy_charge.tkm");

	return true;
}

void Wipe::Update()
{

}

void Wipe::Render(RenderContext& rc)
{
    //�r���[�|�[�g�̐�����for������
    for (int i = 0; i < sizeof(viewPorts) / sizeof(viewPorts[0]); i++) {
        //�r���[�|�[�g��ݒ�
        d3dDeviceContext->RSSetViewports(1, (viewPorts + i));
        //���f����`��
        m_modelRender.Draw(rc);
    }
}