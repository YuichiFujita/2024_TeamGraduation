//==========================================================================
// 
//  ���[�h��ʃw�b�_�[ [loadscreen.h]
//  Author : ���n�Ή�
//  Adder  : ���c�E��
// 
//==========================================================================

#ifndef _LOADSCREEN_H_
#define _LOADSCREEN_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �O���錾
//==========================================================================
#include "object2D_Anim.h"

//==========================================================================
// �N���X��`
//==========================================================================
// ���[�h��ʃN���X��`
class CLoadScreen
{
public:

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CLoadScreen();
	~CLoadScreen();

	//=============================
	// �����o�֐�
	//=============================
	HRESULT Init();
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void Draw();
	void Kill();
	inline DWORD GetNumLoop() const	{ return m_pLoad->GetNumLoop(); }
	inline void ResetNumLoop()		{ m_pLoad->ResetNumLoop(); }
	inline void SetEnableAutoPlay(const bool bPlay) { m_pLoad->SetEnableAutoPlay(bPlay); }

	//=============================
	// �ÓI�����o�֐�
	//=============================
	static CLoadScreen *Create();

private:

	//=============================
	// �����o�֐�
	//=============================


	//=============================
	// �����o�ϐ�
	//=============================
	CObject2D_Anim* m_pLoad;	// ���[�h��ʏ��

};

#endif