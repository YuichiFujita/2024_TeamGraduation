//=============================================================================
// 
//  ���[�V�����w�b�_�[ [motion.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _MOTION_H_
#define _MOTION_H_	// ��d�C���N���[�h�h�~

#include "gamemanager.h"
#include "constans.h"

class CModel;
class CObjectChara;

#define MAX_MOTION	(28)	// ���[�V�����̍ő吔
#define MAX_KEY		(24)	// �L�[�̍ő吔
#define MAX_PARTS	(32)	// �p�[�c�̍ő吔

//==========================================================================
// �N���X��`
//==========================================================================
// ���[�V�����N���X��`
class CMotion
{
public:

	// ����̍\����
	typedef struct
	{
		int nCollisionNum;		// �����蔻��̃p�[�c�ԍ�
		float fRangeSize;		// ����̃T�C�Y
		MyLib::Vector3 Offset;	// ����̃I�t�Z�b�g
		int nMinCnt;			// ����̍Œ�J�E���g
		int nMaxCnt;			// ����̍ő�J�E���g
		int nDamage;			// �_���[�W
		int nInpactCnt;			// �Ռ��̃J�E���g
		bool bInpactAct;		// �Ռ��J�E���g�̍s����������
		bool bInpactActSet;		// �Ռ��J�E���g�̍s���ݒ肵����
		bool bEndAtk;			// �U���̏I���t���O
		bool bOnlyOneTime;		// 1�x�������̔���
		CGameManager::AttackType AtkType;	// �U���̎��
	}AttackInfo;

	// ���[�V�����̍\����
	typedef struct
	{
		MyLib::Vector3 rot;		// ����
		MyLib::Vector3 rotDest;	// �ڕW�̌���
		MyLib::Vector3 pos;		// �ʒu
		MyLib::Vector3 posDest;	// �ڕW�̈ʒu
		MyLib::Vector3 posOrigin;	// �ʒu�̌��_
		MyLib::Vector3 scale;		// �X�P�[��
	}Parts;

	typedef struct
	{
		Parts aParts[MAX_PARTS];
		int nFrame;				// �Đ��t���[��
		float fRotMove;			// �ړ��̌���
	}Key;

	typedef struct
	{
		Key aKey[MAX_KEY];
		int nNumKey;			// �L�[�̐�
		int nLoop;				// ���[�v����
		int nMove;				// �ړ�����
		int nNumAttackInfo;		// �U�����̐�
		int nCancelableFrame;	// �L�����Z���\�t���[��
		int nCombolableFrame;	// �R���{�\�t���[��
		AttackInfo *AttackInfo[MAX_PARTS];	// �����蔻��p
	}Info;

	CMotion();
	~CMotion();

	HRESULT Init();
	void Uninit();
	void Update(float fBuff = 1.0f);
	static CMotion *Create(const std::string& file, CObjectChara* pObjChara);
	void Set(int nType, bool bBlend = true);	// ���[�V�����̐ݒ菈��
	int GetType();							// ���݂̃��[�V�����^�C�v�擾
	int GetOldType();						// �O��̃��[�V�����^�C�v�擾
	bool IsFinish();						// �I�����Ă��邩�̔���
	void ToggleFinish(bool bFinish);			// �I�����Ă��邩�̔���؂�ւ�
	bool IsGetCancelable() { return m_bCancelable; }	// �L�����Z���̔���擾
	bool IsGetCombiable() { return m_bCombiable; }	// �L�����Z���̔���擾
	bool IsBeforeInAttack() { return m_bBeforeInAction; }		// �U���O�t���O�擾
	bool IsAttacking() { return m_bAttaking; }					// �U�����蒆�t���O�擾
	bool IsAllAfterAttack() { return m_bAllAfterAttack; }		// �S�Ă̍U����t���O�擾
	void SetSlowFactor(float fFactor);			// �x���W���̐ݒ�

	bool IsImpactFrame(AttackInfo attackInfo);	// �Ռ��̃t���[�����ǂ����擾
	MyLib::Vector3 GetAttackPosition(CModel **ppModel, AttackInfo attackInfo);	// �U���̈ʒu�擾
	MyLib::Vector3 GetAttackPosition(CModel *pModel, AttackInfo attackInfo);	// �U���̈ʒu�擾
	float GetAllCount();	// �S�ẴJ�E���g�擾
	float GetMaxAllCount() { return m_fMaxAllFrame; }
	int GetMaxAllCount(int nType);	// �S�ẴJ�E���g�擾
	void SetFrameCount(float fCnt);					// �t���[���J�E���g�ݒ�
	float GetFrameCount() { return m_fCntFrame; }	// �t���[���J�E���g�擾
	int GetNumAll() { return m_nNumModel; }		// �p�[�c���擾
	int GetNumMotion() { return m_nNumMotion; }	// ���[�V�����̑����擾
	void SetNowPattern(int nPattern) { m_nPatternKey = nPattern; }		// ���݂̃p�^�[���ݒ�
	int GetNowPattern() { return m_nPatternKey; }	// ���݂̃p�^�[���擾
	void AddNumAttackInfo(int nType);	// �U�����̑������Z
	void SubNumAttackInfo(int nType);	// �U�����̑������Z

	Parts GetPartsOld(int nParts);
	void SetPartsOld(int nParts, Parts parts);
	void SetInfo(Info info);	// ���[�V�������̓o�^
	void SetInfoData(int nType, Info info);	// ���[�V�������̓o�^
	void SetAttackInfo(AttackInfo info);	// �U�����̓o�^
	Info GetInfo(int nType);	// ���[�V�������̎擾
	Info *GetInfoPtr(int nType);	// ���[�V�������̎擾
	void SetInfoSave(int nType, int nKey, int nParts, MyLib::Vector3 rot);	// ���[�V�������̓o�^
	void ChangeKeyNum(int nType, int nNum);	// �L�[�����ύX
	void ChangeKeyFrame(int nType, int nKey, int nNum);	// �L�[�t���[���ύX
	void ChangeAttackInfo(int nType, int nIdx, AttackInfo info);	// �U�����ύX
	int IsGetMove(int nType);	// �ړ��̔���擾
	void SetModel(CModel **pModel, int nNumModel);	// ���[�V���������郂�f���̓o�^
	void ResetPose(int nType);	// �|�[�Y�̃��Z�b�g

private:
	// �񋓌^��`
	typedef enum
	{
		LOOP_OFF = 0,	// ���[�v����
		LOOP_ON,		// ���[�v����
		LOOP_MAX
	}LOOP;

	// �����o�֐�
	void ReadText(const std::string& file);
	void LoadMotion(const std::string& file, int nMotion);

	// �����o�ϐ�
	Info* m_pInfo;	// ���[�V�����̏��
	Parts* m_pPartsOld;	// �ߋ��̏��
	int m_nNumAll;				// ���[�V�����̑���
	int m_nType;				// ���݂̃��[�V�����̎��
	int m_nOldType;				// �O��̃��[�V�����̎��
	bool m_bLoop;				// ���[�v���邩�ǂ���
	int m_nPatternKey;			// ���ڂ̃L�[��
	float m_fCntFrame;			// �t���[���̃J�E���g
	float m_fCntAllFrame;		// �S�ẴJ�E���g
	float m_fMaxAllFrame;		// �S�ẴJ�E���g�̍ő�l
	float m_fSlowFactor;		// �x���W��
	bool m_bFinish;				// �I���������ǂ���
	bool m_bCancelable;			// �L�����Z���\��
	bool m_bCombiable;			// �R���{�\��
	bool m_bBeforeInAction;		// �U���O�t���O
	bool m_bAttaking;			// �U�����蒆�t���O
	bool m_bAllAfterAttack;		// �S�Ă̍U����t���O

	CObjectChara *m_pObjChara;	// �I�u�W�F�N�g�L�����N�^�[�̃|�C���^
	CModel **m_ppModel;			// ���f���̃|�C���^
	int m_nNumModel;			// �p�[�c�̑���
	int m_nNumMotion;			// ���[�V�����̑���
	static std::vector<std::string> m_sTextFile;	// �e�L�X�g�t�@�C����
	static int m_nNumLoad;	// �ǂݍ��񂾐�
	static std::vector<int> m_nNumLoadData;				// ���[�V�������̃f�[�^��
	static std::vector<CMotion::Info> m_aLoadData[MAX_MOTION];	// ���[�V�����̓ǂݍ��ݏ��
	static std::vector<AttackInfo> m_aLoadAttackData[MAX_MOTION][MAX_PARTS];	// �U���̓ǂݍ��ݏ��
};

#endif