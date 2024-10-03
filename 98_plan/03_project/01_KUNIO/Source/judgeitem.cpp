//=============================================================================
// 
//  �W���b�W�A�C�e������ [judgeitem.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "judgeitem.h"
#include "judgeitemManager.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"
#include "spline.h"
#include "course.h"
#include "baggage.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* MODEL = "data\\MODEL\\coin\\coin.x";	// ���f���p�X
	const float TIME_ROTATION = 2.0f;	// ��]���邽�߂̎���
}

namespace StateTime
{
	const float SWING = 2.0f;	// �����
	const float ROTATE = 0.6f;	// ��]
}

//==========================================================================
// �ÓI�����o�ϐ�
//==========================================================================
CListManager<CJudgeItem> CJudgeItem::m_List = {};	// ���X�g
std::map<int, CListManager<CJudgeItem>> CJudgeItem::m_ListBlock = {};	// �u���b�N�P�ʃ��X�g

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CJudgeItem::CJudgeItem(int nPriority) : CObjectX(nPriority)
{
	// �l�̃N���A
	m_pMgr = nullptr;	// �}�l�[�W��
	m_fLength = 0.0f;
	m_nMyBlockIdx = 0;		// �u���b�N�C���f�b�N�X
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CJudgeItem::~CJudgeItem()
{

}

//==========================================================================
// ��������
//==========================================================================
CJudgeItem* CJudgeItem::Create(CJudgeItemManager* pMgr, int mapBlock, const float length, const float height)
{
	// �������̊m��
	CJudgeItem* pObj = DEBUG_NEW CJudgeItem;

	if (pObj != nullptr)
	{
		// �������
		pObj->m_pMgr = pMgr;
		pObj->m_nMyBlockIdx = mapBlock;
		pObj->m_fHeight = height;
		pObj->SetLength(length);

		// ����������
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CJudgeItem::Init()
{
	// ���X�g�ɒǉ�
	m_List.Regist(this);
	m_ListBlock[m_nMyBlockIdx].Regist(this);

	// ��ނ̐ݒ�
	CObject::SetType(TYPE_OBJECTX);

	// ����������
	HRESULT hr = CObjectX::Init(MODEL);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CJudgeItem::Uninit()
{
	// ���X�g����폜
	m_List.Delete(this);
	m_ListBlock[m_nMyBlockIdx].Delete(this);

	// �I������
	CObjectX::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CJudgeItem::Kill()
{
	// �}�l�[�W�����폜
	m_pMgr->DeleteJudgeItem(this);

	// ���X�g����폜
	m_List.Delete(this);
	m_ListBlock[m_nMyBlockIdx].Delete(this);

	// �I������
	CObjectX::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CJudgeItem::Update()
{
	MyLib::Vector3 rot = GetRotation();

	// ��]
	rot.y += ((D3DX_PI * 2.0f) * (1.0f / TIME_ROTATION)) * CManager::GetInstance()->GetDeltaTime();
	UtilFunc::Transformation::RotNormalize(rot.y);

	SetRotation(rot);
}

//==========================================================================
// �擾
//==========================================================================
void CJudgeItem::Get()
{
	// �폜
	Kill();

	// �T�E���h����
	CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_COIN);

	// �G�t�F�N�g����
	for (int i = 0; i < 8; i++)
	{
		// �ړ���
		float randmoveZ = UtilFunc::Transformation::Random(-45, 45) * 1.0f;
		float randmoveX = UtilFunc::Transformation::Random(-45, 45) * 1.0f;
		float randmoveY = UtilFunc::Transformation::Random(200, 350) * 0.1f;
		float randRadius = UtilFunc::Transformation::Random(-30, 30) * 0.1f;

		MyLib::Vector3 move;
		move.x = randmoveX;
		move.z = randmoveZ;
		move.y = randmoveY;

		// ���a
		float radius = 120.0f + randRadius;

		// �����ʒu�̂Ԃ�
		MyLib::Vector3 pos = GetPosition();
		pos.x += randmoveX * 2.0f;
		pos.z += randmoveZ * 2.0f;
		pos.y += randmoveY * 2.0f;

		CEffect3D* pEffect = CEffect3D::Create(
			pos,
			move,
			D3DXCOLOR(1.0f, 0.8f + UtilFunc::Transformation::Random(-100, 100) * 0.001f, 0.0f, 1.0f),
			radius,
			30,
			CEffect3D::MOVEEFFECT::MOVEEFFECT_GENSUI,
			CEffect3D::TYPE::TYPE_JUJI2);
		pEffect->SetEnableGravity();
		pEffect->SetGravityValue(0.8f);
	}
}

//==========================================================================
// �����ݒ�
//==========================================================================
void CJudgeItem::SetLength(const float length)
{
	// ������ݒ�
	m_fLength = length;

	// ���W��ݒ�
	MyLib::Vector3 pos = MySpline::GetSplinePosition_NonLoop(CGame::GetInstance()->GetCourse()->GetVecPosition(), m_fLength);
	pos.y += m_fHeight;
	SetPosition(pos);
}
