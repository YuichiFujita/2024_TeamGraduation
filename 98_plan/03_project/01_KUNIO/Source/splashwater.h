//=============================================================================
// 
//  �����Ԃ��w�b�_�[ [splashwater.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _SPLASHWATER_H_
#define _SPLASHWATER_H_	// ��d�C���N���[�h�h�~

#include "object2d.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �����Ԃ��N���X
class CSplashwater : public CObject2D
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

	CSplashwater(int nPriority = 7);
	~CSplashwater();

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
	static CSplashwater* Create();

private:

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԍn
	void StateAppearance();	// �o�����

	// ���̑��֐�
	MyLib::Vector3 CalSetmove();	// �ړ��ʌv�Z

	//=============================
	// �����o�ϐ�
	//=============================
	// ��Ԍn
	float m_fLife;				// ����
	float m_fOriginLife;		// ����
	MyLib::Vector3 m_posDest;	// �ڕW�ʒu
	D3DXVECTOR2 m_sizeDest;		// �ڕW�T�C�Y
	State m_state;				// ���
	float m_fStateTime;			// ��ԃ^�C�}�[
};


#endif