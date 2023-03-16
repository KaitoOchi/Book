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
		//ライト用定数バッファの構造体
		struct LightCB
		{
			DirectionLight::directionLight directionLig;
			PointLight::pointLight pointLig;
			SpotLight::spotLight spotLig;
			HemiSphereLight::hemiSphereLight hemiSphereLig;
		};

		//スプライト用定数バッファの構造体
		struct SpriteCB
		{
			float clipSize;
		};

	private:
		RenderingEngine();
		~RenderingEngine();

	public:
		/// <summary>
		/// インスタンスの作成。
		/// </summary>
		static void CreateInstance()
		{
			m_instance = new RenderingEngine;
			m_instance->Init();
		}
		/// <summary>
		/// インスタンスの破棄。
		/// </summary>
		static void DeleteInstance()
		{
			delete m_instance;
		}
		/// <summary>
		/// インスタンスを取得。
		/// </summary>
		/// <returns></returns>
		static RenderingEngine* GetInstance()
		{
			return m_instance;
		}

		/// <summary>
		/// 描画オブジェクトを追加。
		/// </summary>
		/// <param name="renderObject"></param>
		void AddRenderObject(IRenderer* renderObject)
		{
			m_renderObjects.push_back(renderObject);
		}

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
		/// ポイントライトを設定
		/// </summary>
		/// <param name="ptlig"></param>
		void SetPointLight(PointLight::pointLight& ptlig)
		{
			//GetLightCB().pointLig = ptlig.GetPointLig();
			GetLightCB().pointLig.ptPosition = ptlig.ptPosition;
			GetLightCB().pointLig.ptColor = ptlig.ptColor;
			GetLightCB().pointLig.ptRange = ptlig.ptRange;
		}

		/// <summary>
		/// スポットライトを設定
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

		/// <summary>
		/// スプライト用の構造体を取得。
		/// </summary>
		/// <returns></returns>
		SpriteCB& GetSpriteCB()
		{
			return m_spriteCB;
		}

		/// <summary>
		/// ブルームが発生する閾値を設定
		/// </summary>
		void SetBloomThreshold(const float threshold)
		{
			m_bloom.SetThreshold(threshold);
		}

		void Init();

		/// <summary>
		/// 実行処理。
		/// </summary>
		/// <param name="rc"></param>
		void Execute(RenderContext& rc);

	private:
		/// <summary>
		/// 2D描画用のレンダ―ターゲットを初期化
		/// </summary>
		void Init2DRenderTarget();

		/// <summary>
		/// メインレンダリングターゲットのカラーバッファの内容を
		/// フレームバッファにコピーするためのスプライトを初期化する
		/// </summary>
		void InitCopyMainRenderTargetToFrameBufferSprite();

		/// <summary>
		/// フォワードレンダリングの処理。
		/// </summary>
		/// <param name="rc"></param>
		void ForwardRendering(RenderContext& rc);

		/// <summary>
		/// 2D描画
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void Render2D(RenderContext& rc);

		/// <summary>
		/// メインレンダリングターゲットの内容をフレームバッファにコピーする
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void CopyMainRenderTargetToFrameBuffer(RenderContext& rc);

	private:
		static RenderingEngine* m_instance;

		LightCB m_lightCB;
		SpriteCB m_spriteCB;

		DirectionLight m_directionLig;
		HemiSphereLight m_hemiSphereLig;

		Bloom m_bloom;

		RenderTarget m_mainRenderTarget;								//メインレンダリングターゲット

		RenderTarget m_2DRenderTarget;                                  //2D描画用のレンダ―ターゲット
		Sprite m_2DSprite;                                              //2D合成用のスプライト
		Sprite m_mainSprite;
		Sprite m_copyMainRtToFrameBufferSprite;                         // メインレンダリングターゲットをフレームバッファにコピーするためのスプライト
		std::vector<IRenderer*> m_renderObjects;
	};
}