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
    viewPorts[0].Width = FRAME_BUFFER_W;   //画面の横サイズ
    viewPorts[0].Height = FRAME_BUFFER_H;   //画面の縦サイズ
    viewPorts[0].TopLeftX = 0;   //画面左上のx座標
    viewPorts[0].TopLeftY = 0;   //画面左上のy座標
    viewPorts[0].MinDepth = 0.0f;   //深度値の最小値
    viewPorts[0].MaxDepth = 1.0f;   //深度値の最大値

    //ワイプ画面の描画
    viewPorts[1].Width = FRAME_BUFFER_W / 4;   //画面の横サイズ
    viewPorts[1].Height = FRAME_BUFFER_H / 4;   //画面の縦サイズ
    viewPorts[1].TopLeftX = 0;   //画面左上のx座標
    viewPorts[1].TopLeftY = FRAME_BUFFER_H / 2;   //画面左上のy座標
    viewPorts[1].MinDepth = 0.0f;   //深度値の最小値
    viewPorts[1].MaxDepth = 1.0f;   //深度値の最大値


    m_modelRender.Init("Assets/modelData/enemy/enemy_charge.tkm");

	return true;
}

void Wipe::Update()
{

}

void Wipe::Render(RenderContext& rc)
{
    //ビューポートの数だけfor文を回す
    for (int i = 0; i < sizeof(viewPorts) / sizeof(viewPorts[0]); i++) {
        //ビューポートを設定
        d3dDeviceContext->RSSetViewports(1, (viewPorts + i));
        //モデルを描画
        m_modelRender.Draw(rc);
    }
}