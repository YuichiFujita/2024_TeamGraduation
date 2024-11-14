//==========================================================================
// 
//  �I�u�W�F�N�g3D�A�j������ [object3D_Anim.cpp]
//  Author : ���n�Ή�
//  Adder  : ���c�E��
// 
//==========================================================================
#include "object3D_Anim.h"
#include "calculation.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CObject3DAnim::CObject3DAnim(int nPriority) : CObject3D(nPriority),
	m_funcPattern	(nullptr),				// �p�^�[���ύX�֐��|�C���^
	m_ptrn			(MyLib::PosGrid2(1)),	// �e�N�X�`��������
	m_split			(MyLib::Vector2()),		// �X�v���C�g��
	m_pNextTime		(nullptr),				// �p�^�[���ύX����
	m_fCurTime		(0.0f),					// ���݂̑ҋ@����
	m_fCurWholeTime	(0.0f),					// ���݂̑S�̎���
	m_fMaxWholeTime	(0.0f),					// ���S�̎���
	m_nCurPtrn		(0),					// ���݂̃p�^�[��
	m_nMaxPtrn		(0),					// �p�^�[���̑���
	m_nNumLoop		(0),					// �p�^�[���J��Ԃ���
	m_bStop			(false),				// ��~�t���O
	m_bPlayBack		(false),				// �t�Đ��t���O
	m_bLoop			(true),					// ���[�v�t���O
	m_bFinish		(false),				// �I���t���O
	m_bAutoDeath	(false)					// �����j���t���O
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CObject3DAnim::~CObject3DAnim()
{

}

//==========================================================================
// ��������
//==========================================================================
CObject3DAnim *CObject3DAnim::Create
(
	const MyLib::PosGrid2& rPtrn,	// �e�N�X�`��������
	const MyLib::Vector3& rPos,		// �ʒu
	const float fNextTime,			// �p�^�[���ύX����
	const bool bAutoDeath,			// �����j���t���O
	int nPriority
)
{
	// �������̊m��
	CObject3DAnim* pObject3D = DEBUG_NEW CObject3DAnim(nPriority);
	if (pObject3D != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// �N���X�̏�����
		if (FAILED(pObject3D->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �N���X�̏I��
			SAFE_UNINIT(pObject3D);
			return nullptr;
		}

		// �������
		pObject3D->SetPosition(rPos);			// �ʒu�ݒ�
		pObject3D->SetOriginPosition(rPos);		// ���_�ʒu�ݒ�
		pObject3D->SetTexPtrn(rPtrn);			// �e�N�X�`���������ݒ�
		pObject3D->SetNextTime(fNextTime);		// �p�^�[���ύX���Ԑݒ�
		pObject3D->m_bAutoDeath = bAutoDeath;	// �����폜�t���O�ݒ�
	}

	return pObject3D;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObject3DAnim::Init()
{
	// �e�N���X�̏�����
	HRESULT hr = CObject3D::Init();
	if (FAILED(hr)) { return E_FAIL; }

	// �ʏ�Đ���������
	SetEnablePlayBack(false);

	// ���_���ݒ�
	SetVtx();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CObject3DAnim::Uninit()
{
	// �p�^�[���ύX���Ԃ̔j��
	SAFE_DEL_ARRAY(m_pNextTime);

	// �I������
	CObject3D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CObject3DAnim::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ��~���̏ꍇ������
	if (m_bStop) { return; }

	// �p�^�[���̍X�V
	assert(m_funcPattern != nullptr);
	if (m_funcPattern(fDeltaTime, fDeltaRate, fSlowRate))
	{ // �֐������Ŕj�����ꂽ�ꍇ

		return;
	}

	// �X�V����
	CObject3D::Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CObject3DAnim::SetVtx()
{
	// ���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���E���̃x�N�g��
	MyLib::Vector3 vecLine0 = pVtx[1].pos - pVtx[0].pos;
	MyLib::Vector3 vecLine1 = pVtx[2].pos - pVtx[0].pos;

	// �O�ϋ��߂�
	MyLib::Vector3 Nor[4];

	// �O��
	D3DXVec3Cross(&Nor[0], &vecLine0, &vecLine1);
	D3DXVec3Normalize(&Nor[0], &Nor[0]);

	Nor[0] = MyLib::Vector3(
		((vecLine0.y * vecLine1.z) - (vecLine0.z * vecLine1.y)),
		((vecLine0.z * vecLine1.x) - (vecLine0.x * vecLine1.z)),
		((vecLine0.x * vecLine1.y) - (vecLine0.y * vecLine1.x)));
	D3DXVec3Normalize(&Nor[0], &Nor[0]);

	// ���E���̃x�N�g��
	vecLine0 = pVtx[2].pos - pVtx[3].pos;
	vecLine1 = pVtx[1].pos - pVtx[3].pos;

	// �O�ϋ��߂�
	Nor[3] = MyLib::Vector3(
		((vecLine0.y * vecLine1.z) - (vecLine0.z * vecLine1.y)),
		((vecLine0.z * vecLine1.x) - (vecLine0.x * vecLine1.z)),
		((vecLine0.x * vecLine1.y) - (vecLine0.y * vecLine1.x)));

	// �O��
	D3DXVec3Cross(&Nor[3], &vecLine0, &vecLine1);
	D3DXVec3Normalize(&Nor[3], &Nor[3]);

	Nor[1] = (Nor[0] + Nor[3]) / 2.0f;
	Nor[2] = (Nor[0] + Nor[3]) / 2.0f;

	// �@���x�N�g���̐ݒ�
	pVtx[0].nor = Nor[0];
	pVtx[1].nor = Nor[1];
	pVtx[2].nor = Nor[2];
	pVtx[3].nor = Nor[3];

	MyLib::Vector3 size = GetSize();
	D3DXCOLOR col = GetColor();

	// �ʒu���X�V
	pVtx[0].pos = MyLib::Vector3(-size.x, +size.y, +size.z);
	pVtx[1].pos = MyLib::Vector3(+size.x, +size.y, +size.z);
	pVtx[2].pos = MyLib::Vector3(-size.x, -size.y, -size.z);
	pVtx[3].pos = MyLib::Vector3(+size.x, -size.y, -size.z);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// �e�N�X�`�����W�̐ݒ�
	int nWidthCurrent = m_nCurPtrn % m_ptrn.x;					// ���݂̉��̃p�^�[��
	int nHeightCurrent = (m_nCurPtrn / m_ptrn.x) % m_ptrn.y;	// ���݂̏c�̃p�^�[��
	pVtx[0].tex = D3DXVECTOR2(m_split.x *  nWidthCurrent,		m_split.y *  nHeightCurrent);
	pVtx[1].tex = D3DXVECTOR2(m_split.x * (nWidthCurrent + 1),	m_split.y *  nHeightCurrent);
	pVtx[2].tex = D3DXVECTOR2(m_split.x *  nWidthCurrent,		m_split.y * (nHeightCurrent + 1));
	pVtx[3].tex = D3DXVECTOR2(m_split.x * (nWidthCurrent + 1),	m_split.y * (nHeightCurrent + 1));

	// ���_�o�b�t�@���A�����b�N���b�N
	m_pVtxBuff->Unlock();
}

//============================================================
//	���݃p�^�[���̐ݒ菈��
//============================================================
void CObject3DAnim::SetCurPtrn(const int nPtrn)
{
	// �����̃p�^�[������ݒ�
	m_nCurPtrn = nPtrn;

	// �I���t���O��OFF�ɂ���
	m_bFinish = false;

	// �S�̎��Ԃ̏�����
	m_fCurWholeTime = 0.0f;

	// �S�̎��Ԃ��p�^�[���̊J�n���Ԃɐݒ�
	int nAddTime = (!m_bPlayBack) ? 0 : 1;	// �t�Đ��̏ꍇ�͌��݃p�^�[���̑ҋ@���Ԃ��܂�
	int nLoop = m_nCurPtrn + nAddTime;		// �J��Ԃ��������߂�
	for (int i = 0; i < nLoop; i++)
	{ // �J�n�p�^�[���܂ŌJ��Ԃ�

		// �ҋ@���Ԃ����Z
		m_fCurWholeTime += m_pNextTime[i];
	}

	// ���_�̐ݒ�
	SetVtx();
}

//============================================================
//	�e�N�X�`���������̐ݒ菈��
//============================================================
void CObject3DAnim::SetTexPtrn(const MyLib::PosGrid2 &rPtrn)
{
	// �����̃e�N�X�`����������ݒ�
	m_ptrn = rPtrn;

	// �X�v���C�g�ʂ�ݒ�
	m_split.x = 1.0f / static_cast<float>(m_ptrn.x);	// U�̃X�v���C�g�ʐݒ�
	m_split.y = 1.0f / static_cast<float>(m_ptrn.y);	// V�̃X�v���C�g�ʐݒ�

	// �p�^�[��������ݒ�
	SetMaxPtrn(m_ptrn.x * m_ptrn.y);
}

//============================================================
//	�e�N�X�`�����������̐ݒ菈��
//============================================================
void CObject3DAnim::SetTexPtrnWidth(const int nTexPtrnW)
{
	// �����̃e�N�X�`������������ݒ�
	m_ptrn.x = nTexPtrnW;

	// U�X�v���C�g�ʂ�ݒ�
	m_split.x = 1.0f / static_cast<float>(m_ptrn.x);

	// �p�^�[��������ݒ�
	SetMaxPtrn(m_ptrn.x * m_ptrn.y);
}

//============================================================
//	�e�N�X�`���c�������̐ݒ菈��
//============================================================
void CObject3DAnim::SetTexPtrnHeight(const int nTexPtrnH)
{
	// �����̃e�N�X�`���c��������ݒ�
	m_ptrn.y = nTexPtrnH;

	// V�X�v���C�g�ʂ�ݒ�
	m_split.y = 1.0f / static_cast<float>(m_ptrn.y);

	// �p�^�[��������ݒ�
	SetMaxPtrn(m_ptrn.x * m_ptrn.y);
}

//============================================================
//	�Đ��t���O�̐ݒ菈��
//============================================================
void CObject3DAnim::SetEnableStop(const bool bStop)
{
	// ��~�����ꍇ�ɌJ��Ԃ�����������
	if (!bStop) { m_nNumLoop = 0; }

	// �����̒�~�󋵂�ݒ�
	m_bStop = bStop;
}

//============================================================
//	�t�Đ��t���O�̐ݒ菈��
//============================================================
void CObject3DAnim::SetEnablePlayBack(const bool bPlayBack)
{
	// �Đ�/�t�Đ������]�����ꍇ�ɌJ��Ԃ����ƏI���t���O��������
	if (m_bPlayBack != bPlayBack) { m_nNumLoop = 0; m_bFinish = false; }

	// �����̋t�Đ��󋵂�ݒ�
	m_bPlayBack = bPlayBack;

	if (!m_bPlayBack)
	{ // �ʏ�Đ��̏ꍇ

		// �p�^�[�����Z�֐���ݒ�
		m_funcPattern = std::bind(&CObject3DAnim::NextPtrn, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	}
	else
	{ // �t�Đ��̏ꍇ

		// �p�^�[�����Z�֐���ݒ�
		m_funcPattern = std::bind(&CObject3DAnim::BackPtrn, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	}
}

//============================================================
//	���[�v�t���O�̐ݒ菈��
//============================================================
void CObject3DAnim::SetEnableLoop(const bool bLoop)
{
	// ���[�v����ꍇ�I���t���O��������
	if (bLoop) { m_bFinish = false; }

	// �����̃��[�v�󋵂�ݒ�
	m_bLoop = bLoop;
}

//============================================================
//	���݃p�^�[���̏���������
//============================================================
void CObject3DAnim::ResetCurPtrn()
{
	// �I���t���O��OFF�ɂ���
	m_bFinish = false;

	if (!m_bPlayBack)
	{ // �ʏ�Đ��̏ꍇ

		// �J�n�p�^�[���̏�����
		m_nCurPtrn = 0;

		// �ҋ@���Ԃ̏�����
		m_fCurTime = 0.0f;
		m_fCurWholeTime = 0.0f;
	}
	else
	{ // �t�Đ��̏ꍇ

		// �J�n�p�^�[���̏�����
		m_nCurPtrn = m_nMaxPtrn - 1;

		// �ҋ@���Ԃ̏�����
		m_fCurTime = m_pNextTime[m_nCurPtrn];
		m_fCurWholeTime = m_fMaxWholeTime;
	}
}

//============================================================
//	�p�^�[���ύX���Ԃ̐ݒ菈�� (�p�^�[���w��)
//============================================================
void CObject3DAnim::SetNextTime(const int nPtrnIdx, const float fNextTime)
{
	// �p�^�[���C���f�b�N�X���͈͊O�̏ꍇ������
	if (nPtrnIdx <= -1 || nPtrnIdx >= m_nMaxPtrn) { assert(false); return; }

	// �ύX���Ԃ��v���X�ł͂Ȃ��ꍇ������
	if (fNextTime <= 0.0f) { assert(false); return; }

	// �ύX�O�̃p�^�[���ύX���Ԃ�ۑ�
	float fOldNextTime = m_pNextTime[nPtrnIdx];

	// �����̃p�^�[���ύX���Ԃ�ݒ�
	m_pNextTime[nPtrnIdx] = fNextTime;

	// ���S�̎��Ԃ�ύX
	m_fMaxWholeTime -= fOldNextTime;
	m_fMaxWholeTime += fNextTime;
}

//============================================================
//	�p�^�[���ύX���Ԃ̐ݒ菈�� (�S�p�^�[��)
//============================================================
void CObject3DAnim::SetNextTime(const float fNextTime)
{
	// �ύX���Ԃ��v���X�ł͂Ȃ��ꍇ������
	if (fNextTime <= 0.0f) { assert(false); return; }

	for (int i = 0; i < m_nMaxPtrn; i++)
	{ // �p�^�[���̑������J��Ԃ�

		// �����̃p�^�[���ύX���Ԃ�ݒ�
		m_pNextTime[i] = fNextTime;
	}

	// ���S�̎��Ԃ�ύX
	m_fMaxWholeTime = fNextTime * (float)m_nMaxPtrn;
}

//============================================================
//	�p�^�[�������̐ݒ菈��
//============================================================
HRESULT CObject3DAnim::SetMaxPtrn(const int nMaxPtrn)
{
	// �����̃p�^�[���̑�����ݒ�
	m_nMaxPtrn = nMaxPtrn;

	// �p�^�[���ύX���Ԃ̔j��
	SAFE_DEL_ARRAY(m_pNextTime);

	// �p�^�[���ύX���Ԃ̍Đ���
	m_pNextTime = new float[m_nMaxPtrn];
	if (m_pNextTime == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �p�^�[���ύX���Ԃ�������
	SetNextTime(DEF_NEXT);

	// ���_�̐ݒ�
	SetVtx();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�p�^�[�����Z����
//============================================================
bool CObject3DAnim::NextPtrn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �A�j���[�V�������I�����Ă���ꍇ������
	if (m_bFinish) { return false; }

	// ���݂̑ҋ@���Ԃ����Z
	m_fCurTime += fDeltaTime * fDeltaRate * fSlowRate;
	m_fCurWholeTime += fDeltaTime * fDeltaRate * fSlowRate;

	if (m_fCurTime >= m_pNextTime[m_nCurPtrn])
	{ // �ҋ@���I������ꍇ

		// ���݂̑ҋ@���Ԃ��獡��̑ҋ@���Ԃ����Z
		m_fCurTime -= m_pNextTime[m_nCurPtrn];

		// �p�^�[�������Z
		m_nCurPtrn = (m_nCurPtrn + 1) % m_nMaxPtrn;
		if (m_nCurPtrn == 0)
		{ // �擪�p�^�[���̏ꍇ

			if (m_bLoop)
			{ // ���[�v����ꍇ

				// �J��Ԃ��������Z
				m_nNumLoop++;

				// �S�̎��Ԃ�������
				m_fCurWholeTime = 0.0f;
			}
			else
			{ // ���[�v���Ȃ��ꍇ

				// �p�^�[����␳
				m_nCurPtrn = m_nMaxPtrn - 1;

				// �S�̎��Ԃ�������
				m_fCurWholeTime = m_fMaxWholeTime;

				// �A�j���[�V�����I����ۑ�
				m_bFinish = true;
			}

			if (m_bAutoDeath)
			{ // �����j����ON�̏ꍇ

				// �I�u�W�F�N�g�j��
				Uninit();
				return true;
			}
		}
	}

	return false;
}

//============================================================
//	�p�^�[�����Z����
//============================================================
bool CObject3DAnim::BackPtrn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �A�j���[�V�������I�����Ă���ꍇ������
	if (m_bFinish) { return false; }

	// ���݂̑ҋ@���Ԃ����Z
	m_fCurTime -= fDeltaTime * fDeltaRate * fSlowRate;
	m_fCurWholeTime -= fDeltaTime * fDeltaRate * fSlowRate;

	if (m_fCurTime <= 0.0f)
	{ // �ҋ@���I������ꍇ

		// ���݂̑ҋ@���Ԃ��獡��̑ҋ@���Ԃ����Z
		m_fCurTime += m_pNextTime[m_nCurPtrn];

		// �p�^�[�������Z
		m_nCurPtrn = (m_nCurPtrn + (m_nMaxPtrn - 1)) % m_nMaxPtrn;

		if (m_nCurPtrn == m_nMaxPtrn - 1)
		{ // �ŏI�p�^�[���̏ꍇ

			if (m_bLoop)
			{ // ���[�v����ꍇ

				// �J��Ԃ��������Z
				m_nNumLoop++;

				// �S�̎��Ԃ�������
				m_fCurWholeTime = m_fMaxWholeTime;
			}
			else
			{ // ���[�v���Ȃ��ꍇ

				// �p�^�[����␳
				m_nCurPtrn = 0;

				// �S�̎��Ԃ�������
				m_fCurWholeTime = 0.0f;

				// �A�j���[�V�����I����ۑ�
				m_bFinish = true;
			}

			if (m_bAutoDeath)
			{ // �����j����ON�̏ꍇ

				// �I�u�W�F�N�g�j��
				Uninit();
				return true;
			}
		}
	}

	return false;
}
