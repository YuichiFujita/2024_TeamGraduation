//=============================================================================
// 
//  X�t�@�C���w�b�_�[ [Xload.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _XLOAD_H_
#define _XLOAD_H_	// ��d�C���N���[�h�h�~

#include "object.h"

class CObjectX;

//==========================================================================
// �N���X��`
//==========================================================================
// X�t�@�C���N���X��`
class CXLoad
{
public:

	// �}�N����`
#define MAX_MAT				(512)		// �}�e���A���T�C�Y

	struct SXFile
	{
		MyLib::Vector3 vtxMin;					// ���f���̍ŏ��l
		MyLib::Vector3	vtxMax;					// ���f���̍ő�l
		BYTE *pVtxBuff;							// ���_�o�b�t�@�̃|�C���^
		LPD3DXMESH pMesh;						// ���b�V��(���_���)�ւ̃|�C���^
		LPD3DXBUFFER pBuffMat;					// �}�e���A���ւ̃|�C���^
		DWORD dwNumMat;							// �}�e���A���̐�
		D3DXMATERIAL pMatData[MAX_MAT];			// �}�e���A���̃f�[�^
		int nVtxNum;							// ���_��
		int nFaceNum;							// �ʂ̐�
		MyLib::Vector3 *pVtxPos;				// ���_���W
		float fMaxVtxDistance;					// ���_�Ԃ̍ő勗��
		int *nIdxTexture;						// �e�N�X�`���̃C���f�b�N�X�ԍ�
		std::string filename;					// �t�@�C����
		int nFileNameLen;						// �t�@�C�����̕�����
	};

	CXLoad();
	~CXLoad();

	HRESULT Init();
	void Uninit();
	int XLoad(std::string file);
	HRESULT LoadAll();	// �S�Ẵ��f���ǂݍ���
	void Unload();

	SXFile *GetMyObject(int nIdx);
	int GetNumAll();		// ����

	static CXLoad* Create();
	static CXLoad* GetInstance() { return m_pThisPtr; }
protected:

private:
	HRESULT Load(std::string file);
	void SearchAllXModel(const std::wstring& folderPath);	// �S����

	std::vector<SXFile> m_XFileInfo;	// X�t�@�C���̏��
	std::vector<std::string> m_ImageNames;		// �ǂݍ��ݗp������
	std::vector<std::string> m_FolderFilePath;	// �t�H���_�[�i�[�̃t�@�C���p�X
	static CXLoad* m_pThisPtr;	// ���g�̃|�C���^
};



#endif