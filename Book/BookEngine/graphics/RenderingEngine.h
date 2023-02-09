#pragma once


#include "MyRenderer.h"
#include "../geometry/SceneGeometryData.h"

namespace nsBookEngine {

	class RenderingEngine
	{
	public:
		RenderingEngine();
		~RenderingEngine();

		//���C�������_�����O�^�[�Q�b�g�̃X�i�b�v�V���b�g
		enum class EnMainRTSnapshot
		{
			enDrawnOpacity,     // �s�����I�u�W�F�N�g�̕`�抮�����_
			enNum,              // �X�i�b�v�V���b�g�̐�
		};

		// �����_�����O�p�X
		enum class EnRenderingPass
		{
			enComputeAnimatedVertex,    // �A�j���[�V�����ςݒ��_�v�Z�B
			enRenderToShadowMap,        // �V���h�E�}�b�v�ւ̕`��p�X
			enZPrepass,                 // ZPrepass
			enRenderToVolumeLightMap,   // �{�����[�����C�g�}�b�v�ւ̕`��
			enLightCulling,             // ���C�g�J�����O�B
			enRenderToGBuffer,          // G-Buffer�ւ̕`��p�X
			enForwardRender,            // �t�H���[�h�����_�����O�̕`��p�X
			enPostEffect,               // �|�X�g�G�t�F�N�g
			enRender2D,                 // 2D�`��B
			enRender3D,                 // 3D���f���őO��
		};

		/// <summary>
		/// �C�x���g�B
		/// </summary>
		enum EnEvent {
			enEventReInitIBLTexture,    // IBL�e�N�X�`�����ď��������ꂽ�B
		};

		/// <summary>
		/// �����_�����O�p�C�v���C�����������B
		/// </summary>
		/// <param name="isSoftShadow">�\�t�g�V���h�E�ɂ��邩�ǂ����B</param>
		void Init(bool isSoftShadow);

		/// <summary>
		/// �����_�����O�p�C�v���C�������s
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		void Execute(RenderContext& rc);

		/// <summary>
		/// �`��I�u�W�F�N�g��ǉ��B
		/// </summary>
		/// <param name="renderObject"></param>
		void AddRenderObject(IRenderer* renderObject)
		{
			m_renderObjects.push_back(renderObject);
		}

		/// <summary>
		/// �r���[�J�����O�̂��߂̃r���[�v���W�F�N�V�����s����擾�B
		/// </summary>
		/// <returns></returns>
		const Matrix& GetViewProjectionMatrixForViewCulling()
		{
			return m_viewProjMatrixForViewCulling;
		}


		/// <summary>
		/// �X�V�����B
		/// </summary>
		void Update();

		/// <summary>
		/// �C�x���g���X�i�[��ǉ��B
		/// </summary>
		/// <param name="pListenerObj"></param>
		/// <param name="listenerFunc"></param>
		void AddEventListener(
			void* pListenerObj,
			std::function<void(EnEvent enEvent)> listenerFunc
		)
		{
			m_eventListeners.push_back({ pListenerObj, listenerFunc });
		}

		void RemoveEventListener(void* pListenerObj)
		{
			auto it = std::find_if(
				m_eventListeners.begin(),
				m_eventListeners.end(),
				[&](const SEventListenerData& listenerData) {return listenerData.pListenerObj == pListenerObj; }
			);
			if (it != m_eventListeners.end()) {
				m_eventListeners.erase(it);
			}
		}

	private:
		/// <summary>
		/// G-Buffer��������
		/// </summary>
		void InitGBuffer();
		/// <summary>
		/// �A�j���[�V�����ςݒ��_�̌v�Z�B
		/// </summary>
		/// <remark>
		/// </remark>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void ComputeAnimatedVertex(RenderContext& rc);
		/// <summary>
		/// G-Buffer�ւ̕`��
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g�B</param>
		void RenderToGBuffer(RenderContext& rc);
		/// <summary>
		/// 2D�`��B
		/// </summary>
		/// <param name="rc"></param>
		void Render2D(RenderContext& rc);
		/// <summary>
		/// ���C�������_�����O�^�[�Q�b�g�̓��e���t���[���o�b�t�@�ɃR�s�[����
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void CopyMainRenderTargetToFrameBuffer(RenderContext& rc);
		/// <summary>
		/// �t�H���[�h�����_�����O
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void ForwardRendering(RenderContext& rc);
		/// <summary>
		/// ���C�������_�����O�^�[�Q�b�g��������
		/// </summary>
		void InitMainRenderTarget();
		/// <summary>
		/// ���C�������_�����O�^�[�Q�b�g�̂̃X�i�b�v�V���b�g����邽�߂�
		/// �����_�����O�^�[�Q�b�g��������
		/// </summary>
		void InitMainRTSnapshotRenderTarget();
		/// <summary>
		/// ���C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@�̓��e��
		/// �t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g������������
		/// </summary>
		void InitCopyMainRenderTargetToFrameBufferSprite();
		/// <summary>
		/// ���C�������_�����O�^�[�Q�b�g�̃X�i�b�v�V���b�g���B�e�B
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="enSnapshot"></param>
		void SnapshotMainRenderTarget(RenderContext& rc, EnMainRTSnapshot enSnapshot);
		/// <summary>
		/// �r���[�J�����O�p�̃r���[�v���W�F�N�V�����s����v�Z�B
		/// </summary>
		void CalcViewProjectionMatrixForViewCulling();
		/// <summary>
		/// 2D�`��p�̘A�ŗу^�[�Q�b�g���������B
		/// </summary>
		void Init2DRenderTarget();

	private:
		// GBuffer�̒�`
		enum EnGBuffer
		{
			enGBufferAlbedoDepth,           // �A���x�h�Ɛ[�x�l�B���ɐ[�x�l���L������Ă��܂��B
			enGBufferNormal,                // �@��
			enGBufferMetaricShadowSmooth,   // ���^���b�N�A�e�p�����[�^�A�X���[�X�B
			// ���^���b�N��r�A�e�p�����[�^��g�A�X���[�X��a�Bg�͖��g�p�B
			enGBufferNum,                   // G-Buffer�̐�
		};

		RenderTarget m_mainRenderTarget;								//���C�������_�����O�^�[�Q�b�g
		Sprite m_copyMainRtToFrameBufferSprite;                         // ���C�������_�����O�^�[�Q�b�g���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g
		RenderTarget m_mainRTSnapshots[(int)EnMainRTSnapshot::enNum];   // ���C�������_�����O�^�[�Q�b�g�̃X�i�b�v�V���b�g
		RenderTarget m_gBuffer[enGBufferNum];                           // G-Buffer
		std::vector<IRenderer*> m_renderObjects;						//�`��I�u�W�F�N�g�̃��X�g
		Matrix m_viewProjMatrixForViewCulling;
		SceneGeometryData m_sceneGeometryData;							//�V�[���W�I���g�����
		static RenderingEngine* m_instance;		                        // �B��̃C���X�^���X�̃A�h���X���L�^����ϐ��B
		RenderTarget m_2DRenderTarget;                                  // 2D�`��p�̃����_�\�^�[�Q�b�g�B
		Sprite m_2DSprite;												//2D�����p�̃X�v���C�g
		Sprite m_mainSprite;

		/// <summary>
		/// �C�x���g���X�i�[�̃f�[�^�B
		/// </summary>
		struct SEventListenerData {
			void* pListenerObj;	//���X�i�[�I�u�W�F�N�g
			std::function<void(EnEvent enEvent)> listnerFunc;
		};

		std::list<SEventListenerData> m_eventListeners;		//�C�x���g���X�i�[
	};
}