//=============================================================================
// 
//  �S�[���t���O���� [goalflag.cpp]
//  Author : �����V����̃v���O����(���؂肵�Ă܂���)
// 
//=============================================================================
#include "checkpoint.h"
#include "manager.h"
#include "calculation.h"
#include "player.h"
#include "game.h"
#include "spline.h"
#include "course.h"
#include "timer.h"
#include "splashwater_manager.h"
#include "gamemanager.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* MODEL = "data\\MODEL\\checkpoint\\flag.x";	// ���f���p�X
	const std::string TEX_EFFECT = "data\\TEXTURE\\effect\\rolling2.png";	// �G�t�F�N�g�p�X
	const ImVec4 WATERCOLOR = ImVec4(0.658f, 0.658f, 1.0, 0.87f); // RGBA
}

namespace StateTime
{
	const float SWING = 2.0f;	// �����
	const float ROTATE = 0.6f;	// ��]
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CCheckpoint::STATE_FUNC CCheckpoint::m_StateFunc[] =
{
	&CCheckpoint::StateSwing,	// �����
	&CCheckpoint::StateRotate,	// ��]
};

//==========================================================================
// �ÓI�����o�ϐ�
//==========================================================================
CListManager<CCheckpoint> CCheckpoint::m_List = {};	// ���X�g
int CCheckpoint::m_nAll = 0;
int CCheckpoint::m_nSaveID = 0;

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCheckpoint::CCheckpoint(int nPriority) : CObjectX(nPriority)
{
	// �l�̃N���A
	m_fLength = 0.0f;
	m_bIsPassed = false;
	m_state = State::SWING;	// ���
	m_fStateTime = 0.0f;	// ��ԃJ�E���^�[
	m_fPassedTime = 0.0f;
	m_pEffect = nullptr;		// �G�t�F�N�g�̃|�C���^
	m_pEffekseerObj = nullptr;	// �G�t�F�N�V�A�̃I�u�W�F�N�g
	m_MyIndex = 0;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CCheckpoint::~CCheckpoint()
{

}

//==========================================================================
// ��������
//==========================================================================
CCheckpoint* CCheckpoint::Create(const float length)
{
	// �������̊m��
	CCheckpoint* pObj = DEBUG_NEW CCheckpoint;

	if (pObj != nullptr)
	{
		// ����������
		pObj->Init();

		// �����ݒ�
		pObj->SetLength(length);

		// �������Z
		m_nAll++;
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CCheckpoint::Init()
{
	// ���X�g�ɒǉ�
	m_List.Regist(this);

	// ��ނ̐ݒ�
	CObject::SetType(TYPE_OBJECTX);

	// ID�ݒ�
	m_MyIndex = m_nAll;

	// ����������
	HRESULT hr = CObjectX::Init(MODEL);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// �����̌����ݒ�
	m_DestRot.x = UtilFunc::Transformation::Random(-110, 110) * 0.001f;
	m_DestRot.y = UtilFunc::Transformation::Random(-110, 110) * 0.001f;
	m_DestRot.z = UtilFunc::Transformation::Random(40, 80) * 0.001f;

	return S_OK;
}

//==========================================================================
// �G�t�F�N�g����
//==========================================================================
void CCheckpoint::CreateEffect()
{
	// �G�t�F�N�g����
	if (m_pEffect == nullptr)
	{
		m_pEffect = CObjectBillboard::Create(GetPosition(), 0.0f);
	}
	m_pEffect->SetType(CObject::TYPE::TYPE_OBJECTBILLBOARD);

	// �e�N�X�`���ݒ�
	CTexture* pTexture = CTexture::GetInstance();
	if (pTexture == nullptr) return;

	int texIdx = pTexture->Regist(TEX_EFFECT);
	m_pEffect->BindTexture(texIdx);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = UtilFunc::Transformation::AdjustSizeByWidth(pTexture->GetImageSize(texIdx), 400.0f);
	m_pEffect->SetSize(size);

	m_pEffect->SetAlpha(0.7f);


	// ���G�t�F�N�g����
	CreateWaterEffect(8);
}

//==========================================================================
// �I������
//==========================================================================
void CCheckpoint::Uninit()
{
	// �������炷
	if (m_nAll > 0)
	{
		m_nAll--;
	}

	// ���X�g����폜
	m_List.Delete(this);

	// �I������
	CObjectX::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CCheckpoint::Kill()
{
	// ���X�g����폜
	m_List.Delete(this);

	// �I������
	CObjectX::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CCheckpoint::Update()
{
	// ��ԃ^�C�}�[���Z
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// ��ԕʍX�V
	(this->*(m_StateFunc[m_state]))();


	// �ʉߍς݂Ȃ珈�����Ȃ�
	if (m_bIsPassed) return;

	// ���X�g���[�v
	CListManager<CPlayer> PlayerList = CPlayer::GetListObj();
	CPlayer* pPlayer = PlayerList.GetData(0);
	float playerlen = pPlayer->GetPosition().x;

	if (playerlen >= GetPosition().x)
	{// �`�F�b�N�|�C���g�ʉ߂�����

		if (m_nSaveID < m_MyIndex)
		{
			// ID�ۑ�
			m_nSaveID = m_MyIndex;

			// �ʉ߂������Ԃ�ۑ�
			m_fPassedTime = CTimer::GetInstance()->GetTime();
			m_bIsPassed = true;

			// �ʉ߂������̃|�C���g�ݒ�
			{
				CGameManager* pMgr = CGame::GetInstance()->GetGameManager();

				if (pMgr != nullptr)
				{
					pMgr->SaveEvaluationPoint();
				}
			}

			// ��Ԑݒ�
			m_state = State::ROTATE;
			m_fStateTime = 0.0f;

			// SE�Đ�
			CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_KARAKURI);

			// �G�t�F�N�g�쐬
			CreateEffect();
		}
	}
}

//==========================================================================
// �����
//==========================================================================
void CCheckpoint::StateSwing()
{
	MyLib::Vector3 rot = GetRotation();

	// �T�C���J�[�u���
	float ratio = UtilFunc::Correction::EasingEaseInOutSine(0.0f, 1.0f, 0.0f, StateTime::SWING, m_fStateTime);
	rot = m_DestRot_Old + (m_DestRot - m_DestRot_Old) * ratio;

	if (m_bIsPassed)
	{// �����ς݂͔���]
		rot.z += D3DX_PI;
	}
	SetRotation(rot);

	if (m_fStateTime >= StateTime::SWING)
	{
		m_fStateTime = 0.0f;

		// �ߋ��̌����ۑ�
		m_DestRot_Old = m_DestRot;

		// �ڕW�̌����ݒ�
		m_DestRot.x = UtilFunc::Transformation::Random(-110, 110) * 0.001f;
		m_DestRot.y = UtilFunc::Transformation::Random(-110, 110) * 0.001f;
		m_DestRot.z *= -1;
	}
}

//==========================================================================
// ��]
//==========================================================================
void CCheckpoint::StateRotate()
{
	if (m_fStateTime <= StateTime::ROTATE * 0.5f)
	{
		// ���G�t�F�N�g����
		CreateWaterEffect(4);
	}
	else if (m_pEffekseerObj == nullptr)
	{
		// �G�t�F�N�g
		m_pEffekseerObj = CEffekseerObj::Create(
			CMyEffekseer::EFKLABEL::EFKLABEL_WATERJUMP,
			GetPosition() + MyLib::Vector3(300.0f, 0.0f, 0.0f), 0.0f, 0.0f, 60.0f, true);

		// ���Ԃ�����
		CSplashwater_Manager::Create();
	}


	// ��]
	MyLib::Vector3 rot = GetRotation();
	rot.z = UtilFunc::Correction::EasingEaseIn(0.0f, -D3DX_PI, 0.0f, StateTime::ROTATE, m_fStateTime);
	SetRotation(rot);

	// �G�t�F�N�g��]
	MyLib::Vector3 effectRot = m_pEffect->GetRotation();
	effectRot.z += UtilFunc::Correction::EasingLinear(0.0f, D3DX_PI * 0.5f, 0.0f, StateTime::ROTATE, m_fStateTime);
	m_pEffect->SetRotation(effectRot);

	// �G�t�F�N�g�s�����x
	float alpha = UtilFunc::Correction::EasingEaseOut(0.7f, 0.0f, 0.0f, StateTime::ROTATE, m_fStateTime);
	m_pEffect->SetAlpha(alpha);

	// ���Ԍo��
	if (m_fStateTime >= StateTime::ROTATE)
	{
		m_state = State::SWING;
		m_fStateTime = 0.0f;

		// �����̌����ݒ�
		m_DestRot.x = UtilFunc::Transformation::Random(-110, 110) * 0.001f;
		m_DestRot.y = UtilFunc::Transformation::Random(-110, 110) * 0.001f;
		m_DestRot.z = UtilFunc::Transformation::Random(40, 80) * 0.001f;

		if (m_pEffect != nullptr)
		{
			m_pEffect->Uninit();
			m_pEffect = nullptr;
		}
	}
}

//==========================================================================
// ���G�t�F�N�g����
//==========================================================================
void CCheckpoint::CreateWaterEffect(int max)
{
	for (int i = 0; i < max; i++)
	{
		// �ړ���
		MyLib::Vector3 setmove;
		float randmove = UtilFunc::Transformation::Random(30, 50) * 0.1f;
		float randmoveY = UtilFunc::Transformation::Random(130, 170) * 0.1f;

		float randAngle = UtilFunc::Transformation::Random(-20, 20) * 0.01f;
		setmove.x = sinf(D3DX_PI * 0.5f + randAngle) * randmove;
		setmove.z = cosf(D3DX_PI * 0.5f + randAngle) * randmove;
		setmove.y = randmoveY;

		// �F
		float colorrand = UtilFunc::Transformation::Random(-22, 22) * 0.01f;

		// ���a
		float randRadius = UtilFunc::Transformation::Random(-100, 100) * 0.1f;
		float radius = 25.0f + randRadius;

		// ����
		MyLib::Vector3 distance(
			-250.0f + UtilFunc::Transformation::Random(-200, 200) * 0.1f,
			0.0f,
			UtilFunc::Transformation::Random(-100, 100) * 0.1f);

		CEffect3D* pEffect = CEffect3D::Create(
			GetPosition() + distance,
			setmove,
			D3DXCOLOR(WATERCOLOR.x + colorrand, WATERCOLOR.y + colorrand, WATERCOLOR.z, WATERCOLOR.w),
			radius,
			50,
			CEffect3D::MOVEEFFECT::MOVEEFFECT_ADD,
			CEffect3D::TYPE::TYPE_SMOKE);
		pEffect->SetEnableGravity();
		pEffect->SetGravityValue(0.4f);
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CCheckpoint::Draw()
{
	// �{�b�N�X�R���C�_�[�̕`��
	if (!CManager::IsDisp_CheckPoint()) return;

	// �`��
	CObjectX::Draw();
}

//==========================================================================
// �����ݒ�
//==========================================================================
void CCheckpoint::SetLength(const float length)
{
	// ������ݒ�
	m_fLength = length;

	// ���W��ݒ�
	MyLib::Vector3 pos = MySpline::GetSplinePosition_NonLoop(CGame::GetInstance()->GetCourse()->GetVecPosition(), m_fLength);
	SetPosition(pos);
}

//==========================================================================
// �`�F�b�N�|�C���g�t�@�C���ǂݍ���
//==========================================================================
void CCheckpoint::Load(const std::string filename)
{
	// �t�@�C�����J��
	std::ifstream File(filename);
	if (!File.is_open()) {
		return;
	}

	float length = 0.0f;

	// �R�����g�p
	std::string hoge;

	// �f�[�^�ǂݍ���
	std::string line;
	while (std::getline(File, line))
	{
		// �R�����g�̓X�L�b�v
		if (line.empty() ||
			line[0] == '#')
		{
			continue;
		}

		if (line.find("SET_LENGTH") != std::string::npos)
		{// TYPE�Ŕz�u���̎��

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// ���n��
			lineStream >>
				hoge >>
				hoge >>	// ��
				length;	// �z�u���̎��

			CCheckpoint::Create(length);

			continue;
		}

		if (line.find("END_SCRIPT") != std::string::npos)
		{
			break;
		}
	}

	// �t�@�C�������
	File.close();
}

//==========================================================================
// �`�F�b�N�|�C���g�ʉߏ�񃊃Z�b�g
//==========================================================================
void CCheckpoint::ResetSaveID()
{
	m_nSaveID = -1;

	// ���X�g���[�v
	std::list<CCheckpoint*>::iterator itr = m_List.GetEnd();
	CCheckpoint* pObj = nullptr;

	while (m_List.ListLoop(itr))
	{
		pObj = (*itr);
		pObj->m_bIsPassed = false;
		pObj->m_fPassedTime = 0.0f;
		pObj->SetRotation(0.0f);
		// pObj��(*itr)���g���ď���
	}
}