#pragma once
//Programmer : KaitoOchi

#include "geometry/AABB.h"
//#include "geometry/geometryData.h"



namespace nsBookEngine {

	class ModelRender
	{
	public:
		ModelRender();
		~ModelRender();

		/// <summary>
		/// 初期化処理。
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		/// <param name="animationClip">アニメーションクリップ。</param>
		/// <param name="numAnimationClip">アニメーションクリップの数。</param>
		/// <param name="enModelUpAxis">モデルの上方向。</param>
		/// <param name="isShadowReceiver">trueなら影が生成される。</param>
		/// <param name="maxInstance">インスタンスの最大数。インスタンシング描画用。</param>
		/// <param name="isFrontCullingOnDrawShadowMap">シャドウマップ描画時にフロントカリングを行うかどうか。</param>
		void Init(
			const char* filePath,
			AnimationClip* animationClip = nullptr,
			int numAnimationClip = 0,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			bool isShadowReceiver = true,
			int maxInstance = 1,
			bool isFrontCullingOnDrawShadowMap = false);

		/// <summary>
		/// 更新処理。
		/// </summary>
		void Update();

		/// <summary>
		/// 描画処理。
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void Draw(RenderContext& rc);

		/// <summary>
		/// アニメーションを再生。
		/// </summary>
		/// <param name="animNo">アニメーションクリップの番号。</param>
		/// <param name="interpolateTime">線形補間の時間</param>
		void PlayAnimation(int animNo, float interpolateTime = 0.0f)
		{
			m_animation.Play(animNo, interpolateTime);
		}

		/// <summary>
		/// アニメーションの再生中かどうか。
		/// </summary>
		/// <returns></returns>
		bool IsPlayingAniamtion()
		{
			return m_animation.IsPlaying();
		}

		/// <summary>
		/// モデルを取得。
		/// </summary>
		/// <returns></returns>
		Model& GetModel()
		{
			/*
			if (m_renderToGBufferModel.IsInited()) {
				return m_renderToGBufferModel;
			}
			else if (m_forwardRenderModel.IsInited()) {
				return m_forwardRenderModel;
			}
			else if (m_translucentModel.IsInited())
			{
				return m_translucentModel;
			}
			*/
			// ここまで来るのはおかしい。
			return m_zprepassModel;
		}

		/// <summary>
		/// 座標を設定。
		/// </summary>
		/// <param name="pos"></param>
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
		}

		/// <summary>
		/// 回転を設定。
		/// </summary>
		/// <param name="rot"></param>
		void SetRotation(const Quaternion& rot)
		{
			m_rotation = rot;
		}

		/// <summary>
		/// 拡大率を設定。
		/// </summary>
		/// <param name="scale"></param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}

		/// <summary>
		/// シャドウキャスターのフラグを設定する。
		/// </summary>
		/// <param name="flag"></param>
		void SetShadowCasterFlag(bool flag)
		{
			m_isShadowCaster = flag;
		}

		/// <summary>
		/// アニメーションの速度を設定する。
		/// </summary>
		/// <param name="animationSpeed"></param>
		void SetAnimationSpeed(const float animationSpeed)
		{
			m_animationSpeed = animationSpeed;
		}

		/// <summary>
		/// ボーンの名前からボーン番号を検索。
		/// </summary>
		/// <param name="boneName"></param>
		/// <returns></returns>
		int FindBoneID(const wchar_t* boneName)
		{
			return m_skeleton.FindBoneID(boneName);
		}

		/// <summary>
		/// ボーン番号からボーンを取得。
		/// </summary>
		/// <param name="boneNo"></param>
		/// <returns></returns>
		Bone* GetBone(int boneNo)
		{
			return m_skeleton.GetBone(boneNo);
		}

		/// <summary>
		/// シャドウキャスターのフラグがあるかどうか。
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
		/// <param name="filePath">ファイルパス</param>
		void InitSkeleton(const char* filePath);


		/// <summary>
		/// アニメーションの初期化。
		/// </summary>
		/// <param name="animationClips">アニメーションクリップ</param>
		/// <param name="numAnimationClips">アニメーションクリップの数</param>
		/// <param name="enModelUpAxis">モデルの上向き</param>
		void InitAnimation(AnimationClip* animationClips,
			int numAnimationClips,
			EnModelUpAxis enModelUpAxis);

		/// <summary>
		/// 各種モデルのワールド行列を更新する。
		/// </summary>
		void UpdateWorldMatrixInModes();

		/// <summary>
		/// ZPrepass描画用のモデルを初期化。
		/// </summary>
		/// <param name="renderingEngine"></param>
		/// <param name="tkmFilePath"></param>
		void InitModelOnZprepass(
			//RenderingEngine& renderingEngine,
			const char* tkmFilePath,
			EnModelUpAxis modelUpAxis
		);

		/// <summary>
		/// 各種モデルの頂点シェーダーのエントリーポイントを設定。
		/// </summary>
		void SetupVertexShaderEntryPointFunc(ModelInitData& modelInitData);

	private:
		AnimationClip*	m_animationClips = nullptr;
		int				m_numAnimationClips = 0;

		Vector3			m_position = Vector3::Zero;
		Quaternion		m_rotation = Quaternion::Identity;
		Vector3			m_scale = Vector3::Zero;
		EnModelUpAxis	m_enFbxUpAxis = enModelUpAxisZ;
		Animation		m_animation;
		Model			m_zprepassModel;					// ZPrepassで描画されるモデル
		bool			m_isUpdateAnimation = true;
		Skeleton		m_skeleton;
		bool			m_isShadowCaster = true;
		float			m_animationSpeed = 1.0f;


	};

}
