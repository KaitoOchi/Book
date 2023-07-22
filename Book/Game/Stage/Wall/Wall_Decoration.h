#pragma once
#include "Wall.h"

/// <summary>
/// 装飾モデルクラス。
/// </summary>
class Wall_Decoration :public Wall
{
public:
	Wall_Decoration();
	~Wall_Decoration()override;
	bool Start();
	void Render(RenderContext& rc);

public:
	/// <summary>
	/// モデルのロード。
	/// </summary>
	void Load_Model(int num)
	{
		switch (num) {
		case 1:
			m_wallRender.Init("Assets/modelData/level_test/tkm/decoration.tkm", 0, 0, enModelUpAxisZ, true, true, ModelRender::enOutlineMode_None, D3D12_CULL_MODE_BACK, true);
			break;
		case 2:
			m_wallRender.Init("Assets/modelData/level_test/tkm/decoration_2.tkm", 0, 0, enModelUpAxisZ, true, true, ModelRender::enOutlineMode_None, D3D12_CULL_MODE_BACK, true);
			break;
		case 3:
			m_wallRender.Init("Assets/modelData/level_test/tkm/decoration_3.tkm", 0, 0, enModelUpAxisZ, true, true, ModelRender::enOutlineMode_None, D3D12_CULL_MODE_BACK, true);
			break;
		case 4:
			m_wallRender.Init("Assets/modelData/level_test/tkm/decoration_4.tkm", 0, 0, enModelUpAxisZ, true, true, ModelRender::enOutlineMode_None, D3D12_CULL_MODE_BACK, true);
			break;
		}
		m_wallRender.SetScale({ 50.0f, 50.0f, 50.0f });
	}
};