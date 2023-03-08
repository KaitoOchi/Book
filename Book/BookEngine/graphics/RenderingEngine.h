#pragma once

#include "graphics/light/DirectionLight.h"
#include "graphics/light/PointLight.h"
#include "graphics/light/HemiSphereLight.h"

namespace nsBookEngine {

	class RenderingEngine
	{
	public:
		//ライト用定数バッファの構造体
		struct LightCB
		{
			DirectionLight::directionLight directionLig;
			PointLight::pointLight pointLig;
			HemiSphereLight::hemiSphereLight hemiSphereLig;
		};

		RenderingEngine();
		~RenderingEngine();

		/// <summary>
		/// ディレクションライトを設定。
		/// </summary>
		/// <param name="dir">ライトの方向</param>
		/// <param name="color">ライトのカラー</param>
		void SetDirectionLight(const Vector3& dir, const Vector3 color)
		{
			m_directionLig.SetDirection(dir);
			m_directionLig.SetColor(color);
		}

		/// <summary>
		/// 環境光を設定。
		/// </summary>
		/// <param name="amb">環境光</param>
		void SetAmbient(const float amb)
		{
			m_directionLig.SetAmbientLight(amb);
		}

		/// <summary>
		/// 半球ライトを設定。
		/// </summary>
		/// <param name="grdColor">照り返しのライトRGB。0.0f～1.0fの間</param>
		/// <param name="skyColor">天球ライトRGB。0.0f～1.0fの間</param>
		/// <param name="grdNormal">地面の法線0.0f～1.0fの間</param>
		void SetHemiSphereLight(const Vector3& grdColor, const Vector3& skyColor, const Vector3& grdNormal)
		{
			m_hemiSphereLig.SetGroundColor(grdColor);
			m_hemiSphereLig.SetSkyColor(skyColor);
			m_hemiSphereLig.SetGroundNormal(grdNormal);
		}

		/// <summary>
		/// ライト用の構造体を取得。
		/// </summary>
		LightCB& GetLightCB()
		{
			return m_lightCB;
		}

		void Init();
		void Update();

	private:
		LightCB m_lightCB;

		DirectionLight m_directionLig;
		HemiSphereLight m_hemiSphereLig;
	};
}