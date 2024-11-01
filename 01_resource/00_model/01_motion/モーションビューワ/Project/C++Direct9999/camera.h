//=============================================================================
//
// �J�������� [camera.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _CAMERA_H_		// ���̃}�N����`������Ă��Ȃ�������
#define _CAMERA_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �J�����N���X��`
class CCamera
{
public:

	CCamera();
	~CCamera();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);
	void SetRotation(const D3DXVECTOR3 rot);	// �����ݒ�
	D3DXVECTOR3 GetRotation(void) const;		// �����擾
	void SetShake(int nTime, float fLength, float fLengthY);	// �U���ݒ�
	void EnableChase(void);

private:

	// �񋓌^��`
	typedef enum
	{
		CAMERASTATE_NONE = 0,	// �ʏ�
		CAMERASTATE_SHAKE,		// �U��
	}STATE;

	void MoveCameraInput(void);
	void MoveCameraStick(void);
	void MoveCameraMouse(void);
	void MoveCameraVR(void);
	void MoveCameraV(void);
	void MoveCameraR(void);
	void MoveCameraDistance(void);
	void SetCameraV(void);
	void SetCameraR(void);
	void MoveCameraFollow(void);
	void Shake(void);
	void UpdateState(void);

	D3DXMATRIX	m_mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX	m_mtxView;			// �r���[�}�g���b�N�X
	D3DVIEWPORT9 m_viewport;		// �r���[�|�[�g
	D3DXVECTOR3 m_posV;				// ���_(�J�����̈ʒu)
	D3DXVECTOR3 m_posR;				// �����_(�������ꏊ)
	D3DXVECTOR3 m_posVDest;			// �ڕW�̎��_
	D3DXVECTOR3 m_posRDest;			// �ڕW�̒����_
	D3DXVECTOR3 m_vecU;				// ������x�N�g��
	D3DXVECTOR3 m_move;				// �ړ���
	D3DXVECTOR3 m_rot;				// ����
	D3DXVECTOR3 m_rotVDest;			// �ڕW�̎��_�̌���
	float m_fDistance;				// ����
	float m_nShakeLength;			// �h��̒���
	float m_nShakeLengthY;			// Y�̗h��̒���
	float m_fMoveShake;				// �h��̈ړ���
	float m_fMoveShakeY;			// Y�̗h��̈ړ���
	bool m_bFollow;					// �Ǐ]���邩�ǂ���
	int m_nCntState;				// ��ԃJ�E���^�[
	STATE m_state;					// ���
};

#endif