#include "PreCompile.h"
#include "ModelRender.h"


namespace k2Engine {

	ModelRender::ModelRender()
	{
	}

	ModelRender::~ModelRender()
	{

	}

	void ModelRender::Init(const char* filePath,
		AnimationClip* animationClips,
		int numAnimationClips,
		EnModelUpAxis enModelUpAxis,
		bool isShadowReceiver,
		int maxInstance,
		bool isFrontCullingOnDrawShadowMap)
	{
		//インスタンシング描画用のデータを初期化。
		InitInstancingDraw(maxInstance);

		//スケルトンを初期化。
		InitSkeleton(filePath);

		//アニメーションを初期化。
		InitAnimation(animationClips, numAnimationClips, enModelUpAxis);

		

	}

	void ModelRender::InitSkeleton(const char* filePath)
	{
		//スケルトンデータを読み込む
		std::string skeletonFilePath = filePath;
		int pos = (int)skeletonFilePath.find(".tkm");
		skeletonFilePath.replace(pos, 4, ".tks");
		m_skeleton.Init(skeletonFilePath.c_str());
	}

	void ModelRender::InitAnimation(AnimationClip* animationClips, int numAnimationClips, EnModelUpAxis enModelUpAxis)
	{
		m_animationClips = animationClips;
		m_numAnimationClips = numAnimationClips;

		if (m_animationClips != nullptr) {
			m_animation.Init(m_skeleton,
				m_animationClips,
				numAnimationClips);
		}
	}
	/*
	void ModelRender::InitInstancingDraw(int maxInstance)
	{
		m_maxInstance = maxInstance;

		if (m_maxInstance > 1) {
			//インスタンス描画用のデータを構築
			m_worldMatrixArray = std::make_unique<Matrix[]>(m_maxInstance);

			//ワールド行列をGPUに転送するためのストラクチャードバッファを確保
			m_worldMatricArraySB.Init(
				sizeof(Matrix),
				m_maxInstance,
				nullptr;
			);
			m_isEnableInstancingDraw = true;

			// インスタンス番号からワールド行列の配列のインデックスに変換するテーブルを初期化する。
			m_instanceNoToWorldMatrixArrayIndexTable = std::make_unique<int[]>(m_maxInstance);
			for (int instanceNo = 0; instanceNo < m_maxInstance; instanceNo++) {
				m_instanceNoToWorldMatrixArrayIndexTable[instanceNo] = instanceNo;
			}
		}
	}
	*/

	void ModelRender::Update()
	{
		if (m_isEnableInstancingDraw) {
			return;
		}
	}
}