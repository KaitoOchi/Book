#pragma once
#include "Ghost.h"

class PlayerManagement;

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

public:
	PhysicsGhostObject m_physicsGhostObj;
private:
	EffectEmitter*		m_kirakiraEffect;		//エフェクト
	PlayerManagement*	m_playerManagement;		//プレイヤーマネジメント
};

