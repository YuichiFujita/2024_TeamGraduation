//=============================================================================
//
// �J�������� [camera.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _CAMERA_H_		// ���̃}�N����`������Ă��Ȃ�������
#define _CAMERA_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include "scene.h"
#include "camera_motion.h"	// �J�������[�V����

class CStateCameraR;	// �����_�̏��
class CStateCameraV;	// ���_�̏��
class CCameraControlState;	// ��ԕʑ���

//==========================================================================
// �N���X��`
//==========================================================================
// �J�����N���X��`
class CCamera
{
private:

	// �񋓌^��`
	enum STATE
	{
		CAMERASTATE_NONE = 0,	// �ʏ�
		CAMERASTATE_SHAKE,		// �U��
		CAMERASTATE_PRAYER,		// �F��
		CAMERASTATE_MAX
	};

public:

	// �Ǐ]�̎��
	enum CHASETYPE
	{
		CHASETYPE_NORMAL = 0,	// �ʏ�
		CHASETYPE_MAP,			// �}�b�v�ɒǏ]
		CHASETYPE_NONE,			// �Ǐ]�Ȃ�
		CHASETYPE_MAX
	};

	// ���b�N�I�����̌���
	enum RockOnDir
	{
		ROCKON_DIR_RIGHT = 0,
		ROCKON_DIR_LEFT,
		ROCKON_DIR_MAX
	};

	// �����_�̏��
	enum PosRState
	{
		POSR_STATE_NORMAL = 0,
		POSR_STATE_ROCKON,		// ���b�N�I�����
		POSR_STATE_PRAYER,		// �F����
		POSR_STATE_MAX
	};

	enum RockOnState
	{
		ROCKON_NORMAL = 0,	// �ʏ�
		ROCKON_COUNTER,	// �J�E���^�[
		ROCKON_ROCKON,	// ���b�N�I��
		ROCKON_MAX
	};

	CCamera();
	~CCamera();

	HRESULT Init();
	void Uninit();
	void Update();
	void SetCamera();

	void SetRotation(const MyLib::Vector3 rot);	// �����ݒ�
	MyLib::Vector3 GetRotation() const;			// �����擾
	void SetOriginRotation(const MyLib::Vector3 rot);	// ���̌����ݒ�
	MyLib::Vector3 GetOriginRotation() const { return m_rotOrigin; }		// ���̌����擾
	MyLib::Vector3 GetPositionV() const;		// �J�����̈ʒu�擾
	MyLib::Vector3 GetPositionR() const;		// �J�����̒����_�擾
	void SetOriginDistance(float fDistance);	// ���ɂȂ�J�����̋����ݒ�
	float GetOriginDistance();				// ���ɂȂ�J�����̋����擾
	void SetDestRotation(const MyLib::Vector3& rot);	// �ڕW�̌����ݒ�
	MyLib::Vector3 GetDestRotation();				// �ڕW�̌����擾
	void SetTargetPosition(const MyLib::Vector3 pos);	// �Ǐ]�ڕW�̈ʒu�ݒ�
	MyLib::Vector3 GetTargetPosition();					// �Ǐ]�ڕW�̈ʒu�擾
	void SetTargetPositionDest(const MyLib::Vector3& pos) { m_TargetPosDest = pos; }	// �Ǐ]�ڕW�̖ڕW�ʒu�ݒ�
	MyLib::Vector3 GetTargetPositionDest() { return m_TargetPosDest; }					// �Ǐ]�ڕW�̖ڕW�ʒu�擾
	void SetTargetRotation(const MyLib::Vector3 rot);	// ���b�N�I���̌����ݒ�
	void SetPositionV();

	// �ړ��ʌn
	MyLib::Vector3 GetMoveRot() { return m_Moverot; }	// �����̈ړ��ʎ擾
	void SetMoveRot(const MyLib::Vector3& rot) { m_Moverot = rot; }	// �����̈ړ��ʎ擾


	// ���b�N�I��
	void SetRockOnPosition(const MyLib::Vector3 pos);	// ���b�N�I���̈ʒu�ݒ�
	void SetRockOnDistance(const float distance);	// ���b�N�I���̉��������ݒ�
	MyLib::Vector3 GetRockOnPosition();			// �Ǐ]�ڕW�̈ʒu�擾
	void SetRockOn(const MyLib::Vector3 pos, bool bSet);	// ���b�N�I���ݒ�
	void SetRockDir(RockOnDir dir) { m_RockOnDir = dir; }	// ���b�N�I�����̃Y�������ݒ�

	// �J�������[�V����
	bool IsMotion() { return m_bMotion; }					// ���[�V���������ǂ���
	void SetEnableMotion(bool frag) { m_bMotion = frag; }	// ���[�V���������ǂ���
	CCameraMotion* GetCameraMotion() { return m_pCameraMotion; }		// �J�������[�V�����̃|�C���^


	// �֐����X�g
	typedef void(CCamera::* ROCKON_STATE_FUNC)();
	static ROCKON_STATE_FUNC m_RockOnStateFunc[];
	void SetRockOnState(RockOnState state, float height);	// ���b�N�I����Ԑݒ�
	CCamera::RockOnState GetRockOnState() { return m_stateRockOn; }	// ���b�N�I����Ԏ擾

	MyLib::Vector3 GetTargetRotation();			// �Ǐ]�ڕW�̌����擾
	D3DXMATRIX GetMtxView() { return m_mtxView; }
	D3DXMATRIX* GetMtxViewPtr() { return &m_mtxView; }
	D3DXMATRIX GetMtxProjection() { return m_mtxProjection; }
	D3DXMATRIX* GetMtxProjectionPtr() { return &m_mtxProjection; }
	D3DVIEWPORT9 GetViewPort() { return m_viewport; };			// �r���[�|�[�g

	void SetShake(int nTime, float fLength, float fLengthY);	// �U���ݒ�

	/**
	@brief	�ڕW�̒����ݒ�
	@param	fLength			[in]	�ڕW�̒���
	@param	nCntTime		[in]	���Z����܂ł̎���
	@param	DecrementValue	[in]	������
	@param	fCorrection		[in]	�����␳�W��
	@return	void
	*/
	void SetLenDest(float fLength, int nCntTime = 120, float DecrementValue = 2.0f, float fCorrection = 0.1f);
	void SetPlayerChaseIndex(int nIdx);				// �Ǐ]����v���C���[�̃C���f�b�N�X�ԍ��ݒ�
	int GetPlayerChaseIndex();					// �Ǐ]����v���C���[�̃C���f�b�N�X�ԍ��擾
	bool IsFollow();										// �Ǐ]��Ԏ擾
	bool IsRockOn() { return m_bRockON; }					// �Ǐ]��Ԏ擾
	bool IsRotationZ() { return m_bRotationZ; }
	bool IsRotationY() { return m_bRotationY; }
	void SetEnableFollow(bool bFollow);							// �Ǐ]�̔���ݒ�
	void SetEnableRotationY(bool bRotation) { m_bRotationY = bRotation; }	// Y��]�̃t���O
	void SetViewPort(MyLib::Vector3 pos, D3DXVECTOR2 size);		// �r���[�|�[�g�̐ݒ�
	void Reset(CScene::MODE mode);				// ���Z�b�g
	void ResetBoss();						// ���Z�b�g
	bool IsOnScreen(const MyLib::Vector3 pos);	// �X�N���[�����̔���
	MyLib::Vector3 GetScreenPos(const MyLib::Vector3 pos);	// �X�N���[�����W�擾
	void MoveCameraStick(int nIdx = 0);			// �X�e�B�b�N����
	void WarpCamera(MyLib::Vector3 pos);		// �J�������[�v
	void SetAutoDistanceDest(float fDistance) { m_fAutoDistance_Dest = fDistance; }
	void SetDistance(float fDistance) { m_fDistance = fDistance; }
	int GetCntDistance() { return m_nCntDistance; }
	int GetCntDistanceOrigin() { return m_nOriginCntDistance; }

	// ���擾
	void SetPositionR(const MyLib::Vector3& pos) { m_posR = pos; }
	MyLib::Vector3 GetPositionR() { return m_posR; }
	void SetPositionRDest(const MyLib::Vector3& pos) { m_posRDest = pos; }
	MyLib::Vector3 GetPositionRDest() { return m_posRDest; }
	void SetPositionVDest(const MyLib::Vector3& pos) { m_posVDest = pos; }
	MyLib::Vector3 GetPositionVDest() { return m_posVDest; }
	float GetAutoDistanceDest() { return m_fAutoDistance_Dest; }
	float GetDistance() { return m_fDistance; }
	float GetDistanceDecrementValue() { return m_fDistanceDecrementValue; }
	float GetDistanceDest() { return m_fDestDistance; }
	int GetDistanceCnt() { return m_nCntDistance; }
	void SetAutoMovingPosR(const MyLib::Vector3& pos) { m_AutoMovingPosR = pos; }	// �����J�����ړ����̒����_
	MyLib::Vector3 GetAutoMovingPosR() { return m_AutoMovingPosR; }					// �����J�����ړ����̒����_�擾
	CCameraMotion* GetMotion() { return m_pCameraMotion; }

	// �X�e�[�g�p�^�[���ݒ�
	void SetStateCamraR(CStateCameraR* state);	// �����_�̏�Ԑݒ�
	void SetStateCameraV(CStateCameraV* state);	// ���_�̏�Ԑݒ�
	void SetStateCameraR(PosRState state) { m_StateCameraR = state; }
	PosRState GetStateCameraR() { return m_StateCameraR; }

	void SetControlState(CCameraControlState* state);	// ����̏�Ԑݒ�
private:

	// �����o�ϐ�
	void UpdateByMode();	// ���[�h�ʍX�V����
	void MoveCameraInput();
	void MoveCameraMouse();
	void MoveCameraVR();
	void MoveCameraV();
	void MoveCameraR();
	void MoveCameraDistance();
	void SetCameraV();
	void SetCameraVTitle();
	void SetCameraVGame();
	void SetCameraVResult();
	void SetCameraVRanking();
	void SetCameraR();
	void SetCameraRTitle();
	void SetCameraRGame();
	void SetCameraRResult();
	void SetCameraRRanking();
	void MoveCameraFollow();
	void Shake();
	void StatePrayer();	// �F��
	void UpdateState();
	void UpdateSpotLightVec();
	void RockOnStateNormal();	// �ʏ��Ԃ̃��b�N�I������
	void RockOnStateCounter();	// �J�E���^�[��Ԃ̃��b�N�I������

	

	// ���Z�b�g
	void ResetGame();
	void ResetTitle();
	void ResetResult();
	void ResetRanking();

	D3DXMATRIX	m_mtxProjection;		// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX	m_mtxView;				// �r���[�}�g���b�N�X
	D3DVIEWPORT9 m_viewport;			// �r���[�|�[�g
	MyLib::Vector3 m_posV;				// ���_(�J�����̈ʒu)
	MyLib::Vector3 m_posR;				// �����_(�������ꏊ)
	MyLib::Vector3 m_posVDest;			// �ڕW�̎��_
	MyLib::Vector3 m_posRDest;			// �ڕW�̒����_
	MyLib::Vector3 m_vecU;				// ������x�N�g��
	MyLib::Vector3 m_move;				// �ړ���
	MyLib::Vector3 m_moveWASD;			// WASD�̈ړ���
	MyLib::Vector3 m_rot;				// ����
	MyLib::Vector3 m_rotOrigin;			// ���̌���
	MyLib::Vector3 m_rotDest;			// �ڕW�̌���
	MyLib::Vector3 m_Moverot;			// �����̈ړ���
	MyLib::Vector3 m_rotVDest;			// �ڕW�̎��_�̌���
	MyLib::Vector3 m_TargetPos;			// �Ǐ]�ڕW�̈ʒu
	MyLib::Vector3 m_TargetPosDest;		// �ڕW�̒Ǐ]�ڕW�̈ʒu
	MyLib::Vector3 m_TargetRot;			// �Ǐ]�ڕW�̌���
	MyLib::Vector3 m_RockOnPos;			// ���b�N�I���Ώۂ̈ʒu
	MyLib::Vector3 m_AutoMovingPosR;	// �����J�����ړ����̒����_
	float m_fRockOnDistance;
	float m_fDistance;					// ����
	float m_fDestDistance;				// �ڕW�̋���
	float m_fOriginDistance;			// ���̋���
	float m_nShakeLength;				// �h��̒���
	float m_nShakeLengthY;				// Y�̗h��̒���
	float m_fMoveShake;					// �h��̈ړ���
	float m_fMoveShakeY;				// Y�̗h��̈ړ���
	float m_fHeightMaxDest;				// �J�����̍ő卂���̖ڕW
	float m_fHeightMax;					// �J�����̍ő卂��
	float m_fDiffHeight;				// �����̍���
	float m_fDiffHeightSave;			// �����̍����ۑ��p
	float m_fDiffHeightDest;			// �ڕW�̍����̍���
	float m_fChaseLerpStart;			// �Ǐ]�␳�̏����l
	float m_fChaseDistance;				// �Ǐ]�̊Ԋu
	float m_fDestChaseDistance;			// �ڕW�̒Ǐ]�̊Ԋu
	bool m_bFollow;						// �Ǐ]���邩�ǂ���
	bool m_bMotion;						// ���[�V���������ǂ���
	bool m_bRotationZ;					// Z��]�o���邩�ǂ���
	bool m_bRotationY;					// Y��]�o���邩�ǂ���
	bool m_bRockON;						// ���b�N�I������
	int m_nCntState;					// ��ԃJ�E���^�[
	int m_nCntDistance;					// �����J�E���^�[
	int m_nOriginCntDistance;			// ���̋����J�E���^�[
	float m_fDistanceCorrection;		// �����̊����␳�W��
	float m_fDistanceDecrementValue;	// �����̌����W��
	STATE m_state;						// ���
	CHASETYPE m_ChaseType;				// �Ǐ]�̎��
	CHASETYPE m_OldChaseType;			// �O��̒Ǐ]�̎��
	int m_nCntChaseType;				// �Ǐ]�̃J�E���^�[
	int m_nChasePlayerIndex;			// �Ǐ]����v���C���[�̃C���f�b�N�X�ԍ�
	RockOnDir m_RockOnDir;				// ���b�N�I�����̌���
	RockOnState m_stateRockOn;			// ���b�N�I�����̏��
	CCameraMotion* m_pCameraMotion;		// �J�������[�V�����̃|�C���^


	float m_fAutoDistance_Dest;					// ����
	float m_fAutoRot_Dest;					// ����



	PosRState m_StateCameraR;		// �����_�̏��
	CStateCameraR* m_pStateCameraR;	// �����_�̏�ԃ|�C���^
	CStateCameraV* m_pStateCameraV;	// ���_�̏�ԃ|�C���^
	CCameraControlState* m_pControlState;	// ����̏�ԃ|�C���^
};

//=============================
// �����_�̏�ԃN���X
//=============================
class CStateCameraR
{
public:
	CStateCameraR() {}

	virtual void SetCameraR(CCamera* pCamera);
};

class CStateCameraR_RockOn : public CStateCameraR
{
public:
	CStateCameraR_RockOn() {}

	virtual void SetCameraR(CCamera* pCamera) override;
};

class CStateCameraR_Prayer : public CStateCameraR
{
public:
	CStateCameraR_Prayer() {}

	virtual void SetCameraR(CCamera* pCamera) override;
};

//=============================
// ���_�̏�ԃN���X
//=============================
class CStateCameraV
{
public:
	CStateCameraV() {}

	virtual void LimitPos(CCamera* pCamera);	// �ʒu����
	virtual void Distance(CCamera* pCamera);	// ��������
	virtual void SetMotion(CCamera* pCamera){}	// ���[�V�����ݒ�

protected:
	float GetDistance(CCamera* pCamera, const float fMultiply);
};

class CStateCameraV_Main : public CStateCameraV
{
public:
	CStateCameraV_Main() {}
};

class CStateCameraV_Enhance : public CStateCameraV
{
public:
	CStateCameraV_Enhance() {}

	virtual void LimitPos(CCamera* pCamera) override;	// �ʒu����
};

class CStateCameraV_Goal : public CStateCameraV
{
public:
	CStateCameraV_Goal() {}

	virtual void LimitPos(CCamera* pCamera) override;	// �ʒu����
	virtual void Distance(CCamera* pCamera) override;	// ��������
	virtual void SetMotion(CCamera* pCamera) override;	// ���[�V�����ݒ�
};

class CStateCameraV_Distance : public CStateCameraV
{
public:
	CStateCameraV_Distance() {
		m_fMultiPly = 0.0f;
		m_fStartDistance = 0.0f;
	}

	virtual void Distance(CCamera* pCamera) override;	// ��������
	void SetStartDistance(float fStartDistance) { m_fStartDistance = fStartDistance; }

	float m_fMultiPly;
	float m_fStartDistance;
};

//=============================
// ��ԃp�^�[��
//=============================
// ���N���X
class CCameraControlState
{
public:
	CCameraControlState(CCamera* pCamera) { pCamera->SetStateCameraR(CCamera::POSR_STATE_NORMAL); }

	virtual void MoveCamera(CCamera* pCamera);	// �ړ�����
};

// �ʏ�
class CCameraControlState_Normal : public CCameraControlState
{
public:
	CCameraControlState_Normal(CCamera* pCamera) : CCameraControlState(pCamera) { pCamera->SetStateCameraR(CCamera::POSR_STATE_NORMAL); }
};

// ���b�N�I��
class CCameraControlState_RockOn : public CCameraControlState
{
public:
	CCameraControlState_RockOn(CCamera* pCamera) : CCameraControlState(pCamera) { pCamera->SetStateCameraR(CCamera::POSR_STATE_ROCKON); }

	virtual void MoveCamera(CCamera* pCamera) override;	// �ړ�����
};

// �F�菀��
class CCameraControlState_BeforePrayer : public CCameraControlState
{
public:
	CCameraControlState_BeforePrayer(CCamera* pCamera) : CCameraControlState(pCamera) { pCamera->SetStateCameraR(CCamera::POSR_STATE_NORMAL); }

	virtual void MoveCamera(CCamera* pCamera) override;	// �ړ�����
};

// �F���
class CCameraControlState_AfterPrayer : public CCameraControlState_BeforePrayer
{
public:
	CCameraControlState_AfterPrayer(CCamera* pCamera) : CCameraControlState_BeforePrayer(pCamera) { pCamera->SetStateCameraR(CCamera::POSR_STATE_NORMAL); }
};

// �F��
class CCameraControlState_Prayer : public CCameraControlState
{
public:
	CCameraControlState_Prayer(CCamera* pCamera) : CCameraControlState(pCamera) { pCamera->SetStateCameraR(CCamera::POSR_STATE_PRAYER); }

	virtual void MoveCamera(CCamera* pCamera) override;	// �ړ�����
};

// �{�X���o
class CCameraControlState_BossAppearance : public CCameraControlState
{
public:
	CCameraControlState_BossAppearance(CCamera* pCamera) : CCameraControlState(pCamera) { pCamera->SetStateCameraR(CCamera::POSR_STATE_NORMAL); }

};

#endif