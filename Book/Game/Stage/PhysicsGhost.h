#pragma once
#include "Ghost.h"

class PlayerManagement;
class TutorialUI;

/// <summary>
/// 壁用の当たり判定クラス。
/// </summary>
class PhysicsGhost:public Ghost
{
public:
	PhysicsGhost();
	~PhysicsGhost()override;
	bool Start();
	void Update();
	void EffectDraw();
	/// <summary>
	/// プレイヤーとの衝突判定
	/// </summary>
	bool IsHitPlayer();

public:
	/// <summary>
	/// エフェクトの取得。
	/// </summary>
	/// <returns></returns>
	EffectEmitter* GetEffect()
	{
		if (m_kirakiraEffect != nullptr) {
			return m_kirakiraEffect;
		}
		return nullptr;
	}

	/// <summary>
	/// フラグを設定する
	/// </summary>
	/// <param name="flag">trueなら描画</param>
	void SetIsDraw(const bool flag)
	{
		m_isDraw = flag;
	}

public:
	PhysicsGhostObject m_physicsGhostObj;
private:
	EffectEmitter*		m_kirakiraEffect;		//エフェクト
	PlayerManagement*	m_playerManagement;		//プレイヤーマネジメント
	TutorialUI*			m_turorialUI = nullptr;	//ゲームUI
	bool				m_isDraw = false;		// エフェクトを表示するかどうか
};

