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
private:

	// �񋓌^��`
	enum STATE
	{
		CAMERASTATE_NONE = 0,	// �ʏ�
		CAMERASTATE_SHAKE,		// �U��
		CAMERASTATE_FOLLOW,		// �Ǐ]
		CAMERASTATE_MAX
	};

public:

	// �����_�̏��
	enum PosRState
	{
		POSR_STATE_NORMAL = 0,
		POSR_STATE_MAX
	};

	CCamera();	// �R���X�g���N�^
	~CCamera();	// �f�X�g���N�^

	HRESULT Init();		// ������
	void Uninit();		// �I��
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �X�V
	void SetCamera();	// �J�����ݒ�
	void Reset();		// �J�������Z�b�g
	void WarpCamera(const MyLib::Vector3& pos);	// �J�������[�v�ݒ�

	//--------------------------
	// �X�N���[��
	//--------------------------
	bool IsOnScreen(const MyLib::Vector3& pos);				// �X�N���[��������
	MyLib::Vector3 GetScreenPos(const MyLib::Vector3& pos);	// �X�N���[�����W�擾

	//--------------------------
	// �����_
	//--------------------------
	inline void SetPositionR(const MyLib::Vector3& pos)		{ m_posR = pos; }			// �����_�̐ݒ�
	inline MyLib::Vector3 GetPositionR()					{ return m_posR; }			// �����_�̎擾
	inline void SetPositionRDest(const MyLib::Vector3& pos)	{ m_posRDest = pos; }		// �ڕW�����_�̐ݒ�
	inline MyLib::Vector3 GetPositionRDest()				{ return m_posRDest; }		// �ڕW�����_�̎擾
	inline MyLib::Vector3 GetPositionROrigin()				{ return m_posROrigin; }	// ���_�����_�̎擾

	//--------------------------
	// ���_
	//--------------------------
	inline void SetPositionV(const MyLib::Vector3& pos)		{ m_posV = pos; }		// ���_�̐ݒ�
	inline MyLib::Vector3 GetPositionV()					{ return m_posV; }		// ���_�̎擾
	inline void SetPositionVDest(const MyLib::Vector3& pos)	{ m_posVDest = pos; }	// �ڕW���_�̐ݒ�
	inline MyLib::Vector3 GetPositionVDest()				{ return m_posVDest; }	// �ڕW���_�̎擾

	//--------------------------
	// �Ǐ]�ڕW�l
	//--------------------------
	inline void SetTargetPosition(const MyLib::Vector3& pos)		{ m_posTarget = pos; }		// �Ǐ]�ʒu�̐ݒ�
	inline MyLib::Vector3 GetTargetPosition()						{ return m_posTarget; }		// �Ǐ]�ʒu�̎擾
	inline void SetTargetPositionDest(const MyLib::Vector3& pos)	{ m_posTargetDest = pos; }	// �Ǐ]�ڕW�ʒu�̐ݒ�
	inline MyLib::Vector3 GetTargetPositionDest()					{ return m_posTargetDest; }	// �Ǐ]�ڕW�ʒu�̎擾

	//--------------------------
	// ����
	//--------------------------
	inline void SetDistance(float distance)			{ m_fDistance = distance; }			// �����̐ݒ�
	inline float GetDistance()						{ return m_fDistance; }				// �����̎擾
	inline void SetDistanceDest(float distance)		{ m_fDestDistance = distance; }		// �ڕW�����̎擾
	inline float GetDistanceDest()					{ return m_fDestDistance; }			// �ڕW�����̎擾
	inline void SetDistanceOrigin(float distance)	{ m_fOriginDistance = distance; }	// ���_�����̐ݒ�
	inline float GetDistanceOrigin()				{ return m_fOriginDistance; }		// ���_�����̎擾

	//--------------------------
	// ����
	//--------------------------
	inline void SetRotation(const MyLib::Vector3& rot)			{ m_rot = rot; }		// �����̐ݒ�
	inline MyLib::Vector3 GetRotation() const					{ return m_rot; }		// �����̎擾
	inline void SetDestRotation(const MyLib::Vector3& rot)		{ m_rotDest = rot; }	// �ڕW�����̐ݒ�
	inline MyLib::Vector3 GetDestRotation()						{ return m_rotDest; }	// �ڕW�����̎擾
	inline void SetOriginRotation(const MyLib::Vector3& rot)	{ m_rotOrigin = rot; }	// ���_�����̐ݒ�
	inline MyLib::Vector3 GetOriginRotation()					{ return m_rotOrigin; }	// ���_�����̎擾

	//--------------------------
	// �J�������[�V����
	//--------------------------
	inline CCameraMotion* GetCameraMotion() { return m_pCameraMotion; }	// �J�������[�V�����|�C���^�擾

	//--------------------------
	// �t���O
	//--------------------------
	inline void SetEnableLight(bool bLight)		{ m_pLight->SetEnableLight(bLight); }	// ���C�g�t���O�̐ݒ�
	inline void SetEnableFollow(bool bFollow)	{ m_bFollow = bFollow; }	// �Ǐ]����ݒ�
	inline bool IsFollow()						{ return m_bFollow; }		// �Ǐ]����擾
	inline void SetEnableMotion(bool frag)		{ m_bMotion = frag; }		// ���[�V����������ݒ�
	inline bool IsMotion()						{ return m_bMotion; }		// ���[�V����������擾
	
	//--------------------------
	// ���_�}�g���b�N�X
	//--------------------------
	void SetViewPort(const MyLib::Vector3& pos, const D3DXVECTOR2& size);		// �r���[�|�[�g�ݒ�
	inline D3DVIEWPORT9 GetViewPort()			{ return m_viewport; };			// �r���[�|�[�g�擾
	inline D3DXMATRIX GetMtxView()				{ return m_mtxView; }			// �r���[�}�g���b�N�X�擾
	inline D3DXMATRIX* GetMtxViewPtr()			{ return &m_mtxView; }			// �r���[�}�g���b�N�X�|�C���^�擾
	inline D3DXMATRIX GetMtxProjection()		{ return m_mtxProjection; }		// �v���W�F�N�V�����}�g���b�N�X�擾
	inline D3DXMATRIX* GetMtxProjectionPtr()	{ return &m_mtxProjection; }	// �v���W�F�N�V�����}�g���b�N�X�|�C���^�擾

private:

	// �����o�֐�
	void ReflectCameraR();		// �����_�̔��f
	void ReflectCameraV();		// ���_�̔��f
	void UpdateSpotLightVec();	// ���C�g�̃x�N�g���X�V
	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ��ԍX�V

	// �����o�ϐ�
	D3DXMATRIX m_mtxProjection;		// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX m_mtxView;			// �r���[�}�g���b�N�X
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

	float m_fDistance;			// ����
	float m_fDestDistance;		// �ڕW�̋���
	float m_fOriginDistance;	// ���̋���
	float m_nShakeLength;		// �h��̒���
	float m_nShakeLengthY;		// Y�̗h��̒���
	float m_fMoveShake;			// �h��̈ړ���
	float m_fMoveShakeY;		// Y�̗h��̈ړ���
	
	bool m_bFollow;	// �Ǐ]����
	bool m_bMotion;	// ���[�V����������
	
	float m_fTimerState;	// ��ԃJ�E���^�[
	STATE m_state;			// ���
	CLightDir* m_pLight;	// �f�B���N�V���i�����C�g
	CCameraMotion* m_pCameraMotion;		// �J�������[�V�����̃|�C���^
	CCamera_Debug* m_pDebugControll;	// �f�o�b�O����
};

#endif