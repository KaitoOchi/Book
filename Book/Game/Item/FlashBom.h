#pragma once
#include "Item.h"
class Game;

class FlashBom:public Item
{
public:
	FlashBom();
	~FlashBom()override;
	bool Start()override;
	void Update();
	void ItemHit()override;
	void Render(RenderContext& rc);
	/// <summary>
	/// α値と影響率を設定する
	/// </summary>
	void SetFlashEffect();

	enum EnFlashState
	{
		m_enFlash_No,				//フラッシュしていない
		m_enFlash_Start,			//フラッシュスタート
		m_enFlash_Flashing,			//フラッシュ中
		m_enFlash_End				//フラッシュエンド
	};

	EnFlashState m_FlashState = m_enFlash_No;
	int m_flashCount = 5;						//フラッシュボムの所持数
private:
	/// <summary>
	/// フラッシュの効果
	/// </summary>
	void FlashEffect();
	/// <summary>
	/// ステートの管理
	/// </summary>
	void ManageState()override;
	/// <summary>
	/// 開始時の遷移
	/// </summary>
	void ProcessStartState();

	/// <summary>
	/// フラッシュ中の遷移
	/// </summary>
	void ProcessFlashingState();

	


	SpriteRender m_flashRender;

	SphereCollider m_sphereCollider;					//スフィアコライダー
	RigidBody m_rigidBody;								//動態

	PointLight m_pointLight;							//ポイントライトの座標
	float m_range=0;
	float m_alpha=0;
	float m_ambient=0;
	float m_color = 0;

};

