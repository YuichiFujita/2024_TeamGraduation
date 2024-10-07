//=============================================================================
// 
//  �����L���O�}�l�[�W���w�b�_�[ [rankingmanager.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _RANKINGMANAGER_H_
#define _RANKINGMANAGER_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "constans.h"

// �O���錾

//==========================================================================
// �N���X��`
//==========================================================================
// �����L���O�}�l�[�W���N���X��`
class CRankingManager
{
public:

	CRankingManager();
	~CRankingManager();

	HRESULT Init();
	void Uninit();
	void Update();

	void Reset();				// ���Z�b�g
	void SetNowScore(int nScore);	// ����̃X�R�A�ݒ�
	int GetNowScore();			// ����̃X�R�A�擾
	static CRankingManager *Create();	// ��������
	
private:
	int m_nNowScore;	// ����̃X�R�A
};



#endif