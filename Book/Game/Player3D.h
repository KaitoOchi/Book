#pragma once
#include "Player.h"
class Player3D :public Player
{
public:
	Player3D();
	~Player3D();
	bool Start();
	void Update();
	void Animation();
	void Throw();
	void Render(RenderContext& rc);
	enum EnPlayer3DState {
		m_enPlayer3D_Throw,//投げる
		m_enPlayer3D_Steal,//盗む
		m_enPlayer3D_Push,//押す
	};


	CharacterController m_3Dcharacon;//キャラコン
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

};

