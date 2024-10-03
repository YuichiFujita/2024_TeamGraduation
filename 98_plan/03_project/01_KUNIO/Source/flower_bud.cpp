//=============================================================================
// 
//  �ڂݏ��� [flower_bud.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "flower_bud.h"
#include "manager.h"
#include "sound.h"
#include "particle.h"
#include "calculation.h"
#include "flower_decide.h"
#include "resultscore.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* MODEL = "data\\MODEL\\flower\\ranunculus.x";	// ���f���t�@�C��
	const int MAX_SPAWNNUM = 2048;		// �ő吶����
	const int TIME_CHARGE = 90;		// �`���[�W����
	const int TIME_FLOWERING = 100;	// �J�Ԏ���
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CFlowerBud::STATE_FUNC CFlowerBud::m_StateFuncList[] =
{
	&CFlowerBud::StateWait,		// �ҋ@
	&CFlowerBud::StateCharge,	// ����
	&CFlowerBud::StateFlowering,// �J��
};
CFlowerBud* CFlowerBud::m_ThisPtr = nullptr;

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CFlowerBud::CFlowerBud(int nPriority) : CObjectX(nPriority)
{
	// �l�̃N���A
	m_state = STATE::STATE_WAIT;	// ���
	m_fStateTime = 0.0f;			// ��ԃ^�C�}�[
	m_fRatio = 0.0f;				// ����
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CFlowerBud::~CFlowerBud()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CFlowerBud* CFlowerBud::Create(const MyLib::Vector3& pos, int maxPollen, int currentPollen)
{
	if (m_ThisPtr == nullptr)
	{// �܂��������Ă��Ȃ�������

		// �C���X�^���X����
		m_ThisPtr = DEBUG_NEW CFlowerBud;

		if (m_ThisPtr != nullptr)
		{// �������̊m�ۂ��o���Ă�����

			// �ʒu���蓖��
			m_ThisPtr->SetPosition(pos);
			m_ThisPtr->SetOriginPosition(pos);

			// �����ݒ�
			m_ThisPtr->m_fRatio = static_cast<float>(currentPollen) / static_cast<float>(maxPollen);

			// ����������
			m_ThisPtr->Init();
		}
	}

	return m_ThisPtr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CFlowerBud::Init()
{
	// ��ނ̐ݒ�
	CObject::SetType(TYPE::TYPE_OBJECTX);

	m_nSpawnNum = MAX_SPAWNNUM * m_fRatio;	// ������

	// ����������
	HRESULT hr = CObjectX::Init(MODEL);
	if (FAILED(hr))
	{// ���s�����Ƃ�
		return E_FAIL;
	}
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CFlowerBud::Uninit()
{


	m_ThisPtr = nullptr;

	// �I������
	CObjectX::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CFlowerBud::Update()
{
	// ��ԕʏ���
	(this->*(m_StateFuncList[m_state]))();
}

void CFlowerBud::SetCurrentPollen(int maxPollen, int pollen)
{
	m_fRatio = static_cast<float>(pollen) / static_cast<float>(maxPollen);
	m_nSpawnNum = MAX_SPAWNNUM * m_fRatio;	// ������
}

//==========================================================================
// ����
//==========================================================================
void CFlowerBud::StateCharge()
{
	// ��ԃ^�C�}�[���Z
	m_fStateTime++;

	my_particle::Create(GetPosition(), my_particle::TYPE::TYPE_FLOWERINGCHARGE);

	if (static_cast<int>(m_fStateTime) % 6 == 0)
	{
		CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_CHARGE);
	}

	if (static_cast<int>(m_fStateTime) >= TIME_CHARGE)
	{
		m_fStateTime = 0.0f;			// ��ԃ^�C�}�[
		m_state = STATE::STATE_FLOWERING;
	}
}

//==========================================================================
// �J��
//==========================================================================
void CFlowerBud::StateFlowering()
{
	// ��ԃ^�C�}�[���Z
	m_fStateTime++;

	if (m_nSpawnNum > 0 &&
		static_cast<int>(m_fStateTime) % 8 == 0)
	{
		CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_BLOOM);
	}

	int num = m_nSpawnNum / TIME_FLOWERING;
	//CManager::GetInstance()->GetCamera()->SetLenDest(4000.0f, 2, 1.0f, 0.015f);

	for (int i = 0; i < num; i++)
	{
		MyLib::Vector3 move;
		move.x = UtilFunc::Transformation::Random(-40, 40) * 10.0f;
		move.z = UtilFunc::Transformation::Random(-40, 40) * 10.0f;

		CDecideFlower::Create(GetPosition(), move);
	}
	

	if (static_cast<int>(m_fStateTime) >= TIME_FLOWERING)
	{
		CResultScore::Create(m_nSpawnNum);
		Uninit();
	}
}
