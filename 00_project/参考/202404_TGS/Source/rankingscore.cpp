//=============================================================================
// 
//  �����L���O�X�R�A���� [rankingscore.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "rankingscore.h"
#include "manager.h"
#include "texture.h"
#include "renderer.h"
#include "object2D.h"
#include "multinumber.h"
#include "calculation.h"
#include "ranking.h"
#include "rankingmanager.h"
#include "sound.h"

//==========================================================================
// �}�N����`
//==========================================================================
#define RANKINGFILE			"data\\BIN\\ranking.bin"
#define NUMBERTEXTURE		"data\\TEXTURE\\number\\number_blackclover_01.png"	// �e�N�X�`���̃t�@�C��
#define TEX_U				(0.1f)				// U�̕���
#define BASE_WIDTH			(40.0f)		// ����
#define BASE_HEIGHT			(50.0f)		// �c��
#define BASE_DIS_X			(50.0f)		// �Ԋu
#define DIS_X				(50.0f)				// �Ԋu
#define POSY_BASE			(330.0f)			// Y�ʒu
#define BASETEXT_POSX		(180.0f)			// X�ʒu
#define TEXT_MOVETIME		(20)				// �ړ�����
#define INIT_POSX			(1600.0f)			// �����ʒu

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
const char *CRankingScore::m_apTextureFile[VTX_MAX] =		// �e�N�X�`���̃t�@�C��
{
	"data\\TEXTURE\\rankingnum_01.png",
	"data\\TEXTURE\\rankinglogo_01.png",
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CRankingScore::CRankingScore(int nPriority) : CObject(nPriority)
{
	// �l�̃N���A
	m_nNumRanking = 0;				// �����L���O��
	memset(&m_nScore[0], 0, sizeof(m_nScore));	// �X�R�A
	m_nNowScore = 0;				// ����̃X�R�A
	memset(&m_pObj2D[0], 0, sizeof(m_pObj2D));	// �I�u�W�F�N�g2D�̃I�u�W�F�N�g
	memset(&m_nTexIdx[0], 0, sizeof(m_nTexIdx));	// �e�N�X�`���̃C���f�b�N�X�ԍ�
	m_nTexIdxNumber = 0;			// �����e�N�X�`���̃C���f�b�N�X�ԍ�
	m_nIdxNewRecord = 0;			// �j���[���R�[�h�̃C���f�b�N�X�ԍ�
	m_nCntNewRecord = 0;			// �j���[���R�[�h�̃J�E���^�[
	memset(&m_fPosDestX[0], 0, sizeof(m_fPosDestX));	// �ڕW�̈ʒu
	m_bNewRecord = false;			// �j���[���R�[�h�̃t���O
	memset(&m_bArrival[0], 0, sizeof(m_bArrival));	// ��������
	memset(&m_pScore[0], 0, sizeof(m_pScore));	// �����̃I�u�W�F�N�g
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CRankingScore::~CRankingScore()
{

}

//==========================================================================
// ��������
//==========================================================================
CRankingScore *CRankingScore::Create()
{
	// �����p�̃I�u�W�F�N�g
	CRankingScore *pScore = nullptr;

	if (pScore == nullptr)
	{// nullptr��������

		// �������̊m��
		pScore = DEBUG_NEW CRankingScore;

		if (pScore != nullptr)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			pScore->Init();
		}

		return pScore;
	}

	return nullptr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CRankingScore::Init()
{
	// ��ސݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		// ��������
		m_pObj2D[nCntVtx] = CObject2D::Create(7);

		// �e�N�X�`���̊��蓖��
		m_nTexIdx[nCntVtx] = CTexture::GetInstance()->Regist(m_apTextureFile[nCntVtx]);

		// �e�N�X�`���̊��蓖��
		m_pObj2D[nCntVtx]->GetObject2D()->BindTexture(m_nTexIdx[nCntVtx]);

		// �e��ϐ��̏�����
		switch (nCntVtx)
		{
		case VTX_LOGO:
			m_pObj2D[nCntVtx]->GetObject2D()->SetSize(CTexture::GetInstance()->GetImageSize(m_nTexIdx[nCntVtx]) * 0.3f);	// �T�C�Y
			m_pObj2D[nCntVtx]->GetObject2D()->SetPosition(MyLib::Vector3(280.0f, 60.0f, 0.0f));	// �ʒu
			break;

		case VTX_NUM:
			m_pObj2D[nCntVtx]->GetObject2D()->SetSize(CTexture::GetInstance()->GetImageSize(m_nTexIdx[nCntVtx]) * 0.5f);	// �T�C�Y
			m_pObj2D[nCntVtx]->GetObject2D()->SetPosition(MyLib::Vector3(m_pObj2D[nCntVtx]->GetSize().x, 360.0f, 0.0f));	// �ʒu
			break;

		default:
			break;
		}

		// ��ނ̐ݒ�
		m_pObj2D[nCntVtx]->GetObject2D()->SetType(CObject::TYPE::TYPE_OBJECT2D);
	}

	for (int nCntRanking = 0; nCntRanking < RANKINGNUM; nCntRanking++)
	{
		// �ڕW�̈ʒu
		m_fPosDestX[nCntRanking] = m_pObj2D[VTX_LOGO]->GetSize().x;


		// ��������
		m_pScore[nCntRanking] = CMultiNumber::Create(
			MyLib::Vector3(INIT_POSX, nCntRanking * 120.0f + 170.0f, 0.0f),
			D3DXVECTOR2(BASE_WIDTH, BASE_WIDTH), 6,
			CNumber::OBJECTTYPE_2D,
			NUMBERTEXTURE, true, GetPriority());
	}

	// �����L���O�ǂݍ���
	Load();

	
		// ����̃X�R�A�擾
		m_nNowScore = CManager::GetInstance()->GetRankingManager()->GetNowScore();

		// �\�[�g����
		Sort();

		// �Z�[�u����
		Save();
	

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CRankingScore::Uninit()
{
	// �I������
	for (int nCntVtx = 0; nCntVtx < VTX_MAX; nCntVtx++)
	{
		if (m_pObj2D[nCntVtx] != nullptr)
		{// �������̊m�ۂ�����Ă�����

			// �I������
			m_pObj2D[nCntVtx] = nullptr;
		}
	}

	for (int nCntRanking = 0; nCntRanking < RANKINGNUM; nCntRanking++)
	{
		// �����̃I�u�W�F�N�g�̏I������
		if (m_pScore[nCntRanking] != nullptr)
		{
			m_pScore[nCntRanking]->Uninit();
			m_pScore[nCntRanking] = nullptr;
		}
	}

	// ���폜
	Release();
}

//==========================================================================
// �X�V����
//==========================================================================
void CRankingScore::Update()
{
	for (int nCntRanking = 0; nCntRanking < RANKINGNUM; nCntRanking++)
	{
		if (m_bArrival[nCntRanking] == false &&
			(nCntRanking == 0 || m_bArrival[nCntRanking - 1]))
		{
			// �ړ�����
			Moving(nCntRanking);
		}

		m_pScore[nCntRanking]->Update();
		m_pScore[nCntRanking]->SetValue(m_nScore[nCntRanking]);
	}

	if (m_bNewRecord)
	{// �L�^�X�V���Ă�����

		// �j���[���R�[�h�̍X�V����
		UpdateNewRecord();
	}

}

//==========================================================================
// �ړ�����
//==========================================================================
void CRankingScore::Moving(int nCntRanking)
{

	// �ʒu�擾
	MyLib::Vector3 pos = m_pScore[nCntRanking]->GetPosition();

	if (pos.x == INIT_POSX)
	{
		// �T�E���h�Đ�
		//CSound::GetInstance()->PlaySound(CSound::LABEL_SE_NUMBERMOVE);
	}

	// �F�擾
	D3DXCOLOR col = m_pScore[nCntRanking]->GetColor();

	float fDest = m_fPosDestX[nCntRanking];

	// �ڕW�̈ʒu�֕␳
	if (fDest < pos.x)
	{
		pos.x += (fDest - INIT_POSX) / (float)TEXT_MOVETIME;
		col.a += 1.0f / (float)TEXT_MOVETIME;
	}

	if (fDest >= pos.x)
	{// �ڕW�ŌŒ�
		pos.x = fDest;
		col.a = 1.0f;
		m_bArrival[nCntRanking] = true;

		if (nCntRanking == RANKINGNUM - 1)
		{
			CRanking::SetEnableArrival();
		}
	}

	// �ʒu�ݒ�
	m_pScore[nCntRanking]->SetPosition(pos);

	// �F�ݒ�
	m_pScore[nCntRanking]->SetColor(col);
}

//==========================================================================
// �j���[���R�[�h�̍X�V����
//==========================================================================
void CRankingScore::UpdateNewRecord()
{

	// �ʒu�擾
	MyLib::Vector3 pos = m_pScore[m_nIdxNewRecord]->GetPosition();

	// �T�C�Y�擾
	D3DXVECTOR2 size = m_pScore[m_nIdxNewRecord]->GetSize();

	// �F�擾
	D3DXCOLOR col = m_pScore[m_nIdxNewRecord]->GetColor();

	// �Ȑ��␳
	UtilFunc::Correction::CuadricCurveComp(col.a, 60, 0.3f, 1.0f, m_nCntNewRecord);

	// �ʒu�ݒ�
	m_pScore[m_nIdxNewRecord]->SetPosition(pos);

	// �T�C�Y�ݒ�
	m_pScore[m_nIdxNewRecord]->SetSize(size);

	// �F�ݒ�
	m_pScore[m_nIdxNewRecord]->SetColor(col);
}

//==========================================================================
// �S�Ă̓����ݒ�
//==========================================================================
void CRankingScore::SetAllArrival()
{

	for (int nCntScore = 0; nCntScore < RANKINGSCORE_DIGIT; nCntScore++)
	{
		m_bArrival[nCntScore] = true;	// ��������
	}

	for (int nCntRanking = 0; nCntRanking < RANKINGNUM; nCntRanking++)
	{
		
			// �ʒu�擾
			MyLib::Vector3 pos = m_pScore[nCntRanking]->GetPosition();

			if (pos.x == INIT_POSX)
			{
				// �T�E���h�Đ�
				//CSound::GetInstance()->PlaySound(CSound::LABEL_SE_NUMBERMOVE);
			}

			// �F�擾
			D3DXCOLOR col = m_pScore[nCntRanking]->GetColor();

			// �ړ�
			float fDest = m_fPosDestX[nCntRanking];
			pos.x = fDest;

			// �s�����x�ݒ�
			col.a = 1.0f;

			// �ʒu�ݒ�
			m_pScore[nCntRanking]->SetPosition(pos);

			// �F�ݒ�
			m_pScore[nCntRanking]->SetColor(col);
	}

}

//==========================================================================
// �`�揈��
//==========================================================================
void CRankingScore::Draw()
{

}

//==============================================================
// �\�[�g����
//==============================================================
void CRankingScore::Sort()
{
	// �~������
	for (int nCntData = 0; nCntData < m_nNumRanking - 1; nCntData++)
	{
		for (int nCntSort = nCntData + 1; nCntSort < m_nNumRanking; nCntSort++)
		{
			if (m_nScore[nCntData] < m_nScore[nCntSort])
			{// �v�f1���v�f2���傫��������

				int nTemp = m_nScore[nCntData];
				m_nScore[nCntData] = m_nScore[nCntSort];
				m_nScore[nCntSort] = nTemp;
			}
		}
	}

	// �����L���O�ő�l��������̂�������������
	if (m_nScore[m_nNumRanking - 1] < m_nNowScore)
	{// ����ւ�

		m_nScore[m_nNumRanking - 1] = m_nNowScore;	// �ŏ��l�ƍ���̃f�[�^����ւ�
		m_bNewRecord = true;	// �t���OON
	}

	// �~������
	for (int nCntData = 0; nCntData < m_nNumRanking - 1; nCntData++)
	{
		for (int nCntSort = nCntData + 1; nCntSort < m_nNumRanking; nCntSort++)
		{
			if (m_nScore[nCntData] < m_nScore[nCntSort])
			{// �v�f1���v�f2���傫��������

				int nTemp = m_nScore[nCntData];
				m_nScore[nCntData] = m_nScore[nCntSort];
				m_nScore[nCntSort] = nTemp;
			}
		}
	}

	if (m_bNewRecord)
	{// �j���[���R�[�h�̎�

		for (int nCount = RANKINGNUM; nCount > 0; nCount--)
		{// �v�f�ԍ��L�����Ĕ�����

			if (m_nScore[nCount - 1] == m_nNowScore)
			{
				m_nIdxNewRecord = nCount - 1;
				break;
			}
		}
	}
}

//==============================================================
// �Z�[�u����
//==============================================================
void CRankingScore::Save()
{
	FILE *pFile;

	// �t�@�C�����J��
	pFile = fopen(RANKINGFILE, "wb");

	if (pFile != nullptr)
	{// �t�@�C�����J�����ꍇ

		// �t�@�C���ɐ��l�������o��
		fwrite(&m_nNumRanking, sizeof(int), 1, pFile);
		fwrite(&m_nScore[0], sizeof(int), m_nNumRanking, pFile);

		// �t�@�C�������
		fclose(pFile);
	}
}

//==============================================================
// ���[�h����
//==============================================================
void CRankingScore::Load()
{
	FILE *pFile;

	// �t�@�C�����J��
	pFile = fopen(RANKINGFILE, "rb");

	if (pFile != nullptr)
	{// �t�@�C�����J�����ꍇ

		// �t�@�C���ɐ��l��ǂݍ���
		fread(&m_nNumRanking, sizeof(int), 1, pFile);
		fread(&m_nScore[0], sizeof(int), m_nNumRanking, pFile);

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{// �t�@�C�����J���Ȃ������ꍇ

		m_nNumRanking = RANKINGNUM;
		for (int nCount = 0; nCount < m_nNumRanking; nCount++)
		{// �S���[��
			m_nScore[nCount] = 500;
			//m_nScore[nCount] = 150000 - nCount * 25000;
		}

		// �Z�[�u
		Save();
	}
}