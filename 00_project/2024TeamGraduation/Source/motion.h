//=============================================================================
// 
//  ���[�V�����w�b�_�[ [motion.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _MOTION_H_
#define _MOTION_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "gamemanager.h"
#include "constans.h"

//==========================================================================
// �O���錾
//==========================================================================
class CModel;
class CObjectChara;

//==========================================================================
// �N���X��`
//==========================================================================
// ���[�V�����N���X��`
class CMotion
{
public:

	//=============================
	// �\���̒�`
	//=============================
	// ����̍\����
	struct AttackInfo
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
	};

	// �p�[�c���
	struct Parts
	{
		MyLib::Vector3 rot;			// ����
		MyLib::Vector3 rotDest;		// �ڕW�̌���
		MyLib::Vector3 pos;			// �ʒu
		MyLib::Vector3 posDest;		// �ڕW�̈ʒu
		MyLib::Vector3 posOrigin;	// �ʒu�̌��_
		MyLib::Vector3 scale;		// �X�P�[��

		Parts() : scale(1.0f) {}
	};

	// �L�[���
	struct Key
	{
		std::vector<Parts> aParts;	// �p�[�c���
		int nFrame;					// �Đ��t���[��
		float fRotMove;				// �ړ��̌���
	};

	// �S�̂̏��
	struct Info
	{
		std::vector<Key> aKey;	// �L�[���
		int nNumKey;			// �L�[�̐�
		int nLoop;				// ���[�v����
		int nMove;				// �ړ�����
		bool bSpecial;			// �X�y�V��������
		int nNumAttackInfo;		// �U�����̐�
		int nCancelableFrame;	// �L�����Z���\�t���[��
		int nCombolableFrame;	// �R���{�\�t���[��
		std::vector<AttackInfo> AttackInfo;	// �����蔻��p

		// �R���X�g���N�^
		Info() : bSpecial(false), nCancelableFrame(-1), nCombolableFrame(-1) {}
	};

	CMotion();
	~CMotion();

	HRESULT Init();
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	static CMotion* Create(const std::string& file, CObjectChara* pObjChara);

	//--------------------------
	// �Đ������
	//--------------------------
	Info GetInfo()			{ return m_vecInfo[m_nType]; }	// ���擾
	Info GetInfo(int type)	{ return m_vecInfo[type]; }		// ���擾
	std::vector<AttackInfo> GetAttackInfo() { return m_vecInfo[m_nType].AttackInfo; }	// �U�����擾
	int GetType()			{ return m_nType; }				// ���݂̃��[�V�����^�C�v�擾
	int GetOldType()		{ return m_nOldType; }			// �O��̃��[�V�����^�C�v�擾
	float GetAllCount()		{ return m_fAllFrame; }		// �S�ẴJ�E���g�擾
	float GetMaxAllCount()	{ return m_fMaxAllFrame; }	// �S�ẴJ�E���g�̍ő�l�擾
	int GetMaxAllCount(int nType);	// �S�ẴJ�E���g�擾
	float GetFrameCount() { return m_fCntFrame; }	// �t���[���J�E���g�擾
	int GetNumAll() { return m_nNumModel; }		// �p�[�c���擾
	int GetNumMotion() { return m_nNumMotion; }	// ���[�V�����̑����擾
	void SetNowPattern(int nPattern) { m_nPatternKey = nPattern; }		// ���݂̃p�^�[���ݒ�
	int GetNowPattern() { return m_nPatternKey; }	// ���݂̃p�^�[���擾

	//--------------------------
	// �t���O
	//--------------------------
	inline int IsGetMove() { return m_vecInfo[m_nType].nMove; }			// �ړ��̔���擾
	inline int IsGetMove(int nType) { return m_vecInfo[nType].nMove; }	// �ړ��̔���擾
	inline bool IsFinish() { return m_bFinish; }						// �I�����Ă��邩�̔���
	inline void ToggleFinish(bool bFinish) { m_bFinish = bFinish; }		// �I�����Ă��邩�̔��苭���؂�ւ�
	inline bool IsGetCancelable() { return m_bCancelable; }				// �L�����Z���\�̔���擾
	inline bool IsGetCombiable() { return m_bCombiable; }				// �R���{�\�̔���擾
	inline bool IsAttacking() { return m_bAttaking; }					// �U�����蒆�t���O�擾
	inline bool IsSpecial() { return m_vecInfo[m_nType].bSpecial; }		// �X�y�V��������擾
	bool IsImpactFrame(int nCntAtk);									// �w�肵���C���f�b�N�X�̏�񂪏Ռ��J�E���g��

	//--------------------------
	// �ݒ�
	//--------------------------
	void Set(int nType, int nStartKey = 0, bool bBlend = true);		// ���[�V�����̐ݒ菈��
	void ResetPose(int nType);					// �|�[�Y�̃��Z�b�g

	MyLib::Vector3 GetAttackPosition(CModel** ppModel, AttackInfo attackInfo);	// �U���̈ʒu�擾
	MyLib::Vector3 GetAttackPosition(CModel* pModel, AttackInfo attackInfo);	// �U���̈ʒu�擾
	
	Parts GetPartsOld(int nParts) { return m_pPartsOld[nParts]; }				// �ߋ��̃p�[�c���擾
	void SetPartsOld(int nParts, Parts parts) { m_pPartsOld[nParts] = parts; }	// �ߋ��̃p�[�c���ݒ�
	void SetModel(CModel** pModel, int nNumModel);	// ���[�V���������郂�f���̓o�^

private:

	//=============================
	// �񋓌^��`
	//=============================
	enum LOOP
	{
		LOOP_OFF = 0,	// ���[�v����
		LOOP_ON,		// ���[�v����
		LOOP_MAX
	};

	// �����o�֐�
	void ReadText(const std::string& file);
	void LoadMotion(const std::string& file, int nMotion);
	void UpdateRotation(int i);	// �����̍X�V

	// �����o�ϐ�
	std::vector<Info> m_vecInfo;	// ���[�V�����̏��
	Parts* m_pPartsOld;	// �ߋ��̏��
	int m_nType;				// ���݂̃��[�V�����̎��
	int m_nOldType;				// �O��̃��[�V�����̎��
	bool m_bLoop;				// ���[�v���邩�ǂ���
	int m_nPatternKey;			// ���ڂ̃L�[��
	float m_fCntFrame;			// �t���[���̃J�E���g
	float m_fAllFrame;			// �S�ẴJ�E���g
	float m_fMaxAllFrame;		// �S�ẴJ�E���g�̍ő�l
	float m_fSlowFactor;		// �x���W��
	bool m_bFinish;				// �I���������ǂ���
	bool m_bCancelable;			// �L�����Z���\��
	bool m_bCombiable;			// �R���{�\��
	bool m_bAttaking;			// �U�����蒆�t���O

	CObjectChara* m_pObjChara;	// �I�u�W�F�N�g�L�����N�^�[�̃|�C���^
	CModel** m_ppModel;			// ���f���̃|�C���^
	int m_nNumModel;			// �p�[�c�̑���
	int m_nNumMotion;			// ���[�V�����̑���
	static std::vector<std::string> m_sTextFile;	// �e�L�X�g�t�@�C����
	static int m_nNumLoad;	// �ǂݍ��񂾐�
	static std::vector<int> m_nNumLoadData;				// ���[�V�������̃f�[�^��
	static std::vector<std::vector<CMotion::Info>> m_vecLoadData;		// ���[�V�����̓ǂݍ��ݏ��
};

#endif