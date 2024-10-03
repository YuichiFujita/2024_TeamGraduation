//=============================================================================
// 
// �����L���O2D���� [rankingItem.cpp]
//  Author : ���������Y
// 
//=============================================================================
#include "rankingItem.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "gamemanager.h"
#include "game.h"
#include "game_pressenter.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	//���S���
	//const std::string TEXTURE_RESULT = "data\\TEXTURE\\result\\ninja_resultlogo.png";
	//const MyLib::Vector3 LOGO_POS = MyLib::Vector3(220.0f, 60.0f, 0.0f);
	//const D3DXVECTOR2 LOGO_SIZE = D3DXVECTOR2(200.0f, 50.0f);
	
	const MyLib::Vector3 ITEM_POS = MyLib::Vector3(250.0f,250.0f, 0.0f);
	const D3DXVECTOR2 ITEM_SIZE = D3DXVECTOR2(100.0f, 50.0f);
	const std::string TEXTURE_ITEM[CRankingItem::ITEM_MAX] = { "data\\TEXTURE\\result\\ranking_num00.png",
										"data\\TEXTURE\\result\\toatalrank.png",
										"data\\TEXTURE\\result\\num00.png",
										"data\\TEXTURE\\result\\rank00.png",
										"data\\TEXTURE\\result\\new_records.png"};

	const std::string TEXTURE_COLON = "data\\TEXTURE\\result\\num00_colon.png";
	const D3DXVECTOR2 SPRITE_SIZE = D3DXVECTOR2(50.0f, 50.0f);
	const int MAX_SCORE_DEGIT = 8;

	const float TIME_SET = 0.2f;
}
//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
int CRankingItem::m_nObjNum = NULL;	//���I�u�W�F�N�g�̐�����

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CRankingItem::CRankingItem(int nPriority) : CObject(nPriority)
{
	m_fStateTime = 0.0f;
	
	for (int nCnt = 0; nCnt < ITEM_MAX; nCnt++)
	{
		m_pItem[nCnt] = nullptr;
	}

	for (int nCnt = 0; nCnt < MAX_SCORE_DEGIT; nCnt++)
	{
		m_pScoreItem[nCnt] = nullptr;
	}

	//���I�u�W�F�N�g�̐������̏������Ɖ��Z����
	if (m_nObjNum == NULL)
	{
		m_nObjNum = 1;
	}
	else
	{
		m_nObjNum++;
	}
	
}
//==========================================================================
// �f�X�g���N�^
//==========================================================================
CRankingItem::~CRankingItem()
{

}

//==========================================================================
// ��������
//==========================================================================
CRankingItem* CRankingItem::Create(int nNumRank, int nMin_Time, int nSec_Time, int nMilliSec_Time, int nAllRank,MyLib::Vector3 posAll,float fSize, bool bNewRecord)
{
	// �������̊m��
	CRankingItem *pMarker = DEBUG_NEW CRankingItem;

	if (pMarker != nullptr)
	{
		//���ʂ��擾
		pMarker->m_nNumRank = nNumRank;

		//�^�C�����擾
		pMarker->m_nTime.nMinutes = nMin_Time;
		pMarker->m_nTime.nSeconds = nSec_Time;
		pMarker->m_nTime.nMilliSeconds = nMilliSec_Time;

		//���]���擾
		pMarker->m_nAllRank = nAllRank;

		//�V�L�^�����擾
		pMarker->m_bUpdate = bNewRecord;

		//�ʒu�E�T�C�Y�ݒ�
		pMarker->m_posAll = posAll;
		pMarker->m_fSize = fSize;

		// ����������
		pMarker->Init();
	}

	return pMarker;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CRankingItem::Init()
{
	//���ڂ̏���������
	MyLib::Vector3 difItemPos = MyLib::Vector3(-610.0f * m_fSize,15.0f * m_fSize,0.0f);
	MyLib::Vector3 difRankPos = MyLib::Vector3(0.0f, 100.0f * (m_nObjNum - 1), 0.0f);
	D3DXVECTOR2 persize = D3DXVECTOR2(0.5f, 0.5f);

	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{
		if (nCntItem < ITEM_TIME)
		{//��������OBJ�̏�����

			//Obj2D����
			m_pItem[nCntItem] = CObject2D::Create(GetPriority());

			//�e���ڂ̋������Z
			difItemPos += MyLib::Vector3(610.0f*m_fSize, -15.0f * m_fSize, 0.0f);

			m_posItem[nCntItem] = m_posAll + difItemPos;

			m_pItem[nCntItem]->SetPosition(m_posItem[nCntItem]);

			//�e�N�X�`�����蓖��
			int nIdxTex = CTexture::GetInstance()->Regist(TEXTURE_ITEM[nCntItem]);
			m_pItem[nCntItem]->BindTexture(nIdxTex);

			//�T�C�Y�ݒ�
			D3DXVECTOR2 size = ITEM_SIZE*m_fSize;

			if (nCntItem == 1)
			{//�k������
				size.x *= persize.x;
				size.y *= persize.y;
			}
			m_pItem[nCntItem]->SetSize(size);
			m_pItem[nCntItem]->SetSizeOrigin(size);

			//�^�C�v�ݒ�
			CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

			//�X�v���C�g����
			if (nCntItem == 0)
			{//���ʂ̒���
				D3DXVECTOR2 uvpos[4] = { D3DXVECTOR2(0.0f,m_nNumRank * 0.1f - 0.1f), D3DXVECTOR2(1.0f,m_nNumRank * 0.1f - 0.1f),  D3DXVECTOR2(0.0f,m_nNumRank * 0.1f),  D3DXVECTOR2(1.0f,m_nNumRank * 0.1f) };
				m_pItem[nCntItem]->SetTex(uvpos);
			}
		}
		else if (nCntItem == ITEM_TIME)
		{//�X�R�A�̏�����
			difItemPos = MyLib::Vector3(150.0f * m_fSize, 0.0f, 0.0f);
			int nDegitNum[MAX_SCORE_DEGIT + 1];

			m_posItem[nCntItem] = m_posAll + difItemPos;

			//�X�R�A�̂ݕʊǗ��̂���nullptr����
			m_pItem[nCntItem] = nullptr;

			//�X�R�A�������Ƃɕ���
			int nMinutes = m_nTime.nMinutes;
			for (int nCnt = 0, nDegitCnt = 8; nDegitCnt > 6; nDegitCnt--, nCnt++)
			{
				nDegitNum[nDegitCnt] = nMinutes / pow(10, 2 - nCnt - 1);
				nMinutes = nMinutes - pow(10, 2 - nCnt - 1) * nDegitNum[nDegitCnt];
			}

			nDegitNum[6] = 11;

			int nSeconds = m_nTime.nSeconds;
			for (int nCnt = 0, nDegitCnt = 5; nDegitCnt > 3; nDegitCnt--, nCnt++)
			{
				nDegitNum[nDegitCnt] = nSeconds / pow(10, 2 - nCnt - 1);
				nSeconds = nSeconds - pow(10, 2 - nCnt - 1) * nDegitNum[nDegitCnt];
			}

			nDegitNum[3] = 11;

			int nMilliSeconds = m_nTime.nMilliSeconds;
			for (int nCnt = 0, nDegitCnt = 2; nDegitCnt > 0; nDegitCnt--, nCnt++)
			{
				nDegitNum[nDegitCnt] = nMilliSeconds / pow(10, 2 - nCnt - 1);
				nMilliSeconds = nMilliSeconds - pow(10, 2 - nCnt - 1) * nDegitNum[nDegitCnt];
			}

			//�e���̏�����
			for (int nCnt = 0, nDegitCnt = MAX_SCORE_DEGIT; nDegitCnt > 0; nDegitCnt--, nCnt++)
			{
				MyLib::Vector3 difDegitPos = MyLib::Vector3(50.0f * nCnt * m_fSize, 0.0f, 0.0f);
				m_pScoreItem[nDegitCnt] = CObject2D::Create(GetPriority());

				m_pScoreItem[nDegitCnt]->SetPosition(m_posItem[nCntItem] + difDegitPos);

				if (nDegitNum[nDegitCnt] == 11)
				{
					int nIdxTex = CTexture::GetInstance()->Regist(TEXTURE_COLON);
					m_pScoreItem[nDegitCnt]->BindTexture(nIdxTex);
				}
				else
				{
					int nIdxTex = CTexture::GetInstance()->Regist(TEXTURE_ITEM[nCntItem]);
					m_pScoreItem[nDegitCnt]->BindTexture(nIdxTex);
				}

				D3DXVECTOR2 size = SPRITE_SIZE * m_fSize;

				m_pScoreItem[nDegitCnt]->SetSize(size);
				m_pScoreItem[nDegitCnt]->SetSizeOrigin(size);

				CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

				if (nDegitNum[nDegitCnt] != 11)
				{
					D3DXVECTOR2 uvpos[4] = { D3DXVECTOR2(nDegitNum[nDegitCnt] * 0.1f,0.0f), D3DXVECTOR2(nDegitNum[nDegitCnt] * 0.1f + 0.1f,0.0f),  D3DXVECTOR2(nDegitNum[nDegitCnt] * 0.1f ,1.0f),  D3DXVECTOR2(nDegitNum[nDegitCnt] * 0.1f + 0.1f,1.0f) };
					m_pScoreItem[nDegitCnt]->SetTex(uvpos);
				}
			}
		}

		else if (nCntItem == ITEM_ALLRANK_DATA)
		{//���]�����N�A�C�R���̏�����
			m_pItem[nCntItem] = CObject2D::Create(GetPriority());

			difItemPos += MyLib::Vector3(575.0f * m_fSize, 0.0f, 0.0f);

			m_posItem[nCntItem] = m_posAll + difItemPos;

			m_pItem[nCntItem]->SetPosition(m_posItem[nCntItem]);

			int nIdxTex = CTexture::GetInstance()->Regist(TEXTURE_ITEM[nCntItem]);
			m_pItem[nCntItem]->BindTexture(nIdxTex);

			D3DXVECTOR2 size = SPRITE_SIZE*m_fSize;

			m_pItem[nCntItem]->SetSize(size);
			m_pItem[nCntItem]->SetSizeOrigin(size);

			CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);
			
			D3DXVECTOR2 uvpos[4] = { D3DXVECTOR2(0.0f,m_nAllRank * 0.25f), D3DXVECTOR2(1.0f,m_nAllRank * 0.25f),  D3DXVECTOR2(0.0f,m_nAllRank * 0.25f+0.25f),  D3DXVECTOR2(1.0f,m_nAllRank * 0.25f+0.25f) };
			m_pItem[nCntItem]->SetTex(uvpos);
		}
		else if (nCntItem == ITEM_YOU)
		{//YOU�A�C�R���̏�����
			if (m_bUpdate == true)
			{
				m_pItem[nCntItem] = CObject2D::Create(GetPriority());

				difItemPos = MyLib::Vector3(600.0f * m_fSize, 55.0f*m_fSize, 0.0f);

				m_posItem[nCntItem] = m_posAll + difItemPos;

				m_pItem[nCntItem]->SetPosition(m_posItem[nCntItem]);

				int nIdxTex = CTexture::GetInstance()->Regist(TEXTURE_ITEM[nCntItem]);
				m_pItem[nCntItem]->BindTexture(nIdxTex);

				D3DXVECTOR2 size = ITEM_SIZE * m_fSize;

				size.x = size.x * 1.8f;
				size.y = size.y * 0.6f;

				m_pItem[nCntItem]->SetSize(size);
				m_pItem[nCntItem]->SetSizeOrigin(size);

				CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);
			}
			else
			{
				m_pItem[nCntItem] = nullptr;
			}
		}
	}
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CRankingItem::Uninit()
{

	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{
		if (m_pItem[nCntItem]!=nullptr)
		{
			m_pItem[nCntItem]->Uninit();
			m_pItem[nCntItem] = nullptr;
		}
	}

	for (int nCntScoreDegit = 0; nCntScoreDegit <= MAX_SCORE_DEGIT; nCntScoreDegit++)
	{
		if (m_pScoreItem[nCntScoreDegit] != nullptr)
		{
			m_pScoreItem[nCntScoreDegit]->Uninit();
			m_pScoreItem[nCntScoreDegit] = nullptr;
		}
	}

	Release();
}

//==========================================================================
// �폜
//==========================================================================
void CRankingItem::Kill()
{

	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{
		if (m_pItem[nCntItem] != nullptr)
		{
			m_pItem[nCntItem]->Uninit();
			m_pItem[nCntItem] = nullptr;
		}
	}

	for (int nCntScoreDegit = 0; nCntScoreDegit <= MAX_SCORE_DEGIT; nCntScoreDegit++)
	{
		if (m_pScoreItem[nCntScoreDegit] != nullptr)
		{
			m_pScoreItem[nCntScoreDegit]->Uninit();
			m_pScoreItem[nCntScoreDegit] = nullptr;
		}
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CRankingItem::Update()
{
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	//���ڂ̍X�V
	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{
		if (m_pItem[nCntItem] != nullptr)
		{
			D3DXVECTOR2 size = m_pItem[nCntItem]->GetSize();
			size.x = UtilFunc::Correction::EasingEaseInOut(ITEM_SIZE.x * 10.0f, ITEM_SIZE.x, 0.0f, TIME_SET, m_fStateTime);
			size.y = UtilFunc::Correction::EasingEaseInOut(ITEM_SIZE.y * 10.0f, ITEM_SIZE.y, 0.0f, TIME_SET, m_fStateTime);
			m_pItem[nCntItem]->SetSize(size);

			D3DXVECTOR2 sizetext = m_pItem[nCntItem]->GetSize();
			D3DXVECTOR2 sizetextOR = m_pItem[nCntItem]->GetSizeOrigin();
			sizetext.x = UtilFunc::Correction::EasingEaseInOut(sizetextOR.x * 10.0f, sizetextOR.x, 0.0f, TIME_SET, m_fStateTime);
			sizetext.y = UtilFunc::Correction::EasingEaseInOut(sizetextOR.y * 10.0f, sizetextOR.y, 0.0f, TIME_SET, m_fStateTime);
			m_pItem[nCntItem]->SetSize(sizetext);

			//�ʒu�̍X�V
			m_posItem[nCntItem] += m_moveAll;
			m_pItem[nCntItem]->SetPosition(m_posItem[nCntItem]);

			m_pItem[nCntItem]->Update();
		}
		if (nCntItem == ITEM_TIME)
		{
			//�ʒu�̍X�V
			m_posItem[nCntItem] += m_moveAll;

			//�e���̏�����
			for (int nCnt = 0, nDegitCnt = MAX_SCORE_DEGIT; nDegitCnt > 0; nDegitCnt--, nCnt++)
			{
				MyLib::Vector3 difDegitPos = MyLib::Vector3(50.0f * nCnt, 0.0f, 0.0f);

				m_pScoreItem[nDegitCnt]->SetPosition(m_posItem[nCntItem] + difDegitPos);
				m_pScoreItem[nDegitCnt]->Update();
			}
		}
		
	}
	m_posAll += m_moveAll;
}
//==========================================================================
// �`�揈��
//==========================================================================
void CRankingItem::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// �I�u�W�F�N�g2D�̕`��

	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{
		if (m_pItem[nCntItem] != nullptr)
		{
			m_pItem[nCntItem]->Draw();
		}
	}

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}
//==========================================================================
// �ړ��l�Z�b�g����
//==========================================================================
void CRankingItem::SetMove(MyLib::Vector3 move)
{
	m_moveAll = move;
}
//==========================================================================
// �ړ��l�Z�b�g����
//==========================================================================
MyLib::Vector3 CRankingItem::GetPos()
{
	return m_posAll;
}