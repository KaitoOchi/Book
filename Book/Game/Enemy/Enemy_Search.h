#pragma once
#include "Enemy.h"
class Enemy_Search:public Enemy
{
public:
	Enemy_Search();
	~Enemy_Search()override;
	bool Start();
	void Update();
	void Rotaition();
	void Render(RenderContext& rc);

	void Update_OnSearch();				// ���G
	void Update_OnCall();				// ����̓G���Ă�
	void Update_OnConfusion();			// �M���e�ɓ��������Ƃ�
	void Update_OnMissingPlayer();		// �v���C���[������������
	void Update_OnEvent();				// �C�x���g���̏���
		
private:
	ModelRender m_searchModelRender;	// �z�C�b�X�����Ăяo�����f��

	//SpotLight m_spotLight;				//�X�|�b�g���C�g
	bool m_rotState=true;				//��]�̌����̐ؑ�
	float m_rotTime = 2.0f;
};