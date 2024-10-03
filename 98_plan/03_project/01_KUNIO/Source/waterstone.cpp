//=============================================================================
// 
//  �����Ώ��� [waterstone.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "waterstone.h"
#include "manager.h"
#include "calculation.h"
#include "3D_effect.h"
#include "player.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	float DEFAULT_SPLASHTIME = 0.14f;	// �ʏ�̂��Ԃ�����
	const float UPDATE_LEN = 8000.0f;	// �X�V�̋���
}
CListManager<CWaterStone> CWaterStone::m_List = {};				// ���X�g
const std::vector<std::string> CWaterStone::m_vecModelFile =	// ���f���t�@�C���̃R���e�i
{
	"data\\MODEL\\map_object\\rock_01_small.x",
	"data\\MODEL\\map_object\\rock_02.x",
	"data\\MODEL\\map_object\\rock_03.x",
	"data\\MODEL\\map_object\\rock_04.x",
	"data\\MODEL\\map_object\\rock_05.x",
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CWaterStone::CWaterStone(int nPriority) : CObjectX(nPriority)
{
	// �l�̃N���A
	m_fSplashTimer = 0.0f;		// ���Ԃ��^�C�}�[
	m_fIntervalSplash = 0.0f;	// ���Ԃ��܂ł̃C���^�[�o��
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CWaterStone::~CWaterStone()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CWaterStone *CWaterStone::Create(const CWaterStone_Manager::SStoneInfo& info)
{
	// �������̊m��
	CWaterStone* pObj = DEBUG_NEW CWaterStone;

	if (pObj != nullptr)
	{
		// �������ݒ�
		pObj->m_StoneInfo = info;

		// ����������
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CWaterStone::Init()
{
	// �ʒu�ݒ�
	SetPosition(m_StoneInfo.pos);

	// ��ނ̐ݒ�
	CObject::SetType(TYPE_OBJECTX);

	// ����������
	HRESULT hr = CObjectX::Init(m_vecModelFile[m_StoneInfo.type]);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// ���X�g�ǉ�
	m_List.Regist(this);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CWaterStone::Uninit()
{
	// ���X�g�폜
	m_List.Delete(this);

	// �I������
	CObjectX::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CWaterStone::Kill()
{
	// ���X�g�폜
	m_List.Delete(this);

	// �I������
	CObjectX::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CWaterStone::Update()
{
	CPlayer* pPlayer = CPlayer::GetListObj().GetData(0);
	MyLib::Vector3 playerPos = pPlayer->GetPosition();

	if (GetPosition().x > playerPos.x + UPDATE_LEN ||
		GetPosition().x < playerPos.x - UPDATE_LEN)
	{
		return;
	}

	// ���Ԃ��^�C�}�[���Z
	m_fSplashTimer += CManager::GetInstance()->GetDeltaTime();

#if 0
	static int createIdx = 8;
	static int Life = 50;
	static int Randrange = 28;
	static int posXZrange = 50;
	static int XZrange = 117;
	static int MinYRange = 37;
	static int MaxYRange = 43;
	static int colorRandRange = 22;
	static float defradius = 25.2f;
	static float gravity = 0.16f;

#else

	static int createIdx = 8;
	static int Life = 50;
	static int Randrange = 11;
	static int posXZrange = 67;
	static int XZrange = 23;
	static int MinYRange = 37;
	static int MaxYRange = 43;
	static int ForwardRange_Min = 544;
	static int ForwardRange = 775;
	static int colorRandRange = 22;
	static float defradius = 25.2f;
	static float gravity = 0.16f;
#endif

	// �J���[�G�f�B�b�g
	static ImVec4 myColor = ImVec4(0.658f, 0.658f, 1.0, 0.87f); // RGBA

#if 0
	if (ImGui::TreeNode("Stone Water"))
	{
		ImGui::DragInt("CreateNum", &createIdx, 1);
		ImGui::DragFloat("Interval", &DEFAULT_SPLASHTIME, 0.01f, 0.0f, 0.0f, "%.2f");
		ImGui::DragInt("Interval_Randrange", &Randrange, 1);
		ImGui::DragInt("Life", &Life, 1);
		ImGui::DragInt("pos XZrange", &posXZrange, 1);
		ImGui::DragInt("XZrange", &XZrange, 1, 0);
		ImGui::DragInt("MinYRange", &MinYRange, 1);
		ImGui::DragInt("MaxYRange", &MaxYRange, 1, MinYRange);
		ImGui::DragInt("ForwardRange_Min", &ForwardRange_Min, 1, 0);
		ImGui::DragInt("ForwardRange_Max", &ForwardRange, 1, ForwardRange_Min +1);
		ImGui::DragInt("colorRandRange", &colorRandRange, 1);
		ImGui::DragFloat("defradius", &defradius, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat("gravity", &gravity, 0.01f, 0.0f, 0.0f, "%.2f");
		ImGui::ColorEdit4("Color", &myColor.x);

		ImGui::TreePop();
	}
#endif

//#ifndef _DEBUG
	// �X�v���b�V���I
	if (m_fIntervalSplash <= m_fSplashTimer)
	{

		MyLib::Vector3 move;
		MyLib::Vector3 posOrigin = GetPosition();
		MyLib::Vector3 pos;

		for (int i = 0; i < createIdx; i++)
		{
			// �ړ���
			float randmoveX = UtilFunc::Transformation::Random(0, XZrange) * 0.1f;
			float randmoveY = UtilFunc::Transformation::Random(MinYRange, MaxYRange) * 0.1f;
			float randRadius = UtilFunc::Transformation::Random(-30, 30) * 0.1f;
			

			float angle = 0.3f;
			angle += UtilFunc::Transformation::Random(-20, 20) * 0.001f;

			if (i >= createIdx / 2)
			{
				angle *= -1;
			}
			move.x = sinf(D3DX_PI * angle + m_StoneInfo.rot.y) * randmoveX;
			move.z = cosf(D3DX_PI * angle + m_StoneInfo.rot.y) * randmoveX;
			move.y = randmoveY;

			// ���a
			float radius = defradius + randRadius;

			// �����ʒu�̂Ԃ�
			int xz = UtilFunc::Transformation::Random(0, posXZrange);
			if (i >= createIdx / 2)
			{
				xz *= -1;
			}
			
			// �����ʒu
			MyLib::Vector3 randpos;
			randpos.x = sinf(D3DX_PI * 0.5f + m_StoneInfo.rot.y) * xz;
			randpos.z = cosf(D3DX_PI * 0.5f + m_StoneInfo.rot.y) * xz;

			float forwardRandPos = UtilFunc::Transformation::Random(ForwardRange_Min, ForwardRange) * 0.1f;
			randpos.x += sinf(D3DX_PI + m_StoneInfo.rot.y) * forwardRandPos;
			randpos.z += cosf(D3DX_PI + m_StoneInfo.rot.y) * forwardRandPos;
			pos = posOrigin + randpos;


			float colorrand = UtilFunc::Transformation::Random(-colorRandRange, colorRandRange) * 0.01f;
			CEffect3D* pEffect = CEffect3D::Create(
				pos,
				move,
				D3DXCOLOR(myColor.x + colorrand, myColor.y + colorrand, myColor.z, myColor.w),
				radius,
				Life,
				CEffect3D::MOVEEFFECT::MOVEEFFECT_ADD,
				CEffect3D::TYPE::TYPE_SMOKE);
			pEffect->SetEnableGravity();
			pEffect->SetGravityValue(gravity);
		}

		// �C���^�[�o���X�V
		m_fIntervalSplash = DEFAULT_SPLASHTIME + UtilFunc::Transformation::Random(-Randrange, Randrange) * 0.01f;

		// ���Ԃ��^�C�}�[
		m_fSplashTimer = 0.0f;
	}
//#endif

	m_StoneInfo.pos = GetPosition();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CWaterStone::Draw()
{
	// �`��
	CObjectX::Draw();
}

//==========================================================================
// ���f���̎�ޕύX
//==========================================================================
void CWaterStone::ChangeModelType(int type)
{
	// X�t�@�C���̃f�[�^�擾
	CXLoad* pXLoad = CXLoad::GetInstance();

	// X�t�@�C���̃��[�h
	int modelIdx = pXLoad->XLoad(m_vecModelFile[type]);
	BindXData(modelIdx);
}
