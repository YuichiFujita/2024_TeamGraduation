//==========================================================================
// 
//  �J�b�g�C���w�b�_�[ [cutin.h]
//  Author : ���c�E��
// 
//==========================================================================

#ifndef _CUT_IN_H_
#define _CUT_IN_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "object2D.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �J�b�g�C���N���X
class CCutIn : public CObject2D
{
public:

	//=============================
	// ��ԗ�
	//=============================
	enum EState
	{
		STATE_NONE = 0,	// �Ȃɂ��Ȃ����
		STATE_TIME,		// ���Ԍo�ߏ��
		STATE_END,		// �I�����
		STATE_MAX
	};

	CCutIn();
	~CCutIn();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Kill() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;

	//=============================
	// �ÓI�����o�֐�
	//=============================
	static CCutIn* Create();

	//=============================
	// �����o�֐�
	//=============================
	bool IsEnd() const;	// �I���t���O�擾

private:

	//=============================
	// �����o�֐�
	//=============================
	// �����֘A
	HRESULT CreateRenderTexture();	// �����_�[�e�N�X�`������
	HRESULT CreateTextureObject();	// �e�N�X�`���p�I�u�W�F�N�g����
	HRESULT CreateObject2D();		// �v���C���[�e�N�X�`���\�t�|���S��/�P������

	// �e�N�X�`���֘A
	void CreateTexture();	// �e�N�X�`���쐬

	// ���̑��֐�
	void UpdateBlinkColor(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �w�i�F�̍X�V

	//=============================
	// �����o�ϐ�
	//=============================
	// �e�N�X�`���֘A
	CRenderTexture* m_pRenderScene;	// �V�[�������_�[�e�N�X�`��

	// ���̑��ϐ�
	CObject2D* m_pBlink;	// �P�����
	CObject2D* m_pPlayer2D;	// �v���C���[�e�N�X�`�����
	float m_fBlinkTime;		// �w�i�P���J�E���^�[
};


#endif