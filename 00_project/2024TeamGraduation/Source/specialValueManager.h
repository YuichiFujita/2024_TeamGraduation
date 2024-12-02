//=============================================================================
//
// �X�y�V�����l�}�l�[�W������ [specialValueManager.h]
// Author: Kai Takada
//
//=============================================================================
#ifndef _SPECIALVALUEMANAGER_H_		// ���̃}�N����`������Ă��Ȃ�������
#define _SPECIALVALUEMANAGER_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

//==========================================================================
// �O���錾
//==========================================================================

//==========================================================================
// �N���X��`
//==========================================================================
// ���e�}�l�[�W���N���X
class CSpecialValueManager
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	// �l������
	enum ETypeAdd
	{
		ADD_ALLWAYS = 0,	// ���
		ADD_HIT ,			// �q�b�g
		ADD_CATCH,			// �L���b�`
		ADD_JUSTCATCH,		// �W���X�g�L���b�`
		ADD_COVERCATCH,		// �J�o�[�L���b�`
		ADD_DODGE,			// ���
		ADD_CATCH_SPECIAL,	// �X�y�V�������L���b�`����
		ADD_MAX
	};

	CSpecialValueManager();
	~CSpecialValueManager();

	//=============================
	// �����o�֐�
	//=============================
	virtual HRESULT Init();		// ������
	virtual void Uninit();		// �I��

	//--------------------------
	// ���̑�
	//--------------------------
	virtual float GetAddValue(ETypeAdd type);	// ���Z�ʎ擾

	// �ÓI�֐�
	static CSpecialValueManager* Create();	// ��������
	static CSpecialValueManager* GetInstance() { return m_pThisPtr; }	// �C���X�^���X�擾

private:


	//=============================
	// �����o�ϐ�
	//=============================
	float m_fAddValue[ETypeAdd::ADD_MAX];	// ���Z��
	static CSpecialValueManager* m_pThisPtr;	// ���g�̃|�C���^
};

#endif