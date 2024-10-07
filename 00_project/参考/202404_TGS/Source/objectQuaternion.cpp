//=============================================================================
// 
//  �N�H�[�^�j�I������ [objectQuaternion.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "objectQuaternion.h"
#include "calculation.h"
#include "manager.h"
#include "debugproc.h"
#include "input.h"


//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	float DEFAULT_MOVEFACTOR = 0.1f;	// �f�t�H���g�̈ړ��␳�W��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CObjectQuaternion::CObjectQuaternion(int nPriority) : CObjectX(nPriority)
{
	// �l�̃N���A
	D3DXQuaternionIdentity(&m_quaternion);
	m_vecAxis = 0.0f;			// ��]��
	m_fValueRot = 0.0f;			// ��]�p
	m_fMoveValueRot = 0.0f;		// ��]�p�̈ړ���
	m_fMoveFactor = 0.0f;		// �ړ��ʂ̕␳�W��
	m_RotationMtx.Identity();	// �v�Z�p�}�g���b�N�X�錾
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CObjectQuaternion::~CObjectQuaternion()
{

}

//==========================================================================
// ��������
//==========================================================================
CObjectQuaternion* CObjectQuaternion::Create()
{
	// �������̊m��
	CObjectQuaternion* pObjChara = DEBUG_NEW CObjectQuaternion;

	if (pObjChara != nullptr)
	{
		// ����������
		HRESULT hr = pObjChara->Init();
		if (FAILED(hr)){
			return nullptr;
		}
	}

	return pObjChara;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObjectQuaternion::Init()
{
	CObjectX::Init("data\\MODEL\\box.x");
	CObject::SetType(CObject::TYPE::TYPE_OBJECTX);

	m_fMoveFactor = DEFAULT_MOVEFACTOR;		// �ړ��ʂ̕␳�W��

	return S_OK;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObjectQuaternion::Init(const std::string& file)
{
	CObjectX::Init(file);
	CObject::SetType(CObject::TYPE::TYPE_OBJECTX);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CObjectQuaternion::Uninit()
{
	// �I������
	CObjectX::Uninit();

	// �I�u�W�F�N�g�̔j��
	Release();
}

//==========================================================================
// �X�V����
//==========================================================================
void CObjectQuaternion::Update()
{
	// �I�u�W�F�N�gX�̍X�V����
	CObjectX::Update();
#if 0
	// ���擾
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 move = GetMove();
	MyLib::Vector3 rot = GetRotation();


	MyLib::Vector3 vec = MyLib::Vector3(sinf(D3DX_PI + rot.y), 0.0f, cosf(D3DX_PI + rot.y));

	m_vecAxis.x = vec.z;
	m_vecAxis.z = -vec.x;



	// �ʒu�X�V
	pos += move;

	// ��]�p�ݒ�
	m_fValueRot = move.x * 0.005f;

	move.x += (0.0f - move.x) * 0.05f;


	// �ݒ�
	SetPosition(pos);
	SetMove(move);
	SetRotation(rot);
#endif

	//// ��]�p���Z
	//m_fValueRot = m_fMoveValueRot;
	//m_fMoveValueRot += (0.0f - m_fMoveValueRot) * m_fMoveFactor;

}

//==========================================================================
// �N�H�[�^�j�I�����f
//==========================================================================
void CObjectQuaternion::BindQuaternion(const MyLib::Vector3& vecAxis, float valueRot)
{
	MyLib::Matrix mtxRot;	// �v�Z�p�}�g���b�N�X�錾

	D3DXQuaternionIdentity(&m_quaternion);

	// �C�ӂ̉�]���ɂ������]�p����N�H�[�^�j�I���v�Z
	D3DXQuaternionRotationAxis(&m_quaternion, &vecAxis, valueRot);

	// ��]�}�g���b�N�X���쐬
	mtxRot.Identity();
	D3DXMATRIX calRotMtx = mtxRot.ConvertD3DXMATRIX();
	D3DXMatrixRotationQuaternion(&calRotMtx, &m_quaternion);
	mtxRot = calRotMtx;

	// �N�H�[�^�j�I���𐳋K��
	D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);

	// �����𔽉f����
	m_RotationMtx.Multiply(m_RotationMtx, mtxRot);
}

//==========================================================================
// �N�H�[�^�j�I���v�Z
//==========================================================================
void CObjectQuaternion::CalQuaternion()
{
	MyLib::Matrix mtxRot;	// �v�Z�p�}�g���b�N�X�錾

	D3DXQuaternionIdentity(&m_quaternion);

	// �C�ӂ̉�]���ɂ������]�p����N�H�[�^�j�I���v�Z
	D3DXQuaternionRotationAxis(&m_quaternion, &m_vecAxis, m_fValueRot);

	// ��]�}�g���b�N�X���쐬
	mtxRot.Identity();
	D3DXMATRIX calRotMtx = mtxRot.ConvertD3DXMATRIX();
	D3DXMatrixRotationQuaternion(&calRotMtx, &m_quaternion);
	mtxRot = calRotMtx;

	// �N�H�[�^�j�I���𐳋K��
	D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);


	// �����𔽉f����
	m_RotationMtx.Multiply(m_RotationMtx, mtxRot);
}

//==========================================================================
// ���[���h�}�g���b�N�X�̌v�Z����
//==========================================================================
void CObjectQuaternion::CalWorldMtx()
{
#if 1

	// �N�H�[�^�j�I���v�Z
	//CalQuaternion();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	MyLib::Matrix mtxTrans, mtxScale;	// �v�Z�p�}�g���b�N�X�錾
	MyLib::Matrix mtxWorld = GetWorldMtx();

	// ���擾
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 scale = GetScale();

	// ������
	mtxWorld.Identity();

	// �X�P�[���𔽉f����
	mtxScale.Scaling(scale);
	mtxWorld.Multiply(mtxWorld, mtxScale);

	// �����𔽉f����
	mtxWorld.Multiply(mtxWorld, m_RotationMtx);

	// �ʒu�𔽉f����
	mtxTrans.Translation(pos);
	mtxWorld.Multiply(mtxWorld, mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	D3DXMATRIX setMtx = mtxWorld.ConvertD3DXMATRIX();
	pDevice->SetTransform(D3DTS_WORLD, &setMtx);
	SetWorldMtx(mtxWorld);
#else


	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	MyLib::Matrix mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X�錾

	// ���擾
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();
	MyLib::Vector3 rotOld = GetOldRotation();
	MyLib::Matrix mtxWorld = GetWorldMtx();


	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld.Identity();
	D3DXQuaternionIdentity(&m_quaternion);

	MyLib::Vector3 vec;

	D3DXQUATERNION quatX;
	vec = { 1.0f, 0.0f, 0.0f };
	D3DXQuaternionRotationAxis(&quatX, &vec, rotOld.x);

	D3DXQUATERNION quatZ;
	vec = { 0.0f, 0.0f, 1.0f };
	D3DXQuaternionRotationAxis(&quatZ, &vec, rotOld.z);

	D3DXQUATERNION quatY;
	vec = { 0.0f, 1.0f, 0.0f };
	D3DXQuaternionRotationAxis(&quatY, &vec, rotOld.y);

	// x���Ay���Az���̏��ŉ�]��K�p
	m_quaternion = quatY * m_quaternion;
	m_quaternion = quatX * m_quaternion;
	m_quaternion = quatZ * m_quaternion;

	// �N�H�[�^�j�I���𐳋K��
	D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);

	// �N�H�[�^�j�I����g�ݍ��킹�ĉ�]�}�g���b�N�X���쐬
	mtxRot.Identity();
	D3DXMATRIX D3DmtxRot = mtxRot.ConvertD3DXMATRIX();
	D3DXMatrixRotationQuaternion(&D3DmtxRot, &m_quaternion);
	mtxRot = D3DmtxRot;

	// �����𔽉f����
	mtxWorld.Multiply(mtxWorld, mtxRot);


	// �ʒu�𔽉f����
	mtxTrans.Translation(pos);
	mtxWorld.Multiply(mtxWorld, mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	D3DXMATRIX mtx = mtxWorld.ConvertD3DXMATRIX();
	pDevice->SetTransform(D3DTS_WORLD, &mtx);
	SetWorldMtx(mtxWorld);
#endif
}

//==========================================================================
// �`�揈��
//==========================================================================
void CObjectQuaternion::Draw()
{
	// ���[���h�}�g���b�N�X�̌v�Z����
	CalWorldMtx();

	// �`��̂�
	CObjectX::DrawOnly();
}
