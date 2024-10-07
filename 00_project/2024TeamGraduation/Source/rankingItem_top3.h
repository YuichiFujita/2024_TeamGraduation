//=============================================================================
// 
//  �����L���O2D_TOP3�w�b�_�[[rankinfItem_top3.h]
//  Author : ���������Y
// 
//=============================================================================

#ifndef _RANKINGITEM_TOP3_H_
#define _RANKINGITEM_TOP3_H_	// ��d�C���N���[�h�h�~

#include "rankingItem.h"

//==========================================================================
// �N���X��`
//==========================================================================
// ��ʃN���X
class CRankingItem_top3 : public CRankingItem
{
public:

	//�񋓌^
	typedef enum Items_TOP3
	{
		ITEM_NUM_RANK = 0,
		ITEM_ALLRANK,
		ITEM_TIME,
		ITEM_DATE,
		ITEM_ALLRANK_DATA,
		ITEM_YOU,
		ITEM_MAX
	};

	CRankingItem_top3(int nPriority = 11);
	~CRankingItem_top3();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Kill();	// �폜
	MyLib::Vector3 GetPos() override;
	void SetMove(MyLib::Vector3 move) override;	//�����L���O�A�C�e���̈ړ��l�Z�b�g
	static CRankingItem_top3* Create(int nNumRank, int nMin_Time, int nSec_Time, int nMilliSec_Time,int nAllRank,int nDate[3], MyLib::Vector3 posAll,float fSize, bool bNewRecord);	// ��������

private:
	//�\����
	typedef struct Time
	{
		int nMinutes;
		int nSeconds;
		int nMilliSeconds;
	};
	
	/*void SetScoreData()*/
	//=============================
	// �����o�ϐ�
	//=============================
	float m_fStateTime;
	CObject2D* m_pItem[ITEM_MAX];	//���ڐ�
	CObject2D* m_pScoreItem[8];		//�X�R�A�̌���obj
	CObject2D* m_pDateItem[10];		//���t�f�[�^obj
	int m_nNumRank;					//����
	Time m_nTime;					//�^�C��

	int m_nDate[3];					//���t�f�[�^
	
	int m_nAllRank;					//���]�̒l
	static int m_nObjNum;			//���I�u�W�F�N�g�̐�����

	MyLib::Vector3 m_posAll;		//�S���ڂ̊�ʒu
	MyLib::Vector3 m_posItem[ITEM_MAX];//�e���ڂ̈ʒu
	MyLib::Vector3 m_moveAll;		//�ړ��l
	float m_fSize;					//�g�嗦

	bool m_bUpdate;					//�X�V���ꂽ�X�R�A���H
};


#endif