//==========================================================================
//
// �J�������� [camera.h]
// Author : ���n�Ή�
// Adder  : ���c�E��
//
//==========================================================================

#ifndef _CAMERA_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _CAMERA_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "scene.h"
#include "lightDir.h"
#include "camera_motion.h"

//==========================================================================
// �O���錾
//==========================================================================
class CCamera_Debug;	// �f�o�b�O����

//==========================================================================
// �N���X��`
//==========================================================================
// �J�����N���X��`
class CCamera
{
public:

	// ��ԗ�
	enum STATE
	{
		STATE_NONE = 0,	// �ʏ�
		STATE_FOLLOW,	// �Ǐ]
		STATE_OUTFIELD,	// �O��
		STATE_MAX
	};

	// �J�����h��\����
	struct SSwing
	{
	public:
		// �R���X�g���N�^
		SSwing() :
			shiftPos	 (VEC3_ZERO),	// �ʒu�����
			fShiftAngle	 (0.0f),		// �ʒu�����炷�p�x
			fShiftLength (0.0f),		// �ʒu�����炷����
			fSubAngle	 (0.0f),		// ���炷�p�x�̌��Z��
			fSubLength	 (0.0f)			// ���炷�����̌��Z��
		{}

		// �����t���R���X�g���N�^
		SSwing(const float in_fShiftLength, const float in_fSubAngle, const float in_fSubLength) :
			shiftPos	 (VEC3_ZERO),		// �ʒu�����
			fShiftAngle	 (0.0f),			// �ʒu�����炷�p�x
			fShiftLength (in_fShiftLength),	// �ʒu�����炷����
			fSubAngle	 (in_fSubAngle),	// ���炷�p�x�̌��Z��
			fSubLength	 (in_fSubLength)	// ���炷�����̌��Z��
		{}

		// �f�X�g���N�^
		~SSwing() {}

		// �����o�ϐ�
		MyLib::Vector3 shiftPos;	// �ʒu�����
		float fShiftAngle;	// �ʒu�����炷�p�x
		float fShiftLength;	// �ʒu�����炷����
		float fSubAngle;	// ���炷�p�x�̌��Z��
		float fSubLength;	// ���炷�����̌��Z��
	};

	// ���E���W�\����
	struct SSide
	{
	public:
		// �f�t�H���g�R���X�g���N�^
		SSide() :
			l (0.0f),	// ��
			r (0.0f)	// �E
		{}

		// �����t���R���X�g���N�^
		SSide(const float in_fLR) :
			l (in_fLR),	// ��
			r (in_fLR)	// �E
		{}
		SSide(const float in_fL, const float in_fR) :
			l (in_fL),	// ��
			r (in_fR)	// �E
		{}

		// �f�X�g���N�^
		~SSide() {}

		// �����o�ϐ�
		float l;	// ��
		float r;	// �E
	};

	// �J�����|�C���g���
	struct SCameraPoint
	{
		// �f�t�H���g�R���X�g���N�^
		SCameraPoint() :
			posR		(VEC3_ZERO),	// �����_
			posV		(VEC3_ZERO),	// ���_
			rot			(VEC3_ZERO),	// ����
			fDistance	(0.0f)			// ����
		{}

		// �f�X�g���N�^
		~SCameraPoint() {}

		// �����o�ϐ�
		MyLib::Vector3 posR;	// �����_
		MyLib::Vector3 posV;	// ���_
		MyLib::Vector3 rot;		// ����
		float fDistance;		// ����
	};

	//-----------------------------
	// �R���X�g���N�^/�f�X�g���N�^
	//-----------------------------
	CCamera();
	virtual ~CCamera();

	//-----------------------------
	// ���z�֐�
	//-----------------------------
	virtual HRESULT Init();	// ������
	virtual void Uninit();	// �I��
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �X�V
	virtual void Reset();	// �J�������Z�b�g

	//-----------------------------
	// �����o�֐�
	//-----------------------------
	void SetCamera();							// �J�����ݒ� (�ʏ�)
	void SetCameraDressup();					// �J�����ݒ� (�����ւ�)
	void ResetByMode(CScene::MODE mode);		// ���[�h�ʃ��Z�b�g
	void SetSwing(const SSwing& swing);			// �J�����h��ݒ�
	void ResetSwing();							// �J�����h�ꃊ�Z�b�g
	void SetWarp(const MyLib::Vector3& pos);	// �J�������[�v�ݒ�
	void SetState(const STATE state, const bool bReset = true);	// ��Ԑݒ�
	inline STATE GetState() const { return m_state; }			// ��Ԏ擾

	//-----------------------------
	// �X�N���[��
	//-----------------------------
	bool IsOnScreen(const MyLib::Vector3& pos);				// �X�N���[��������
	MyLib::Vector3 GetScreenPos(const MyLib::Vector3& pos);	// �X�N���[�����W�擾

	//-----------------------------
	// �����_
	//-----------------------------
	inline void SetPositionR(const MyLib::Vector3& pos)			{ m_posR = pos; }			// �����_�̐ݒ�
	inline MyLib::Vector3 GetPositionR()						{ return m_posR; }			// �����_�̎擾
	inline void SetPositionRDest(const MyLib::Vector3& pos)		{ m_posRDest = pos; }		// �ڕW�����_�̐ݒ�
	inline MyLib::Vector3 GetPositionRDest()					{ return m_posRDest; }		// �ڕW�����_�̎擾
	inline void SetPositionROrigin(const MyLib::Vector3& pos)	{ m_posROrigin = pos; }		// ���_�����_�̐ݒ�
	inline MyLib::Vector3 GetPositionROrigin()					{ return m_posROrigin; }	// ���_�����_�̎擾

	//-----------------------------
	// ���_
	//-----------------------------
	inline void SetPositionV(const MyLib::Vector3& pos)		{ m_posV = pos; }		// ���_�̐ݒ�
	inline MyLib::Vector3 GetPositionV()					{ return m_posV; }		// ���_�̎擾
	inline void SetPositionVDest(const MyLib::Vector3& pos)	{ m_posVDest = pos; }	// �ڕW���_�̐ݒ�
	inline MyLib::Vector3 GetPositionVDest()				{ return m_posVDest; }	// �ڕW���_�̎擾

	//-----------------------------
	// ����
	//-----------------------------
	inline void SetDistance(float distance)			{ m_fDistance = distance; }			// �����̐ݒ�
	inline float GetDistance()						{ return m_fDistance; }				// �����̎擾
	inline void SetDistanceDest(float distance)		{ m_fDestDistance = distance; }		// �ڕW�����̎擾
	inline float GetDistanceDest()					{ return m_fDestDistance; }			// �ڕW�����̎擾
	inline void SetDistanceOrigin(float distance)	{ m_fOriginDistance = distance; }	// ���_�����̐ݒ�
	inline float GetDistanceOrigin()				{ return m_fOriginDistance; }		// ���_�����̎擾

	//-----------------------------
	// ����
	//-----------------------------
	inline void SetRotation(const MyLib::Vector3& rot)			{ m_rot = rot; }		// �����̐ݒ�
	inline MyLib::Vector3 GetRotation() const					{ return m_rot; }		// �����̎擾
	inline void SetDestRotation(const MyLib::Vector3& rot)		{ m_rotDest = rot; }	// �ڕW�����̐ݒ�
	inline MyLib::Vector3 GetDestRotation()						{ return m_rotDest; }	// �ڕW�����̎擾
	inline void SetOriginRotation(const MyLib::Vector3& rot)	{ m_rotOrigin = rot; }	// ���_�����̐ݒ�
	inline MyLib::Vector3 GetOriginRotation()					{ return m_rotOrigin; }	// ���_�����̎擾

	//-----------------------------
	// ����p
	//-----------------------------
	inline void SetViewAngle(const float fAngle)		{ m_fViewAngle = fAngle; }		// ����p
	inline float GetViewAngle()							{ return m_fViewAngle; }		// ����p
	inline void SetDestViewAngle(const float fAngle)	{ m_fDestViewAngle = fAngle; }	// �ڕW����p
	inline float GetDestViewAngle()						{ return m_fDestViewAngle; }	// �ڕW����p

	//-----------------------------
	// �J�������[�V����
	//-----------------------------
	inline CCameraMotion* GetCameraMotion() { return m_pCameraMotion; }	// �J�������[�V�����|�C���^�擾

	//-----------------------------
	// �t���O
	//-----------------------------
	inline void SetEnableLight(bool bLight)	{ m_pLight->SetEnableLight(bLight); }	// ���C�g�t���O�̐ݒ�
	inline void SetEnableMotion(bool frag)	{ m_bMotion = frag; }	// ���[�V����������ݒ�
	inline bool IsMotion()					{ return m_bMotion; }	// ���[�V����������擾
	
	//-----------------------------
	// ���_�}�g���b�N�X
	//-----------------------------
	void SetViewPort(const MyLib::Vector3& pos, const D3DXVECTOR2& size);		// �r���[�|�[�g�ݒ�
	inline D3DVIEWPORT9 GetViewPort()			{ return m_viewport; };			// �r���[�|�[�g�擾
	inline D3DXMATRIX GetMtxView()				{ return m_mtxView; }			// �r���[�}�g���b�N�X�擾
	inline D3DXMATRIX* GetMtxViewPtr()			{ return &m_mtxView; }			// �r���[�}�g���b�N�X�|�C���^�擾
	inline D3DXMATRIX GetMtxProjection()		{ return m_mtxProjection; }		// �v���W�F�N�V�����}�g���b�N�X�擾
	inline D3DXMATRIX* GetMtxProjectionPtr()	{ return &m_mtxProjection; }	// �v���W�F�N�V�����}�g���b�N�X�|�C���^�擾

	//-----------------------------
	// �Q�[���J����
	//-----------------------------
	SCameraPoint CameraPoint(const STATE state = STATE::STATE_NONE);	// ���݂̃J�����|�C���g�擾
	SCameraPoint FollowPoint();		// ���݂̒Ǐ]�|�C���g�擾
	SCameraPoint OutFieldPoint();	// ���݂̊O��|�C���g�擾

protected:
	//-----------------------------
	// �����o�֐�
	//-----------------------------
	void UpdateSwing();	// �J�����h��̍X�V

private:

	//-----------------------------
	// �֐����X�g
	//-----------------------------
	// ��Ԋ֐�
	typedef void(CCamera::*STATE_FUNC)(const float, const float, const float);	// �֐��^
	static STATE_FUNC m_StateFuncList[];	// �֐����X�g

	// ��ԃ��Z�b�g�֐�
	typedef void(CCamera::*RESET_FUNC)();	// �֐��^
	static RESET_FUNC m_ResetFuncList[];	// �֐����X�g

	//-----------------------------
	// �����o�֐�
	//-----------------------------
	// ��Ԋ֐�
	void UpdateNoneState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �ʏ��Ԃ̍X�V
	void UpdateFollowState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �Ǐ]��Ԃ̍X�V
	void UpdateOutFieldState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �O���Ԃ̍X�V

	// ���Z�b�g�֐�
	void ResetNoneState();		// �ʏ��ԃ��Z�b�g
	void ResetFollowState();	// �Ǐ]��ԃ��Z�b�g
	void ResetOutFieldState();	// �O���ԃ��Z�b�g

	// �Q�[���J�����֐�
	void UpdateFollow(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �Ǐ]�J�����̍X�V
	void UpdateOutField(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �O��J�����̍X�V
	bool UpdateTrans(const STATE state);	// �J������ԑJ�ڂ̍X�V
	bool IsPlayerOutWithBall();	// �O�삪�{�[���������Ă��邩
	SSide GetPlayerMaxSide();	// �v���C���[�ő卶�E���W�擾
	float CalcDistanceRate(const SSide& rSide);		// ���E�Ԃ̋��������v�Z
	float CalcFollowDistance(const float fDisRate);	// �Ǐ]�J�����̋����v�Z
	MyLib::Vector3 CalcFollowPositionR(const SSide& rSide, const float fDisRate);	// �Ǐ]�J�����̒����_�v�Z
	void RevFollowPositionR(float* pTargetX, const float fDisRate);					// X�����_�͈͕̔␳

	// �ėp�֐�
	MyLib::Vector3 CalcSpherePosition(const MyLib::Vector3& rPos, const MyLib::Vector3& rRot, const float fDis);	// ���ʍ��W�ϊ��ɂ�鑊�Έʒu�擾
	void UpdateSpotLightVec(const MyLib::Vector3& rPosR, const MyLib::Vector3& rPosV);	// ���C�g�x�N�g���̍X�V

	//-----------------------------
	// �����o�ϐ�
	//-----------------------------
	CCamera_Debug* m_pDebugControll;	// �f�o�b�O���
	CCameraMotion* m_pCameraMotion;		// �J�������[�V�������
	CLightDir* m_pLight;				// �f�B���N�V���i�����C�g���
	D3DXMATRIX m_mtxProjection;			// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX m_mtxView;				// �r���[�}�g���b�N�X
	D3DVIEWPORT9 m_viewport;			// �r���[�|�[�g���
	SSwing m_swing;						// �J�����h����
	MyLib::Vector3 m_posV;				// ���_
	MyLib::Vector3 m_posVDest;			// �ڕW���_
	MyLib::Vector3 m_posR;				// �����_
	MyLib::Vector3 m_posRDest;			// �ڕW�����_
	MyLib::Vector3 m_posROrigin;		// ���_�����_
	MyLib::Vector3 m_vecU;				// ������x�N�g��
	MyLib::Vector3 m_rot;				// ����
	MyLib::Vector3 m_rotDest;			// �ڕW����
	MyLib::Vector3 m_rotOrigin;			// ���_����
	float m_fDistance;					// ����
	float m_fDestDistance;				// �ڕW����
	float m_fOriginDistance;			// ���_����
	float m_fViewAngle;					// ����p
	float m_fDestViewAngle;				// �ڕW����p
	SSide m_side;						// ���E�ő�ʒu
	SSide m_sideDest;					// �ڕW���E�ő�ʒu
	STATE m_state;						// ���
	bool m_bMotion;						// ���[�V����������
	bool m_bOldWithBall;				// �O��̊O��{�[�������t���O
	float m_fTransTime;					// �Q�[���J������ԑJ�ڂ̎���
	float m_fEndTime;					// �Q�[���J������ԑJ�ڂ̏I������
	bool b;
	SCameraPoint m_transStartPoint;		// �Q�[���J������ԑJ�ڂ̊J�n�|�C���g
};

#endif
