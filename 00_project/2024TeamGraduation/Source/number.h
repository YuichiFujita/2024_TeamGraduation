//=============================================================================
// 
//  �����w�b�_�[ [number.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _NUMBER_H_
#define _NUMBER_H_	// ��d�C���N���[�h�h�~

#include "object.h"

//==========================================================================
// �O���錾
//==========================================================================
class CObject2D;
class CObjectBillboard;

//==========================================================================
// �N���X��`
//==========================================================================
// �����N���X
class CNumber : public CObject
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum EObjectType
	{
		OBJECTTYPE_2D = 0,		// 2D
		OBJECTTYPE_3D,			// 3D
		OBJECTTYPE_BILLBOARD,	// �r���{�[�h
		OBJECTTYPE_MAX
	};

	CNumber(int nPriority = 6, const LAYER layer = LAYER::LAYER_DEFAULT);
	~CNumber();


	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;


	virtual void Kill();	// �폜

	virtual void SetPosition(const MyLib::Vector3& pos) = 0;	// �ʒu�ݒ�
	virtual MyLib::Vector3 GetPosition() const = 0;		// �ʒu�擾
	virtual void SetMove(const MyLib::Vector3& move) = 0;		// �ړ��ʐݒ�
	virtual MyLib::Vector3 GetMove() const = 0;			// �ړ��ʎ擾
	virtual void SetRotation(const MyLib::Vector3& rot) = 0;	// �����ݒ�
	virtual MyLib::Vector3 GetRotation() const = 0;		// �����擾

	virtual void SetColor(const D3DXCOLOR col) = 0;		// �F�ݒ�
	virtual D3DXCOLOR GetColor() const = 0;				// �F�擾
	virtual void SetSize(const D3DXVECTOR2 size) = 0;		// �T�C�Y�̐ݒ�
	virtual D3DXVECTOR2 GetSize() const = 0;			// �T�C�Y�̎擾
	virtual void SetSizeOrigin(const D3DXVECTOR2 size) = 0;	// ���̃T�C�Y�̐ݒ�
	virtual D3DXVECTOR2 GetSizeOrigin() const = 0;		// ���̃T�C�Y�̎擾
	virtual void SetTex(D3DXVECTOR2* tex) = 0;				// �e�N�X�`�����W�̐ݒ�
	virtual D3DXVECTOR2* GetTex() = 0;					// �e�N�X�`�����W�̎擾

	virtual void BindTexture(int nIdx) = 0;	// �e�N�X�`�����蓖��
	virtual void SetType(const CObject::TYPE type);

	void SetNum(int num) { m_nNum = num; }	// �����̐����ݒ�
	int GetNum() { return m_nNum; }			// �����̐����擾

	static CNumber* Create(EObjectType objtype, int nPriority);

private:


	//=============================
	// �����o�ϐ�
	//=============================
	int m_nNum = 0;			// �����̐���
	EObjectType m_objType;	// �I�u�W�F�N�g�̎��
};

#endif