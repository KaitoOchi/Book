#pragma once
#include "collision/CollisionObject.h"
class TransparentBox:public IGameObject
{
public:
	void SetPosition(const Vector3 m_pos)
	{
		m_position = m_pos;
		m_coli = NewGO<CollisionObject>(0);
		m_coli->SetPosition(m_position);
		m_coli->SetName("transparent");
	}
	Vector3 m_position = Vector3::Zero;				//透明ブロックの座標
	CollisionObject *m_coli;						//コリジョンオブジェト
};

