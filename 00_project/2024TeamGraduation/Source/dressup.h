//=============================================================================
//
// �����ւ����� [dressup.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _DRESSUP_H_		// ���̃}�N����`������Ă��Ȃ�������
#define _DRESSUP_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

//==========================================================================
// �O���錾
//==========================================================================
class CObjectChara;

//==========================================================================
// �N���X��`
//==========================================================================
// �����ւ��N���X
class CDressup
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum EType
	{
		TYPE_HAIR = 0,	// ��
		TYPE_ACCESSORY,	// �A�N�Z�T���[
		TYPE_FACE,		// ��
		TYPE_UNIFORM,	// ���j�t�H�[��
		TYPE_MAX
	};

	CDressup();
	~CDressup();

	//=============================
	// �����o�֐�
	//=============================
	virtual HRESULT Init();		// ������
	virtual void Uninit();		// �I��
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �X�V

	//--------------------------
	// �L�����N�^�[
	//--------------------------
	void BindObjectCharacter(CObjectChara* pObjChara) { m_pObjChara = pObjChara; }	// �L�����N�^�[�̃|�C���^

	//--------------------------
	// ���̑�
	//--------------------------
	void SetControllIdx(int idx)	{ m_nControllIdx = idx; }	// ���삷��C���f�b�N�X�ݒ�
	void SetNowIdx(int idx)			{ m_nNowIdx = idx; }		// ���݂̃C���f�b�N�X�ݒ�
	int GetNowIdx()					{ return m_nNowIdx; }		// ���݂̃C���f�b�N�X�擾
	virtual void ReRegist(int idx);								// �Ċ��蓖��
	virtual void ReRegist() = 0;								// �Ċ��蓖��
	virtual void RandSet();										// �����_���ݒ�
	virtual void Debug();

	// �ÓI�֐�
	static CDressup* Create(EType type, CObjectChara* pObjChara, int nSwitchType);	// ��������

protected:

	void LoadAllModel(const std::wstring& folder);		// �S�ǂݍ���
	void LoadAllTexture(const std::wstring& folder);	// �S�ǂݍ���

	//=============================
	// �����o�ϐ�
	//=============================
	int m_nSwitchIdx;			// �؂�ւ���C���f�b�N�X
	int m_nNowIdx;				// ���݂̃C���f�b�N�X
	int m_nControllIdx;			// ���삷��C���f�b�N�X	// TODO�F�����̓v���C���[��MyIdx���Q�Ƃ��悤
	CObjectChara* m_pObjChara;	// �L�����N�^�[�̃|�C���^
	std::vector<std::string> m_vecModelName;	// ���f����

};

#endif