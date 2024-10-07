//=============================================================================
// 
//  �W���b�W�A�C�e���}�l�[�W���w�b�_�[ [judgeitemManager.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _JUDGEITEMMANAGER_H_
#define _JUDGEITEMMANAGER_H_	// ��d�C���N���[�h�h�~

#include "object.h"
#include "map_block.h"

class CJudgeItem;

//==========================================================================
// �N���X��`
//==========================================================================
// �W���b�W�A�C�e���}�l�[�W���N���X
class CJudgeItemManager : public CObject
{
public:

	CJudgeItemManager();
	virtual ~CJudgeItemManager() {}

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	void Reset();	// ���Z�b�g
	void DeleteJudgeItem(CJudgeItem* pItem);	// �W���b�W�A�C�e���̍폜

	std::vector<CMapBlockInfo::SJudgeInfo> GetJudgeInfo() { return m_vecJudgeInfo; }	// �W���b�W���擾
	std::vector<CJudgeItem*> GetJudgeItem() { return m_vecJudgeItem; }					// �W���b�W�A�C�e��
	bool IsCheckCollision() { return m_bCheckCollision; }	// �R���W�����`�F�b�N����擾

	static CJudgeItemManager* Create(const std::vector<CMapBlockInfo::SJudgeInfo>& vecJudge, float startLen, int mapBlock);	// ����
	static CListManager<CJudgeItemManager> GetList() { return m_List; }	// ���X�g�擾
	static CListManager<CJudgeItemManager> GetListBlock(int mapblock) { return m_ListBlock[mapblock]; };	// ���X�g

protected:

	//=============================
	// �����o�֐�
	//=============================
	bool CheckCollisionRange();	// �R���W�����͈͊m�F
	void BindPoint();	// �|�C���g���f

	//=============================
	// �����o�ϐ�
	//=============================
	float m_fStartLength;	// �����n�_����
	MyLib::Vector3 m_Top;		// �擪
	MyLib::Vector3 m_Cur;		// �I�[
	bool m_bCheckCollision;	// �R���W�����`�F�b�N����
	bool m_bBindPoint;		// �|�C���g���f
	int m_nMyBlockIdx;		// �u���b�N�C���f�b�N�X
	std::vector<CMapBlockInfo::SJudgeInfo> m_vecJudgeInfo;	// �W���b�W���
	std::vector<CJudgeItem*> m_vecJudgeItem;		// �W���b�W�A�C�e��
	static CListManager<CJudgeItemManager> m_List;	// ���X�g
	static std::map<int, CListManager<CJudgeItemManager>> m_ListBlock;	// ���X�g


};

#endif