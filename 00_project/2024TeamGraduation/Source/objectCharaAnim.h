#if 0
//============================================================
//
//	�I�u�W�F�N�g�L�����N�^�[�A�j���[�V�����w�b�_�[ [objectCharaAnim.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _OBJECTCHARA_ANIM_H_
#define _OBJECTCHARA_ANIM_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object3D_Anim.h"

//************************************************************
//	�N���X��`
//************************************************************
// �I�u�W�F�N�g�L�����N�^�[�A�j���[�V�����N���X
class CObjectCharaAnim : public CObject3DAnim
{
public:
	// �R���X�g���N�^
	explicit CObjectCharaAnim(int nPriority = mylib_const::PRIORITY_DEF2D);

	// �f�X�g���N�^
	~CObjectCharaAnim() override;

	// �����蔻��Ǘ��\����
	struct SColl
	{
		// �R���X�g���N�^
		SColl() :
			offset	(VEC3_ZERO),	// ���茴�_�I�t�Z�b�g
			size	(VEC3_ZERO)		// ����傫��
		{}

		// �����o�ϐ�
		MyLib::Vector3 offset;	// ���茴�_�I�t�Z�b�g
		MyLib::Vector3 size;	// ����傫��
	};

	// �L�����N�^�[�Ǘ��\����
	struct SChara
	{
		// �R���X�g���N�^
		SChara() :
			ptrnTexture	(MyLib::PosGrid2()),	// �e�N�X�`��������
			nMaxPtrn	(0),			// �ő�p�^�[����
			offset		(VEC3_ZERO),	// ���_�I�t�Z�b�g
			size		(VEC3_ZERO),	// �L�����N�^�[�傫��
			bLoop		(false)			// ���[�vON/OFF
		{
			vecNextTime.clear();	// �p�^�[���ύX���Ԕz����N���A
			sPathTexture.clear();	// �e�N�X�`���p�X���N���A
		}

		// �e�N�X�`���������E�p�^�[�������̐ݒ�
		inline HRESULT SetTexPtrn(const MyLib::PosGrid2& rPtrn)
		{
			// �e�N�X�`���������E�p�^�[��������ݒ�
			ptrnTexture = rPtrn;
			nMaxPtrn = rPtrn.x * rPtrn.y;

			// �p�^�[���ύX���Ԃ̔j��
			vecNextTime.clear();

			// �p�^�[���ύX���Ԃ̐���
			vecNextTime.resize(nMaxPtrn);

			// �p�^�[���ύX���Ԃ�������
			if (FAILED(SetNextTime(DEF_NEXT))) { return E_FAIL; }

			return S_OK;
		}

		// �p�^�[���ύX���Ԃ̐ݒ�
		inline HRESULT SetNextTime(const float fNextTime)
		{
			if (fNextTime <= 0.0f)					 { return E_FAIL; }	// �ύX���Ԃ��v���X�ł͂Ȃ��ꍇ���s
			if (nMaxPtrn != (int)vecNextTime.size()) { return E_FAIL; }	// �p�^�[���ő吔�Ɠ����T�C�Y�ł͂Ȃ��ꍇ���s

			for (int i = 0; i < nMaxPtrn; i++)
			{
				// �����̃p�^�[���ύX���Ԃ�ݒ�
				vecNextTime[i] = fNextTime;
			}

			return S_OK;
		}

		// �����o�ϐ�
		std::vector<float> vecNextTime;	// �p�^�[���ύX���Ԕz��
		std::string sPathTexture;		// �e�N�X�`���p�X
		MyLib::PosGrid2 ptrnTexture;	// �e�N�X�`��������
		int nMaxPtrn;			// �ő�p�^�[����
		MyLib::Vector3 offset;	// ���_�I�t�Z�b�g
		MyLib::Vector3 size;	// �L�����N�^�[�傫��
		bool bLoop;				// ���[�vON/OFF
	};

	// ���[�V�����Ǘ��\����
	struct SMotion
	{
		// �R���X�g���N�^
		SMotion() :
			fCancelTime	(-1.0f),	// �L�����Z���\����
			fComboTime	(-1.0f)		// �R���{�\����
		{}

		// �����o�ϐ�
		SChara infoChara;	// �L�����N�^�[���
		SColl infoColl;		// �����蔻����
		float fCancelTime;	// �L�����Z���\����
		float fComboTime;	// �R���{�\����
	};

	// ���[�V�������\����
	struct SInfo
	{
		// �R���X�g���N�^
		SInfo() :
			nType	(0)	// ���[�V�������
		{
			vecMotion.clear();	// ���[�V���������N���A
		}

		// �����o�֐�
		inline int GetNumMotion() { return (int)vecMotion.size(); }	// ���[�V�������̑����擾

		// �����o�ϐ�
		std::vector<SMotion> vecMotion;	// ���[�V�������
		int nType;	// ���[�V�������
	};

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V
	void Draw() override;		// �`��

	// �ÓI�����o�֐�
	static CObjectCharaAnim* Create(const MyLib::Vector3& rPos, const MyLib::Vector3& rRot = VEC3_ZERO);	// ����

	// �����o�֐�
	void BindCharaData(const char* pCharaPath);	// �L�����N�^�[��񊄓�
	void SetMotion(const int nType);			// ���[�V�����ݒ�
	void AddInfo(const SMotion& rMotion);		// ���[�V�������ǉ�
	void SetAllInfo(const SInfo& rInfo);		// ���[�V�������S�ݒ�
	bool IsCancel() const;	// �L�����Z���擾
	bool IsCombo() const;	// �R���{�擾
	inline int GetMotion() const	{ return m_info.nType; }			// ���[�V�����擾
	inline int GetNumType()			{ return m_info.GetNumMotion(); }	// ��ޑ����擾
	inline float GetCancelTime(const int nType) const	{ return m_info.vecMotion[nType].fCancelTime; }				// �L�����Z�����Ԏ擾
	inline float GetComboTime(const int nType) const	{ return m_info.vecMotion[nType].fComboTime; }				// �R���{���Ԏ擾
	inline MyLib::Vector3 GetCollSize() const			{ return m_info.vecMotion[m_info.nType].infoColl.size; }	// ����傫���擾

	MyLib::Vector3 CalcOriginOffsetPosition(const MyLib::Vector3& rPos, const MyLib::Vector3& rRot) const;	// ���_�I�t�Z�b�g���f�ʒu�v�Z
	MyLib::Vector3 CalcCollOffsetPosition(const MyLib::Vector3& rPos, const MyLib::Vector3& rRot) const;	// ���茴�_�I�t�Z�b�g���f�ʒu�v�Z
	MyLib::Vector3 GetOriginOffsetPosition() const;	// ���_�I�t�Z�b�g���f�ʒu�擾
	MyLib::Vector3 GetCollOffsetPosition() const;	// ���茴�_�I�t�Z�b�g���f�ʒu�擾

private:
	// �I�[�o�[���C�h�֐�
	void CalWorldMtx() override;	// �`��}�g���b�N�X�v�Z

	// �����o�֐�
	MyLib::Vector3 CalcOffsetPosition	// �I�t�Z�b�g���f�ʒu�v�Z
	( // ����
		const MyLib::Vector3& rPos,		// �ʒu
		const MyLib::Vector3& rRot,		// ����
		const MyLib::Vector3& rOffset	// �I�t�Z�b�g
	) const;

	// �����o�ϐ�
	SInfo m_info;	// ���[�V�������
};

#endif	// _OBJECTCHARA_ANIM_H_
#endif