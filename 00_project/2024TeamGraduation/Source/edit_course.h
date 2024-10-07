//=============================================================================
// 
//  �R�[�X�G�f�B�^�\�w�b�_�[ [edit_course.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _EDIT_COURSE_H_
#define _EDIT_COURSE_H_	// ��d�C���N���[�h�h�~

#include "listmanager.h"
#include "objectX.h"
#include "edithandle.h"
#include "edit.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �R�[�X�G�f�B�^�\�N���X
class CEdit_Course : public CEdit
{
public:

	CEdit_Course();
	~CEdit_Course();
	
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

	//=============================
	// �����o�֐�
	//=============================
	void FileControl();			// �t�@�C������
	void ChangeLevel();			// ���x���ύX
	void TransCheckPoint();		// �`�F�b�N�|�C���g�ҏW
	void TransJudge();			// �W���b�W�ҏW
	void TransObstacle();		// ��Q���ҏW
	void ChangeEditCourse();	// �ҏW����R�[�X�ύX
	void SelectLine();		// ���C���I��
	void DragLine();		// ���C���͂�
	void Transform();		// �g�����X�t�H�[��
	void ChangeLineNum();	// �ӂ̐��ύX
	void ChangeMode();		// ���[�h�ύX
	void TransformBeginEnd();	// �ŏ��ƍŌ�ό`
	void TransformBeginEnd(std::vector<MyLib::Vector3>* pVecpos);	// �ŏ��ƍŌ�ό`
	void AddPoint();		// �n�_�ǉ�

	void ObstacleEdit();	// ��Q���G�f�B�b�g
	void ResetObstacle();	// ��Q�����Z�b�g
	void SaveObstacle();	// ��Q���Z�[�u

	void MapEdit();		// �}�b�v�G�f�B�b�g
	void ResetMap();	// �}�b�v���Z�b�g
	void SaveMap();		// �}�b�v�Z�[�u

	void WaterStoneEdit();	// ������G�f�B�b�g
	void ResetWaterStone();	// �����⃊�Z�b�g
	void SaveWaterStone();	// ������Z�[�u


	//=============================
	// �����o�ϐ�
	//=============================
	LineData m_lineData;	// �ӏ��
	int m_nCourseEditIdx;	// ���삷��C���f�b�N�X�ԍ�
	int m_nCheckPointEditIdx;	// ���삷��C���f�b�N�X�ԍ�
	int m_nJudgeEditIdx;	// ���삷��C���f�b�N�X�ԍ�
	int m_nJudgeObjEditIdx;	// ���삷��C���f�b�N�X�ԍ�
	int m_nObstacleEditIdx;	// ���삷��C���f�b�N�X�ԍ�
	int m_nVtxEditIdx;		// ���삷��C���f�b�N�X�ԍ�
	bool m_bEdit;			// ���쒆����
	bool m_bDrag;			// �͂ݔ���
	bool m_bHoverWindow;	// �}�E�X�̃E�B���h�E�z�o�[����
	bool m_bSetMode;		// �ݒ胂�[�h����
	bool m_bAutoCreateMode;	// ������������
	CEdit* m_pEditObstacle;	// ��Q���G�f�B�^�[
	CEdit* m_pEditMap;		// �}�b�v�G�f�B�^�[
	CEdit* m_pEditWaterStone;	// ������G�f�B�^�[
};


#endif