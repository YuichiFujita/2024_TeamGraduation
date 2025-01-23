//=============================================================================
// 
// ���e�l�}�l�[�W������ [charmValueManager.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "charmValueManager.h"
#include "manager.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const float ADDVALUE[CCharmValueManager::ETypeAdd::ADD_MAX] =	// ���Z��
	{
		1.0f,	// �q�b�g
		0.6f,	// �L���b�`
		2.0f,	// �W���X�g�L���b�`
		3.0f,	// �J�o�[�L���b�`
		1.0f,	// �W���X�g�L���b�`����̓���
		1.0f,	// �h���b�v�L���b�`����̓���
		4.0f,	// ���
		10.0f,	// �X�y�V����
		10.0f,	// �X�y�V�������L���b�`����
		3.0f,	// �h��
	};

	const float SUBVALUE[CCharmValueManager::ETypeSub::SUB_MAX] =	// ���Z��
	{
		1.0f,	// ���C���z���Ė߂��Ă�Œ��ɂ�����
		2.0f,	// �[�ɓ����܂���
		3.0f,	// �����Ă����ă��C���z��(�{�[������)
		4.0f,	// �����ƃ{�[�������ē����Ȃ�
		10.0f,	// �X�y�V�������L���b�`���ꂽ
	};
}

//==========================================================================
// �X�^�e�B�b�N�A�T�[�g
//==========================================================================
static_assert(NUM_ARRAY(ADDVALUE) == CCharmValueManager::ETypeAdd::ADD_MAX, "ERROR : Type Count Mismatch");
static_assert(NUM_ARRAY(SUBVALUE) == CCharmValueManager::ETypeSub::SUB_MAX, "ERROR : Type Count Mismatch");

//==========================================================================
// �ÓI�����o�ϐ�
//==========================================================================
CCharmValueManager* CCharmValueManager::m_pThisPtr = nullptr;	// ���g�̃|�C���^

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCharmValueManager::CCharmValueManager()
{
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CCharmValueManager::~CCharmValueManager()
{

}

//==========================================================================
// ��������
//==========================================================================
CCharmValueManager* CCharmValueManager::Create()
{
	if (m_pThisPtr != nullptr)
	{// ���ɂ���ꍇ
		return m_pThisPtr;
	}

	// �������̊m��
	m_pThisPtr = DEBUG_NEW CCharmValueManager;

	if (m_pThisPtr != nullptr)
	{
		// ����������
		m_pThisPtr->Init();
	}

	return m_pThisPtr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CCharmValueManager::Init()
{
	for (int i = 0; i < CCharmValueManager::ETypeAdd::ADD_MAX; i++)
	{
		m_fAddValue[i] = ADDVALUE[i];
	}
	
	for (int i = 0; i < CCharmValueManager::ETypeSub::SUB_MAX; i++)
	{
		m_fSubValue[i] = SUBVALUE[i];
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CCharmValueManager::Uninit()
{
	delete m_pThisPtr;
	m_pThisPtr = nullptr;
}

//==========================================================================
// ���Z�ʎ擾
//==========================================================================
float CCharmValueManager::GetAddValue(ETypeAdd type)
{
	// �͈͊O
	if (ETypeAdd::ADD_MAX <= type) return 0.0f;

	return m_fAddValue[type];
}

//==========================================================================
// ���Z�ʎ擾
//==========================================================================
float CCharmValueManager::GetSubValue(ETypeSub type)
{
	// �͈͊O
	if (ETypeSub::SUB_MAX <= type) return 0.0f;

	return m_fSubValue[type];
}

//==========================================================================
// �f�o�b�O
//==========================================================================
void CCharmValueManager::Debug()
{
	if (ImGui::TreeNode("CCharmValueManager"))
	{// ���e�l

		if (ImGui::TreeNode("Add"))
		{// ���Z

			for (int i = 0; i < ETypeAdd::ADD_MAX; i++)
			{
				ETypeAdd type = static_cast<ETypeAdd>(i);
				std::string label = magic_enum::enum_name(type).data();
				ImGui::DragFloat(label.c_str(), &m_fAddValue[i], 0.01f, 0.0f, 100.0f, "%f");
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Sub"))
		{// ���Z

			for (int i = 0; i < ETypeSub::SUB_MAX; i++)
			{
				ETypeSub type = static_cast<ETypeSub>(i);
				std::string label = magic_enum::enum_name(type).data();
				ImGui::DragFloat(label.c_str(), &m_fAddValue[i], 0.01f, 0.0f, 100.0f, "%.2f");
			}

			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}
