//=============================================================================
// 
//  �����L���O�w�b�_�[ [ranking.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _RANKING_H_
#define _RANKING_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "manager.h"

//==========================================================================
// �O���錾
//==========================================================================
class CRankingScore;
class CRankingItem;
class CPeopleManager;
class CSpawnEnvironment;

//==========================================================================
// �N���X��`
//==========================================================================
// �����L���O�N���X��`
class CRanking : public CScene
{
public:

	struct SRankdata
	{
		int year;	// �N
		int month;	// ��
		int day;	// ��
		int minutes;		// ��
		int seconds;		// �b
		int milliSeconds;	// �~���b
		int allrank;
		bool rankin;

		// �R���X�g���N�^
		SRankdata() : year(0), month(0), day(0), minutes(0), seconds(0), milliSeconds(0), allrank(0), rankin(false) {}
		SRankdata(int _year, int _month, int _day, int _minnutes, int _seconds, int _milliseconds, int _allrank) : 
			year(_year), month(_month), day(_day), minutes(_minnutes), seconds(_seconds), milliSeconds(_milliseconds), allrank(_allrank), rankin(false) {}
	};

public:

	CRanking();
	~CRanking();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	static void SetNowData(SRankdata& nowdata) { m_NowData = nowdata; }
	static void SetEnableArrival();
private:

	void Load();
	void Save();
	void Sort();
	void RankIn();
	void CreateMap();	// �}�b�v����
	void ControllScroll(float deltaTime);	// �X�N���[���̑���

	static bool m_bAllArrival;		// �S�ē�����������
	static CRankingScore *m_pRankingScore;	// �����L���O�X�R�A�̃I�u�W�F�N�g
	CRankingItem* m_pRanking[10];
	SRankdata* m_pRankData;
	CPeopleManager* m_pPeopleManager;	// �l�}�l�[�W��
	CSpawnEnvironment* m_pSpawn_Air;	// ��C����
	CSpawnEnvironment* m_pSpawn_Leaf;	// �~��t����
	MyLib::Vector3 m_ScrollMove;		// �X�N���[���̈ړ���
	float m_fNoneScrollTimer;			// �X�N���[���Ȃ��̎���
	static SRankdata m_NowData;
};



#endif