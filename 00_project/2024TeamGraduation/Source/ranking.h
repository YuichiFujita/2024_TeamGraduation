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

private:

	void Load();
	void Save();
	void Sort();
	void CreateMap();	// �}�b�v����

};



#endif