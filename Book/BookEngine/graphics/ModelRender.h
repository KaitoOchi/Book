#pragma once
#include "graphics/light/DirectionLight.h"
#include "graphics/light/PointLight.h"

namespace nsBookEngine {

	/// <summary>
	/// モデルレンダークラス。
	/// </summary>
	class ModelRender : public IRenderer
	{
	public:
		enum OutlineMode
		{
			enOutlineMode_None,
			enOutlineMode_3DPlayer,
			enOutlineMode_Enemy,
			enOutlineMode_TranslucentEnemy,
			enOutlineMode_2DPlayer,
		};

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
		/// <param name="useWipe">ワイプモデルを使用する。</param>
		void Init(
			const char* filePath,
			AnimationClip* animationClip = nullptr,
			int numAnimationClip = 0,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			const bool isShadow = false,
			const bool isShadowReceiver = false,
			const OutlineMode outlineMode = enOutlineMode_None,
			D3D12_CULL_MODE m_cullMode = D3D12_CULL_MODE_BACK,
			const bool useWipe = false
		);

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
		const bool IsPlayingAniamtion() const
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
		Model& GetShadowModel()
		{
			return m_shadowModel;
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
			EnModelUpAxis enModelUpAxis
		);

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
			const OutlineMode outlineMode,
			D3D12_CULL_MODE cullMode,
			const bool useWipe
		);

		/// <summary>
		/// シャドウ用モデルの初期化。
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		/// <param name="enModelUpAxis">モデルの上方向。</param>
		/// <param name="outlineMode">1でプレイヤー、2で敵の輪郭線</param>
		/// <param name="isFrontCullingOnDrawShadowMap">カリングモード。</param>
		void InitShadowModel(
			const char* tkmFilePath,
			EnModelUpAxis modelUpAxis,
			const int outlineMode,
			D3D12_CULL_MODE cullMode
		);

		/// <summary>
		/// ZPrepass用モデルの初期化。
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		/// <param name="enModelUpAxis">モデルの上方向。</param>
		/// <param name="isFrontCullingOnDrawShadowMap">カリングモード。</param>
		void InitZPrepassModel(
			const char* tkmFilePath,
			EnModelUpAxis modelUpAxis,
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
		/// <summary>
		/// ワイプ越しのフォワードレンダリングの描画処理。
		/// </summary>
		void OnWipeForwardRender(RenderContext& rc, Camera& camera) override;

	private:
		AnimationClip*	m_animationClips = nullptr;			//アニメーションクリップ
		int				m_numAnimationClips = 0;			//アニメーションの数
		EnModelUpAxis	m_enFbxUpAxis = enModelUpAxisZ;		/* モデルの上方向 */
		Animation		m_animation;						//アニメーション
		Skeleton		m_skeleton;							//スケルトン
		Model			m_model;							//通常モデル
		Model			m_shadowModel;						//シャドウモデル
		Model			m_zprepassModel;					//ZPrepassモデル
		Model			m_wipeModel;						//ワイプ用モデル
		Vector3			m_position;							//座標
		Quaternion		m_rotation;							//回転
		Vector3			m_scale = Vector3::One;				//拡大率
		bool			m_isUpdateAnimation = true;			//アニメーションの更新
		bool			m_isUseWipeCamera = false;			//ワイプカメラに写すかどうか
		float			m_animationSpeed = 1.0f;			//アニメーション速度
	};
}
