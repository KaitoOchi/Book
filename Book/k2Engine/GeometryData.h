#pragma once

#include "../k2EngineLow/geometry/AABB.h"

namespace nsK2Engine {

	class ModelRender;

	class GeometryData
	{
	public:
		void Init(ModelRender* modelRender, int instanceId);

		/// <summary>
		/// �r���[�t���X�^���Ɋ܂܂�Ă��邩�ǂ����B
		/// </summary>
		/// <returns></returns>
		bool IsInViewFrustum()
		{
			return m_isInViewFrustum;
		}

		/// <summary>
		/// ���[���h���W�n�ł�AABB�̍ő���W���擾�B
		/// </summary>
		/// <returns></returns>
		const Vector3& GetAabbMaxPositionInWorld()
		{
			return m_aabbMax;
		}

		/// <summary>
		/// ���[���h���W�n�ł�AABB�̍ŏ����W���擾�B
		/// </summary>
		/// <returns></returns>
		const Vector3& GetAabbMinPositionInWorld()
		{
			return m_aabbMin;
		}

		/// <summary>
		/// �W�I���g�������X�V�B
		/// </summary>
		void Update();

	private:
		bool m_isInViewFrustum = false;		//�r���[�t���X�^���Ɋ܂܂�Ă��邩�ǂ���
		AABB m_aabb;							//AABB
		Vector3 m_aabbWorldVertexPositions[8];	//AABB���[���h��Ԃł�8���_
		Vector3 m_aabbMax;
		Vector3 m_aabbMin;
		ModelRender* m_modelRender = nullptr;	// ���̃W�I���g�����Ɗ֘A�Â��Ă��郂�f�������_���[�B
		int m_instanceId = 0;
	};
}

