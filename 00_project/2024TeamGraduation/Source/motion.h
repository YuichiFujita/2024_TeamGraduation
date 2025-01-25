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
#include "motionManager.h"
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
	// �R���X�g���N�^, �f�X�g���N�^
	//=============================
	CMotion();
	~CMotion();

	//=============================
	// �����o�֐�
	//=============================
	HRESULT Init(const std::string& file);
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);


	//--------------------------
	// �Đ������
	//--------------------------
	CMotionManager::Info GetInfo()			{ return m_vecInfo[m_nType]; }	// ���擾
	CMotionManager::Info GetInfo(int type)	{ return m_vecInfo[type]; }		// ���擾
	std::vector<CMotionManager::AttackInfo> GetAttackInfo() { return m_vecInfo[m_nType].AttackInfo; }	// �U�����擾
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
	inline int IsGetMove() { return (m_vecInfo[m_nType].nMove == 0) ? 0 : 1; }			// �ړ��̔���擾
	inline int IsGetMove(int nType) { return (m_vecInfo[nType].nMove == 0) ? 0 : 1; }	// �ړ��̔���擾
	inline bool IsFinish() { return m_bFinish; }						// �I�����Ă��邩�̔���
	inline void ToggleFinish(bool bFinish) { m_bFinish = bFinish; }		// �I�����Ă��邩�̔��苭���؂�ւ�
	inline bool IsGetCancelable() { return m_bCancelable; }				// �L�����Z���\�̔���擾
	inline bool IsGetCombiable() { return m_bCombiable; }				// �R���{�\�̔���擾
	inline bool IsAttacking() { return m_bAttaking; }					// �U�����蒆�t���O�擾
	inline bool IsSpecial() { return m_vecInfo[m_nType].bSpecial; }		// �X�y�V��������擾
	bool IsImpactFrame(const CMotionManager::Info& info);								// �w�肵����ނ̏�񂪏Ռ��J�E���g��
	bool IsImpactFrame(int nCntAtk);									// �w�肵���C���f�b�N�X�̏�񂪏Ռ��J�E���g��
	bool IsAlignFrame(const CMotionManager::Info& info);								// �w�肵����񂪃t���[����

	//--------------------------
	// �ݒ�
	//--------------------------
	void Set(int nType, int nStartKey = 0, bool bBlend = true, float fCntFrame = 0.0f);	// ���[�V�����̐ݒ菈��
	void ResetPose(int nType);	// �|�[�Y�̃��Z�b�g

	MyLib::Vector3 GetAttackPosition(CModel** ppModel, CMotionManager::AttackInfo attackInfo);	// �U���̈ʒu�擾
	MyLib::Vector3 GetAttackPosition(CModel* pModel, CMotionManager::AttackInfo attackInfo);	// �U���̈ʒu�擾
	
	CMotionManager::Parts GetPartsOld(int nParts) { return m_pPartsOld[nParts]; }				// �ߋ��̃p�[�c���擾
	void SetPartsOld(int nParts, CMotionManager::Parts parts) { m_pPartsOld[nParts] = parts; }	// �ߋ��̃p�[�c���ݒ�
	void SetModel(CModel** pModel, int nNumModel);	// ���[�V���������郂�f���̓o�^

	//=============================
	// �ÓI�����o�֐�
	//=============================
	static CMotion* Create(const std::string& file, CObjectChara* pObjChara);

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
	void UpdateRotation(int i, const CMotionManager::Parts& nowParts, const CMotionManager::Parts& nextParts, float ratio);	// �����̍X�V
	void UpdateScale(int i, const CMotionManager::Parts& nowParts, const CMotionManager::Parts& nextParts, float ratio);	// �X�P�[���̍X�V
	void UpdatePosition(int i, const CMotionManager::Info& nowInfo, const CMotionManager::Parts& nowParts, const CMotionManager::Parts& nextParts, float ratio, int nMaxFrame, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �ʒu�X�V
	void UpdateEntityPosition(int i, const CMotionManager::Info& nowInfo, const CMotionManager::Parts& nowParts, const CMotionManager::Parts& nextParts, float ratio, int nMaxFrame, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �{�̂��ƈʒu�X�V
	void UpdateVisualPosition(int i, const CMotionManager::Info& nowInfo, const CMotionManager::Parts& nowParts, const CMotionManager::Parts& nextParts, float ratio, int nMaxFrame, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �����ڂ݈̂ʒu�X�V

	// �����o�ϐ�
	std::vector<CMotionManager::Info> m_vecInfo;	// ���[�V�����̏��
	CMotionManager::Parts* m_pPartsOld;	// �ߋ��̏��
	int m_nType;				// ���݂̃��[�V�����̎��
	int m_nOldType;				// �O��̃��[�V�����̎��
	bool m_bLoop;				// ���[�v���邩�ǂ���
	int m_nPatternKey;			// ���ڂ̃L�[��
	float m_fCntFrame;			// �t���[���̃J�E���g
	float m_fAllFrame;			// �Đ����̑S�ẴJ�E���g
	float m_fMaxAllFrame;		// �Đ����̑S�ẴJ�E���g�̍ő�l
	float m_fSlowFactor;		// �x���W��
	bool m_bFinish;				// �I���������ǂ���
	bool m_bCancelable;			// �L�����Z���\��
	bool m_bCombiable;			// �R���{�\��
	bool m_bAttaking;			// �U�����蒆�t���O

	CObjectChara* m_pObjChara;			// �I�u�W�F�N�g�L�����N�^�[�̃|�C���^
	CModel** m_ppModel;					// ���f���̃|�C���^
	int m_nNumModel;					// �p�[�c�̑���
	int m_nNumMotion;					// ���[�V�����̑���
	std::vector<int> m_vecDefaultIdx;	// �f�t�H���g�̃C���f�b�N�X
};

#endif