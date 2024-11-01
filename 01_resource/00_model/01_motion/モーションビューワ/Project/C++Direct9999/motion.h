//=============================================================================
// 
//  ���[�V�����w�b�_�[ [motion.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _MOTION_H_
#define _MOTION_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "constans.h"

class CModel;
class CObjectChara;

#define MAX_MOTION	(32)	// ���[�V�����̍ő吔
#define MAX_KEY		(24)	// �L�[�̍ő吔
#define MAX_PARTS	(48)	// �p�[�c�̍ő吔

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
		D3DXVECTOR3 Offset;		// ����̃I�t�Z�b�g
		int nMinCnt;			// ����̍Œ�J�E���g
		int nMaxCnt;			// ����̍ő�J�E���g
		int nDamage;			// �_���[�W
		int nInpactCnt;			// �Ռ��̃J�E���g
	}AttackInfo;

	// ���[�V�����̍\����
	typedef struct
	{
		D3DXVECTOR3 rot;		// ����
		D3DXVECTOR3 rotDest;	// �ڕW�̌���
		D3DXVECTOR3 pos;		// �ʒu
		D3DXVECTOR3 posDest;	// �ڕW�̈ʒu
		D3DXVECTOR3 posOrigin;	// �ʒu�̌��_
		D3DXVECTOR3 scale;		// �X�P�[��
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
		std::string filename;	// �t�@�C����
		int nNumKey;			// �L�[�̐�
		int nLoop;				// ���[�v����
		int nMove;				// �ړ�����
		int nNumAttackInfo;		// �U�����̐�
		int nMaxAllFrame;		// �S�ẴJ�E���g
		int nCancelableFrame;	// �L�����Z���\�t���[��
		int nCombolableFrame;	// �R���{�\�t���[��
		AttackInfo *AttackInfo[MAX_PARTS];	// �����蔻��p
	}Info;

	CMotion();
	~CMotion();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CMotion *Create(void);
	void Set(int nType);	// ���[�V�����̐ݒ菈��
	int GetType(void);		// ���݂̃��[�V�����^�C�v�擾
	bool IsFinish(void);	// �I�����Ă��邩�̔���
	bool IsGetCancelable(void);	// �L�����Z���̔���擾

	int GetAllCount(void);	// �S�ẴJ�E���g�擾
	int GetFrameCount(void) { return m_nCntFrame; }	// �t���[���J�E���g�擾
	int GetNumAll(void) { return m_nNumModel; }		// �p�[�c���擾
	int GetNumMotion(void) { return m_nNumMotion; }	// ���[�V�����̑����擾
	int GetNowPattern(void) { return m_nPatternKey; }	// ���݂̃p�^�[���擾
	void AddNumAttackInfo(int nType);	// �U�����̑������Z
	void SubNumAttackInfo(int nType);	// �U�����̑������Z

	void ReadText(const char *pTextFile);
	void SetInfo(Info info);	// ���[�V�������̓o�^
	void SetAttackInfo(AttackInfo info);	// �U�����̓o�^
	void ResetMoveData(int nType, int nKey);	// �ړ����̃��Z�b�g
	Info GetInfo(int nType);	// ���[�V�������̎擾
	void SetInfoSave(int nType, int nKey, int nParts, D3DXVECTOR3 rot);	// ���[�V�������̓o�^
	void SetInfoSavePos(int nType, int nKey, int nParts, D3DXVECTOR3 pos);	// ���[�V�������̓o�^
	void SetInfoSaveScale(int nType, int nKey, int nParts, D3DXVECTOR3 scale);	// ���[�V�������̓o�^
	void ChangeKeyNum(int nType, int nNum);	// �L�[�����ύX
	void ChangeKeyFrame(int nType, int nKey, int nNum);	// �L�[�t���[���ύX
	void ChangeCancelableFrame(int nType, int nFrame);	// �L�����Z���\�t���[���ύX
	void ChangeCombolableFrame(int nType, int nFrame);	// �R���{�\�t���[���ύX
	void ChangeAttackInfo(int nType, int nIdx, AttackInfo info);	// �U�����ύX
	int IsGetMove(int nType);		// �ړ��̔���擾
	void SetModel(CModel **pModel, int nNumModel, CObjectChara *pObjChara);	// ���[�V���������郂�f���̓o�^

	std::string GetMotionFilename(int nType) { return m_sMotionFile[nType]; }
	const std::vector<std::string>& GetMotionFilenameVector() { return m_sMotionFile; }
	std::string* GetMotionFilenameVectorPtr() { return m_sMotionFile.data(); }
private:
	// �񋓌^��`
	typedef enum
	{
		LOOP_OFF = 0,	// ���[�v����
		LOOP_ON,		// ���[�v����
		LOOP_MAX
	}LOOP;

	void LoadMotion(const std::string text, int nMotion);

	Info m_aInfo[MAX_MOTION];	// ���[�V�����̏��
	Parts aPartsOld[MAX_PARTS];	// �ߋ��̏��
	int m_nNumAll;				// ���[�V�����̑���
	int m_nType;				// ���݂̃��[�V�����̎��
	int m_nOldType;				// �O��̃��[�V�����̎��
	bool m_bLoop;				// ���[�v���邩�ǂ���
	int m_nPatternKey;			// ���ڂ̃L�[��
	int m_nCntFrame;			// �t���[���̃J�E���g
	int m_nCntAllFrame;			// �S�ẴJ�E���g
	bool m_bFinish;				// �I���������ǂ���
	bool m_bCancelable;			// �L�����Z���\��

	CObjectChara *m_pObjChara;	// �I�u�W�F�N�g�L�����N�^�[�̃|�C���^
	CModel **m_ppModel;			// ���f���̃|�C���^
	int m_nNumModel;			// �p�[�c�̑���
	int m_nNumMotion;			// ���[�V�����̑���
	std::vector<std::string> m_sMotionFile;	// ���[�V�����t�@�C����
	static int m_nNumLoad;	// �ǂݍ��񂾐�
	static std::vector<std::string> m_sTextFile;	// �e�L�X�g�t�@�C����
	static std::vector<int> m_nNumLoadData;		// ���[�V�������̃f�[�^��
	static std::vector<Info> m_aLoadData[MAX_MOTION];	// ���[�V�����̓ǂݍ��ݏ��
	static std::vector<AttackInfo> m_aLoadAttackData[MAX_MOTION][MAX_PARTS];	// �U���̓ǂݍ��ݏ��
};



#endif