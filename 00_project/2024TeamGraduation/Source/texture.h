//=============================================================================
// 
// �e�N�X�`���w�b�_�[ [texture.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _TEXTURE_H_
#define _TEXTURE_H_	// ��d�C���N���[�h�h�~

#include "constans.h"

//==========================================================================
// �N���X��`
//==========================================================================
//�e�N�X�`���N���X��`
class CTexture
{
private:

	//=============================
	// �\���̒�`
	//=============================
	struct STexture
	{
		LPDIRECT3DTEXTURE9 pTexture;	// �e�N�X�`���̃|�C���^
		D3DXIMAGE_INFO imageInfo;		// �e�N�X�`���f�ޏ��
		D3DXVECTOR2 aspectratio;		// �A�X�y�N�g��
		std::string filename;			// �t�@�C����
		int nFileNameLen;				// �t�@�C�����̕�����
	};

public:

	// �e�N�X�`���������\����
	struct SInfo
	{
	public:
		// �R���X�g���N�^
		SInfo() {}
		SInfo(const UINT nWidth, const UINT nHeight, const UINT nMip, const DWORD dwUsage, const D3DFORMAT format, const D3DPOOL pool)
			: Width(nWidth), Height(nHeight), MipLevels(nMip), Usage(dwUsage), Format(format), Pool(pool) {}

		// �f�X�g���N�^
		~SInfo() {}

		// �����o�ϐ�
		UINT		Width;		// �e�N�X�`������
		UINT		Height;		// �e�N�X�`���c��
		UINT		MipLevels;	// �~�b�v�}�b�v���x��
		DWORD		Usage;		// �����E�m�ۃI�v�V����
		D3DFORMAT	Format;		// �s�N�Z���t�H�[�}�b�g
		D3DPOOL		Pool;		// �i�[������
	};

	CTexture();
	~CTexture();

	void Init();	// ����������
	HRESULT LoadAll();	// �S�Ẵe�N�X�`���ǂݍ���
	void Unload();
	int Regist(const SInfo info);			// �e�N�X�`���o�^ (����)
	int Regist(const std::string& file);	// �e�N�X�`���o�^ (�Ǎ�)
	LPDIRECT3DTEXTURE9 GetAdress(int nIdx);
	D3DXVECTOR2 GetImageSize(int nIdx);		// �e�N�X�`���f�ނ̃T�C�Y�擾

	int GetNumAll();		// �ǂݍ��񂾑���
	STexture GetTextureInfo(const std::string& file);	// �e�N�X�`�����擾
	STexture GetTextureInfo(int nIdxTex);	// �e�N�X�`�����擾

	static CTexture* Create();
	static CTexture* GetInstance() { return m_pTexture; }
private:

	//=============================
	// �����o�֐�
	//=============================
	HRESULT LoadTex(const std::string& file);
	void SearchAllImages(const std::wstring& folderPath);

	//=============================
	// �����o�ϐ�
	//=============================
	std::vector<STexture> m_TexInfo;			// �e�N�X�`���̏��
	std::vector<std::string> m_ImageNames;		// �ǂݍ��ݗp������
	std::vector<std::string> m_FolderFilePath;	// �t�H���_�[�i�[�̃t�@�C���p�X
	static CTexture* m_pTexture;				// ���g�̃|�C���^
};



#endif