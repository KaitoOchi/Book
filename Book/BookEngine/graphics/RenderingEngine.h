#pragma once

#include "graphics/IRenderer.h"

#include "graphics/light/DirectionLight.h"
#include "graphics/light/PointLight.h"
#include "graphics/light/SpotLight.h"
#include "graphics/light/HemiSphereLight.h"
#include "graphics/postEffect/Bloom.h"

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
			int playerAnim2D;									//2Dアニメーションの番号
		};

		//ライト用の構造体
		struct LightCB
		{
			DirectionLight::directionLight directionLig;
			PointLight::pointLight pointLig[4];
			SpotLight::spotLight spotLig[42];
			HemiSphereLight::hemiSphereLight hemiSphereLig;
			ShadowParamCB shadowCB;	
			int ptNum;											//ポイントライトの数
			int spNum;											//スポットライトの数
		};

		//スプライト用の構造体
		struct SpriteCB
		{
			Vector3 clipSize;
			int clipMode = 0;
			int processMode = 0;
			float processRate = 0.0f;
		};

	private:
		RenderingEngine();
		~RenderingEngine();

	public:

		const void Setm_directionLig(Vector3 light,float ambi,Vector3 color)
		{
			m_directionLig.SetDirection(light);
			m_directionLig.SetAmbientLight(ambi);
			m_directionLig.SetColor(color);
		}



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
			GetLightCB().directionLig = m_directionLig.GetDirectionLig();
		}

		/// <summary>
		/// 環境光の設定。
		/// </summary>
		/// <param name="amb">環境光。</param>
		void SetAmbient(const float amb)
		{
			m_directionLig.SetAmbientLight(amb);
			GetLightCB().directionLig = m_directionLig.GetDirectionLig();
		}

		/// <summary>
		/// ポイントライトの設定。
		/// </summary>
		/// <param name="ptlig"></param>
		void SetPointLight(const int ptNum, PointLight::pointLight& ptlig)
		{
			//GetLightCB().pointLig = ptlig.GetPointLig();
			GetLightCB().pointLig[ptNum].ptPosition = ptlig.ptPosition;
			GetLightCB().pointLig[ptNum].ptColor = ptlig.ptColor;
			GetLightCB().pointLig[ptNum].ptRange = ptlig.ptRange;
		}

		/// <summary>
		/// スポットライトの設定。
		/// </summary>
		/// <param name="spLig"></param>
		void SetSpotLight(const int spNum, SpotLight::spotLight& spLig)
		{
			GetLightCB().spotLig[spNum].spPosition = spLig.spPosition;
			GetLightCB().spotLig[spNum].spColor = spLig.spColor;
			GetLightCB().spotLig[spNum].spRange = spLig.spRange;
			GetLightCB().spotLig[spNum].spDirection = spLig.spDirection;
			GetLightCB().spotLig[spNum].spAngle = spLig.spAngle;
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
		/// ブルームの閾値を設定。
		/// </summary>
		void SetBloomThreshold(const float threshold)
		{
			m_bloom.SetThreshold(threshold);
		}

		/// <summary>
		/// 画像加工の設定。
		/// </summary>
		/// <param name="mode">1でセピア調</param>
		void SetScreenProcess(const int mode)
		{
			m_spriteCB.processMode = mode;
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

		/// <summary>
		/// シャドウ用の構造体を取得。
		/// </summary>
		/// <returns></returns>
		ShadowParamCB& GetShadowCB()
		{
			return m_lightCB.shadowCB;
		}

		/// <summary>
		/// ZPrepassレンダーターゲットテクスチャを取得。
		/// </summary>
		/// <returns></returns>
		Texture& GetZPrepassDepthTexture()
		{
			return m_zprepassRenderTarget.GetRenderTargetTexture();
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
		/// ワイプ用のビューポートを取得。
		/// </summary>
		D3D12_VIEWPORT& GetWipeViewPort()
		{
			return m_viewPorts[1];
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
		/// ZPrepass用のレンダーターゲットを初期化。
		/// </summary>
		void InitZPrepassRenderTarget();

		/// <summary>
		/// ビューポートの初期化。
		/// </summary>
		void InitViewPort();

		/// <summary>
		/// シャドウマップの描画処理。
		/// </summary>
		/// <param name="rc"></param>
		void RenderShadowMap(RenderContext& rc);

		/// <summary>
		/// ZPrepassの描画処理。
		/// </summary>
		void ZPrepass(RenderContext& rc);

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

		LightCB					m_lightCB;							//ライト用の構造体
		SpriteCB				m_spriteCB;							//スプライト用の構造体
		DirectionLight			m_directionLig;						//ディレクショナルライトの構造体
		HemiSphereLight			m_hemiSphereLig;					//半球ライトの構造体
		Bloom					m_bloom;							//ブルーム

		RenderTarget			m_mainRenderTarget;					//メインレンダーターゲット
		RenderTarget			m_2DRenderTarget;					//2Dレンダーターゲット
		Sprite					m_2DSprite;                         //2Dスプライト
		Sprite					m_mainSprite;
		Sprite					m_copyMainRtToFrameBufferSprite;    //メインレンダーターゲットのスプライト
		RenderTarget			m_shadowMapRenderTarget;			//シャドウマップ用のレンダーターゲット
		GaussianBlur			m_shadowBlur;						//シャドウ用のガウシアンブラー
		RenderTarget			m_zprepassRenderTarget;				//ZPrepass用のレンダーターゲット
		std::vector<IRenderer*> m_renderObjects;					//レンダリングするオブジェクト
		Camera					m_lightCamera;						//ライトカメラ
		Camera					m_wipeCamera;						//ワイプカメラ。

		D3D12_VIEWPORT			m_viewPorts[2];						//ビューポート
	};
}