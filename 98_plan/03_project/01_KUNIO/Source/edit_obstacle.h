//=============================================================================
// 
//  ��Q���G�f�B�^�\�w�b�_�[ [edit_obstacle.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _EDIT_OBSTACLE_H_
#define _EDIT_OBSTACLE_H_	// ��d�C���N���[�h�h�~

#include "listmanager.h"
#include "objectX.h"
#include "edithandle.h"
#include "edit.h"
#include "collisionLine_Box.h"
#include "map_obstacleManager.h"

class CEdit_Obstacle_Super;
class CMap_Obstacle;

//==========================================================================
// �N���X��`
//==========================================================================
// ��Q���G�f�B�^�\�N���X
class CEdit_Obstacle : public CEdit
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum EditType
	{
		TYPE_ARRANGMENT = 0,
		TYPE_COLLIDER,
		TYPE_MAX
	};

	CEdit_Obstacle();
	~CEdit_Obstacle();
	
	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

private:

	//=============================
	// �����o�֐�
	//=============================
	void ChangeMode(EditType type);	// ���[�h�ύX

	//=============================
	// �����o�ϐ�
	//=============================
	EditType m_editType;				// �G�f�B�b�g�^�C�v
	CEdit_Obstacle_Super* m_pEditControl;	// ��ޑ���
};

//------------------------------------
// ��Q���G�f�B�^�\�̃X�[�p�[�N���X
//------------------------------------
class CEdit_Obstacle_Super
{
public:

	CEdit_Obstacle_Super() {}
	~CEdit_Obstacle_Super() {}

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
};

//------------------------------------
// ��Q���G�f�B�^�\�̔z�u
//------------------------------------
class CEdit_Obstacle_Arrangment : public CEdit_Obstacle_Super
{
public:

	CEdit_Obstacle_Arrangment() : CEdit_Obstacle_Super() {}
	~CEdit_Obstacle_Arrangment() {}

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
	CMap_Obstacle* m_pGrabObj = nullptr;	// �͂݃I�u�W�F�N�g

	CHandle* m_pHandle = nullptr;					// �ړ��n���h��
	CHandle::HandleType m_HandleType = CHandle::HandleType::TYPE_MOVE;	// �n���h���̎��
	CHandle::HandleAngle m_moveAngle = CHandle::HandleAngle::ANGLE_X;	// �ړ��̌���

	CMap_ObstacleManager::SObstacleInfo m_ObstacleInfo;	// ��Q�����

};

//------------------------------------
// ��Q���G�f�B�^�\�̔���
//------------------------------------
class CEdit_Obstacle_Collider : public CEdit_Obstacle_Super
{
public:

	CEdit_Obstacle_Collider() : CEdit_Obstacle_Super() {}
	~CEdit_Obstacle_Collider() {}

	void Init() override;	// ������
	void Update() override; // �X�V

	// �I������
	void Uninit() override
	{
		for (const auto& obj : m_pObjX)
		{
			obj->Kill();
		}
		m_pObjX.clear();

		CEdit_Obstacle_Super::Uninit();
	}
	void CreateBoxLine() override;	// �����蔻��{�b�N�X����

private:

	//=============================
	// �����o�֐�
	//=============================
	void Resize();	// ���T�C�Y
	void MenuBar();	// ���j���[�o�[

	//=============================
	// �����o�ϐ�
	//=============================
	int m_nEditIdx = 0;			// ��������C���f�b�N�X
	int m_nColliderIdx = 0;		// ��������R���C�_�[�̃C���f�b�N�X
	std::vector<CObjectX*> m_pObjX;				// �I�u�W�F�N�gX�̃|�C���^

};


#endif