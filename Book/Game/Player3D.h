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
	//アニメーション
	enum EnAnimationClip {
		m_enAnimationClip_Idle,//待機アニメーション
		m_enAnimationClip_Walk,//歩きアニメーション
		m_enAnimationClip_Run,//走るアニメーション
		m_enAnimationClip_Jump,//ジャンプアニメーション
		m_enAnimationClip_Jumpend,//ジャンプ終わりアニメーション
		m_enAnimationClip_Down,//ダウンアニメーション
		m_enAnimationClip_Throw,//投げるアニメーション
		m_enAnimationClip_Num,//アニメーションの数
	};
	AnimationClip m_animationClips[m_enAnimationClip_Num];
	PlayerManagement* m_playerManagement = nullptr;

};

