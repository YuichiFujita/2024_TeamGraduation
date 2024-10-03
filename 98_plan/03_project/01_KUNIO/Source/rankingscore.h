//=============================================================================
// 
//  �����L���O�X�R�A�w�b�_�[ [rankingscore.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _RANKINGSCORE_H_
#define _RANKINGSCORE_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define RANKINGSCORE_DIGIT	(6)	// ����
#define RANKINGNUM	(5)	// ����

//==========================================================================
// �O���錾
//==========================================================================
class CMultiNumber;

//==========================================================================
// �N���X��`
//==========================================================================
// �����L���O�X�R�A�N���X��`
class CRankingScore : public CObject
{
public:

	CRankingScore(int nPriority = 8);
	~CRankingScore();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	void SetAllArrival();	// �S�Ă̓����ݒ�
	void UpdateNewRecord();	// �j���[���R�[�h�̍X�V����
	static CRankingScore *Create();
private:

	// �񋓌^��`
	enum VTX
	{
		VTX_NUM = 0,		// ���U���g���S
		VTX_LOGO,
		VTX_MAX
	};

	void Load();	// ���[�h
	void Save();	// �Z�[�u
	void Sort();	// �\�[�g
	void Moving(int nCntRanking);	// �ړ�����
	
	int m_nNumRanking;				// �����L���O��
	int m_nScore[RANKINGNUM];		// �X�R�A
	int m_nNowScore;				// ����̃X�R�A
	int m_nTexIdx[VTX_MAX];					// �e�N�X�`���̃C���f�b�N�X�ԍ�
	int m_nTexIdxNumber;			// �����e�N�X�`���̃C���f�b�N�X�ԍ�
	int m_nIdxNewRecord;			// �j���[���R�[�h�̃C���f�b�N�X�ԍ�
	int m_nCntNewRecord;			// �j���[���R�[�h�̃J�E���^�[
	float m_fPosDestX[RANKINGNUM];	// �ڕW�̈ʒu
	bool m_bNewRecord;				// �j���[���R�[�h�̃t���O
	bool m_bArrival[RANKINGNUM];	// ��������
	CObject2D *m_pObj2D[VTX_MAX];	// �I�u�W�F�N�g2D�̃I�u�W�F�N�g
	CMultiNumber*m_pScore[RANKINGNUM];			// �����̃I�u�W�F�N�g
	static const char *m_apTextureFile[VTX_MAX];	// �e�N�X�`���̃t�@�C��
};



#endif