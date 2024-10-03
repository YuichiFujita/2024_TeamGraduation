//=============================================================================
// 
//  ���f���w�b�_�[ [model.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _MODEL_H_
#define _MODEL_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "constans.h"

class CObjectX;

//==========================================================================
// �N���X��`
//==========================================================================
// ���f���N���X��`
class CModel
{
public:

	CModel(int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CModel();

	HRESULT Init(const std::string& file);
	void Uninit();
	void Update();
	void Draw();
	void Draw(D3DXCOLOR col);
	void Draw(float fAlpha);
	void BindXData(int nIdxXFile);

	int *GetIdxTexture() {return m_nIdxTexture; }
	int GetIdxTexture(int nIdx);	// �e�N�X�`���C���f�b�N�X�ԍ��擾
	void SetIdxTexture(int i, int nIdx);	// �e�N�X�`���̃C���f�b�N�X���蓖��
	int GetIdxXFile() const { return m_nIdxXFile; }
	void SetWorldMtx(const MyLib::Matrix mtx);		// �}�g���b�N�X�ݒ�
	MyLib::Matrix GetWorldMtx() ;			// �}�g���b�N�X�擾
	MyLib::Matrix*GetPtrWorldMtx();			// �|�C���^�}�g���b�N�X�擾
	void SetPosition(const MyLib::Vector3 pos);	// �ʒu�ݒ�
	MyLib::Vector3 GetPosition() const;		// �ʒu�擾
	void SetOldPosition(const MyLib::Vector3 posOld);	// �O��̈ʒu�ݒ�
	MyLib::Vector3 GetOldPosition() const;		// �O��̈ʒu�擾
	void SetOriginPosition(const MyLib::Vector3 pos);	// ���̈ʒu�ݒ�
	MyLib::Vector3 GetOriginPosition() const;		// ���̈ʒu�擾
	void SetRotation(const MyLib::Vector3 rot);	// �����ݒ�
	MyLib::Vector3 GetRotation() const;		// �����擾
	void SetOriginRotation(const MyLib::Vector3 rot);	// ���̌����ݒ�
	MyLib::Vector3 GetOriginRotation() const;		// ���̌����擾
	void SetScale(const MyLib::Vector3 scale);		// �X�P�[���ݒ�
	MyLib::Vector3 GetOriginScale() const { return m_scaleOrigin; }				// ���̃X�P�[���擾
	void SetOriginScale(const MyLib::Vector3& scale) { m_scaleOrigin = scale; }	// ���̃X�P�[���ݒ�
	MyLib::Vector3 GetScale() const;			// �X�P�[���擾
	MyLib::Vector3 GetVtxMax() const;			// ���_�̍ő�l�擾
	MyLib::Vector3 GetVtxMin() const;			// ���_�̍ŏ��l�擾
	void SetParent(CModel *pModel);				// �e�̃|�C���^�ݒ�
	CModel* GetParent(){ return m_pParent; }		// �e�̃|�C���^�擾

	void SetMtxParent(MyLib::Matrix* pMtx);

	static CModel *Create(const std::string& file, MyLib::Vector3 pos = MyLib::Vector3(0.0f, 0.0f, 0.0f), MyLib::Vector3 rot = MyLib::Vector3(0.0f, 0.0f, 0.0f));

	CModel *GetModel();
	static int GetNumAll();
protected:

private:

	void BindTexture();	// �e�N�X�`�����蓖��
	void CalWorldMtx();	// ���[���h�}�g���b�N�X�̌v�Z����
	void DrawShadowMtx();

	MyLib::Matrix m_mtxWorld;	// ���[���h�}�g���b�N�X
	MyLib::Matrix *m_mtxParent;	// �e�}�g���b�N�X�̃|�C���^
	MyLib::Vector3 m_pos;		// �ʒu
	MyLib::Vector3 m_posOld;	// �O��̈ʒu
	MyLib::Vector3 m_posOrigin;	// ���̈ʒu
	MyLib::Vector3 m_rot;		// ����
	MyLib::Vector3 m_rotOrigin;// ����
	MyLib::Vector3 m_scale;		// �X�P�[��
	MyLib::Vector3 m_scaleOrigin;	// ���̃X�P�[��
	int m_nIdxXFile;			// X�t�@�C���̃C���f�b�N�X�ԍ�
	int *m_nIdxTexture;			// �e�N�X�`���̃C���f�b�N�X�ԍ�
	CModel *m_pParent;			// �e���f���̃|�C���^
	static int m_nNumAll;		// ����
};



#endif