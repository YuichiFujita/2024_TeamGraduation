//=============================================================================
// 
//  �N�H�[�^�j�I���w�b�_�[ [objectQuaternion.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _OBJECTQUATERNION_H_
#define _OBJECTQUATERNION_H_	// ��d�C���N���[�h�h�~

#include "objectX.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �N�H�[�^�j�I���N���X��`
class CObjectQuaternion : public CObjectX
{
public:

	CObjectQuaternion(int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CObjectQuaternion();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
	virtual HRESULT Init(const std::string& file);
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	// ���ݒ�
	void SetValueRot(float rot)				{ m_fValueRot = rot; }		// ��]�p�ݒ�
	float GetValueRot()						{ return m_fValueRot; }		// ��]�p�擾
	void SetMoveValueRot(float rot)			{ m_fMoveValueRot = rot; }	// ��]�p�ړ��ʐݒ�
	float GetMoveValueRot()					{ return m_fMoveValueRot; }	// ��]�p�ړ��ʎ擾
	void SetMoveFactor(float factor)		{ m_fMoveFactor = factor; }	// �ړ��ʂ̕␳�W���ݒ�
	float GetMoveFactor()					{ return m_fMoveFactor; }	// �ړ��ʂ̕␳�W���擾
	void SetAxis(const MyLib::Vector3& vec) { m_vecAxis = vec; }		// ��]���ݒ�
	MyLib::Vector3 GetAxis()				{ return m_vecAxis; }		// ��]���擾

	void CalQuaternion();	// �N�H�[�^�j�I���v�Z
	void BindQuaternion(const MyLib::Vector3& vecAxis, float valueRot);	// �N�H�[�^�j�I�����f

	static CObjectQuaternion *Create();	// ����

private:

	//=============================
	// �����o�֐�
	//=============================
	void CalWorldMtx();		// ���[���h�}�g���b�N�X�̌v�Z����

	//=============================
	// �����o�ϐ�
	//=============================
	D3DXQUATERNION m_quaternion;	// �N�H�[�^�j�I��
	MyLib::Vector3 m_vecAxis;		// ��]��
	float m_fValueRot;				// ��]�p
	float m_fMoveValueRot;			// ��]�p�̈ړ���
	float m_fMoveFactor;			// �ړ��ʂ̕␳�W��
	MyLib::Matrix m_RotationMtx;	// �v�Z�p�}�g���b�N�X�錾

};



#endif