//============================================================
//
//	�Q�[�W2D�w�b�_�[ [gauge2D.h]
//	Author�FKai Takada
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _GAUGE2D_H_
#define _GAUGE2D_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object2D.h"
#include "gamemanager.h"

//************************************************************
//�@�O���錾
//************************************************************
class CString2D;

//************************************************************
//	�N���X��`
//************************************************************
// �Q�[�W2D�N���X
class CGauge2D : public CObject
{
public:
	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,	// �ʏ���
		STATE_CHANGE,	// �Q�[�W�ϓ����
		STATE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	explicit CGauge2D(const float nFrame);

	// �f�X�g���N�^
	~CGauge2D() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Kill() override;		// �폜
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V
	void Draw() override;		// �`��
	void SetEnableDisp(const bool bDisp) override;	// �`��󋵐ݒ�

	// �ÓI�����o�֐�
	static CGauge2D* Create	// ����
	( // ����
		const float fMax,				// �ő�\���l
		const float fFrame,				// �\���l�ϓ��t���[��
		const std::string& rTeamName,	// �`�[����
		const MyLib::Vector3& rPos,		// �ʒu
		const MyLib::Vector3& rOffsetFrame,		// �t���[���I�t�Z�b�g
		const MyLib::Vector3& rOffsetAssist,	// �A�V�X�g�I�t�Z�b�g
		const MyLib::Vector3& rOffsetName,		// �`�[�����I�t�Z�b�g
		const MyLib::Vector2& rSizeGauge,		// �Q�[�W�傫��
		const MyLib::Vector2& rSizeFrame,		// �t���[���傫��
		const MyLib::Vector2& rSizeAssist,		// �A�V�X�g�傫��
		const D3DXCOLOR& rColFront = MyLib::color::White(),	// �\�Q�[�W�F
		const D3DXCOLOR& rColBack = MyLib::color::Black() 	// ���Q�[�W�F
	);
	static inline void  SetBrightTime(float time)		{ m_fBrightTime = time; }		// max�̎�����J�E���^�[�ݒ�
	static inline float GetBrightTime()					{ return m_fBrightTime; }		// max�̎�����J�E���^�[�擾
	static inline void  SetBrightTimeEnd(float time)	{ m_fBrightTimeEnd = time; }	// max�̎�����I���^�C�~���O�ݒ�
	static inline float GetBrightTimeEnd()				{ return m_fBrightTimeEnd; }	// max�̎�����I���^�C�~���O�擾
	static inline void  SetAssistTime(float time)		{ m_fAssistTime = time; }		// �A�V�X�g�J�E���^�[�ݒ�
	static inline float GetAssistTime()					{ return m_fAssistTime; }		// �A�V�X�g�J�E���^�[�擾
	static inline void  SetAssistTimeEnd(float time)	{ m_fAssistTimeEnd = time; }	// �A�V�X�g�I���^�C�~���O�ݒ�
	static inline float GetAssistTimeEnd()				{ return m_fAssistTimeEnd; }	// �A�V�X�g�I���^�C�~���O�擾

	// �����o�֐�
	CObject2D* GetBg() { return m_pBg; }		// �w�i�擾
	CObject2D* GetBar() { return m_pBar; }		// �Q�[�W�擾
	CObject2D* GetFrame() { return m_pFrame; }	// �t���[���擾
	void BindTextureBG(const int nTexID);		// �w�i�e�N�X�`������
	void BindTextureBar(const int nTexID);		// �Q�[�W�e�N�X�`������
	void BindTextureFrame(const int nTexID);	// �t���[���e�N�X�`������
	void BindTextureAssist(const int nTexID);	// �A�V�X�g�e�N�X�`������
	void AddNum(const float fAdd);				// �Q�[�W���Z
	void SubNum(const float fSub);				// �Q�[�W���Z
	void SetNum(const float nNum);				// �Q�[�W�ݒ�
	void SetMaxNum(const float nMax);			// �Q�[�W�ő�l�ݒ�
	inline float GetNum() const					{ return m_fNumGauge; }		// �Q�[�W�擾
	inline float GetMaxNum() const				{ return m_fMaxNumGauge; }	// �Q�[�W�ő�l�擾
	void InitPosition();									// �ʒu������
	void InitSize();										// �傫��������
	void SetPosition(const MyLib::Vector3& rPos) override;	// �ʒu�ݒ�
	void SetTexUV(const std::vector<D3DXVECTOR2>& uv);		// �e�N�X�`�����W�̐ݒ�
	void SetAnchorType(const CObject2D::AnchorPoint& type);	// �A���J�[�|�C���g�ݒ�
	void SetSizeGaugeRadius(const float fRadius);			// ��������Q�[�W�傫���ݒ�
	void SetSizeGauge(const MyLib::Vector2& rSize);			// �Q�[�W�傫���ݒ�
	void SetSizeFrame(const MyLib::Vector2& rSize);			// �t���[���傫���ݒ�
	void SetSizeAssist(const MyLib::Vector2& rSize);		// �A�V�X�g�傫���ݒ�
	void SetOffsetFrame(const MyLib::Vector3& rOffset);		// �t���[���I�t�Z�b�g�ݒ�
	void SetOffsetAssist(const MyLib::Vector3& rOffset);	// �A�V�X�g�I�t�Z�b�g�ݒ�
	void SetOffsetTeamName(const MyLib::Vector3& rOffset);	// �`�[�����I�t�Z�b�g�ݒ�
	void SetColorFront(const D3DXCOLOR& rCol);				// �Q�[�W�F�ݒ�
	void SetColorBack(const D3DXCOLOR& rCol);				// �w�i�Q�[�W�F�ݒ�
	void SetAlignX(const EAlignX alignX);					// ���z�u�ݒ�
	void SetString(const std::string& rStr);				// ������ݒ�
	inline void SetTeam(CGameManager::ETeamSide team)		{ m_team = team; }	// �`�[���ݒ�
	inline CGameManager::ETeamSide	GetTeam() const			{ return m_team; }	// �`�[���擾

	// �f�o�b�O
	void Debug();	// �f�o�b�O

private:

	// �����o�֐�
	void BrightBar();				// �Q�[�W����
	void BrightAssist();			// �A�V�X�g����
	void SetPositionRelative();		// ���Έʒu�ݒ�

	// �ÓI�����o�ϐ�
	static float m_fBrightTime;		// max�̎�����J�E���^�[
	static float m_fBrightTimeEnd;	// max�̎�����J�E���^�[
	static float m_fAssistTime;		// �A�V�X�g�J�E���^�[
	static float m_fAssistTimeEnd;	// �A�V�X�g�J�E���^�[

	// �����o�ϐ�
	CObject2D* m_pBg;				// �w�i
	CObject2D* m_pBar;				// �Q�[�W
	CObject2D* m_pFrame;			// �t���[��
	CObject2D* m_pAssist;			// �{�^���A�V�X�g
	CString2D* m_pTeamName;			// �`�[����
	MyLib::Vector3 m_offsetFrame;	// �t���[���I�t�Z�b�g
	MyLib::Vector3 m_offsetAssist;	// �A�V�X�g�I�t�Z�b�g
	MyLib::Vector3 m_offsetName;	// �`�[�����I�t�Z�b�g
	CGameManager::ETeamSide m_team;	// �`�[��
	EState m_state;					// ���
	float m_fChange;				// �Q�[�W�ϓ���
	float m_fStateTime;				// ��ԊǗ��J�E���^�[
	float m_fCurrentNumGauge;		// ���ݕ\���l
	float m_fNumGauge;				// �ڕW�l
	float m_fMaxNumGauge;			// �\���l�̍ő�l
	const float m_fFrame;			// �\���l�̕ϓ��t���[���萔

	// �f�o�b�O�ϐ�
	float m_fSizeFrame;	// �g�傫���{��
};

#endif	// _GAUGE2D_H_
