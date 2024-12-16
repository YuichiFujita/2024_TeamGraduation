//==========================================================================
// 
//	�����o���̘g�w�b�_�[ [balloonFrame.h]
//  Author : ���n�Ή�
// 
//==========================================================================

#ifndef _BALLOONFRAME_H_
#define _BALLOONFRAME_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "object2D.h"
#include "gameManager.h"

//==========================================================================
// �O���錾
//==========================================================================

//==========================================================================
// �N���X��`
//==========================================================================
// �����o���̘g�N���X
class CBalloonFrame : public CObject2D
{
public:

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CBalloonFrame(int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CBalloonFrame();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;		// �`��
	virtual void Kill() override;	// �폜
	void SetVtx() override;			// ���_���W

	//=============================
	// �����o�֐�
	//=============================

	//=============================
	// �ÓI�����o�֐�
	//=============================
	static CBalloonFrame* Create(const MyLib::Vector2& size, CGameManager::ETeamSide side);	// ����

private:

	//=============================
	// �����o�֐�
	//=============================
	void CreateFillPoly();	// ���߂�|���S������
	void CreateTail();		// ������
	HRESULT CreateIndex();	// �C���f�b�N�X����
	void DrawIdxBuff();	// �C���f�b�N�X�o�b�t�@�`��

	//=============================
	// �����o�ϐ�
	//=============================
	MyLib::Vector2 m_TextSize;	// �����T�C�Y
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;		// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	int m_nNumIndex;						// �C���f�b�N�X��
	CObject2D* m_pFillPoly;					// ���߂�ׂ̃|���S��
	CObject2D* m_pTail;						// ��
	CGameManager::ETeamSide m_TeamSide;		// �`�[���T�C�h
};

#endif
