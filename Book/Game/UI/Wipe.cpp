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
    //通常画面の描画
    m_viewPorts[0].Width = FRAME_BUFFER_W;   //画面の横サイズ
    m_viewPorts[0].Height = FRAME_BUFFER_H;   //画面の縦サイズ
    m_viewPorts[0].TopLeftX = 0;   //画面左上のx座標
    m_viewPorts[0].TopLeftY = 0;   //画面左上のy座標
    m_viewPorts[0].MinDepth = 0.0f;   //深度値の最小値
    m_viewPorts[0].MaxDepth = 1.0f;   //深度値の最大値

    //ワイプ画面の描画
    m_viewPorts[1].Width = FRAME_BUFFER_W / 4;   //画面の横サイズ
    m_viewPorts[1].Height = FRAME_BUFFER_H / 4;   //画面の縦サイズ
    m_viewPorts[1].TopLeftX = 0;   //画面左上のx座標
    m_viewPorts[1].TopLeftY = FRAME_BUFFER_H / 2;   //画面左上のy座標
    m_viewPorts[1].MinDepth = 0.0f;   //深度値の最小値
    m_viewPorts[1].MaxDepth = 1.0f;   //深度値の最大値


    m_modelRender.Init("Assets/modelData/enemy/enemy_charge.tkm");

	return true;
}

void Wipe::Update()
{

}

void Wipe::Render(RenderContext& rc)
{
    //ビューポートの数だけfor文を回す
    for (int i = 0; i < sizeof(m_viewPorts) / sizeof(m_viewPorts[0]); i++) {
        //ビューポートを設定
        rc.SetViewportAndScissor(m_viewPorts[i]);

        if (i == 0) {
            return;
        }

        //モデルを描画
        m_modelRender.Draw(rc);
    }
}