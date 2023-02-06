#pragma once


#include "MyRenderer.h"

namespace nsK2Engine {

	class RenderingEngine
	{
	public:
		RenderingEngine();
		~RenderingEngine();

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
		/// �r���[�J�����O�p�̃r���[�v���W�F�N�V�����s����v�Z�B
		/// </summary>
		void CalcViewProjectionMatrixForViewCulling();

	private:
		std::vector<IRenderer* > m_renderObjects;		//�`��I�u�W�F�N�g�̃��X�g
		Matrix m_viewProjMatrixForViewCulling;

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