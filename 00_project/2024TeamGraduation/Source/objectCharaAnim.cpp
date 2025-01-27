//============================================================
//
//	�I�u�W�F�N�g�L�����N�^�[�A�j���[�V�������� [objectCharaAnim.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectCharaAnim.h"
#include "manager.h"
#include "renderer.h"
#include "characterAnim.h"

//************************************************************
//	�q�N���X [CObjectCharaAnim] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObjectCharaAnim::CObjectCharaAnim(int nPriority) : CObject3DAnim(nPriority)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CObjectCharaAnim::~CObjectCharaAnim()
{

}

//============================================================
//	����������
//============================================================
HRESULT CObjectCharaAnim::Init()
{
	// �A�j���[�V����3D�̏�����
	if (FAILED(CObject3DAnim::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CObjectCharaAnim::Uninit()
{
	// �A�j���[�V����3D�̏I��
	CObject3DAnim::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CObjectCharaAnim::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���[�V�������Ȃ��ꍇ������
	if (m_info.vecMotion.empty()) { assert(false); return; }

	// �A�j���[�V����3D�̍X�V
	CObject3DAnim::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//============================================================
//	�`�揈��
//============================================================
void CObjectCharaAnim::Draw()
{
	// �A�j���[�V����3D�̕`��
	CObject3DAnim::Draw();
}

//============================================================
//	��������
//============================================================
CObjectCharaAnim* CObjectCharaAnim::Create(const MyLib::Vector3& rPos, const MyLib::Vector3& rRot)
{
	// �I�u�W�F�N�g�L�����N�^�[�A�j���[�V�����̐���
	CObjectCharaAnim* pObjectCharaAnim = DEBUG_NEW CObjectCharaAnim;
	if (pObjectCharaAnim == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �I�u�W�F�N�g�L�����N�^�[�A�j���[�V�����̏�����
		if (FAILED(pObjectCharaAnim->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �I�u�W�F�N�g�L�����N�^�[�A�j���[�V�����̔j��
			SAFE_DELETE(pObjectCharaAnim);
			return nullptr;
		}

		// �ʒu��ݒ�
		pObjectCharaAnim->SetPosition(rPos);

		// ������ݒ�
		pObjectCharaAnim->SetRotation(rRot);

		// �m�ۂ����A�h���X��Ԃ�
		return pObjectCharaAnim;
	}
}

//============================================================
//	�L�����N�^�[��񊄓�
//============================================================
void CObjectCharaAnim::BindCharaData(const char* pCharaPath)
{
	// ���蓖�Ă郂�[�V�����p�X�����݂��Ȃ��ꍇ������
	if (pCharaPath == nullptr) { assert(false); return; }

	// ���[�V�������̑S�ݒ�
	SetAllInfo(GET_MANAGER->GetCharacterAnim()->Regist(pCharaPath));
}

//============================================================
//	���[�V�����̐ݒ菈��
//============================================================
void CObjectCharaAnim::SetMotion(const int nType)
{
	// �w�肳�ꂽ���[�V���������݂��Ȃ��ꍇ������
	if (nType <= -1 || nType >= m_info.GetNumMotion()) { assert(false); return; }

	// �����̃��[�V�����̎�ނ�ݒ�
	m_info.nType = nType;

	// �ϐ����|�C���^�����ȗ���
	SChara* pInfoChara = &m_info.vecMotion[nType].infoChara;	// �L�����N�^�[���

	// �w��e�N�X�`���̊���
	BindTexture(CTexture::GetInstance()->Regist(pInfoChara->sPathTexture.c_str()));

	// �L�����N�^�[����ݒ�
	SetTexPtrn(pInfoChara->ptrnTexture);	// �e�N�X�`��������
	SetEnableLoop(pInfoChara->bLoop);		// ���[�vON/OFF
	SetSize(pInfoChara->size);				// �傫��

	assert((int)pInfoChara->vecNextTime.size() == pInfoChara->nMaxPtrn);
	for (int i = 0; i < pInfoChara->nMaxPtrn; i++)
	{
		// �p�^�[���ύX���Ԃ�ݒ�
		SetNextTime(pInfoChara->vecNextTime[i]);
	}

	// �L�����N�^�[����������
	ResetCurPtrn();	// �J�n�p�^�[��
	ResetNumLoop();	// �J��Ԃ���
}

//============================================================
//	���[�V�������̒ǉ�����
//============================================================
void CObjectCharaAnim::AddInfo(const SMotion& rMotion)
{
	int nSetMotionIdx = m_info.GetNumMotion();	// ���[�V������ݒ肷��z��ԍ�

	// ��̗v�f���Ō���ɒǉ�
	m_info.vecMotion.emplace_back();

	// �����̃��[�V��������ݒ�
	m_info.vecMotion[nSetMotionIdx] = rMotion;
}

//============================================================
//	���[�V�������S�ݒ菈��
//============================================================
void CObjectCharaAnim::SetAllInfo(const SInfo& rInfo)
{
	// ���[�V���������N���A
	m_info.vecMotion.clear();

	for (auto& rVec : rInfo.vecMotion)
	{ // �ǂݍ��񂾃��[�V���������J��Ԃ�

		// ���[�V�������̒ǉ�
		AddInfo(rVec);
	}
}

//============================================================
//	�L�����Z���擾����
//============================================================
bool CObjectCharaAnim::IsCancel() const
{
	float fCancelTime = m_info.vecMotion[m_info.nType].fCancelTime;	// �L�����Z���\����
	if (fCancelTime >= 0.0f)
	{ // �L�����Z���\���Ԃ��ݒ肳��Ă���ꍇ

		// ���[�V�����̃L�����Z���󋵂�Ԃ�
		return (GetCurWholeTime() >= fCancelTime);
	}

	// �L�����Z���s��Ԃ�
	return false;
}

//============================================================
//	�R���{�擾����
//============================================================
bool CObjectCharaAnim::IsCombo() const
{
	float fComboTime = m_info.vecMotion[m_info.nType].fComboTime;	// �R���{�\����
	if (fComboTime >= 0.0f)
	{ // �R���{�\���Ԃ��ݒ肳��Ă���ꍇ

		// ���[�V�����̃R���{�󋵂�Ԃ�
		return (GetCurWholeTime() >= fComboTime);
	}

	// �R���{�s��Ԃ�
	return false;
}

//============================================================
//	���_�I�t�Z�b�g���f�ʒu�̌v�Z����
//============================================================
MyLib::Vector3 CObjectCharaAnim::CalcOriginOffsetPosition(const MyLib::Vector3& rPos, const MyLib::Vector3& rRot) const
{
	// �I�t�Z�b�g���f�ʒu�̌v�Z���ʂ�Ԃ�
	const MyLib::Vector3& rOffset = m_info.vecMotion[m_info.nType].infoChara.offset;	// �I�t�Z�b�g
	return CalcOffsetPosition(rPos, rRot, rOffset);
}

//============================================================
//	���茴�_�I�t�Z�b�g���f�ʒu�̌v�Z����
//============================================================
MyLib::Vector3 CObjectCharaAnim::CalcCollOffsetPosition(const MyLib::Vector3& rPos, const MyLib::Vector3& rRot) const
{
	// �I�t�Z�b�g���f�ʒu�̌v�Z���ʂ�Ԃ�
	const MyLib::Vector3& rOffset = m_info.vecMotion[m_info.nType].infoColl.offset;	// �I�t�Z�b�g
	return CalcOffsetPosition(rPos, rRot, rOffset);
}

//============================================================
//	���_�I�t�Z�b�g���f�ʒu�̎擾����
//============================================================
MyLib::Vector3 CObjectCharaAnim::GetOriginOffsetPosition() const
{
	// ���_�I�t�Z�b�g���f�ʒu�̌v�Z���ʂ�Ԃ�
	const MyLib::Vector3& rPos = GetPosition();	// �ʒu
	const MyLib::Vector3& rRot = GetRotation();	// ����
	return CalcOriginOffsetPosition(rPos, rRot);
}

//============================================================
//	���茴�_�I�t�Z�b�g���f�ʒu�̎擾����
//============================================================
MyLib::Vector3 CObjectCharaAnim::GetCollOffsetPosition() const
{
	// ���茴�_�I�t�Z�b�g���f�ʒu�̌v�Z���ʂ�Ԃ�
	const MyLib::Vector3& rPos = GetPosition();	// �ʒu
	const MyLib::Vector3& rRot = GetRotation();	// ����
	return CalcCollOffsetPosition(rPos, rRot);
}

//============================================================
//	�`��}�g���b�N�X�̌v�Z����
//============================================================
void CObjectCharaAnim::CalWorldMtx()
{
	MyLib::Vector3 offset = m_info.vecMotion[m_info.nType].infoChara.offset;	// �I�t�Z�b�g
	MyLib::Matrix mtxWorld = GetWorldMtx();		// ���[���h�}�g���b�N�X
	MyLib::Matrix mtxRot, mtxTrans, mtxOffset;	// �v�Z�p�}�g���b�N�X
	MyLib::Vector3 pos = GetPosition();			// �ʒu
	MyLib::Vector3 rot = GetRotation();			// ����

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld.Identity();

	// �ʒu�𔽉f����
	mtxOffset.Translation(offset);
	mtxWorld.Multiply(mtxWorld, mtxOffset);

	// �����𔽉f����
	mtxRot.RotationYawPitchRoll(rot.y, rot.x, rot.z);
	mtxWorld.Multiply(mtxWorld, mtxRot);

	// �ʒu�𔽉f����
	mtxTrans.Translation(pos);
	mtxWorld.Multiply(mtxWorld, mtxTrans);

	// ���[���h�}�g���b�N�X�𔽉f
	SetWorldMtx(mtxWorld);
}

//============================================================
//	�I�t�Z�b�g���f�ʒu�̌v�Z����
//============================================================
MyLib::Vector3 CObjectCharaAnim::CalcOffsetPosition
(
	const MyLib::Vector3& rPos,		// �ʒu
	const MyLib::Vector3& rRot,		// ����
	const MyLib::Vector3& rOffset	// �I�t�Z�b�g
) const
{
	MyLib::Matrix mtxWorld, mtxRot, mtxTrans, mtxOffset;	// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld.Identity();

	// �ʒu�𔽉f����
	mtxOffset.Translation(rOffset);
	mtxWorld.Multiply(mtxWorld, mtxOffset);

	// �����𔽉f����
	mtxRot.RotationYawPitchRoll(rRot.y, rRot.x, rRot.z);
	mtxWorld.Multiply(mtxWorld, mtxRot);

	// �ʒu�𔽉f����
	mtxTrans.Translation(rPos);
	mtxWorld.Multiply(mtxWorld, mtxTrans);

	// �Z�o�����}�g���b�N�X�̈ʒu��Ԃ�
	return mtxWorld.GetWorldPosition();
}
