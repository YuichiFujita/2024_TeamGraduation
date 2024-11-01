//=============================================================================
// 
//  �v���C���[�w�b�_�[ [player.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _PLAYER_H_
#define _PLAYER_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "objectChara.h"
#include "motion.h"

// �O���錾
class CShadow;
class CMagicCircle;
class CTargetPoint;
class CHP_Gauge;

//==========================================================================
// �N���X��`
//==========================================================================
// �v���C���[�N���X��`
class CPlayer : public CObjectChara
{
public:

	// �񋓌^��`
#if ACTION_MODE
	typedef enum
	{
		SHOTMODE_TRIGGER = 0,	// �g���K�[
		SHOTMODE_RELEASE,		// �����[�X
		SHOTMODE_PRESS,			// �v���X
		SHOTMODE_REPEAT,		// ���s�[�g
		SHOTMODE_MAX
	}SHOTMODE;

#else
	typedef enum
	{
		SHOTMODE_NONE = 0,	// �Ȃ�
		SHOTMODE_REPEAT,	// ���s�[�g
		SHOTMODE_MAX
	}SHOTMODE;

	typedef enum
	{
		ATK_DEBUG_POWER = 0,	// �p���[
		ATK_DEBUG_CROWD,	// �Q��
		ATK_DEBUG_SPEED,	// �X�s�[�h
		ATK_DEBUG_MAX
	}ATK_DEBUG;
#endif

	CPlayer(int nPriority = mylib_const::DEF2D_PRIORITY);
	~CPlayer();

	static CPlayer *Create(const std::string pTextFile);

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init(void);
	HRESULT Init(const std::string pTextFile);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Hit(const int nValue);	// �q�b�g����


	void SetPartsRot(void);		// �����ݒ�
	void SetSphereCollider(void);		// ���̓����蔻��ݒ�
	void SaveNowMotion(void);	// ���݂̃��[�V�����Z�[�u

private:
	// �񋓌^��`
	typedef enum
	{
		MOTION_DEF = 0,	// �j���[�g�������[�V����
		MOTION_WALK,	// �ړ����[�V����
		MOTION_SUPERATK,		// ���U��
		MOTION_MAGIC_ICE,		// �X���@
		MOTION_SPEED_RUSH,		// �������b�V��
		MOTION_SPEED_RUSH_MOVE,		// �������b�V��(�ړ�)
		MOTION_MAGIC_WAIT,		// ���@�ҋ@
		MOTION_MAX
	}MOTION;

	// ��Ԓ�`
	typedef enum
	{
		STATE_NONE = 0,	// �Ȃɂ��Ȃ�
		STATE_DMG,		// �_���[�W
		STATE_DEAD,		// ��
		STATE_ATTACK,	// �U������
		STATE_MAX
	}STATE;

	void UpdateState(void);	// ��ԍX�V����
	void Controll(void);	// ����
	void MotionSet(void);	// ���[�V�����̐ݒ�
	void Collision(void);	// �����蔻��
	void Atack(void);		// �U��
	void SetAttackInfo(void);


	void SaveMotionInfo(void);	// ���݂̃��[�V�����Z�[�u
	void SaveSetUpInfo(void);	// ���݂̃Z�b�g�A�b�v���Z�[�u

	STATE m_state;				// ���
	D3DXCOLOR m_mMatcol;		// �}�e���A���̐F
	int m_Shotmode;				// �ˌ��̎��
	bool m_bJump;				// �W�����v�����ǂ���
	bool m_bMove;				// �ړ������ǂ���
	bool m_bATK;				// �U�������ǂ���
	bool m_bMWait;				// ���@�ҋ@�����ǂ���
	int m_nCntState;			// ��ԑJ�ڃJ�E���^�[
	int m_nLife;				// �̗�
	int m_nTexIdx;				// �e�N�X�`���̃C���f�b�N�X�ԍ�
	int m_nIdxXFile;			// X�t�@�C���̃C���f�b�N�X�ԍ�
	CMotion *m_pMotion;			// ���[�V�����̏��
	CShadow *m_pShadow;			// �e�̏��

	int m_nNowParts;			// ���݂̃p�[�c
	int m_nNowMotion;			// ���݂̃��[�V����
	int m_nNowKey;				// ���݂̃L�[
	int m_nNowAttackIdx;		// ���݂̍U���C���f�b�N�X�ԍ�
	float m_fSetUpBuff;			// �Z�b�g�A�b�v�o�t

	int m_nNowColliderIdx;		// ���݂̃R���C�_�[�C���f�b�N�X�ԍ�
	CMotion::Key m_CopyData;	// �R�s�[�p�p�[�c�f�[�^
	bool m_bStop;				// �X�g�b�v���邩
};



#endif