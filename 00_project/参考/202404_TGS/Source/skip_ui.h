//=============================================================================
// 
//  �X�L�b�vUI�w�b�_�[ [skip_ui.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _SKIP_UI_H_
#define _SKIP_UI_H_	// ��d�C���N���[�h�h�~

#include "object2d.h"
class CObjectCircleGauge2D;

//==========================================================================
// �N���X��`
//==========================================================================
// �X�L�b�vUI�N���X
class CSkip_UI : public CObject
{
public:
	//=============================
	// ��ԗ�
	//=============================
	enum State
	{
		STATE_NONE = 0,	// �Ȃɂ��Ȃ�
		STATE_FADEOUT,	// �t�F�[�h�A�E�g
		STATE_MAX
	};

	CSkip_UI(int nPriority = 8, CObject::LAYER layer = CObject::LAYER::LAYER_2D);
	~CSkip_UI();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;


	void Kill();		// �폜
	void SetState(State state);	// ��Ԑݒ�
	bool IsComplete() { return m_bCompleteSkip; }	// ��������

	//=============================
	// �ÓI�֐�
	//=============================
	static CSkip_UI* Create();

private:
	//=============================
	// �����o�֐�
	//=============================
	// ��Ԍn
	void StateFadeout();	// �t�F�[�h�A�E�g

	// ���̑�
	void CreateButton();	// �{�^���쐬
	void CreateGauge();		// �Q�[�W�쐬
	void CreateUI();	// UI����
	void KillUI();		// UI�폜

	//=============================
	// �����o�ϐ�
	//=============================
	// ��Ԍn
	State m_state;				// ���
	float m_fStateTime;			// ��ԃ^�C�}�[

	// ���̑�
	float m_fPressTimer;	// ��������
	bool m_bCompleteSkip;	// �X�L�b�v����
	CObject2D* m_pButton;	// �{�^��
	CObjectCircleGauge2D* m_pObjCircleGauge;	// �~�Q�[�W
};


#endif