//=============================================================================
//
// ���e�l�}�l�[�W������ [charmValueManager.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _CHARMVALUEMANAGER_H_		// ���̃}�N����`������Ă��Ȃ�������
#define _CHARMVALUEMANAGER_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

//==========================================================================
// �O���錾
//==========================================================================

//==========================================================================
// �N���X��`
//==========================================================================
// ���e�}�l�[�W���N���X
class CCharmValueManager
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum ETypeAdd
	{
		ADD_HIT = 0,		// �q�b�g
		ADD_CATCH,			// �L���b�`
		ADD_JUSTCATCH,		// �W���X�g�L���b�`
		ADD_COVERCATCH,		// �J�o�[�L���b�`
		ADD_THROW_JUST,		// �W���X�g�L���b�`����̓���
		ADD_THROW_DROP,		// �h���b�v�L���b�`����̓���
		ADD_DODGE,			// ���
		ADD_SPECIAL,		// �X�y�V����
		ADD_CATCH_SPECIAL,	// �X�y�V�������L���b�`
		ADD_REVIVE,			// �h��
		ADD_MAX
	};

	//=============================
	// �񋓌^��`
	//=============================
	enum ETypeSub
	{
		SUB_INVADE_RETURN_HIT = 0,		// ���C���z���Ė߂��Ă�Œ��ɂ�����
		SUB_EDGE_ESCAPE,				// �[�ɓ����܂���
		SUB_INVADE_RUN,					// �����Ă����ă��C���z��(�{�[������)
		SUB_LONG_HOLD,					// �����ƃ{�[�������ē����Ȃ�
		SUB_SPECIAL_CATCH,				// �X�y�V�����L���b�`���ꂽ
		SUB_MAX
	};

	CCharmValueManager();
	~CCharmValueManager();

	//=============================
	// �����o�֐�
	//=============================
	virtual HRESULT Init();		// ������
	virtual void Uninit();		// �I��

	//--------------------------
	// ���̑�
	//--------------------------
	virtual float GetAddValue(ETypeAdd type);	// ���Z�ʎ擾
	virtual float GetSubValue(ETypeSub type);	// ���Z�ʎ擾
	void Debug();								// �f�o�b�O

	// �ÓI�֐�
	static CCharmValueManager* Create();	// ��������
	static CCharmValueManager* GetInstance() { return m_pThisPtr; }	// �C���X�^���X�擾

private:


	//=============================
	// �����o�ϐ�
	//=============================
	float m_fAddValue[ETypeAdd::ADD_MAX];	// ���Z��
	float m_fSubValue[ETypeSub::SUB_MAX];	// ���Z��
	static CCharmValueManager* m_pThisPtr;	// ���g�̃|�C���^
};

#endif