//=============================================================================
// 
//  �t�F�[�h�w�b�_�[ [fade.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _FADE_H_
#define _FADE_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object2D.h"
#include "scene.h"

//==========================================================================
// �O���錾
//==========================================================================

//==========================================================================
// �N���X��`
//==========================================================================
// �t�F�[�h�N���X��`
class CFade
{
public:

	// �񋓌^��`
	enum STATE
	{
		STATE_NONE = 0,	// �������Ă��Ȃ�
		STATE_FADEOUT,	// �t�F�[�h�A�E�g
		STATE_FADEIN,	// �t�F�[�h�C��
		STATE_FADECOMPLETION,	// �t�F�[�h����
		STATE_MAX
	};

	CFade();
	~CFade();

	// �����o�֐�
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	CObject2D *GetMyObject();
	static CFade *Create();
	void SetFade(CScene::MODE mode);	// ���̃��[�h�ݒ�
	STATE GetState();	// ��Ԏ擾

private:

	CObject2D *m_aObject2D;		// �I�u�W�F�N�g2D�̃I�u�W�F�N�g
	CScene::MODE m_ModeNext;	// ���̃��[�h
	STATE m_state;				// ���
};

#endif