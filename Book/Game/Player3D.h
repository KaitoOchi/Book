#pragma once
#include "Player.h"
class PlayerManagement;
class Player3D :public Player
{
public:
	Player3D();
	~Player3D()override;
	bool Start()override;
	void Update()override;
	void Animation()override;
	void Throw();
	void PlayerChang();
	void Render(RenderContext& rc);
	/// <summary>
	/// キャラコンの作成の関数
	/// </summary>
	void CreatCharcon();
private:
	ModelRender m_model3DRender;//3Dモデル
	PlayerManagement* m_playerManagement = nullptr;

	//透明座標に動かす
	Vector3 m_movePosition = Vector3::Zero;
	float senkeiPos = 0.1f;
	float senkeiTime = 1.0f;
};

