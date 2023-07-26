#include "BookEnginePreCompile.h"
#include "SpotLight.h"

namespace nsBookEngine{

	namespace
	{
		const int POSITION_MAX = 999;		//レイを飛ばす最大座標。
		const float RANGE_RAY = 4.0f;		//飛ばす距離
	}

	SpotLight::SpotLight()
	{
	}

	SpotLight::~SpotLight()
	{
		
	}

	void SpotLight::Update()
	{
		RenderingEngine::GetInstance()->SetSpotLight(m_spNum, m_spotLig);
	}

	const bool SpotLight::IsHit(const Vector3& playerPos)
	{
		Vector3 colPos = m_spotLig.spPosition;

		for (int i = 0; i < POSITION_MAX; i++) {

			colPos += m_spotLig.spDirection * i;

			if (colPos.y < 0) {

				//プレイヤーから敵に向かうベクトルを計算。
				Vector3 diff = playerPos - colPos;

				//ベクトルの長さが小さかったら。
				if (diff.Length() <= m_spotLig.spRange / RANGE_RAY) {

					return true;
				}
			}
		}
		return false;
	}
}