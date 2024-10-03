//=============================================================================
// 
//  �~�`�ŉ�]���钹��Q������(���̂Ȃ�) [obstacle_birdcircle.cpp]
//  Author : Ibuki Okusada
// 
//=============================================================================
#include "obstacle_birdcircle.h"
#include "debugproc.h"
#include "manager.h"
#include "baggage.h"


//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const int	NUM_BIRD = (10);				// ���̑���
	const float BIRD_ROT = (1.0f / NUM_BIRD);	// 1���ӂ�̊p�x����
	const int RANGE_MOTION = 2;					// ���[�V��������
	const float INTERVAL_WING = 0.8f;
}

// �f�t�H���g���
namespace BIRDDEFAULT
{
	const float ROTATE_SPEED = (0.02f);	// ��]���x
	const float LENGTH = (750.0f);			// ���S����̋���
	const float PLUSLENGTH = (250.0f);		// �ω����鋗��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CObstacle_BirdCircle::CObstacle_BirdCircle(int nPriority,
	CObject::LAYER layer) : CMap_Obstacle(nPriority, layer)
{
	// �l�̃N���A
	m_BirdList.clear();
	m_scale = 1.0f;
	m_fIntervalWing = 0.0f;		// �H�΂����̊Ԋu
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CObstacle_BirdCircle::~CObstacle_BirdCircle()
{

}

//==========================================================================
// ��������
//==========================================================================
CObstacle_BirdCircle* CObstacle_BirdCircle::Create(const CMap_ObstacleManager::SObstacleInfo& info)
{
	// �������̊m��
	CObstacle_BirdCircle* pObj = DEBUG_NEW CObstacle_BirdCircle;

	if (pObj != nullptr)
	{
		pObj->SetObstacleInfo(info);
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObstacle_BirdCircle::Init()
{
	// �����̃u���b�N�v�Z
	CMap_Obstacle::CalMyBlock();
	CMap_Obstacle::ListRegist(this);

	
	MyLib::Vector3 rot;

	// ��ނ̐ݒ�
	CObject::SetType(TYPE_OBJECTX);

	// ���𐶐�����
	for (int i = 0; i < NUM_BIRD; i++)
	{
		BirdInfo info;
		info.nIdx = i;
		info.pBird = CMap_Obstacle::Create(GetObstacleInfo(), GetPosition(), false, false);
		rot.y = (i * BIRD_ROT) * (D3DX_PI * 2);
		UtilFunc::Transformation::RotNormalize(rot.y);
		info.pBird->SetRotation(rot);
		SetBirdOffSet(info);
		info.pBird->SetPosition(info.offset);
		m_BirdList.push_back(info);
	}

	// �����l�ݒ�
	m_Info.fDefLength = BIRDDEFAULT::LENGTH;
	m_Info.fPlusLength = BIRDDEFAULT::PLUSLENGTH;
	m_Info.fRotSpeed = BIRDDEFAULT::ROTATE_SPEED;
	m_Info.fNowLength = BIRDDEFAULT::LENGTH;

	// ���C������
	MainControll();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CObstacle_BirdCircle::Uninit()
{
	for (auto& it : m_BirdList)
	{
		if (it.pBird != nullptr)
		{
			it.pBird->Uninit();
			it.pBird = nullptr;
		}
	}

	// ���X�g�N���A
	m_BirdList.clear();

	// �I������
	CMap_Obstacle::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CObstacle_BirdCircle::Kill()
{
	for (auto& it : m_BirdList)
	{
		if (it.pBird != nullptr)
		{
			it.pBird->Kill();
			it.pBird = nullptr;
		}
	}

	// ���X�g�N���A
	m_BirdList.clear();

	// �I������
	CMap_Obstacle::Kill();
}

//==========================================================================
// �X�V����
//==========================================================================
void CObstacle_BirdCircle::Update()
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


	// �T�E���h�Đ�
	m_fIntervalWing += CManager::GetInstance()->GetDeltaTime();
	if (m_fIntervalWing >= INTERVAL_WING)
	{
		m_fIntervalWing = 0.0f;
		CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_HABATAKI);
		
		for (auto& it : m_BirdList)
		{
			if (it.pBird == nullptr) continue;	// ���݂��Ă��Ȃ�

			MyLib::Vector3 rot = it.pBird->GetRotation();

			// �ʒu�̐ݒ�
			{
				MyLib::Matrix mtxRot, mtxTrans, mtxScale, mtxBird;	// �v�Z�p�}�g���b�N�X�錾
				MyLib::Matrix mtxParent = m_mtxWorld;	// �e�̃}�g���b�N�X
				MyLib::Vector3 BirdPos = it.offset;

				// �H�΂����G�t�F�N�g����
				CEffekseerObj::Create(
					CMyEffekseer::EFKLABEL::EFKLABEL_WING,
					it.pBird->GetPosition(), MyLib::Vector3(0.0f, m_rot.y, 0.0f), 0.0f, 10.0f, true);
			}
		}
	}

	// ���C������
	MainControll();
}

//==========================================================================
// ���C������
//==========================================================================
void CObstacle_BirdCircle::MainControll()
{
	// ��](y�͌����Ax�͈ړ��Ɏg�p)
	m_rot.y += m_Info.fRotSpeed;
	m_rot.x -= m_Info.fRotSpeed;
	UtilFunc::Transformation::RotNormalize(m_rot.y);
	UtilFunc::Transformation::RotNormalize(m_rot.x);

	// �����ݒ�
	SetNowLength();

	// �}�g���b�N�X�ݒ�
	CalWorldMtx();

	// ���̍X�V
	ControllBird();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CObstacle_BirdCircle::Draw()
{

}

//==========================================================================
// ���̑���ݒ�
//==========================================================================
void CObstacle_BirdCircle::ControllBird()
{
	for (auto& it : m_BirdList)
	{
		if (it.pBird == nullptr) continue;	// ���݂��Ă��Ȃ�

		MyLib::Vector3 rot = it.pBird->GetRotation();

		// �ʒu�̐ݒ�
		{
			MyLib::Matrix mtxRot, mtxTrans, mtxScale, mtxBird;	// �v�Z�p�}�g���b�N�X�錾
			MyLib::Matrix mtxParent = m_mtxWorld;	// �e�̃}�g���b�N�X
			MyLib::Vector3 BirdPos = it.offset;

			// ���W�p�}�g���b�N�X������
			mtxBird.Identity();

			// �ʒu�𔽉f����
			mtxTrans.Translation(BirdPos);
			mtxBird.Multiply(mtxBird, mtxTrans);

			mtxParent.Multiply(mtxBird, mtxParent);

			// ���W��ݒ�
			BirdPos = (mtxBird._41, mtxBird._42, mtxBird._43);

			it.pBird->SetPosition(mtxParent.GetWorldPosition());
		}

		// �����ƃI�t�Z�b�g�ݒ�
		{
			rot.y = m_rot.y + (D3DX_PI * 2.0f) * (BIRD_ROT * it.nIdx) + D3DX_PI * 0.5f;
			UtilFunc::Transformation::RotNormalize(rot.y);
			it.pBird->SetRotation(rot);
			SetBirdOffSet(it);
			it.pBird->SetScale(GetScale() + 1.0f);
		}
	}
}

//==========================================================================
// ���̃I�t�Z�b�g�ݒ�
//==========================================================================
void CObstacle_BirdCircle::SetBirdOffSet(BirdInfo& info)
{
	// ���g�̑��݂���������擾
	MyLib::Vector3 rot = m_rot;
	rot.x = m_rot.x + (info.nIdx * BIRD_ROT) * (-D3DX_PI * 2);
	UtilFunc::Transformation::RotNormalize(rot.y);

	// ���������߂�
	float rate = rot.x;
	rate = fabsf(fabsf(rate) / D3DX_PI - 1.0f);

	// �I�t�Z�b�g��ݒ�
	info.offset.z = sinf(rot.x) * m_Info.fNowLength;
	info.offset.x = cosf(rot.x) * m_Info.fNowLength;
}

//==========================================================================
// ���݂̋����ݒ�
//==========================================================================
void CObstacle_BirdCircle::SetNowLength()
{
	// �{�̂̌�����������Z������ݒ�
	MyLib::Vector3 rot = m_rot;
	float rate = sinf(rot.x);
	m_Info.fNowLength = m_Info.fDefLength + (m_Info.fPlusLength * rate);
}

//==========================================================================
// �Ϗ��ݒ�
//==========================================================================
void CObstacle_BirdCircle::SetInfo(const float fDefLen, const float fPlusLen, const float fRotSpd)
{
	// �l�̐ݒ�
	m_Info.fDefLength = fDefLen;
	m_Info.fPlusLength = fPlusLen;
	m_Info.fRotSpeed = fRotSpd;
}

//==========================================================================
// ���[���h�}�g���b�N�X�ݒ�
//==========================================================================
void CObstacle_BirdCircle::CalWorldMtx()
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