//=============================================================================
// 
//  �����Ԃ��w�i�w�b�_�[ [splashwater_bg.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _SPLASHWATER_BG_H_
#define _SPLASHWATER_BG_H_	// ��d�C���N���[�h�h�~

#include "object2d.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �����Ԃ��N���X
class CSplashwater_BG : public CObject2D
{
public:
	//=============================
	// ��ԗ�
	//=============================
	enum State
	{
		STATE_NONE = 0,		// �Ȃɂ��Ȃ�
		STATE_APPEARANCE,	// �o��
		STATE_MAX
	};

	CSplashwater_BG(int nPriority = 7);
	~CSplashwater_BG();

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
	static CSplashwater_BG* Create();

private:
	//=============================
	// �����o�֐�
	//=============================
	// ��Ԍn
	void StateAppearance();	// �o�����

	//=============================
	// �����o�ϐ�
	//=============================
	// ��Ԍn
	float m_fLife;				// ����
	float m_fOriginLife;		// ����
	State m_state;				// ���
	float m_fStateTime;			// ��ԃ^�C�}�[
};


#endif