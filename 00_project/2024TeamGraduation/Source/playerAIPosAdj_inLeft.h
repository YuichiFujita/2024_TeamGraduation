//==========================================================================
// 
//  �v���C���[�ʒu�␳_������R�[�g�w�b�_�[ [playerPosAdj_inLeft.h]
//  Author : ���c�E��
// 
//==========================================================================

#ifndef _PLAYER_AI_POSADJ_INLEFT_H_
#define _PLAYER_AI_POSADJ_INLEFT_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "playerPosAdj_inLeft.h"
#include "playerPosAdj_in.h"

class CPlayer;

//==========================================================================
// �v���C���[�ʒu�␳_������R�[�g��`
//==========================================================================
class CPlayerAIPosAdjInLeft : public CPlayerPosAdjInLeft
{
public:

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CPlayerAIPosAdjInLeft();
	virtual ~CPlayerAIPosAdjInLeft() override;

private:
	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual bool IsLineOut(CPlayer* pPlayer) override;								// ���C���z���t���O�擾
	virtual bool IsUnstable(CPlayer* pPlayer) override;								// �����Ƃ��ƃt���O�擾
	virtual CPlayerPosAdjIn::EInputUnstable IsInputLine(CPlayer* pPlayer) override;	// �����Ƃ��Ǝ����̓t���O�擾
};

#endif