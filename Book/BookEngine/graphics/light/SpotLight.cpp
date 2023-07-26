#include "BookEnginePreCompile.h"
#include "SpotLight.h"

namespace nsBookEngine{

	namespace
	{
		const int POSITION_MAX = 999;		//���C���΂��ő���W�B
		const float RANGE_RAY = 4.0f;		//��΂�����
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

				//�v���C���[����G�Ɍ������x�N�g�����v�Z�B
				Vector3 diff = playerPos - colPos;

				//�x�N�g���̒�����������������B
				if (diff.Length() <= m_spotLig.spRange / RANGE_RAY) {

					return true;
				}
			}
		}
		return false;
	}
}