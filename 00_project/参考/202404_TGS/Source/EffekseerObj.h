//=============================================================================
// 
// �G�t�F�N�V�A�I�u�W�F�N�g���� [EffekseerObj.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _EFFEKSEEROBJ_H_
#define _EFFEKSEEROBJ_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "MyEffekseer.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �G�t�F�N�V�A�I�u�W�F�N�g
class CEffekseerObj
{
public:

	CEffekseerObj();
	~CEffekseerObj();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	void Stop();	// ��~
	void DeleteLater(float time);	// ��ŏ���
	Effekseer::Handle GetHandle() { return m_EffekseerInfo.handle; }	// �n���h���擾

	void SetTrigger(int trigger);					// �g���K�[���M 
	void SetPosition(const MyLib::Vector3& pos);	// �ʒu�ݒ�
	void SetRotation(const MyLib::Vector3& rot);	// �����ݒ�
	void SetMatrix(const MyLib::Matrix& mtx);		// �}�g���b�N�X�ݒ�
	Effekseer::Matrix43 GetMatrix();				// �}�g���b�N�X�擾
	void SetScale(const float scale);				// �X�P�[���ݒ�

	/**
	@brief	�G�t�F�N�g����
	@details �p�X�̊g���q��[.efkefc]�ł�
	@param	label		[in]	�G�t�F�N�g���x��
	@param	pos			[in]	�ʒu
	@param	rot			[in]	����
	@param	move		[in]	�ړ���
	@param	scale		[in]	�g�嗦
	@param	bAutoDeath	[in]	�����폜
	@return	CEffekseerObj �|�C���^
	*/
	static CEffekseerObj* Create(const CMyEffekseer::EFKLABEL& label, const MyLib::Vector3& pos, const MyLib::Vector3& rot, const MyLib::Vector3 move, float scale, bool bAutoDeath = true);
	static CListManager<CEffekseerObj> GetListObj() { return m_List; }	// ���X�g�擾

private:

	CMyEffekseer::EFKLABEL m_Label;	// �G�t�F�N�g�̃��x��
	CMyEffekseer::MyEffekseerInfo m_EffekseerInfo;	// �G�t�F�N�g���
	bool m_bDeath;			// ���S�t���O
	bool m_bDeleteLater;	// ��Ŏ��S�t���O
	float m_fDeleteTimer;	// ���S�^�C�}�[
	static CListManager<CEffekseerObj> m_List;	// ���X�g
};

#endif