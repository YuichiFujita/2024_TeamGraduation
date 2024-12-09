//==========================================================================
// 
//  �����_�[�e�N�X�`���}�l�[�W���[�w�b�_�[ [renderTextureManager.h]
//  Author : ���c�E��
// 
//==========================================================================

#ifndef _RENDER_TEXTURE_MANAGER_H_
#define _RENDER_TEXTURE_MANAGER_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "manager.h"

//==========================================================================
// �O���錾
//==========================================================================
class CRenderTexture;

//==========================================================================
// �N���X��`
//==========================================================================
// �����_�[�e�N�X�`���}�l�[�W���[�N���X
class CRenderTextureManager
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	// ���C���[��
	enum ELayer
	{
		LAYER_BALLOON = 0,	// �����o��
		LAYER_MAIN,			// ���C�����
		LAYER_MAX			// ���̗񋓌^�̑���
	};

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CRenderTextureManager();
	virtual ~CRenderTextureManager();

	//=============================
	// �����o�֐�
	//=============================
	HRESULT Init();	// ������
	void Uninit();	// �I��
	void Draw();	// �`��
	void RegistRenderTexture(CRenderTexture* pRenderTexture, const ELayer layer);	// �����_�[�e�N�X�`���o�^
	HRESULT DeleteRenderTexture(CRenderTexture* pRenderTexture);					// �����_�[�e�N�X�`���폜

	//=============================
	// �ÓI�����o�֐�
	//=============================
	static CRenderTextureManager* Create();			// ����
	static CRenderTextureManager* GetInstance();	// �C���X�^���X�擾

private:

	//=============================
	// �ÓI�����o�ϐ�
	//=============================
	static CRenderTextureManager* m_pInstance;	// ���g�̃C���X�^���X

	//=============================
	// �����o�ϐ�
	//=============================
	std::vector<CRenderTexture*> m_vecRenderTexture[LAYER_MAX];	// �����_�[�e�N�X�`��
};

#endif
