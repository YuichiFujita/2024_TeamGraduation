//=============================================================================
// 
//  ������G�f�B�^�\�w�b�_�[ [edit_waterstone.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _EDIT_WATERSTONE_H_
#define _EDIT_WATERSTONE_H_	// ��d�C���N���[�h�h�~

#include "listmanager.h"
#include "objectX.h"
#include "edithandle.h"
#include "edit.h"

//==========================================================================
// �N���X��`
//==========================================================================
// ������N���X
class CEdit_WaterStone : public CEdit
{
public:

	CEdit_WaterStone();
	~CEdit_WaterStone();
	
	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

private:

	//=============================
	// �\���̒�`
	//=============================
	struct LineData
	{
		MyLib::Vector3 pos;	// �ʒu
		MyLib::Vector3 rot;	// ����
		float width;		// ��
	};

	// �����o�֐�
	void FileControl();		// �t�@�C������
	void SelectLine();		// ���C���I��
	void DragLine();		// ���C���͂�
	void Transform();		// �g�����X�t�H�[��
	void ChangeMode();		// ���[�h�ύX
	void AddPoint();		// �n�_�ǉ�

	LineData m_lineData;	// �ӏ��
	int m_nEditIdx;			// ���삷��C���f�b�N�X�ԍ�
	bool m_bEdit;			// ���쒆����
	bool m_bDrag;			// �͂ݔ���
	bool m_bHoverWindow;	// �}�E�X�̃E�B���h�E�z�o�[����
	bool m_bSetMode;		// �ݒ胂�[�h����
};


#endif