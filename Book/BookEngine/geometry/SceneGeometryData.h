#pragma once


#include "GeometryData.h"

namespace nsBookEngine {

	/// <summary>
	/// シーンのジオメトリ情報
	/// </summary>
	class SceneGeometryData
	{
	public:
		/// <summary>
		/// 幾何学データを登録。
		/// </summary>
		/// <param name="geomData"></param>
		void RegisterGeometryData(GeometryData* geomData)
		{
			m_geometryDatas.push_back(geomData);
		}

		/// <summary>
		/// 幾何学データの登録解除。
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
		/// 幾何学データを更新。
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
		Vector3 m_shadowCasterMaxPositionInViewFrustum;		//ビューフラスタムに含まれているシャドウキャスターの最大座標
		Vector3 m_shadowCasterMinPositionInViewFrustum;		//ビューフラスタムに含まれているシャドウキャスターの最小座標
		bool m_isBuildShadowCasterGeometryData = false;		//シャドウキャスターに関するジオメトリ情報を構築できたかどうか
		std::list<GeometryData*> m_geometryDatas;			//シーンを構成するジオメトリ情報のリスト
	};

}
