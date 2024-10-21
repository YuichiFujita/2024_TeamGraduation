//=============================================================================
//
// �J�������� [camera.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _CAMERA_H_		// ���̃}�N����`������Ă��Ȃ�������
#define _CAMERA_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "scene.h"
#include "camera_motion.h"	// �J�������[�V����

class CCameraControlState;	// ��ԕʑ���

class CCamera_Debug;	// �f�o�b�O����

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

	// �����_�̏��
	enum PosRState
	{
		POSR_STATE_NORMAL = 0,
		POSR_STATE_MAX
	};

	CCamera();
	~CCamera();

	HRESULT Init();		// ����������
	void Uninit();		// �I������
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �X�V����
	void SetCamera();	// �J�����ݒ�
	void Reset(CScene::MODE mode);	// ���Z�b�g





	
	//=============================
	// �ʒu���
	//=============================
	//--------------------------
	// �����_
	//--------------------------
	void SetPositionR(const MyLib::Vector3& pos) { m_posR = pos; }			// �����_�ݒ�
	MyLib::Vector3 GetPositionR() { return m_posR; }						// �����_�擾
	void SetPositionRDest(const MyLib::Vector3& pos) { m_posRDest = pos; }	// �����_�̖ڕW�l�ݒ�
	MyLib::Vector3 GetPositionRDest() { return m_posRDest; }				// �����_�̖ڕW�l�擾
	MyLib::Vector3 GetPositionROrigin() { return m_posROrigin; }			// ���̒����_

	//--------------------------
	// ���_
	//--------------------------
	void SetPositionV(const MyLib::Vector3& pos) { m_posV = pos; }			// ���_�ݒ�
	MyLib::Vector3 GetPositionV() { return m_posV; }						// ���_�擾
	void SetPositionVDest(const MyLib::Vector3& pos) { m_posVDest = pos; }	// ���_�̖ڕW�l�ݒ�
	MyLib::Vector3 GetPositionVDest() { return m_posVDest; }				// ���_�̖ڕW�l�擾

	//--------------------------
	// �Ǐ]�ڕW�l
	//--------------------------
	void SetTargetPosition(const MyLib::Vector3& pos) { m_posTarget = pos; }	// �Ǐ]�ڕW�̈ʒu�ݒ�
	MyLib::Vector3 GetTargetPosition() { return m_posTarget; }					// �Ǐ]�ڕW�̈ʒu�擾

	//--------------------------
	// ����
	//--------------------------
	void SetDistance(float distance) { m_fDistance = distance; }				// �����ݒ�
	float GetDistance() { return m_fDistance; }									// �����擾
	void SetDistanceDest(float distance) { m_fDestDistance = distance; }		// �����̖ڕW�l�擾
	float GetDistanceDest() { return m_fDestDistance; }							// �����̖ڕW�l�擾
	void SetOriginDistance(float distance) { m_fOriginDistance = distance; }	// ���ɂȂ�J�����̋����ݒ�
	float GetOriginDistance() { return m_fOriginDistance; }						// ���ɂȂ�J�����̋����擾

	//--------------------------
	// ����
	//--------------------------
	void SetRotation(const MyLib::Vector3& rot) { m_rot = rot; }			// �����ݒ�
	MyLib::Vector3 GetRotation() const { return m_rot; }					// �����擾
	void SetDestRotation(const MyLib::Vector3& rot) { m_rotDest = rot; }	// �ڕW�̌����ݒ�
	MyLib::Vector3 GetDestRotation() { return m_rotDest; }					// �ڕW�̌����擾
	void SetOriginRotation(const MyLib::Vector3& rot) { m_rotOrigin = rot; }	// �ڕW�̌����ݒ�
	MyLib::Vector3 GetOriginRotation() { return m_rotOrigin; }					// �ڕW�̌����擾

	//--------------------------
	// �X�N���[��
	//--------------------------
	bool IsOnScreen(const MyLib::Vector3& pos);				// �X�N���[�����̔���
	MyLib::Vector3 GetScreenPos(const MyLib::Vector3& pos);	// �X�N���[�����W�擾

	//--------------------------
	// �J�������[�V����
	//--------------------------
	CCameraMotion* GetCameraMotion() { return m_pCameraMotion; }		// �J�������[�V�����̃|�C���^�擾

	//--------------------------
	// �t���O
	//--------------------------
	bool IsFollow() { return m_bFollow; }							// �Ǐ]����擾
	void SetEnableFollow(bool bFollow) { m_bFollow = bFollow; }		// �Ǐ]�̔���ݒ�
	bool IsMotion() { return m_bMotion; }						// ���[�V����������擾
	void SetEnableMotion(bool frag) { m_bMotion = frag; }		// ���[�V����������ݒ�
	
	//--------------------------
	// ���_�}�g���b�N�X
	//--------------------------
	D3DXMATRIX GetMtxView() { return m_mtxView; }
	D3DXMATRIX* GetMtxViewPtr() { return &m_mtxView; }
	D3DXMATRIX GetMtxProjection() { return m_mtxProjection; }
	D3DXMATRIX* GetMtxProjectionPtr() { return &m_mtxProjection; }
	void SetViewPort(const MyLib::Vector3& pos, const D3DXVECTOR2& size);		// �r���[�|�[�g�̐ݒ�
	D3DVIEWPORT9 GetViewPort() { return m_viewport; };			// �r���[�|�[�g

	void SetShake(float fTime, float fLength, float fLengthY);	// �U���ݒ�	// TODO�F��Ō��؂���
	void MoveCameraStick(int nIdx = 0);			// �X�e�B�b�N����
	void WarpCamera(const MyLib::Vector3& pos);	// �J�������[�v

	// �X�e�[�g�p�^�[���ݒ�
	void SetControlState(CCameraControlState* state);	// ����̏�Ԑݒ�

private:

	// �����o�ϐ�
	void UpdateByMode();	// ���[�h�ʍX�V����
	void MoveCameraInput();	// ���͋@��̃J�����ړ�
	void ReflectCameraR();		// �����_�̔��f
	void ReflectCameraV();		// ���_���f
	void Shake();


	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// ��ԍX�V
	void UpdateSpotLightVec();	// ���C�g�̃x�N�g���X�V

	// ���Z�b�g
	void ResetGame();

	D3DXMATRIX	m_mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX	m_mtxView;			// �r���[�}�g���b�N�X
	D3DVIEWPORT9 m_viewport;		// �r���[�|�[�g
	MyLib::Vector3 m_posV;			// ���_(�J�����̈ʒu)
	MyLib::Vector3 m_posVDest;		// �ڕW�̎��_
	MyLib::Vector3 m_posR;			// �����_(�������ꏊ)
	MyLib::Vector3 m_posRDest;		// �ڕW�̒����_
	MyLib::Vector3 m_posROrigin;	// ���̒����_
	MyLib::Vector3 m_vecU;			// ������x�N�g��
	MyLib::Vector3 m_move;			// �ړ���
	MyLib::Vector3 m_rot;			// ����
	MyLib::Vector3 m_rotOrigin;		// ���̌���
	MyLib::Vector3 m_rotDest;		// �ڕW�̌���
	MyLib::Vector3 m_posTarget;		// �Ǐ]�ڕW�̈ʒu
	MyLib::Vector3 m_posTargetDest;	// �ڕW�̒Ǐ]�ڕW�̈ʒu

	float m_fRockOnDistance;
	float m_fDistance;			// ����
	float m_fDestDistance;		// �ڕW�̋���
	float m_fOriginDistance;	// ���̋���
	float m_nShakeLength;		// �h��̒���
	float m_nShakeLengthY;		// Y�̗h��̒���
	float m_fMoveShake;			// �h��̈ړ���
	float m_fMoveShakeY;		// Y�̗h��̈ړ���
	
	bool m_bFollow;	// �Ǐ]����
	bool m_bMotion;	// ���[�V����������
	
	float m_fTimerState;			// ��ԃJ�E���^�[
	float m_fTimerShake;			// �U���J�E���^�[
	STATE m_state;					// ���
	CCameraMotion* m_pCameraMotion;	// �J�������[�V�����̃|�C���^


	CCameraControlState* m_pControlState;	// ����̏�ԃ|�C���^
	CCamera_Debug* m_pDebugControll;	// �f�o�b�O����
};


//=============================
// ��ԃp�^�[��
//=============================
// ���N���X
class CCameraControlState
{
public:
	CCameraControlState(CCamera* pCamera) : m_moveRot() {}

	virtual void Controll(CCamera* pCamera);	// �ړ�����

private:

	// �����o�ϐ�
	MyLib::Vector3 m_moveRot;	// ��]�ړ���
};

// �ʏ�
class CCameraControlState_Normal : public CCameraControlState
{
public:
	CCameraControlState_Normal(CCamera* pCamera) : CCameraControlState(pCamera) {}
};

#endif