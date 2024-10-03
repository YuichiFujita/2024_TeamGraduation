//=============================================================================
// 
//  �������w�b�_�[ [scroll.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _SCROLL_H_
#define _SCROLL_H_	// ��d�C���N���[�h�h�~

#include "object2d.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �������N���X
class CScroll : public CObject2D
{
public:

	//=============================
	// ��ԗ�
	//=============================
	enum STATE
	{
		STATE_NONE = 0,	// �Ȃɂ��Ȃ�
		STATE_OPEN,		// �I�[�v��
		STATE_WAIT,		// �ҋ@
		STATE_WAITPRESS,		// �����ҋ@
		STATE_CLOSE,	// �N���[�Y
		STATE_FADEOUT,	// �t�F�[�h�A�E�g
		STATE_MAX
	};

	CScroll(int nPriority = 7);
	~CScroll();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Kill();						// �폜
	void SetState(const STATE& state);		// ��Ԑݒ�
	STATE GetState() { return m_state; }	// ��Ԏ擾

	//=============================
	// �ÓI�֐�
	//=============================
	static CScroll* Create(const MyLib::Vector3& pos, const float toOpenTime, const float height, const float scrollLength, bool bAutoWaitPress = true, bool bFadeOut = false, int nPriority = 7);	// ��������

private:
	
	//=============================
	// �֐����X�g
	//=============================
	// ��ԃ��X�g
	typedef void(CScroll::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԍn
	void UpdateState();	// ��ԍX�V
	void StateNone();	// �Ȃɂ��Ȃ�
	void StateOpen();	// �I�[�v��
	void StateWait();	// �ҋ@
	void StateWaitPress();	// �����ҋ@
	void StateClose();	// �N���[�Y
	void StateFadeout();// �t�F�[�h�A�E�g

	// ���̑�
	void ResetFlag();	// �t���O���Z�b�g
	void CreatePaper();	// ����������
	void CreateEdge();	// �[��������
	void AdjustRoll();	// ���[�������̒���
	void AdjustEdge();	// �[�����̒���
	void OpenSkip();	// �I�[�v���X�L�b�v

	//=============================
	// �����o�ϐ�
	//=============================
	// ��Ԍn
	STATE m_state;			// ���
	float m_fStateTimer;	// ��ԃ^�C�}�[
	float m_fToOpenTimer;	// �I�[�v���܂ł̎���
	float m_fScrollLength;	// �������̒���
	bool m_bFinishOpen;		// �I�[�v���I������
	bool m_bFadeOut;		// �t�F�[�h�A�E�g����
	bool m_bAutoWaitPress;	// ���������ҋ@����

	// ���̑�
	CObject2D* m_pPapaer;	// ������
	CObject2D* m_pEdge;		// �[����
};


#endif