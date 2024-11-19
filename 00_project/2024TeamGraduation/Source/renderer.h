//==========================================================================
// 
//  �����_���[�w�b�_�[ [renderer.h]
//  Author : ���n�Ή�
//  Adder  : ���c�E��
// 
//==========================================================================

#ifndef _RENDERER_H_
#define _RENDERER_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "renderTexture.h"

//==========================================================================
// �O���錾
//==========================================================================
class CScreen;

//==========================================================================
// �N���X��`
//==========================================================================
// �����_���[�N���X��`
class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void Draw();
	HRESULT CreateRenderTexture();							// �����_�[�e�N�X�`���[����
	void DrawRenderTexture(LPDIRECT3DSURFACE9* pSurface);	// �����_�[�e�N�X�`���`��
	void SetEnableShader(const bool bShader);				// �X�N���[���V�F�[�_�[�t���O�ݒ�
	bool IsShader() const;									// �X�N���[���V�F�[�_�[�t���O�擾
	inline LPDIRECT3DDEVICE9 GetDevice() const	{ return m_pD3DDevice; }
	inline LPDIRECT3D9 GetD3D() const			{ return m_pD3D; }
	inline int GetRenderTextureIndex() const	{ return m_pRenderScene->GetTextureIndex(); }	// �����_�[�e�N�X�`���C���f�b�N�X�擾

private:
	// �����o�ϐ�
	CRenderTexture*		m_pRenderScene;		// �V�[�������_�[�e�N�X�`��
	CScreen*			m_pDrawScreen;		// �X�N���[���`��|���S��
	LPDIRECT3DSURFACE9	m_pDefSurScreen;	// ���̕`��T�[�t�F�C�X�ۑ��p
	LPDIRECT3D9			m_pD3D;				// Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9	m_pD3DDevice;		// Direct3D�f�o�C�X
};


#endif