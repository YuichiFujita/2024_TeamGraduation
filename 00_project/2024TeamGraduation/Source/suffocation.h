//=============================================================================
// 
//  �T�t�H�P�[�V�����w�b�_�[ [suffocation.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _SUFFOCATION_H_
#define _SUFFOCATION_H_	// ��d�C���N���[�h�h�~

#include "object2d.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �T�t�H�P�[�V�����N���X
class CSuffocation : public CObject2D
{
public:

	//=============================
	// ��ԗ�
	//=============================
	enum State
	{
		STATE_SURFACING = 0,	// ����
		STATE_BURST,			// �j��
		STATE_MAX
	};

	CSuffocation(int nPriority = 7);
	~CSuffocation();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//=============================
	// �ÓI�֐�
	//=============================
	static CSuffocation* Create();

private:

	//=============================
	// �֐����X�g
	//=============================
	// ��ԃ��X�g
	typedef void(CSuffocation::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԍn
	void UpdateState();		// ��ԍX�V
	void StateSurfacing();	// ����
	void StateBurst();		// �j��

	// ���̑��֐�
	void Reset();

	//=============================
	// �����o�ϐ�
	//=============================
	// ��Ԍn
	float m_fStateTime;		// ��ԃJ�E���^�[
	State m_state;			// ���
	int m_nTexID[6];		// ID

	// ���̑��ϐ�
	float m_fDestWidth;	// �ڕW�̕�
};


#endif