//=============================================================================
// 
//  �}���`�i���o�[�w�b�_�[ [multinumber.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _MULTINUMBER_H_
#define _MULTINUMBER_H_	// ��d�C���N���[�h�h�~


//==========================================================================
// �O���錾
//==========================================================================
#include "object.h"

//==========================================================================
// �O���錾
//==========================================================================
class CNumber;

//==========================================================================
// �N���X��`
//==========================================================================
// �}���`�i���o�[�N���X
class CMultiNumber : public CObject
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	// �I�u�W�F�N�g���
	enum EObjType
	{
		OBJTYPE_NONE = 0,	// �Ȃ�
		OBJTYPE_2D,			// 2D�|���S��
		OBJTYPE_3D,			// 3D�|���S��
		OBJTYPE_BILLBOARD,	// �r���{�[�h
		OBJTYPE_MAX			// ���̗񋓌^�̑���
	};

	// �񂹎��
	enum EAlignmentType
	{
		ALIGNMENT_LEFT = 0,	// ����
		ALIGNMENT_RIGHT,	// �E��
		ALIGNMENT_CENTER,	// ������
		ALIGNMENT_MAX
	};

	CMultiNumber(int nPriority = 8);
	~CMultiNumber();
	
	/**
	@brief	��������
	@param	nNum		[in]	����
	@param	texIdx		[in]	�e�N�X�`���̃C���f�b�N�X
	@param	pos			[in]	�ʒu
	@param	size		[in]	�T�C�Y
	@param	nNum		[in]	����
	@param	objtype		[in]	�I�u�W�F�N�g�̎�� [2D / 3D / �r���r�[�h]
	@param	bDigitDraw	[in]	�L�P�^���̂ݕ`�悷�邩�̔���
	@param	nPriority	[in]	�`��D�揇��
	@return	CMultiNumber*
	*/
	static CMultiNumber* Create(const int nNum, const int texIdx, const MyLib::Vector3& pos, const MyLib::Vector2& size, int nMaxDigit, EObjType objtype, bool bDigitDraw = false, int nPriority = 8);

	// �����o�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;
	void Kill() override;	// �폜����

	//--------------------------
	// �ʒu
	//--------------------------
	virtual void SetPosition(const MyLib::Vector3& pos);		// �ʒu�ݒ�
	virtual void SetOldPosition(const MyLib::Vector3& pos);		// �ߋ��̈ʒu�ݒ�
	virtual void SetOriginPosition(const MyLib::Vector3& pos);	// ���̈ʒu�ݒ�

	//--------------------------
	// ����
	//--------------------------
	virtual void SetRotation(const MyLib::Vector3& rot);		// �����ݒ�
	virtual void SetOldRotation(const MyLib::Vector3& rot);		// �O��̌����ݒ�
	virtual void SetOriginRotation(const MyLib::Vector3& rot);	// ���̌����ݒ�

	//--------------------------
	// �T�C�Y
	//--------------------------
	virtual void SetSize(const MyLib::Vector2& size);						// �T�C�Y�ݒ�
	virtual MyLib::Vector2 GetSize() const { return m_size; }				// �T�C�Y�擾
	virtual void SetSizeOrigin(const MyLib::Vector2& size);					// ���̃T�C�Y�ݒ�
	virtual MyLib::Vector2 GetSizeOrigin() const { return m_sizeOrigin; }	// ���̃T�C�Y�擾
	
	//--------------------------
	// �F
	//--------------------------
	virtual void SetColor(const D3DXCOLOR& col);						// �F�ݒ�
	virtual D3DXCOLOR GetColor() const { return m_col; }				// �F�擾
	virtual void SetOriginColor(const D3DXCOLOR& col);					// ���̐F�ݒ�
	virtual D3DXCOLOR GetOriginColor() const { return m_colOrigin; }	// ���̐F�擾
	virtual void SetAlpha(const float alpha);							// �s�����x�ݒ�
	virtual float GetAlpha() const { return m_col.a; }					// �s�����x�擾

	//--------------------------
	// �l
	//--------------------------
	void SetNum(int nNum);			// �l�ݒ�
	int GetNum() {return m_nNum; }	// �l�擾

	//--------------------------
	// ���̑�
	//--------------------------
	virtual void BindTexture(int nIdxTexture);	// �e�N�X�`�����蓖��
	void SetEnableDigitDraw(bool bDraw) { m_bDigitDraw = bDraw; }			// �����`�攻��ݒ�
	void SetAlignmentType(EAlignmentType type) { m_AlignmentType = type; }	// �񂹎�ސݒ�

private:

	//=============================
	// �����o�֐�
	//=============================
	void AdjustPosition();	// �ʒu����
	void AdjustLeft();		// ������
	void AdjustRight();		// �E����
	void AdjustCenter();	// ��������

	//=============================
	// �����o�ϐ�
	//=============================
	MyLib::Vector2 m_size;			// �T�C�Y
	MyLib::Vector2 m_sizeOrigin;	// ���̃T�C�Y
	D3DXCOLOR m_col;			// �F
	D3DXCOLOR m_colOrigin;		// ���̐F
	int m_nNum;							// ����
	int m_nMaxDigit;					// ����
	int m_nIdxTexture;					// �e�N�X�`���̃C���f�b�N�X
	bool m_bDigitDraw;					// �����`��t���O
	EObjType m_objType;					// �I�u�W�F�N�g�̎��
	EAlignmentType m_AlignmentType;		// �񂹎��
	std::vector<CNumber*> m_vecNumber;	// ����
};

#endif