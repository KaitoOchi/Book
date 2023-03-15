#include "BookEnginePreCompile.h"
#include "SpotLight.h"

namespace nsBookEngine{

	SpotLight::SpotLight()
	{

	}

	SpotLight::~SpotLight()
	{
		DeleteGO(m_collisionObject);
	}

	void SpotLight::Update()
	{
		RenderingEngine::GetInstance()->SetSpotLight(m_spotLig);
		
		m_collisionObject->SetPosition(m_spotLig.spPosition);

		Quaternion rot = m_rotation;
		m_rotation.SetRotationYFromDirectionXZ(m_spotLig.spDirection);
		Vector3 rot_ = Vector3::AxisZ;
		rot.Apply(rot_);
		m_rotation.SetRotation({ 0.0f, 0.0f, -1.0f }, m_spotLig.spDirection);
		m_collisionObject->SetRotation(m_rotation);
	}

	void SpotLight::SetCollisionObject(const Vector3& playerPos)
	{
		m_collisionObject = NewGO<CollisionObject>(0);

		m_collisionObject->CreateBox(m_spotLig.spPosition, Quaternion::Identity, Vector3(1.0f, 1.0f, 50.0f));
		m_collisionObject->SetIsEnableAutoDelete(false);
		m_collisionObject->SetName("ray");

		//for (int i = 0; i < 999; i++) {
		//	m_collisionObject->SetPosition(m_spotLig.spPosition + (m_spotLig.spDirection * i));

		//	if (m_spotLig.spPosition.y < 0) {

		//		//プレイヤーから敵に向かうベクトルを計算。
		//		Vector3 diff = playerPos - colPos;

		//		//ベクトルの長さが小さかったら。
		//		if (diff.Length() <= m_spotLig.spRange) {

		//			//todo 当たり判定
		//		}

		//	}
		//}
	}

	void SpotLight::HitRay()
	{
		m_spotLig.spPosition += m_spotLig.spDirection;
		Vector3 colPos = m_spotLig.spPosition;

		m_collisionObject->SetPosition(colPos);
	}
}