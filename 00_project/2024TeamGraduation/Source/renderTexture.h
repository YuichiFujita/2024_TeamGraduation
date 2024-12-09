//============================================================
//
//	�����_�[�e�N�X�`���w�b�_�[ [renderTexture.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _RENDER_TEXTURE_H_
#define _RENDER_TEXTURE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "renderTextureManager.h"
#include "renderer.h"

//************************************************************
//	�N���X��`
//************************************************************
// �����_�[�e�N�X�`���N���X
class CRenderTexture
{
public:
	// �R���X�g���N�^
	CRenderTexture(const CRenderTextureManager::ELayer layer);

	// �f�X�g���N�^
	~CRenderTexture();

	// �����o�֐�
	HRESULT Init();	// ������
	void Uninit();	// �I��
	void Draw();	// �`��
	inline void SetDrawFunc(CRenderer::ADrawFunc pDrawFunc) { m_pDrawFunc = pDrawFunc; }	// �`��֐��ݒ�
	inline int GetTextureIndex() const { return m_nTextureIdx; }	// �e�N�X�`���C���f�b�N�X�擾

	// �ÓI�����o�֐�
	static CRenderTexture *Create(const CRenderTextureManager::ELayer layer, CRenderer::ADrawFunc pDrawFunc);	// ����
	static void Release(CRenderTexture*& prRenderTexture);	// �j��

private:
	// �����o�ϐ�
	const CRenderTextureManager::ELayer m_layer;	// �����_�[�e�N�X�`�����C���[
	LPDIRECT3DSURFACE9 m_pSurTexture;	// �e�N�X�`���T�[�t�F�C�X�ւ̃|�C���^
	CRenderer::ADrawFunc m_pDrawFunc;	// �I�u�W�F�N�g�`��֐�
	int m_nTextureIdx;		// �����_�[�e�N�X�`���C���f�b�N�X
};

#endif	// _RENDER_TEXTURE_H_
