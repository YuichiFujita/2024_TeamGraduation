//=============================================================================
// 
//  �}�b�v�u���b�N�ł̔z�u���G�f�B�^�\�w�b�_�[ [edit_mapblock.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _EDIT_MAPBLOCK_H_
#define _EDIT_MAPBLOCK_H_	// ��d�C���N���[�h�h�~

#include "listmanager.h"
#include "objectX.h"
#include "edithandle.h"
#include "edit.h"
#include "collisionLine_Box.h"

class CEdit_MapBlock_Super;
class CMap_Obstacle;

//==========================================================================
// �N���X��`
//==========================================================================
// ��Q���G�f�B�^�\�N���X
class CEdit_MapBlock : public CEdit
{
public:

	CEdit_MapBlock();
	~CEdit_MapBlock();

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

private:

	//=============================
	// �����o�֐�
	//=============================

	//=============================
	// �����o�ϐ�
	//=============================
	CEdit_MapBlock_Super* m_pEditControl;	// ��ޑ���
};

//------------------------------------
// ��Q���G�f�B�^�\�̃X�[�p�[�N���X
//------------------------------------
class CEdit_MapBlock_Super
{
public:

	struct SMapModelInfo
	{
		int nIdx;
		std::string fileName;
	};

	CEdit_MapBlock_Super() {}
	~CEdit_MapBlock_Super() {}

	virtual void Init() = 0;
	virtual void Update() = 0;

	// �I������
	virtual void Uninit()
	{
		// �����蔻��{�b�N�X�폜
		DeleteBoxLine();
		delete this;
	}
	virtual void CreateBoxLine() = 0;	// �����蔻��{�b�N�X����

	// �����蔻��{�b�N�X�폜
	virtual void DeleteBoxLine()
	{
		for (const auto& box : m_pCollisionLineBox)
		{
			box->Kill();
		}
		m_pCollisionLineBox.clear();
	}

protected:
	std::vector<CCollisionLine_Box*> m_pCollisionLineBox;	// �����蔻��{�b�N�X
	SMapModelInfo m_MapInfo;	// ��Q�����
};

//------------------------------------
// ��Q���G�f�B�^�\�̔z�u
//------------------------------------
class CEdit_MapBlock_Arrangment : public CEdit_MapBlock_Super
{
public:

	CEdit_MapBlock_Arrangment() : CEdit_MapBlock_Super() { m_FileName.clear(); }
	~CEdit_MapBlock_Arrangment() {}

	void Init() override;	// ������
	void Update() override; // �X�V
	void CreateBoxLine() override;	// �����蔻��{�b�N�X����

private:

	void ObjectSelect();	// �I�u�W�F�N�g�I��
	void ChangeHandle();
	void Transform();

	bool m_bButtonDrag = false;		// �͂ݔ���
	bool m_bReGrab = false;			// �Ē͂ݔ���
	bool m_bGrabHandle = false;			// �͂ݔ���
	bool m_bHoverWindow = false;	// �E�B���h�E�̃z�o�[����
	CObjectX* m_pDragObj = nullptr;	// �h���b�O���̃I�u�W�F�N�g
	CObjectX* m_pGrabObj = nullptr;	// �͂݃I�u�W�F�N�g

	CHandle* m_pHandle = nullptr;					// �ړ��n���h��
	CHandle::HandleType m_HandleType = CHandle::HandleType::TYPE_MOVE;	// �n���h���̎��
	CHandle::HandleAngle m_moveAngle = CHandle::HandleAngle::ANGLE_X;	// �ړ��̌���
	std::vector<std::string> m_FileName;

};

#endif
