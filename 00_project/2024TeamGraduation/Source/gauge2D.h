//============================================================
//
//	�Q�[�W2D�w�b�_�[ [gauge2D.h]
//	Author�F���c�E��
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
#include "object.h"

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

	// �|���S����
	enum EPolygon
	{
		POLYGON_BACK = 0,	// �w�i
		POLYGON_FRONT,		// �Q�[�W
		POLYGON_FRAME,		// �g
		POLYGON_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	explicit CGauge2D(const int nFrame);

	// �f�X�g���N�^
	~CGauge2D() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Kill() override;		// �폜
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V
	void Draw() override;		// �`��
	void SetPosition(const MyLib::Vector3& rPos) override;	// �ʒu�ݒ�
	inline MyLib::Vector3 GetPosition() const override { return m_pos; }	// �ʒu�擾

	// �ÓI�����o�֐�
	static CGauge2D* Create	// ����
	( // ����
		const int nMax,				// �ő�\���l
		const int nFrame,			// �\���l�ϓ��t���[��
		const MyLib::Vector3& rPos,	// �ʒu
		const MyLib::Vector3& rSizeGauge = VEC3_ONE,		// �Q�[�W�傫��
		const D3DXCOLOR& rColFront = MyLib::color::White(),	// �\�Q�[�W�F
		const D3DXCOLOR& rColBack = MyLib::color::Black(),	// ���Q�[�W�F
		const bool bDrawFrame = false,						// �g�`���
		const char* pPathTexture = nullptr,					// �t���[���e�N�X�`���p�X
		const MyLib::Vector3& rSizeFrame = VEC3_ONE,		// �g�傫��
		const MyLib::Vector3& rOffsetFrame = VEC3_ONE		// �g�I�t�Z�b�g
	);

	// �����o�֐�
	void BindTexture(const int nPolygonIdx, const int nTextureIdx);		// �e�N�X�`������ (�C���f�b�N�X)
	void BindTexture(const int nPolygonIdx, const char* pTexturePath);	// �e�N�X�`������ (�p�X)
	int GetTextureIndex(const int nPolygonIdx) const;	// �e�N�X�`���C���f�b�N�X�擾
	void AddNum(const int nAdd);						// �Q�[�W���Z
	void SetNum(const int nNum);						// �Q�[�W�ݒ�
	void SetMaxNum(const int nMax);						// �Q�[�W�ő�l�ݒ�
	void SetOffsetFrame(const MyLib::Vector3& rOffset);	// �g�I�t�Z�b�g�ݒ�
	void SetSizeGauge(const MyLib::Vector3& rSize);		// �Q�[�W�傫���ݒ�
	void SetSizeFrame(const MyLib::Vector3& rSize);		// �w�i�傫���ݒ�
	void SetAlphaFront(const float fAlpha);				// �\�Q�[�W�����x�擾
	void SetColorFront(const D3DXCOLOR& rCol);			// �\�Q�[�W�F�ݒ�
	void SetAlphaBack(const float fAlpha);				// ���Q�[�W�����x�擾
	void SetColorBack(const D3DXCOLOR& rCol);			// ���Q�[�W�F�ݒ�
	void SetEnableDrawFrame(const bool bDraw);			// �g�\���󋵐ݒ�
	inline int GetNum() const						{ return m_nNumGauge; }		// �Q�[�W�擾
	inline int GetMaxNum() const					{ return m_nMaxNumGauge; }	// �Q�[�W�ő�l�擾
	inline MyLib::Vector3 GetOffsetFrame() const	{ return m_offsetFrame; }	// �g�I�t�Z�b�g�擾
	inline MyLib::Vector3 GetSizeGauge() const		{ return m_sizeGauge; }		// �Q�[�W�傫���擾
	inline MyLib::Vector3 GetSizeFrame() const		{ return m_sizeFrame; }		// �w�i�傫���擾
	inline float GetAlphaFront() const				{ return m_colFront.a; }	// �\�Q�[�W�����x�擾
	inline D3DXCOLOR GetColorFront() const			{ return m_colFront; }		// �\�Q�[�W�F�擾
	inline float GetAlphaBack() const				{ return m_colBack.a; }		// ���Q�[�W�����x�擾
	inline D3DXCOLOR GetColorBack() const			{ return m_colBack; }		// ���Q�[�W�F�擾
	inline bool IsEnableDrawFrame() const			{ return m_bDrawFrame; }	// �g�\���󋵎擾

private:
	// �����o�֐�
	void SetVtx();	// ���_���̐ݒ�

	// �����o�ϐ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	MyLib::Vector3 m_pos;				// �ʒu
	MyLib::Vector3 m_offsetFrame;		// �g�I�t�Z�b�g
	MyLib::Vector3 m_sizeGauge;			// �Q�[�W�傫��
	MyLib::Vector3 m_sizeFrame;			// �g�傫��
	D3DXCOLOR m_colFront;				// �\�Q�[�W�F
	D3DXCOLOR m_colBack;				// ���Q�[�W�F
	EState	m_state;					// ���
	bool	m_bDrawFrame;				// �g�\����
	float	m_fChange;					// �Q�[�W�ϓ���
	float	m_fCurrentNumGauge;			// ���ݕ\���l
	float	m_fAddRight;				// �������Z��
	int		m_nCounterState;			// ��ԊǗ��J�E���^�[
	int		m_nNumGauge;				// �\���l
	int		m_nMaxNumGauge;				// �\���l�̍ő�l
	int		m_aTextureIdx[POLYGON_MAX];	// �e�N�X�`���C���f�b�N�X
	const int m_nFrame;					// �\���l�̕ϓ��t���[���萔
};

#endif	// _GAUGE2D_H_
