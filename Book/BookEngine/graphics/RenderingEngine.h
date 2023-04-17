#pragma once

#include "graphics/IRenderer.h"

#include "graphics/light/DirectionLight.h"
#include "graphics/light/PointLight.h"
#include "graphics/light/SpotLight.h"
#include "graphics/light/HemiSphereLight.h"
#include "graphics/postEffect/Bloom.h"

#define MAX_POINT_LIGHT 4;
#define MAX_SPOT_LIGHT 4;

namespace nsBookEngine {

	class RenderingEngine
	{
	public:
		//シャドウマップ用の構造体
		struct ShadowParamCB
		{
			Vector3 lightPos = Vector3::Zero;
			float pad0;
			Matrix mLVP = g_matIdentity;
		};

		//ライト用の構造体
		struct LightCB
		{
			DirectionLight::directionLight directionLig;
			PointLight::pointLight pointLig;
			SpotLight::spotLight spotLig;
			HemiSphereLight::hemiSphereLight hemiSphereLig;
			ShadowParamCB shadowCB;
		};

		//スプライト用の構造体
		struct SpriteCB
		{
			float clipSize;
		};

	private:
		RenderingEngine();
		~RenderingEngine();

	public:
		/// <summary>
		/// インスタンスの生成。
		/// </summary>
		static void CreateInstance()
		{
			m_instance = new RenderingEngine;
			m_instance->Init();
		}
		/// <summary>
		/// インスタンスの削除。
		/// </summary>
		static void DeleteInstance()
		{
			delete m_instance;
		}
		/// <summary>
		/// インスタンスの取得。
		/// </summary>
		/// <returns></returns>
		static RenderingEngine* GetInstance()
		{
			return m_instance;
		}

		/// <summary>
		/// レンダーオブジェクトを追加。
		/// </summary>
		/// <param name="renderObject"></param>
		void AddRenderObject(IRenderer* renderObject)
		{
			m_renderObjects.push_back(renderObject);
		}

		/// <summary>
		/// ディレクショナルライトの設定。
		/// </summary>
		/// <param name="dir">ライトの方向。</param>
		/// <param name="color">ライトのカラー。[</param>
		void SetDirectionLight(const Vector3& dir, const Vector3 color)
		{
			m_directionLig.SetDirection(dir);
			m_directionLig.SetColor(color);
		}

		/// <summary>
		/// 環境光の設定。
		/// </summary>
		/// <param name="amb">環境光。</param>
		void SetAmbient(const float amb)
		{
			m_directionLig.SetAmbientLight(amb);
		}

		/// <summary>
		/// ポイントライトの設定。
		/// </summary>
		/// <param name="ptlig"></param>
		void SetPointLight(const int ptNum, PointLight::pointLight& ptlig)
		{
			//GetLightCB().pointLig = ptlig.GetPointLig();
			GetLightCB().pointLig.ptPosition = ptlig.ptPosition;
			GetLightCB().pointLig.ptColor = ptlig.ptColor;
			GetLightCB().pointLig.ptRange = ptlig.ptRange;
		}

		/// <summary>
		/// スポットライトの設定。
		/// </summary>
		/// <param name="spLig"></param>
		void SetSpotLight(SpotLight::spotLight& spLig)
		{
			GetLightCB().spotLig.spPosition = spLig.spPosition;
			GetLightCB().spotLig.spColor = spLig.spColor;
			GetLightCB().spotLig.spRange = spLig.spRange;
			GetLightCB().spotLig.spDirection = spLig.spDirection;
			GetLightCB().spotLig.spAngle = spLig.spAngle;
		}

		/// <summary>
		/// 半球ライトを設定。
		/// </summary>
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

		/// <summary>
		/// スプライト用の構造体を取得。
		/// </summary>
		/// <returns></returns>
		SpriteCB& GetSpriteCB()
		{
			return m_spriteCB;
		}

		Vector3& GetLightPos()
		{
			return m_lightCB.shadowCB.lightPos;
		}

		/// <summary>
		/// シャドウマップ用レンダーターゲットを取得。
		/// </summary>
		/// <returns></returns>
		RenderTarget& GetShadowRenderTarget()
		{
			return m_shadowMapRenderTarget;
		}

		/// <summary>
		/// シャドウマップ用ガウシアンブラーを取得。
		/// </summary>
		/// <returns></returns>
		GaussianBlur& GetShadowBlur()
		{
			return m_shadowBlur;
		}

		/// <summary>
		/// ブルームの閾値を設定。
		/// </summary>
		void SetBloomThreshold(const float threshold)
		{
			m_bloom.SetThreshold(threshold);
		}

		/// <summary>
		/// 初期化処理。
		/// </summary>
		void Init();

		/// <summary>
		/// 実行処理。
		/// </summary>
		/// <param name="rc"></param>
		void Execute(RenderContext& rc);

	private:
		/// <summary>
		/// 2Dレンダーターゲットを初期化。
		/// </summary>
		void Init2DRenderTarget();

		/// <summary>
		/// シャドウマップ用のレンダーターゲットを初期化。
		/// </summary>
		void InitShadowMapRenderTarget();

		/// <summary>
		/// シャドウマップの描画処理。
		/// </summary>
		/// <param name="rc"></param>
		void RenderShadowMap(RenderContext& rc);

		/// <summary>
		/// フォワードレンダリングの描画処理。
		/// </summary>
		/// <param name="rc"></param>
		void ForwardRendering(RenderContext& rc);

		/// <summary>
		/// 2Dの描画処理。
		/// </summary>
		/// <param name="rc"></param>
		void Render2D(RenderContext& rc);

	private:
		static RenderingEngine* m_instance;

		LightCB m_lightCB;
		SpriteCB m_spriteCB;

		DirectionLight m_directionLig;
		HemiSphereLight m_hemiSphereLig;

		Bloom m_bloom;

		RenderTarget m_mainRenderTarget;								//メインレンダーターゲット

		RenderTarget m_2DRenderTarget;                                  //2Dレンダーターゲット
		Sprite m_2DSprite;                                              //2Dスプライト
		Sprite m_mainSprite;
		Sprite m_copyMainRtToFrameBufferSprite;                         //メインレンダーターゲットのスプライト
		
		RenderTarget m_shadowMapRenderTarget;							//シャドウマップ用のレンダーターゲット
		GaussianBlur m_shadowBlur;										//シャドウ用のガウシアンブラー
		
		std::vector<IRenderer*> m_renderObjects;

		Camera m_lightCamera;
	};
}