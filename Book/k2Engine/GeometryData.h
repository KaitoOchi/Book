#pragma once

#include "../k2EngineLow/geometry/AABB.h"

namespace nsK2Engine {

	class ModelRender;

	class GeometryData
	{
	public:
		void Init(ModelRender* modelRender, int instanceId);

		/// <summary>
		/// ビューフラスタムに含まれているかどうか。
		/// </summary>
		/// <returns></returns>
		bool IsInViewFrustum()
		{
			return m_isInViewFrustum;
		}

		/// <summary>
		/// ワールド座標系でのAABBの最大座標を取得。
		/// </summary>
		/// <returns></returns>
		const Vector3& GetAabbMaxPositionInWorld()
		{
			return m_aabbMax;
		}

		/// <summary>
		/// ワールド座標系でのAABBの最小座標を取得。
		/// </summary>
		/// <returns></returns>
		const Vector3& GetAabbMinPositionInWorld()
		{
			return m_aabbMin;
		}

		/// <summary>
		/// ジオメトリ情報を更新。
		/// </summary>
		void Update();

	private:
		bool m_isInViewFrustum = false;		//ビューフラスタムに含まれているかどうか
		AABB m_aabb;							//AABB
		Vector3 m_aabbWorldVertexPositions[8];	//AABBワールド空間での8頂点
		Vector3 m_aabbMax;
		Vector3 m_aabbMin;
		ModelRender* m_modelRender = nullptr;	// このジオメトリ情報と関連づいているモデルレンダラー。
		int m_instanceId = 0;
	};
}

