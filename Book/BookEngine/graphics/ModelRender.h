#pragma once
//Programmer : KaitoOchi

#include "geometry/AABB.h"
#include "graphics/light/DirectionLight.h"
#include "graphics/light/PointLight.h"
//#include "geometry/geometryData.h"

namespace nsBookEngine {

	class ModelRender : public IRenderer
	{
	public:
		ModelRender();
		~ModelRender();

		/// <summary>
		/// 初期化処理。
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		/// <param name="animationClip">アニメーションクリップ。</param>
		/// <param name="numAnimationClip">アニメーションの数。</param>
		/// <param name="enModelUpAxis">モデルの上方向。</param>
		/// <param name="isShadow">trueなら影を与える。</param>
		/// <param name="isShadowReceiver">trueなら影を落とす。</param>
		/// <param name="outlineMode">1でプレイヤー、2で敵の輪郭線</param>
		/// <param name="isFrontCullingOnDrawShadowMap">カリングモード。</param>
		/// <param name="maxInstance">インスタンスの数。</param>
		void Init(
			const char* filePath,
			AnimationClip* animationClip = nullptr,
			int numAnimationClip = 0,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			const bool isShadow = false,
			const bool isShadowReceiver = false,
			const int outlineMode = 0,
			D3D12_CULL_MODE m_cullMode = D3D12_CULL_MODE_BACK,
			int maxInstance = 1);

		/// <summary>
		/// ModelInitDataを使用した初期化処理。
		/// </summary>
		void InitModelData(ModelInitData& initData);

		/// <summary>
		/// 更新処理。
		/// </summary>
		void Update();

		/// <summary>
		/// 描画処理。
		/// </summary>
		/// <param name="rc"></param>
		void Draw(RenderContext& rc);

		/// <summary>
		/// アニメーションの実行。
		/// </summary>
		/// <param name="animNo">アニメーションの番号。</param>
		/// <param name="interpolateTime">実行速度。</param>
		void PlayAnimation(int animNo, float interpolateTime = 0.0f)
		{
			m_animation.Play(animNo, interpolateTime);
		}

		/// <summary>
		/// アニメーションを再生しているか？
		/// </summary>
		/// <returns></returns>
		bool IsPlayingAniamtion()
		{
			return m_animation.IsPlaying();
		}

		/// <summary>
		/// モデルの取得。
		/// </summary>
		/// <returns></returns>
		Model& GetModel()
		{
			return m_model;
		}

		/// <summary>
		/// 座標の設定。
		/// </summary>
		/// <param name="pos"></param>
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
		}

		/// <summary>
		/// 座標の取得。
		/// </summary>
		const Vector3& GetPosition()
		{
			return m_position;
		}

		/// <summary>
		/// 回転の設定。
		/// </summary>
		/// <param name="rot"></param>
		void SetRotation(const Quaternion& rot)
		{
			m_rotation = rot;
		}

		/// <summary>
		/// 拡大率の設定。
		/// </summary>
		/// <param name="scale"></param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}

		/// <summary>
		/// アニメーションイベントの追加。
		/// </summary>
		/// <param name="eventListener"></param>
		void AddAnimationEvent(AnimationEventListener eventListener)
		{
			m_animation.AddAnimationEventListener(eventListener);
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="flag"></param>
		void SetShadowCasterFlag(bool flag)
		{
			m_isShadowCaster = flag;
		}

		/// <summary>
		/// アニメーションの速度を設定。
		/// </summary>
		/// <param name="animationSpeed"></param>
		void SetAnimationSpeed(const float animationSpeed)
		{
			m_animationSpeed = animationSpeed;
		}

		/// <summary>
		/// ボーンIDの取得。
		/// </summary>
		/// <param name="boneName"></param>
		/// <returns></returns>
		int FindBoneID(const wchar_t* boneName)
		{
			return m_skeleton.FindBoneID(boneName);
		}

		/// <summary>
		/// ボーンの取得。
		/// </summary>
		/// <param name="boneNo"></param>
		/// <returns></returns>
		Bone* GetBone(int boneNo)
		{
			return m_skeleton.GetBone(boneNo);
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		bool IsShadowCaster()
		{
			return m_isShadowCaster;
		}

	private:
		/// <summary>
		/// スケルトンの初期化。
		/// </summary>
		/// <param name="filePath">X</param>
		void InitSkeleton(const char* filePath);


		/// <summary>
		/// アニメーションの初期化。
		/// </summary>
		/// <param name="animationClips"></param>
		/// <param name="numAnimationClips"></param>
		/// <param name="enModelUpAxis"></param>
		void InitAnimation(AnimationClip* animationClips,
			int numAnimationClips,
			EnModelUpAxis enModelUpAxis);

		/// <summary>
		/// モデルの更新処理。
		/// </summary>
		void UpdateWorldMatrixInModes();

		/// <summary>
		/// モデルの初期化。
		/// </summary>
		/// <param name="renderingEngine"></param>
		/// <param name="tkmFilePath"></param>
		void InitModel(
			const char* tkmFilePath,
			EnModelUpAxis modelUpAxis,
			const bool isShadow,
			const bool isShadowReceiver,
			const int outlineMode,
			D3D12_CULL_MODE cullMode
		);

	private:
		/// <summary>
		/// シャドウマップの描画処理。
		/// </summary>
		/// <param name="rc"></param>
		void OnRenderShadowMap(RenderContext& rc, Camera& camera) override;
		/// <summary>
		/// ZPrepassの描画処理。
		/// </summary>
		void OnRenderToZPrepass(RenderContext& rc) override;
		/// <summary>
		/// フォワードレンダリングの描画処理。
		/// </summary>
		void OnForwardRender(RenderContext& rc) override;

	private:
		AnimationClip*	m_animationClips = nullptr;
		int				m_numAnimationClips = 0;

		Vector3			m_position = Vector3::Zero;
		Quaternion		m_rotation = Quaternion::Identity;
		Vector3			m_scale = Vector3::One;
		EnModelUpAxis	m_enFbxUpAxis = enModelUpAxisZ;
		Animation		m_animation;
		Model			m_model;
		Model			m_shadowModel;
		Model			m_zprepassModel;
		bool			m_isUpdateAnimation = true;
		Skeleton		m_skeleton;
		bool			m_isShadowCaster = true;
		float			m_animationSpeed = 1.0f;


	};

}
