#pragma once

#include "graphics/light/DirectionLight.h"
#include "graphics/light/PointLight.h"

namespace nsBookEngine {

	class RenderingEngine
	{
	public:
		//ライト用定数バッファの構造体
		struct LightCB
		{
			DirectionLight::directionLight directionLig;
			PointLight::pointLight pointLig;
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
	};
}