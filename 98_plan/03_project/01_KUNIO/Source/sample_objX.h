//=============================================================================
// 
//  �T���v��_�I�u�W�F�N�gX�w�b�_�[ [sample_objX.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _SAMPLE_OBJX_H_
#define _SAMPLE_OBJX_H_		// ��d�C���N���[�h�h�~

#include "objectX.h"
#include "listmanager.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �T���v��_�I�u�W�F�N�gX�N���X
class CSample_ObjX : public CObjectX
{
public:
	
	//=============================
	// �񋓌^��`
	//=============================
	enum Sample
	{
		SAMPLE_WAO = 0,
		SAMPLE_WHO,
		SAMPLE_MAX
	};

	CSample_ObjX(int nPriority = 6);
	~CSample_ObjX();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	//=============================
	// �����o�֐�
	//=============================
	void Kill();		// �폜

	//=============================
	// �ÓI�֐�
	//=============================
	/**
	@brief		��������
	@details	�K�v������Έ����ǉ�
	*/
	static CSample_ObjX *Create();
	static CListManager<CSample_ObjX> GetListObj() { return m_List; }	// ���X�g�擾

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CSample_ObjX::*SAMPLE_FUNC)();
	static SAMPLE_FUNC m_SampleFuncList[];	// �֐��̃��X�g

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԋ֐�
	void SampleWho();	// �t�[
	void SampleWao();	// ���I

	//=============================
	// �����o�ϐ�
	//=============================
	float m_fStateTime;				// ��ԃJ�E���^�[
	Sample m_state;					// ���
	static CListManager<CSample_ObjX> m_List;	// ���X�g

};


#endif