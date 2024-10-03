//=============================================================================
// 
//  ����]�[���G�f�B�^�\�w�b�_�[ [edit_judgezone.h]
//  Author : �Ό��D�n
// 
//=============================================================================

#ifndef _EDIT_JUDGEZONE_H_
#define _EDIT_JUDGEZONE_H_	// ��d�C���N���[�h�h�~

#include "listmanager.h"
#include "objectX.h"
#include "edithandle.h"
#include "edit.h"
#include "judgezone.h"
#include "judge.h"
#include <map>

//==========================================================================
// �N���X��`
//==========================================================================
// �R�[�X�G�f�B�^�\�N���X
class CEdit_JudgeZone : public CEdit
{
public:

	CEdit_JudgeZone();
	~CEdit_JudgeZone();
	
	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

private:
	//=============================
	// ����]�[���f�[�^�\���̒�`
	//=============================
	struct SJudgeZoneData
	{
		CJudgeZone::SJudgeZone zone;	//����]�[��
		CJudge::SJudgeCondition conditionTop, conditionUnder;	//�������
	};

	// �����o�֐�
	void SelectZone();		// �]�[���I��
	void FileControl();		// �t�@�C������
	void SetJudgeZone();	// ����]�[���ݒ�
	void SetCondition(CJudge::SJudgeCondition& condition,bool* pEnable, CJudge::CONDITIONTYPE& conditionType);
	OPENFILENAMEA CreateOFN(std::string relativeDir, char* filePass);

	CJudgeZone* m_pSelectZone;
	SJudgeZoneData m_JudgeZoneData;			// ����]�[�����
	CJudge::CONDITIONTYPE m_aConditionType[CJudge::BORDER::MAX];	// �������
	bool m_bHoverWindow;					// �}�E�X�̃E�B���h�E�z�o�[����
	bool m_abEnable[CJudge::BORDER::MAX][CJudge::JUDGE::JUDGE_MAX];
};


#endif