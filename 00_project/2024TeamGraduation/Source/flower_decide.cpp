//=============================================================================
// 
//  �����ԏ��� [flower_decide.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "flower_decide.h"
#include "manager.h"
#include "sound.h"
#include "particle.h"
#include "calculation.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* MODEL[] =	// ���f���t�@�C��
	{
		"data\\MODEL\\flower\\blue.x",
		"data\\MODEL\\flower\\pink.x",
		"data\\MODEL\\flower\\ranunculus.x",
		"data\\MODEL\\flower\\red.x",
		"data\\MODEL\\flower\\white.x",
	};
		
	const int MAX_SPAWNNUM = 2048;		// �ő吶����
	const float TIME_CHARGE = 3.0f;		// �`���[�W����
	const int TIME_FLOWERING = 60;	// �J�Ԏ���
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CDecideFlower::CDecideFlower(int nPriority) : CObjectX(nPriority)
{
	// �l�̃N���A
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CDecideFlower::~CDecideFlower()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CDecideFlower* CDecideFlower::Create(const MyLib::Vector3& pos, const MyLib::Vector3& move)
{
	// �������̊m��
	CDecideFlower* pBullet = DEBUG_NEW CDecideFlower;

	if (pBullet != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// ���蓖��
		pBullet->SetPosition(pos);
		pBullet->SetOriginPosition(pos);
		pBullet->SetMove(move);

		// ����������
		pBullet->Init();
	}

	return pBullet;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CDecideFlower::Init()
{
	// ��ނ̐ݒ�
	CObject::SetType(TYPE::TYPE_OBJECTX);

	int rand = UtilFunc::Transformation::Random(0, 4);

	// ����������
	HRESULT hr = CObjectX::Init(MODEL[rand]);
	if (FAILED(hr))
	{// ���s�����Ƃ�
		return E_FAIL;
	}
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CDecideFlower::Uninit()
{

	// �I������
	CObjectX::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CDecideFlower::Update()
{
	MyLib::Vector3 move = GetMove();
	MyLib::Vector3 pos = GetPosition();
	pos += move;

	move.x += (0.0f - move.x) * 0.1f;
	move.z += (0.0f - move.z) * 0.1f;

	SetPosition(pos);
	SetMove(move);

}
