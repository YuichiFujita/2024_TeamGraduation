//=============================================================================
// 
//  ���U���g�w�b�_�[ [result.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _RESULT_H_
#define _RESULT_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "manager.h"

//==========================================================================
// �O���錾
//==========================================================================
class CResultManager;

//==========================================================================
// �N���X��`
//==========================================================================
// ���U���g�N���X��`
class CResult : public CScene
{
public:

	CResult();
	~CResult();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;

private:

	// �����o�ϐ�
	CResultManager* m_pResultManager;			// ���U���g�}�l�[�W���̃I�u�W�F�N�g
};

#endif