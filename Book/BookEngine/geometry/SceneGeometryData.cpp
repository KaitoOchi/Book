#include "../PreCompile.h"
#include "SceneGeometryData.h"


namespace nsBookEngine {

	void SceneGeometryData::Update()
	{
		m_isBuildShadowCasterGeometryData = false;
		m_shadowCasterMaxPositionInViewFrustum = { -FLT_MIN, -FLT_MIN, -FLT_MIN };
		m_shadowCasterMinPositionInViewFrustum = { FLT_MAX,  FLT_MAX,  FLT_MAX };

		//�V�[���̃W�I���g�������X�V
		for (auto& geomData : m_geometryDatas) {
			geomData->Update();
			//�r���[�t���X�^���ɓ����Ă���V���h�E�L���X�^�[��AABB�̍ő���W�ƍŏ����W���X�V
			if (geomData->IsShadowCatser() && geomData->IsInViewFrustum()) {
				m_shadowCasterMaxPositionInViewFrustum.Max(geomData->GetAabbMaxPositionInWorld());
				m_shadowCasterMinPositionInViewFrustum.Min(geomData->GetAabbMinPositionInWorld());
				m_isBuildShadowCasterGeometryData = true;
			}
		}
	}
}
