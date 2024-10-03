//=============================================================================
// 
//  �N���w�b�_�[ [elevation.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _ELEVATION_H_
#define _ELEVATION_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object3DMesh.h"

// �O���錾
class CTargetPoint;

//==========================================================================
// �N���X��`
//==========================================================================
// �N���N���X��`
class CElevation : public CObject3DMesh
{
private:

	struct Info
	{
		std::string TextureFileName;	// �e�N�X�`���t�@�C����
		MyLib::Vector3 pos;		// �ʒu
		float fWidthLength;		// ������
		float fHeightLength;	// �c����
		int nWidthBlock;		// ��������
		int nHeightBlock;		// �c������
		MyLib::Vector3 *pVtxPos;	// ���_���W
	};

public:

	CElevation(int nPriority = 1, const LAYER layer = LAYER::LAYER_MAP);
	~CElevation();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init();
	virtual HRESULT Init(const char *pText);
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
	virtual void SetVtx();

	void Release();
	bool IsEdit();	// �G�f�B�b�g�̔���
	void Save();	// �Z�[�u
	HRESULT Load(const char *pText);	// ���[�h
	bool IsChange() { return m_bChange; }

	static CElevation *Create(const char *pText);
	CElevation *GetElevation();
	float GetHeight(const MyLib::Vector3& pos, bool *pLand);	// �����擾
	bool IsHit(const MyLib::Vector3& pos);	// �����������̔���

protected:

private:

	void UpdateState();				// ��ԍX�V����
	void UPVtxField(MyLib::Vector3 pos);	// ���_�グ����(�f�o�b�O)

	Info m_aInfo;	// �N���̏��
	float m_fBrushStrength;							// �u���V����
	float m_fBrushRange;							// �͈�
	bool m_bEdit;									// �G�f�B�b�gON/OFF
	CTargetPoint *m_pTargetP;						// �ڕW�̒n�_
	MyLib::Vector3 m_VtxPos[mylib_const::MAX_VTX];		// ���_���W
	int m_nTexIdx;									// �e�N�X�`���̃C���f�b�N�X�ԍ�
	bool m_bChange;	// �ύX�̃t���O
	static bool m_bLoadInfo;						// ���ǂݍ��ݔ���
};



#endif