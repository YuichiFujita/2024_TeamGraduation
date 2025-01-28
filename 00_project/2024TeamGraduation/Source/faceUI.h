//============================================================
//
//	�v���C���[��UI�w�b�_�[ [faceUI.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _FACE_UI_H_
#define _FACE_UI_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"
#include "player.h"

//************************************************************
// �O���錾
//************************************************************
class CObject2D;		// �I�u�W�F�N�g2D�N���X
class CObject2D_Anim;	// �I�u�W�F�N�g2D�A�j���N���X

//************************************************************
//	�N���X��`
//************************************************************
// �v���C���[��UI�N���X
class CFaceUI : public CObject
{
public:
	// �R���X�g���N�^
	CFaceUI(const CPlayer* pPlayer);

	// �f�X�g���N�^
	~CFaceUI() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Kill() override;		// �폜
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V
	void Draw() override;		// �`��
	void SetEnableDisp(const bool bDraw) override;			// �`��󋵐ݒ�
	void SetPosition(const MyLib::Vector3& pos) override;	// �ʒu�ݒ�
	void BindDressUp(int nHair, int nAccessory, int nFace);	// �����ւ�����
	void SetMyPlayerIdx(int nIdx);							// �����̃C���f�b�N�X�ݒ�

	// �ÓI�����o�֐�
	static CFaceUI* Create(const CPlayer* pPlayer, const MyLib::Vector3& rPos);

private:
	// �����o�֐�
	HRESULT CreateUI();				// UI����
	HRESULT CreatePlayerIdxUI();	// �v���C���[�C���f�b�N�XUI����
	HRESULT CreatePlayerUI();		// �v���C���[UI����
	HRESULT CreatePlayer();			// �v���C���[����
	HRESULT CreateRenderTexture();	// �����_�[�e�N�X�`������
	void CreateTexture();			// �e�N�X�`���쐬
	void SetPositionRelative();		// ���Έʒu�ݒ�

	// �����o�ϐ�
	const CPlayer* m_pPlayerParent;	// �e�v���C���[
	CRenderTexture* m_pRenderScene;	// �V�[�������_�[�e�N�X�`��
	CObject2D_Anim* m_pPlayerIdxUI;	// �v���C���[�C���f�b�N�XUI���
	CObject2D* m_pPlayerUI;			// �v���C���[UI���
	CPlayer* m_pPlayer;				// �v���C���[
};

#endif	// _FACE_UI_H_
