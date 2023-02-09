#pragma once


#include "GeometryData.h"

namespace nsBookEngine {

	/// <summary>
	/// �V�[���̃W�I���g�����
	/// </summary>
	class SceneGeometryData
	{
	public:
		/// <summary>
		/// �􉽊w�f�[�^��o�^�B
		/// </summary>
		/// <param name="geomData"></param>
		void RegisterGeometryData(GeometryData* geomData)
		{
			m_geometryDatas.push_back(geomData);
		}

		/// <summary>
		/// �􉽊w�f�[�^�̓o�^�����B
		/// </summary>
		/// <param name="geomData"></param>
		void UnregisterGeometryData(GeometryData* geomData)
		{
			auto it = std::find(
				m_geometryDatas.begin(),
				m_geometryDatas.end(),
				geomData
			);

			if (it != m_geometryDatas.end()) {
				m_geometryDatas.erase(it);
			}
		}

		/// <summary>
		/// �􉽊w�f�[�^���X�V�B
		/// </summary>
		void Update();

		const Vector3& GetShadowCasterMaxPositionInViewFrustum() const
		{
			return m_shadowCasterMaxPositionInViewFrustum;
		}
		const Vector3& GetShadowCasterMinPositionInViewFrustum() const
		{
			return m_shadowCasterMinPositionInViewFrustum;
		}
		bool IsBuildshadowCasterGeometryData() const
		{
			return m_isBuildShadowCasterGeometryData;
		}

	private:
		Vector3 m_shadowCasterMaxPositionInViewFrustum;		//�r���[�t���X�^���Ɋ܂܂�Ă���V���h�E�L���X�^�[�̍ő���W
		Vector3 m_shadowCasterMinPositionInViewFrustum;		//�r���[�t���X�^���Ɋ܂܂�Ă���V���h�E�L���X�^�[�̍ŏ����W
		bool m_isBuildShadowCasterGeometryData = false;		//�V���h�E�L���X�^�[�Ɋւ���W�I���g�������\�z�ł������ǂ���
		std::list<GeometryData*> m_geometryDatas;			//�V�[�����\������W�I���g�����̃��X�g
	};

}
