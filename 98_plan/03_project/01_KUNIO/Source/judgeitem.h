//=============================================================================
// 
//  �W���b�W�A�C�e�� [judgeitem.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _JUDGEITEM_H_
#define _JUDGEITEM_H_		// ��d�C���N���[�h�h�~

#include "objectX.h"
class CJudgeItemManager;

//==========================================================================
// �N���X��`
//==========================================================================
// �W���b�W�A�C�e���N���X
class CJudgeItem : public CObjectX
{
public:

	CJudgeItem(int nPriority = 6);
	~CJudgeItem();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;

	// �����o�֐�
	void Kill();		// �폜
	void Get();			// �擾
	float GetLength() { return m_fLength; }	// �����擾
	void SetLength(const float length);		// �����ݒ�


	//=============================
	// �ÓI�֐�
	//=============================
	static CJudgeItem* Create(CJudgeItemManager* pMgr, int mapBlock, const float length, const float height);
	static CListManager<CJudgeItem> GetList() { return m_List; }	// ���X�g�擾
	static CListManager<CJudgeItem> GetListBlock(int mapblock) { return m_ListBlock[mapblock]; }	// ���X�g�擾

private:

	//=============================
	// �����o�ϐ�
	//=============================
	float m_fLength;	// ����
	float m_fHeight;	// ����
	int m_nMyBlockIdx;		// �u���b�N�C���f�b�N�X
	CJudgeItemManager* m_pMgr;	// �}�l�[�W��
	static CListManager<CJudgeItem> m_List;
	static std::map<int, CListManager<CJudgeItem>> m_ListBlock;
};


#endif