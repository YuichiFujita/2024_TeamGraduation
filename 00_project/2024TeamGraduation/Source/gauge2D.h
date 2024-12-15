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

	// �ÓI�����o�֐�
	static CGauge2D* Create	// ����
	( // ����
		const float fMax,				// �ő�\���l
		const float fFrame,				// �\���l�ϓ��t���[��
		const MyLib::Vector3& rPos,		// �ʒu
		const MyLib::Vector2& rSizeGauge = VEC3_ONE,		// �Q�[�W�傫��
		const D3DXCOLOR& rColFront = MyLib::color::White(),	// �\�Q�[�W�F
		const D3DXCOLOR& rColBack = MyLib::color::Black(),	// ���Q�[�W�F
		const bool bDrawFrame = false						// �g�`���
	);

	// �����o�֐�
	CObject2D* GetBg() { return m_pBg; }				// �w�i�擾
	CObject2D* GetBar() { return m_pBar; }				// �Q�[�W�擾
	CObject2D* GetFrame() { return m_pFrame; }			// �t���[���擾

	void BindTexture();									// �e�N�X�`������
	void AddNum(const float fAdd);						// �Q�[�W���Z
	void SubNum(const float fSub);						// �Q�[�W���Z
	void SetNum(const float nNum);						// �Q�[�W�ݒ�
	void SetMaxNum(const float nMax);					// �Q�[�W�ő�l�ݒ�
	inline float GetNum() const						{ return m_fNumGauge; }		// �Q�[�W�擾
	inline float GetMaxNum() const					{ return m_fMaxNumGauge; }	// �Q�[�W�ő�l�擾
	inline bool IsEnableDrawFrame() const			{ return m_bDrawFrame; }	// �g�\���󋵎擾

	void SetPosition(const MyLib::Vector3& rPos) override;	// �ʒu�ݒ�
	void SetSize(const MyLib::Vector2& rSize);				// �S�Ă̑傫���ݒ�
	void SetTexUV(const std::vector<D3DXVECTOR2>& uv);		// �e�N�X�`�����W�̐ݒ�

	void SetAnchorType(const CObject2D::AnchorPoint& type);	// �A���J�[�|�C���g�ݒ�
	void SetSizeGaugeRadius(const float fRadius);			// ��������Q�[�W�傫���ݒ�
	void SetSizeGauge(const MyLib::Vector2& rSize);			// �Q�[�W�傫���ݒ�
	void SetSizeFrame(const MyLib::Vector2& rSize);			// �g�傫���ݒ�
	void SetSizeBg(const MyLib::Vector2& rSize);			// �w�i�傫���ݒ�
	void SetColorFront(const D3DXCOLOR& rCol);				// �Q�[�W�F�ݒ�
	void SetColorBack(const D3DXCOLOR& rCol);				// �w�i�Q�[�W�F�ݒ�
	void SetEnableDrawFrame(const bool bDraw);				// �g�\���󋵐ݒ�
	
	void SetTeam(CGameManager::ETeamSide team) { m_team = team; }				// �`�[���ݒ�
	CGameManager::ETeamSide	GetTeam() { return m_team; }						// �`�[���擾
	
	void Debug();	// �f�o�b�O

	// �ÓI�֐�
	static void		SetBrightTime(float time) { m_fBrightTime = time; }			// max�̎�����J�E���^�[�ݒ�
	static float	GetBrightTime() { return m_fBrightTime; }					// max�̎�����J�E���^�[�擾
	static void		SetBrightTimeEnd(float time) { m_fBrightTimeEnd = time; }	// max�̎�����I���^�C�~���O�ݒ�
	static float	GetBrightTimeEnd() { return m_fBrightTimeEnd; }				// max�̎�����I���^�C�~���O�擾

private:

	// �����o�֐�
	void InitSize();					// �����T�C�Y�ݒ�
	void InitPosition();				// �����ʒu�ݒ�
	void BrightBar();					// �Q�[�W����

	// �����o�ϐ�
	CObject2D* m_pBg;					// �w�i
	CObject2D* m_pBar;					// �Q�[�W
	CObject2D* m_pFrame;				// �t���[��
	CObject2D* m_pAssist;				// �{�^���A�V�X�g

	CGameManager::ETeamSide	m_team;		// �`�[��
	EState	m_state;					// ���
	bool	m_bDrawFrame;				// �g�\����
	float	m_fChange;					// �Q�[�W�ϓ���
	float	m_fStateTime;				// ��ԊǗ��J�E���^�[
	float	m_fCurrentNumGauge;			// ���ݕ\���l
	float	m_fNumGauge;				// �ڕW�l
	float	m_fMaxNumGauge;				// �\���l�̍ő�l
	const float m_fFrame;				// �\���l�̕ϓ��t���[���萔
	static float	m_fBrightTime;		// max�̎�����J�E���^�[
	static float	m_fBrightTimeEnd;	// max�̎�����J�E���^�[

	// �f�o�b�O
	float	m_fSizeFrame;	// �g�傫���{��
};

#endif	// _GAUGE2D_H_
