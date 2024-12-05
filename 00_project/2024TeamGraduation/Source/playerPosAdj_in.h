//==========================================================================
// 
//  �v���C���[�ʒu�␳_����R�[�g�w�b�_�[ [playerPosAdj_in.h]
//  Author : ���c�E��
// 
//==========================================================================

#ifndef _PLAYER_POSADJ_IN_H_
#define _PLAYER_POSADJ_IN_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "playerPosAdj.h"

//==========================================================================
// �v���C���[�ʒu�␳_����R�[�g��`
//==========================================================================
class CPlayerPosAdjIn : public CPlayerPosAdj
{
public:

	//=============================
	// �萔
	//=============================
	static constexpr float COMEBACK_LINE = 100.0f;	// ����R�[�g����߂��Ă��郉�C��
	static constexpr float UNSTABLE_LINE = 1.0f;	// �����Ƃ��ƃ��C��

	//=============================
	// �񋓌^
	//=============================
	enum EInputUnstable
	{
		INPUT_NONE = 0,		// ���͂Ȃ�
		INPUT_FRIEND,		// ������
		INPUT_ENEMY,		// �G��
		INPUT_MAX,
	};

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CPlayerPosAdjIn();
	virtual ~CPlayerPosAdjIn() override;

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual void UpdateAdjuster(CPlayer* pPlayer) override;	// ����

protected:

	//=============================
	// �萔
	//=============================
	static constexpr float RETURN_DISTANCE = 60.0f;	// �߂邽�߂̊Ԋu

	//=============================
	// �������z�֐�
	//=============================
	virtual bool IsLineOut(CPlayer* pPlayer) = 0;		// ���C���z���t���O�擾
	virtual bool IsUnstable(CPlayer* pPlayer) = 0;		// �����Ƃ��ƃt���O�擾
	virtual EInputUnstable IsInputLine(CPlayer* pPlayer) = 0;	// �����Ƃ��Ǝ����̓t���O�擾

private:
	//=============================
	// �����o�֐�
	//=============================
	void CheckReturn(CPlayer* pPlayer);		// �`�[���R�[�g���A
	void CheckUnstable(CPlayer* pPlayer);	// �����Ƃ��Ƃ���
	void ReturnSetting(CPlayer* pPlayer);	// ���A�ݒ�
};

#endif