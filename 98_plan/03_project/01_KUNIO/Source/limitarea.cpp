//=============================================================================
// 
//  �G���A�������� [limitarea.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "limitarea.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "calculation.h"
#include "debugproc.h"
#include "player.h"
#include "game.h"

//==========================================================================
// �����l�[���X�y�[�X
//==========================================================================
namespace
{
	const char* TEXTURE_DEFAULT = "data\\TEXTURE\\cyberwall_03.png";	// �f�t�H���g�e�N�X�`��
	const float MAX_ALPHA = 0.3f;	// �ő�s�����x
	const float TIME_APPEARANCE = 0.5f;	// �o������
	const float TIME_FADEOUT = 0.5f;	// �t�F�[�h�A�E�g����
}

//==========================================================================
// �֐����X�g
//==========================================================================
CLimitArea::STATE_FUNC CLimitArea::m_StateFuncList[] =
{
	&CLimitArea::StateNone,			// �ʏ�
	&CLimitArea::StateAppearance,	// �o�����
	&CLimitArea::StateFadeout,		// �t�F�[�h�A�E�g
};

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CListManager<CLimitArea> CLimitArea::m_List = {};	// ���X�g

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CLimitArea::CLimitArea(int nPriority) : CObject(nPriority)
{
	m_state = STATE_NONE;			// ���
	m_fTimeState = 0.0f;			// ��ԃJ�E���^�[
	memset(&m_pMeshWall[0], 0, sizeof(m_pMeshWall));			// ���b�V���E�H�[���̃I�u�W�F�N�g
	memset(&m_sLimitEreaInfo, 0, sizeof(m_sLimitEreaInfo));	// �G���A�������
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CLimitArea::~CLimitArea()
{

}

//==========================================================================
// ��������
//==========================================================================
CLimitArea *CLimitArea::Create(sLimitEreaInfo info)
{
	// �����p�̃I�u�W�F�N�g
	CLimitArea *pObjMeshField = nullptr;

	if (pObjMeshField == nullptr)
	{// nullptr��������

		// �������̊m��
		pObjMeshField = DEBUG_NEW CLimitArea;

		if (pObjMeshField != nullptr)
		{// �������̊m�ۂ��o���Ă�����

			// �G���A�������
			pObjMeshField->m_sLimitEreaInfo = info;

			// ����������
			pObjMeshField->Init();
		}

		return pObjMeshField;
	}

	return nullptr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CLimitArea::Init()
{

	// ��ސݒ�
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	// ���蓖��
	m_List.Regist(this);

	// �e��ϐ�������
	MyLib::Vector3 WallPos[mylib_const::SHAPE_LIMITEREA];
	float fWidthLen[mylib_const::SHAPE_LIMITEREA];

	// ��
	WallPos[WALL_UP] = MyLib::Vector3(
		(m_sLimitEreaInfo.fMinX + m_sLimitEreaInfo.fMaxX) * 0.5f,
		0.0f,
		m_sLimitEreaInfo.fMaxZ);
	fWidthLen[WALL_UP] = UtilFunc::Calculation::GetPosLength3D(MyLib::Vector3(m_sLimitEreaInfo.fMinX, 0.0f, 0.0f), MyLib::Vector3(m_sLimitEreaInfo.fMaxX, 0.0f, 0.0f));

	// �E
	WallPos[WALL_RIGHT] = MyLib::Vector3(
		m_sLimitEreaInfo.fMinX,
		0.0f,
		(m_sLimitEreaInfo.fMinZ + m_sLimitEreaInfo.fMaxZ) * 0.5f);
	fWidthLen[WALL_RIGHT] = UtilFunc::Calculation::GetPosLength3D(MyLib::Vector3(m_sLimitEreaInfo.fMinZ, 0.0f, 0.0f), MyLib::Vector3(m_sLimitEreaInfo.fMaxZ, 0.0f, 0.0f));

	// ��O
	WallPos[WALL_DOWN] = MyLib::Vector3(
		(m_sLimitEreaInfo.fMinX + m_sLimitEreaInfo.fMaxX) * 0.5f,
		0.0f,
		m_sLimitEreaInfo.fMinZ);
	fWidthLen[WALL_DOWN] = UtilFunc::Calculation::GetPosLength3D(MyLib::Vector3(m_sLimitEreaInfo.fMinX, 0.0f, 0.0f), MyLib::Vector3(m_sLimitEreaInfo.fMaxX, 0.0f, 0.0f));

	// ��
	WallPos[WALL_LEFT] = MyLib::Vector3(
		m_sLimitEreaInfo.fMaxX,
		0.0f,
		(m_sLimitEreaInfo.fMinZ + m_sLimitEreaInfo.fMaxZ) * 0.5f);
	fWidthLen[WALL_LEFT] = UtilFunc::Calculation::GetPosLength3D(MyLib::Vector3(m_sLimitEreaInfo.fMinZ, 0.0f, 0.0f), MyLib::Vector3(m_sLimitEreaInfo.fMaxZ, 0.0f, 0.0f));

	float fRot = (D3DX_PI * 2.0f) / (float)mylib_const::SHAPE_LIMITEREA;
	int nBlock = 4;
	for (int i = 0; i < mylib_const::SHAPE_LIMITEREA; i++)
	{
		m_pMeshWall[i] = CMeshWall::Create(WallPos[i], MyLib::Vector3(0.0f, D3DX_PI + fRot * i, 0.0f), fWidthLen[i] / (float)nBlock, fWidthLen[i] / (float)nBlock, nBlock, 1, 7, TEXTURE_DEFAULT);
		m_pMeshWall[i]->SetType(CObject::TYPE_NONE);
	}

	// �o�����
	m_state = STATE_APPEARANCE;

	return E_FAIL;
}

//==========================================================================
// �I������
//==========================================================================
void CLimitArea::Uninit()
{
	// �폜
	m_List.Delete(this);

	for (int i = 0; i < mylib_const::SHAPE_LIMITEREA; i++)
	{
		if (m_pMeshWall[i] == nullptr)
		{
			continue;
		}
		m_pMeshWall[i]->Uninit();
		m_pMeshWall[i] = nullptr;
	}

	// �������
	CObject::Release();
}

//==========================================================================
// �폜����
//==========================================================================
void CLimitArea::Kill()
{
	// �폜
	m_List.Delete(this);

	for (int i = 0; i < mylib_const::SHAPE_LIMITEREA; i++)
	{
		if (m_pMeshWall[i] == nullptr)
		{
			continue;
		}
		m_pMeshWall[i]->Uninit();
		m_pMeshWall[i] = nullptr;
	}

	// �������
	CObject::Release();
}

//==========================================================================
// �X�V����
//==========================================================================
void CLimitArea::Update()
{
	// ��ԍX�V
	(this->*(m_StateFuncList[m_state]))();

	for (int i = 0; i < mylib_const::SHAPE_LIMITEREA; i++)
	{
		if (m_pMeshWall[i] == nullptr)
		{
			continue;
		}

		m_pMeshWall[i]->Update();
	}
}

//==========================================================================
// �ʏ���
//==========================================================================
void CLimitArea::StateNone()
{
	// ��ԃJ�E���^�[���Z�b�g
	m_fTimeState = 0.0f;
}

//==========================================================================
// �o�����
//==========================================================================
void CLimitArea::StateAppearance()
{
	// ��ԃJ�E���^�[���Z
	m_fTimeState += CManager::GetInstance()->GetDeltaTime();

	// �s�����x
	float fAlpha =  MAX_ALPHA * (m_fTimeState / TIME_APPEARANCE);

	for (int i = 0; i < mylib_const::SHAPE_LIMITEREA; i++)
	{
		if (m_pMeshWall[i] == nullptr)
		{
			continue;
		}

		// ���_�J���[�擾
		D3DXCOLOR *pVtxCol = m_pMeshWall[i]->GetVtxCol();
		D3DXCOLOR newcolor = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);

		// �S�Ă̗v�f����������
		std::fill(pVtxCol, pVtxCol + m_pMeshWall[i]->GetNumVertex(), newcolor);
	}

	if (m_fTimeState >= TIME_APPEARANCE)
	{// �K��l�𒴂�����o����ԉ���
		m_state = STATE_NONE;
	}

}

//==========================================================================
// �t�F�[�h�A�E�g���
//==========================================================================
void CLimitArea::StateFadeout()
{
	// ��ԃJ�E���^�[���Z
	m_fTimeState += CManager::GetInstance()->GetDeltaTime();

	// �s�����x
	float fAlpha = MAX_ALPHA - (m_fTimeState / TIME_FADEOUT);

	for (int i = 0; i < mylib_const::SHAPE_LIMITEREA; i++)
	{
		if (m_pMeshWall[i] == nullptr)
		{
			continue;
		}

		// ���_�J���[�擾
		D3DXCOLOR *pVtxCol = m_pMeshWall[i]->GetVtxCol();
		D3DXCOLOR newcolor = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);

		// �S�Ă̗v�f����������
		std::fill(pVtxCol, pVtxCol + m_pMeshWall[i]->GetNumVertex(), newcolor);
	}

	if (m_fTimeState >= TIME_FADEOUT)
	{// �폜

		// �폜
		Kill();
		return;
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CLimitArea::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �w�ʂ̃J�����O�Ȃ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// �`�揈��
	for (int i = 0; i < mylib_const::SHAPE_LIMITEREA; i++)
	{
		if (m_pMeshWall[i] == nullptr || i == WALL_DOWN)
		{
			continue;
		}

		m_pMeshWall[i]->Draw();
	}

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// �J�����O�f�t�H���g
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//==========================================================================
// ���擾
//==========================================================================
CLimitArea::sLimitEreaInfo CLimitArea::GetLimitEreaInfo()
{
	return m_sLimitEreaInfo;
}

//==========================================================================
// ��Ԏ擾
//==========================================================================
CLimitArea::STATE CLimitArea::GetState()
{
	return m_state;
}

//==========================================================================
// ��Ԏ擾
//==========================================================================
void CLimitArea::SetState(STATE state)
{
	m_state = state;
}