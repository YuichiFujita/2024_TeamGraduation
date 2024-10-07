//=============================================================================
//
// �J�������[�V�������� [camera_motion.h]
// Author : ���n�Ή�
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
		MOTION_PASS = 0,	// �p�X���[�V����
		MOTION_GOAL,		// �S�[��
		MOTION_GOALBAG,		// �S�[���t�b�g��
		MOTION_RESULT,		// ���U���g
		MOTION_MAX
	};

	// �␳���
	enum EASING
	{
		Linear = 0,
		EaseIn,
		EaseOut,
		EaseInOut,
		MAX
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
	void Update();		// �X�V
	void SetMotion(int motion, EASING EasingType);	// ���[�V�����ݒ�

	// ��񑀍�
	void SetPosition(const MyLib::Vector3& pos) { m_pos = pos; }	// �ʒu�ݒ�
	MyLib::Vector3 GetPosition() { return m_pos; }					// �ʒu�擾
	int GetNowMotionIdx() { return m_nNowMotionIdx; }
	void SetFinish(bool bFinish) { m_bFinish = bFinish; }
	bool IsFinish() { return m_bFinish; }
	bool IsEdit() { return m_bEdit; }

	void SetEnablePause(bool bPause) { m_bPause = bPause; }	// �|�[�Y����
	void SetEnableSystemPause(bool bPause) { m_bSystemPause = bPause; }	// �V�X�e���|�[�Y����
	bool IsPause() { return m_bPause; }		// �|�[�Y����

	TriggerInfo GetTrigger();	// �g���K�[����擾

	int GetNowKeyIdx() { return m_nNowKeyIdx; }
	int GetNowKeyMax() { return m_vecMotionInfo[m_nNowMotionIdx].Key.size(); }

	// �ÓI�֐�
	static CCameraMotion* Create();	// ��������

private:

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

		MotionKey() : posRDest(0.0f), rotDest(0.0f), distance(0.0f), playTime(1.0f) {}
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

	//=============================
	// �֐����X�g
	//=============================
	// ���[�V�������X�g
	typedef void(CCameraMotion::*MOTION_FUNC)();
	static MOTION_FUNC m_MotionFunc[];

	//=============================
	// �����o�֐�
	//=============================
	// �Z�[�u�����[�h
	void SaveMotion(const std::string& filename, const MotionInfo& info);
	void LoadMotion(const std::string& filename);	// ���[�V�����ǂݍ���

	// ���[�V����
	void MotionPass();		// �p�X
	void MotionGoal();		// �S�[��
	void MotionGoalBag();	// �S�[���t�b�g��

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
	EASING m_EasingType;	// �␳�̎��
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
	EditInfo m_EditInfo;	// �G�f�B�b�g���

	CCameraMotion_Trigger* m_pCameraMotion_Trigger[MOTION_MAX];
};

#endif