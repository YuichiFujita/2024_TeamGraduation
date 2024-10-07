//=============================================================================
// 
//  �����w�b�_�[ [multinumber.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _MULTINUMBER_H_
#define _MULTINUMBER_H_	// ��d�C���N���[�h�h�~

#include "object2D.h"
#include "number.h"

//==========================================================================
// �O���錾
//==========================================================================
class CNumber;

//==========================================================================
// �N���X��`
//==========================================================================
// �w�i�N���X��`
class CMultiNumber : public CObject
{
public:

	// �񂹎��
	enum AlignmentType
	{
		ALIGNMENT_LEFT = 0,	// ����
		ALIGNMENT_RIGHT,	// �E��
		ALIGNMENT_MAX
	};

	CMultiNumber(int nPriority = 8);
	~CMultiNumber();

	/**
	@brief	��������
	@param	pos			[in]	�ʒu
	@param	size		[in]	�T�C�Y
	@param	nNum		[in]	����
	@param	objtype		[in]	�I�u�W�F�N�g�̎�� [2D / 3D / �r���r�[�h]
	@param	bDigitDraw	[in]	�L�P�^���̂ݕ`�悷�邩�̔���
	@param	nPriority	[in]	�`��D�揇��
	@return	���������|�C���^
	*/
	static CMultiNumber *Create(MyLib::Vector3 pos, D3DXVECTOR2 size, int nNum, CNumber::EObjectType objtype, bool bDigitDraw = false, int nPriority = 8);
	
	/**
	@brief	��������
	@param	pos			[in]	�ʒu
	@param	size		[in]	�T�C�Y
	@param	nNum		[in]	����
	@param	objtype		[in]	�I�u�W�F�N�g�̎�� [2D / 3D / �r���r�[�h]
	@param	file		[in]	�e�N�X�`���t�@�C����
	@param	bDigitDraw	[in]	�L�P�^���̂ݕ`�悷�邩�̔���
	@param	nPriority	[in]	�`��D�揇��
	@return	���������|�C���^
	*/
	static CMultiNumber *Create(MyLib::Vector3 pos, D3DXVECTOR2 size, int nNum, CNumber::EObjectType objtype, const std::string& file, bool bDigitDraw = true, int nPriority = 8);

	// �����o�֐�
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	void Kill();	// �폜����

	void AddNumber(int nValue);
	void SetValue();		// �l�̐ݒ菈��
	void SetValue(int nValue);	// �l�̐ݒ菈��(�I�[�o�[���[�h)
	int GetValue();			// �l�̎擾����
	int GetDigit() { return m_nNumNumber; }	// �����擾

	void SetEnableAutoUVSetting(bool frag) { m_bAutoUVSetting = frag; }	// ����UV���W�ݒ�
	bool IsAutoUVSetting() { return m_bAutoUVSetting; }			// ����UV���W�ݒ�

	CNumber** GetNumber() { return m_ppMultiNumber; }
	CNumber* GetNumber(int idx) { return m_ppMultiNumber[idx]; }

	void SetPosition(const MyLib::Vector3& pos) override;		// �ʒu�ݒ�
	void SetRotation(const MyLib::Vector3& pos) override;		// �����ݒ�

	void SetColor(const D3DXCOLOR col);			// �F�ݒ�
	D3DXCOLOR GetColor() const;				// �F�擾
	void SetSize(const D3DXVECTOR2 size);		// �T�C�Y�ݒ�
	D3DXVECTOR2 GetSize() const;			// �T�C�Y�擾
	void SetSizeOrigin(const D3DXVECTOR2 size);		// �T�C�Y�ݒ�
	D3DXVECTOR2 GetSizeOrigin() const;			// �T�C�Y�擾
	void SetKerning(float kerning);	// �����Ԋu�ݒ�
	void SetAlignmentType(AlignmentType type) { m_Alignment = type; }

private:

	void SettingDisp();

	// �����o�ϐ�
	D3DXCOLOR m_col;				// �F
	D3DXVECTOR2 m_size;				// �����̃T�C�Y
	D3DXVECTOR2 m_sizeOrigin;		// �����̃T�C�Y
	int m_nNum;						// ����
	int m_nNumNumber;				// �����̐�
	int m_nTexIdx;					// �e�N�X�`���̃C���f�b�N�X�ԍ�
	int m_nPriority;				// �D�揇��
	bool m_bDigitDraw;				// �����`��
	bool m_bAutoUVSetting;			// ����UV���W�ݒ�
	float m_fKerning;				// �����Ԋu
	CNumber **m_ppMultiNumber;		// �����̃I�u�W�F�N�g
	CNumber::EObjectType m_objType;	// �I�u�W�F�N�g�̎��
	AlignmentType m_Alignment;		// ����

};

#endif