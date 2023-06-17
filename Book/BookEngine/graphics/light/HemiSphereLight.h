#pragma once

namespace nsBookEngine{

	class HemiSphereLight
	{
	public:
		/// <summary>
		/// 半球ライトの構造体
		/// </summary>
		struct hemiSphereLight
		{
			Vector3 groundColor;	//地面の色
			float pad0 = 0.0f;
			Vector3 skyColor;		//空の色
			float pad1 = 0.0f;
			Vector3 groundNormal;	//地面の法線
			float pad2 = 0.0f;
		};

		HemiSphereLight();
		~HemiSphereLight();

		/// <summary>
		/// 地面の色を設定
		/// </summary>
		/// <param name="groundColor">RGB。0.0f～1.0fの間</param>
		void SetGroundColor(const Vector3& groundColor)
		{
			m_hemiSphereLight.groundColor = groundColor;
		}

		/// <summary>
		/// 空の色を設定
		/// </summary>
		/// <param name="skyColor">RGB。0.0f～1.0fの間</param>
		void SetSkyColor(const Vector3& skyColor)
		{
			m_hemiSphereLight.skyColor = skyColor;
		}

		/// <summary>
		/// 地面の法線を設定
		/// </summary>
		/// <param name="groundNormal">0.0f～1.0fの間</param>
		void SetGroundNormal(const Vector3& groundNormal)
		{
			m_hemiSphereLight.groundNormal = groundNormal;
		}

		/// <summary>
		/// 構造体を取得
		/// </summary>
		/// <returns></returns>
		hemiSphereLight& GetHemiSphereLig()
		{
			return m_hemiSphereLight;
		}

	private:
		hemiSphereLight m_hemiSphereLight;	//半球ライト
	};
}