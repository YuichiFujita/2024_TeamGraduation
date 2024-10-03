//=============================================================================
// 
//  �����Ԃ��}�l�[�W���w�b�_�[ [splashwater_manager.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _SPLASHWATER_MANAGER_H_
#define _SPLASHWATER_MANAGER_H_	// ��d�C���N���[�h�h�~

#include "object2d.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �����Ԃ��N���X
class CSplashwater_Manager : public CObject
{
public:

	CSplashwater_Manager(int nPriority = 7);
	~CSplashwater_Manager();

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
	static CSplashwater_Manager* Create();

};


#endif