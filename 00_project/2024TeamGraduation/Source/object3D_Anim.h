//==========================================================================
// 
//  �I�u�W�F�N�g3D�A�j���w�b�_�[ [object3D_Anim.h]
//  Author : ���n�Ή�
//  Adder  : ���c�E��
// 
//==========================================================================

#ifndef _OBJECT3D_ANIM_H_
#define _OBJECT3D_ANIM_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "object3D.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �I�u�W�F�N�g3D�A�j���N���X��`
class CObject3DAnim : public CObject3D
{
public:

	//=============================
	// �萔
	//=============================
	static constexpr float DEF_NEXT = 1.0f;	// �p�^�[���ύX���� (�f�t�H���g)

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CObject3DAnim(int nPriority = mylib_const::PRIORITY_DEF2D);
	virtual ~CObject3DAnim();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void SetVtx() override;

	//=============================
	// �����o�֐�
	//=============================
	void SetCurPtrn(const int nPtrn);				// ���݃p�^�[���ݒ�
	void SetTexPtrn(const MyLib::PosGrid2& rPtrn);	// �e�N�X�`���������ݒ�
	void SetTexPtrnWidth(const int nTexPtrnW);		// �e�N�X�`�����������ݒ�
	void SetTexPtrnHeight(const int nTexPtrnH);		// �e�N�X�`���c�������ݒ�
	void SetEnableStop(const bool bStop);			// ��~�t���O�ݒ�
	void SetEnablePlayBack(const bool bPlayBack);	// �t�Đ��t���O�ݒ�
	void SetEnableLoop(const bool bLoop);			// ���[�v�t���O�ݒ�
	void ResetCurPtrn();							// ���݃p�^�[��������
	void SetNextTime(const int nPtrnIdx, const float fNextTime);	// �p�^�[���ύX���Ԑݒ� (�p�^�[���w��)
	void SetNextTime(const float fNextTime);						// �p�^�[���ύX���Ԑݒ� (�S�p�^�[��)

	inline int GetCurPtrn() const	{ return m_nCurPtrn; }	// ���݃p�^�[���擾
	inline void ResetNumLoop()		{ m_nNumLoop = 0; }		// �p�^�[���J��Ԃ���������
	inline int GetLoopAnim() const	{ return m_nNumLoop; }	// �p�^�[���J��Ԃ����擾
	inline bool IsStop() const		{ return m_bStop; }		// ��~�t���O�擾
	inline bool IsPlayBack() const	{ return m_bPlayBack; }	// �t�Đ��t���O�擾
	inline bool IsLoop() const		{ return m_bLoop; }		// ���[�v�t���O�擾
	inline bool IsFinish() const	{ return m_bFinish; }	// �I���t���O�擾
	inline float GetCurTime() const			{ return m_fCurTime; }		// ���݂̑ҋ@����
	inline float GetCurWholeTime() const	{ return m_fCurWholeTime; }	// ���݂̑S�̎���
	inline float GetMaxWholeTime() const	{ return m_fMaxWholeTime; }	// ���S�̎���
	inline void SetDesableAutoDeath()		{ m_bAutoDeath = false; }	// �����폜�̔���폜
	inline CObject3DAnim *GetObject3DAnim()	{ return this; }			// ���g�̌^�I�u�W�F�N�g�擾

	//=============================
	// �ÓI�����o�֐�
	//=============================
	static CObject3DAnim* Create	// ����
	( // ����
		const MyLib::PosGrid2& rPtrn,		// �e�N�X�`��������
		const MyLib::Vector3& rPos,			// �ʒu
		const float fNextTime = DEF_NEXT,	// �p�^�[���ύX����
		const bool bAutoDeath = false		// �����j���t���O
	);

private:

	//=============================
	// �����o�֐�
	//=============================
	HRESULT SetMaxPtrn(const int nMaxPtrn);	// �p�^�[�������ݒ�
	bool NextPtrn(const float fDeltaTime, const float fSlowRate);	// �p�^�[�����Z
	bool BackPtrn(const float fDeltaTime, const float fSlowRate);	// �p�^�[�����Z

	//=============================
	// �����o�ϐ�
	//=============================
	std::function<bool(float, float)> m_funcPattern;	// �p�^�[���ύX�֐��|�C���^
	MyLib::PosGrid2 m_ptrn;	// �e�N�X�`��������
	MyLib::Vector2 m_split;	// �X�v���C�g��
	float* m_pNextTime;		// �p�^�[���ύX����
	float m_fCurTime;		// ���݂̑ҋ@����
	float m_fCurWholeTime;	// ���݂̑S�̎���
	float m_fMaxWholeTime;	// ���S�̎���
	int m_nCurPtrn;			// ���݂̃p�^�[��
	int m_nMaxPtrn;			// �p�^�[������
	int m_nNumLoop;			// �p�^�[���J��Ԃ���
	bool m_bStop;			// ��~�t���O
	bool m_bPlayBack;		// �t�Đ��t���O
	bool m_bLoop;			// ���[�v�t���O
	bool m_bFinish;			// �I���t���O
	bool m_bAutoDeath;		// �����j���t���O
};



#endif