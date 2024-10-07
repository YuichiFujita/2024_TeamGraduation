//=============================================================================
// 
//  �A�[�`��̋���Q������(���̂Ȃ�) [obstacle_fisharch.cpp]
//  Author : Ibuki Okusada
// 
//=============================================================================
#include "obstacle_fisharch.h"
#include "debugproc.h"
#include "manager.h"
#include "baggage.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const int NUM_FISH = (10);				// ���̑���
	const float FISH_ROT = (1.0f / NUM_FISH);	// 1���ӂ�̊p�x����
	const int RANGE_MOTION = 2;					// ���[�V��������
}

// �f�t�H���g���
namespace FISHDEFAULT
{
	const float ROTATE_SPEED = (-0.02f);		// ��]���x
	const float WIDTH = (750.0f);			// ���S����̋���
	const float HEIGHT = (1500.0f);			// �ω����鋗��
	const float PLUS_HEIGHT = (0.0f);		// ���Z����
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CObstacle_FishArch::CObstacle_FishArch(int nPriority,
	CObject::LAYER layer) : CMap_Obstacle(nPriority, layer)
{
	// �l�̃N���A
	m_FishList.clear();
	m_fRot = 0.0f;
	m_scale = (1.0f);
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CObstacle_FishArch::~CObstacle_FishArch()
{

}

//==========================================================================
// ��������
//==========================================================================
CObstacle_FishArch* CObstacle_FishArch::Create(const CMap_ObstacleManager::SObstacleInfo& info)
{
	// �������̊m��
	CObstacle_FishArch* pObj = DEBUG_NEW CObstacle_FishArch;

	if (pObj != nullptr)
	{
		pObj->SetObstacleInfo(info);
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObstacle_FishArch::Init()
{
	// �����̃u���b�N�v�Z
	CMap_Obstacle::CalMyBlock();

	CMap_Obstacle::ListRegist(this);
	MyLib::Vector3 rot;

	// ��ނ̐ݒ�
	CObject::SetType(TYPE_OBJECTX);

	// ���𐶐�����
	for (int i = 0; i < NUM_FISH; i++)
	{
		FishInfo info;
		info.nIdx = i;
		info.pFish = CMap_Obstacle::Create(GetObstacleInfo(), GetPosition(), false, false);
		rot.x = (i * FISH_ROT) * (D3DX_PI * 2);
		UtilFunc::Transformation::RotNormalize(rot.x);
		info.pFish->SetRotation(rot);

		// ���̃I�t�Z�b�g�ݒ�
		SetFishOffSet(info);
		info.pFish->SetPosition(info.offset);
		//info.pFish->CalMyBlock();
		m_FishList.push_back(info);
	}

	// �����l�ݒ�
	m_Info.fDefHeight = FISHDEFAULT::HEIGHT;
	m_Info.fPlusHeight = FISHDEFAULT::PLUS_HEIGHT;
	m_Info.fRotSpeed = FISHDEFAULT::ROTATE_SPEED;
	m_Info.fNowHeight = FISHDEFAULT::HEIGHT;

	// ���C������
	MainControll();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CObstacle_FishArch::Uninit()
{
	for (auto& it : m_FishList)
	{
		if (it.pFish != nullptr)
		{
			it.pFish->Uninit();
			it.pFish = nullptr;
		}
	}

	// ���X�g�N���A
	m_FishList.clear();

	// �I������
	CMap_Obstacle::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CObstacle_FishArch::Kill()
{
	for (auto& it : m_FishList)
	{
		if (it.pFish != nullptr)
		{
			it.pFish->Kill();
			it.pFish = nullptr;
		}
	}

	// ���X�g�N���A
	m_FishList.clear();

	// �I������
	CMap_Obstacle::Kill();
}

//==========================================================================
// �X�V����
//==========================================================================
void CObstacle_FishArch::Update()
{

#if _DEBUG
	// �}�g���b�N�X�ݒ�
	CalWorldMtx();
	CMap_Obstacle::Update();
#endif // _DEBUG

	// �ו��擾
	CBaggage* pBaggage = CBaggage::GetListObj().GetData(0);
	if (pBaggage == nullptr) return;

	// �ו��̃u���b�N�͈̔͊O�Ȃ甲����
	int baggageBlock = pBaggage->GetMapBlock();
	if (!(baggageBlock + RANGE_MOTION >= m_nMapBlock &&
		baggageBlock - RANGE_MOTION <= m_nMapBlock))
	{
		return;
	}

	// ���C������
	MainControll();
}

//==========================================================================
// ���C������
//==========================================================================
void CObstacle_FishArch::MainControll()
{
	// ��]
	m_fRot += m_Info.fRotSpeed;
	UtilFunc::Transformation::RotNormalize(m_fRot);

	// �����ݒ�
	SetNowHeight();

	// �}�g���b�N�X�ݒ�
	CalWorldMtx();

	// ���̍X�V
	ControllFish();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CObstacle_FishArch::Draw()
{
	
}

//==========================================================================
// ���̑���ݒ�
//==========================================================================
void CObstacle_FishArch::ControllFish()
{
	for (auto& it : m_FishList)
	{
		if (it.pFish == nullptr) continue;	// ���݂��Ă��Ȃ�

		MyLib::Vector3 rot = it.pFish->GetRotation();

		// �ʒu�̐ݒ�
		{
			// �ߋ��̈ʒu�ݒ�
			it.pFish->SetOldPosition(it.pFish->GetPosition());

			MyLib::Matrix mtxRot, mtxTrans, mtxScale, mtxFish;	// �v�Z�p�}�g���b�N�X�錾
			MyLib::Matrix mtxParent = m_mtxWorld;	// �e�̃}�g���b�N�X
			MyLib::Vector3 FishPos = it.offset;

			// ���W�p�}�g���b�N�X������
			mtxFish.Identity();

			// �ʒu�𔽉f����
			mtxTrans.Translation(FishPos);
			mtxFish.Multiply(mtxFish, mtxTrans);

			//// �����𔽉f����
			//mtxRot.RotationYawPitchRoll(GetRotation().y, GetRotation().x, GetRotation().z);
			//mtxFish.Multiply(mtxFish, mtxRot);

			mtxParent.Multiply(mtxFish, mtxParent);

			// ���W��ݒ�
			FishPos = (mtxFish._41, mtxFish._42, mtxFish._43);

			MyLib::Vector3 setpos = mtxParent.GetWorldPosition();
			if (it.pFish->GetOldPosition().y > 0.0f &&
				setpos.y <= 0.0f)
			{
				// ����
				CEffekseerObj::Create(
					CMyEffekseer::EFKLABEL::EFKLABEL_WATERJUMP,
					setpos, MyLib::Vector3(0.0f, 0.0f, 0.0f), 0.0f, 40.0f, true);

				// �T�E���h�Đ�
				CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_INTHEWATER2);
			}

			// �ʒu�ݒ�
			it.pFish->SetPosition(setpos);
		}

		// �����ƃI�t�Z�b�g�ݒ�
		{	
			rot.x = m_fRot + (D3DX_PI * 2) * (FISH_ROT * it.nIdx);
			rot.y = GetRotation().y;
			UtilFunc::Transformation::RotNormalize(rot.x);
			it.pFish->SetRotation(rot);
			SetFishOffSet(it);
			it.pFish->SetScale(GetScale() + 5.0f);
		}
	}
}

//==========================================================================
// ���̃I�t�Z�b�g�ݒ�
//==========================================================================
void CObstacle_FishArch::SetFishOffSet(FishInfo& info)
{
	float rot = m_fRot;
	rot += (D3DX_PI * 2) * (FISH_ROT * info.nIdx);
	UtilFunc::Transformation::RotNormalize(rot);

	// ���������߂�
	float rate = rot;

	rate = fabsf(fabsf(rate) / D3DX_PI - 1.0f);

	info.offset.z = sinf(rot) * FISHDEFAULT::WIDTH;
	info.offset.y = cosf(rot) * m_Info.fNowHeight;
}

//==========================================================================
// �Ϗ��ݒ�
//==========================================================================
void CObstacle_FishArch::SetInfo(const float fDefHeight, const float fPlusHeight, const float fRotSpd)
{
	// �l�̐ݒ�
	m_Info.fDefHeight = fDefHeight;
	m_Info.fPlusHeight = fPlusHeight;
	m_Info.fRotSpeed = fRotSpd;
}

//==========================================================================
// ���݂̋����ݒ�
//==========================================================================
void CObstacle_FishArch::SetNowHeight()
{
	// �{�̂̌�����������Z������ݒ�
	float rate = sinf(m_fRot);
	m_Info.fNowHeight = m_Info.fDefHeight + (m_Info.fPlusHeight * rate);
}

//==========================================================================
// ���[���h�}�g���b�N�X�ݒ�
//==========================================================================
void CObstacle_FishArch::CalWorldMtx() 
{// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���擾
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();
	MyLib::Matrix mtxRot, mtxTrans, mtxScale;	// �v�Z�p�}�g���b�N�X�錾

	// ���[���h�}�g���b�N�X�̏�����
	m_mtxWorld.Identity();

	// �X�P�[���𔽉f����
	mtxScale.Scaling(GetScale());
	m_mtxWorld.Multiply(m_mtxWorld, mtxScale);

	// �����𔽉f����
	mtxRot.RotationYawPitchRoll(rot.y, rot.x, rot.z);
	m_mtxWorld.Multiply(m_mtxWorld, mtxRot);

	// �ʒu�𔽉f����
	mtxTrans.Translation(pos);
	m_mtxWorld.Multiply(m_mtxWorld, mtxTrans);
}