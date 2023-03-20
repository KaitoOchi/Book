#include "BookEnginePreCompile.h"
#include "SpotLight.h"

namespace nsBookEngine{

	SpotLight::SpotLight()
	{
	}

	SpotLight::~SpotLight()
	{
		
	}

	void SpotLight::Update()
	{
		RenderingEngine::GetInstance()->SetSpotLight(m_spotLig);
	}

	const bool SpotLight::IsHit(const Vector3& playerPos)
	{
		Vector3 colPos = m_spotLig.spPosition;

		for (int i = 0; i < 999; i++) {

			colPos += m_spotLig.spDirection * i;

			if (colPos.y < 0) {

				//プレイヤーから敵に向かうベクトルを計算。
				Vector3 diff = playerPos - colPos;

				//ベクトルの長さが小さかったら。
				if (diff.Length() <= m_spotLig.spRange/10) {

					return true;
				}
			}
		}
		return false;
	}
}