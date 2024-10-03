//=============================================================================
// 
//  ���g���CUI�w�b�_�[ [retryui.h]
//  Author : �x�씋��
// 
//=============================================================================

#ifndef _RETRY_UI_H_
#define _RETRY_UI_H_	// ��d�C���N���[�h�h�~

#include "object2d.h"

// �O���錾
class CControlKeyDisp;

//==========================================================================
// �N���X��`
//==========================================================================
// �T���v��_�I�u�W�F�N�g2D�N���X
class CRetry_Ui : public CObject2D
{
public:

	//=============================
	// UI��
	//=============================
	enum UINUM
	{
		BUTTON_RESTART = 0,		// ���X�^�[�g�{�^��
		BUTTON_RETRY,			// ���g���C�{�^��
		BUTTON_POSE,			// �|�[�Y�{�^��
		BUTTON_MAX
	};

	CRetry_Ui(int nPriority = 7);
	~CRetry_Ui();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//=============================
	// �ÓI�֐�
	//=============================
	static CRetry_Ui* Create();

private:

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԍn

	// ���̑��֐�
	void Moveui();	// �ړ�����
	//=============================
	// �����o�ϐ�
	//=============================
	// ��Ԍn

	// ���̑��ϐ�
	CObject2D* m_Button[BUTTON_MAX];	// 2D�I�u�W�F�N�g
	CControlKeyDisp* m_apKeyDisp[BUTTON_MAX];	// �L�[�R���t�B�O
	MyLib::Vector3 m_DispPos[BUTTON_MAX];
	float m_fRetryPushTime;			// ���g���C�̉�������
};


#endif