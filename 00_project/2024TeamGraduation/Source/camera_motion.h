//=============================================================================
//
// �J�������[�V�������� [camera_motion.h]
// Author : ���n�Ή�
// Adder  : ���c�E��
//
//=============================================================================
#ifndef _CAMERA_MOTION_H_		// ���̃}�N����`������Ă��Ȃ�������
#define _CAMERA_MOTION_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "scene.h"
#include "camera_motion_trigger.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �J�������[�V�����N���X
class CCameraMotion
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum MOTION
	{
		MOTION_PASS = 0,		// �p�X���[�V����
		MOTION_SPAWN,			// �o�ꉉ�o���[�V����
		MOTION_SPECIAL_HYPE,	// �X�y�V��������グ���[�V����
		MOTION_KAMEHAMEHA,		// ���߂͂ߔg���[�V����
		MOTION_MAX
	};

	// �␳���
	enum EEasing
	{
		Linear = 0,
		EaseIn,
		EaseOut,
		EaseInOut,
		EaseInExpo,
		EaseOutExpo,
		EaseInOutExpo,
		MAX
	};

	//=============================
	// �\���̒�`
	//=============================
	// �L�[���
	struct MotionKey
	{
		MyLib::Vector3 posRDest;	// �ڕW�̒����_
		MyLib::Vector3 rotDest;		// �ڕW�̌���
		float distance;				// ����
		float playTime;				// �Đ��t���[��
		EEasing easeType;			// �C�[�W���O�̎��

		MotionKey() : posRDest(0.0f), rotDest(0.0f), distance(0.0f), playTime(1.0f), easeType(EEasing::Linear) {}
	};

	// ���[�V�������
	struct MotionInfo
	{
		std::vector<MotionKey> Key;	// �L�[���
		std::vector<float> trigger;	// �g���K�[

		MotionInfo() : Key() {}
	};

	// �G�f�B�b�g���
	struct EditInfo
	{
		int motionIdx;			// ���[�V�����C���f�b�N�X
		int keyIdx;				// �L�[�C���f�b�N�X
		int triggerIdx;			// �g���K�[�C���f�b�N�X
		float playRatio;		// �Đ�����
		bool bSlide;			// �X���C�h������
		MyLib::Vector3 offset;	// �I�t�Z�b�g
		MotionInfo motionInfo;	// ���[�V�������
		MotionKey Key_copyData;	// �L�[�̃R�s�[�f�[�^

		EditInfo() : motionIdx(0), keyIdx(0), triggerIdx(0), playRatio(0.0f), bSlide(false), motionInfo(),
			Key_copyData(MotionKey()) {}
	};

	// �g���K�[���
	struct TriggerInfo
	{
		bool bTrigger;	// �g���K�[����
		int nIdx;		// �C���f�b�N�X

		TriggerInfo() : bTrigger(false), nIdx(0) {}
		TriggerInfo(bool _bTrigger, int _nIdx) : bTrigger(_bTrigger), nIdx(_nIdx) {}
	};

	CCameraMotion();
	~CCameraMotion();

	//=============================
	// �����o�֐�
	//=============================
	HRESULT Init();		// ������
	void LoadText();	// �e�L�X�g�ǂݍ���
	void Uninit();		// �I��
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �X�V

	TriggerInfo GetTrigger();	// �g���K�[����擾
	void SetMotion				// ���[�V�����ݒ�
	(
		int nMotion,			// ���[�V�������
		bool bInverse = false,	// ���]�t���O
		bool bPos = true,		// �ʒu����t���O
		bool bRot = true,		// ��������t���O
		bool bDis = true		// ��������t���O
	);

	// ��񑀍�
	inline void SetPosition(const MyLib::Vector3& pos)	{ m_pos = pos; }	// �ʒu�ݒ�
	inline MyLib::Vector3 GetPosition()					{ return m_pos; }	// �ʒu�擾
	inline int GetNowMotionIdx()			{ return m_nNowMotionIdx; }
	inline void SetFinish(bool bFinish)		{ m_bFinish = bFinish; }
	inline bool IsFinish()	{ return m_bFinish; }
	inline bool IsEdit()	{ return m_bEdit; }

	inline void SetEnablePause(bool bPause)			{ m_bPause = bPause; }			// �|�[�Y����
	inline void SetEnableSystemPause(bool bPause)	{ m_bSystemPause = bPause; }	// �V�X�e���|�[�Y����
	inline bool IsPause()							{ return m_bPause; }			// �|�[�Y����

	inline int GetNowTriggerIdx() { return m_nNowTriggerIdx; }
	inline int GetNowKeyIdx() { return m_nNowKeyIdx; }
	inline int GetNowKeyMax() { return m_vecMotionInfo[m_nNowMotionIdx].Key.size(); }
	inline int GetKeyMax(const MOTION motion) { return m_vecMotionInfo[motion].Key.size(); }

	MotionKey GetKeyData(const MOTION motion, const int nKeyIdx);	// �L�[���擾
	float GetWholeCurTimer();			// ���݂̑S�̎��Ԏ擾
	float GetWholeMaxTimer();			// �ő�̑S�̎��Ԏ擾
	bool IsImpactFrame(int nCntAtk);	// �w�肵���C���f�b�N�X�̏�񂪏Ռ��J�E���g��

	// �ÓI�֐�
	static CCameraMotion* Create();	// ��������

private:

	//=============================
	// �֐����X�g
	//=============================
	// ���[�V�������X�g
	typedef void(CCameraMotion::*MOTION_FUNC)();
	static MOTION_FUNC m_MotionFunc[];

#if 0
	// vec3���`�␳���X�g
	typedef MyLib::Vector3(*VEC3_EASING_FUNC)(const MyLib::Vector3&, const MyLib::Vector3&, float);
	static VEC3_EASING_FUNC m_Vec3EasingFunc[];

	// float���`�␳���X�g
	typedef float(*FLOAT_EASING_FUNC)(float, float, float);
	static FLOAT_EASING_FUNC m_FloatEasingFunc[];
#else
	// vec3���`�␳���X�g
	typedef MyLib::Vector3(*VEC3_EASING_FUNC)(const MyLib::Vector3&, const MyLib::Vector3&, float, float, float);
	static VEC3_EASING_FUNC m_Vec3EasingFunc[];

	// float���`�␳���X�g
	typedef float(*FLOAT_EASING_FUNC)(float, float, float, float, float);
	static FLOAT_EASING_FUNC m_FloatEasingFunc[];
#endif

	//=============================
	// �����o�֐�
	//=============================
	// �Z�[�u�����[�h
	void SaveMotion(const std::string& filename, const MotionInfo& info);
	void LoadMotion(const std::string& filename);	// ���[�V�����ǂݍ���

	// ���[�V����
	void MotionPass();			// �p�X
	void MotionSpawn();			// �o�ꉉ�o
	void MotionSpecial();		// �X�y�V��������グ
	void MotionKamehameha();	// ���߂͂ߔg

	// �G�f�B�b�g�p
	void UpdateEdit();		// �G�f�B�b�g�X�V
	void SliderPlay();		// �X���C�h�Đ�
	void ChangeMotion();	// ���[�V�����؂�ւ�
	void ChangeKey();		// �L�[�؂�ւ�
	void EditMotion();		// ���[�V�����G�f�B�b�g
	void EditKey();			// �L�[�G�f�B�b�g
	void EditTrigger();		// �g���K�[�G�f�B�b�g

	// ���̑�
	void TriggerMoment();	// �g���K�[����̏u��

	//=============================
	// �����o�ϐ�
	//=============================
	std::vector<MotionInfo> m_vecMotionInfo;	// ���[�V�������
	std::vector<std::string> m_MotionFileName;	// ���[�V�����t�@�C����
	std::string m_PathName;	// �p�X��
	MyLib::Vector3 m_pos;	// �ʒu
	int m_nNowMotionIdx;	// ���݂̃��[�V�����C���f�b�N�X
	int m_nNowKeyIdx;		// ���݂̃L�[�C���f�b�N�X
	int m_nNowTriggerIdx;	// ���݂̃g���K�[�C���f�b�N�X
	float m_fMotionTimer;	// ���[�V�����^�C�}�[
	float m_fTriggerTimer;	// �g���K�[�^�C�}�[
	bool m_bFinish;			// �I������
	bool m_bEdit;			// �G�f�B�b�g�g�p����
	bool m_bPause;			// �|�[�Y����
	bool m_bSystemPause;	// �V�X�e���|�[�Y����
	bool m_bTrigger;		// �g���K�[����
	bool m_bMovePos;		// �ʒu����t���O
	bool m_bMoveRot;		// ��������t���O
	bool m_bMoveDis;		// ��������t���O
	bool m_bInverse;		// ���]�t���O
	EditInfo m_EditInfo;	// �G�f�B�b�g���
	CCameraMotion_Trigger* m_pCameraMotion_Trigger[MOTION_MAX];
};

#endif