//=============================================================================
// 
//  ����]�[���}�l�[�W���w�b�_�[ [judgezoneManager.h]
//  Author : �Ό��D�n
// 
//=============================================================================
#ifndef _JUDGE_ZONE_MANAGER_H_
#define _JUDGE_ZONE_MANAGER_H_		// ��d�C���N���[�h�h�~
#include "listmanager.h"
#include "judgezone.h"
#include "judge.h"
#include <string>

//==========================================================================
// �N���X��`
//==========================================================================
// ����]�[���}�l�[�W���N���X
class CJudgeZoneManager
{
public:
	CJudgeZoneManager();
	~CJudgeZoneManager();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init();
	virtual void Uninit();

	//=============================
	// �����o�֐�
	//=============================
	void Check(float progress);
	void Release();		//��������������]�[����j��
	void ReleaseAll();	//����]�[�������ׂĔj��

	void Load(std::string path);
	void LoadZone(std::string path);
	CJudge::SJudgeCondition LoadCondition(std::string path);
	void Save(std::string path);
	void SaveZone(std::string path, CJudgeZone::SJudgeZone zoneinfo, CJudge::SJudgeCondition conditionUp, CJudge::SJudgeCondition conditionUnder);
	bool SaveCondition(std::string path, CJudge::SJudgeCondition condition);

	//=============================
	// �ÓI�֐�
	//=============================
	static CJudgeZoneManager* Create();
	static CJudgeZoneManager* GetInstance() { return m_ThisPtr; }	// �C���X�^���X�擾

private:
	static CJudgeZoneManager* m_ThisPtr;		// ���g�̃|�C���^
};

#endif