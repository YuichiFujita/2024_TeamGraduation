//=============================================================================
// 
// �G�t�F�N�g���� [3D_effect.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _EFFECT_3D_H_
#define _EFFECT_3D_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include "objectBillboard.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �G�t�F�N�g�N���X��`
class CEffect3D : public CObjectBillboard
{
public:

	// �񋓌^��`
	typedef enum
	{
		MOVEEFFECT_ADD = 0,		// ���Z
		MOVEEFFECT_SUB,			// ���Z
		MOVEEFFECT_SUPERSUB,	// �����Z
		MOVEEFFECT_NONE,		// �ω��Ȃ�
		MOVEEFFECT_GENSUI,		// ����
		MOVEEFFECT_MAX
	}MOVEEFFECT;

	typedef enum
	{
		TYPE_NORMAL = 0,	// �ʏ�G�t�F�N�g
		TYPE_POINT,			// �_
		TYPE_SMOKE,			// ���G�t�F�N�g
		TYPE_SMOKEBLACK,	// ����
		TYPE_BLACK,			// ���G�t�F�N�g
		TYPE_JUJI,			// �\���G�t�F�N�g
		TYPE_JUJI2,			// �\���G�t�F�N�g
		TYPE_THUNDER,		// ���G�t�F�N�g
		TYPE_WATER,			// ���H�G�t�F�N�g
		TYPE_nullptr,		// nullptr�G�t�F�N�g
		TYPE_MAX
	}TYPE;

	CEffect3D(int nPriority = mylib_const::PRIORITY_ZSORT);
	~CEffect3D();

	static CEffect3D *Create();
	static CEffect3D *Create(const MyLib::Vector3& pos, const MyLib::Vector3& move, const D3DXCOLOR& col, const float fRadius, const int nLife, const int moveType, const TYPE type, const float fAddSizeValue = 0.0f);
	static void LoadTexture();	// �e�N�X�`���ǂݍ���

	//  �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init();
	HRESULT Init(const MyLib::Vector3& pos, const MyLib::Vector3& move, const D3DXCOLOR& col, const float fRadius, const int nLife, const int moveType, const TYPE type);
	void Uninit();
	void Update();
	void Draw();
	void SetVtx();

	void SetDisableAddAlpha() { m_bAddAlpha = false; }	// ���Z����������
	void SetPositionDest(MyLib::Vector3 pos);	// �ڕW�̈ʒu�ݒ�
	void SetEnableGravity() { m_bGravity = true; }	// �d�͗L��
	void SetDisableZSort() { m_bZSort = false; }	// Z�\�[�g����
	void SetGravityValue(float fValue);					// �d�͂̒l�ݒ�
	void SetMoveFactor(float factor) { m_fMoveFactor = factor; };		// �ړ��␳�W��
	void SetUp(MyLib::Vector3 setup, D3DXMATRIX *pMtxParent, CObject *pObj, int nParentIdx);	// �Z�b�g�A�b�v
	void UpdatePosition(MyLib::Vector3 rot);	// �ʒu�X�V
	void UninitParent();	// �e�̔j��
	static int GetNumAll();	// �����擾

private:

	// �����o�֐�
	void UpdateMove();	// �ړ�����
	void SubSize();
	void SuperSubSize();
	void AddSize();
	void Gensui();

	// �����o�ϐ�
	MyLib::Vector3 m_posOrigin;		// ���_
	MyLib::Vector3 m_updatePosition;	// �X�V��̈ʒu
	MyLib::Vector3 m_setupPosition;	// �Z�b�g�A�b�v�ʒu
	MyLib::Vector3 m_setupRotation;	// �Z�b�g�A�b�v�ʒu
	MyLib::Vector3 m_posDest;			// �ڕW�̈ʒu
	D3DXCOLOR m_colOrigin;		// �F
	D3DXMATRIX *m_pMtxParent;	// �e�}�g���b�N�X�̃|�C���^
	float m_fRadius;			// ���a
	float m_fMaxRadius;			// �ő唼�a
	float m_fAddSizeValue;		// �T�C�Y�ύX��
	float m_fGravity;			// �d��
	float m_fMoveFactor;		// �ړ��␳�W��
	int m_nLife;				// ����
	int m_nMaxLife;				// �ő����(�Œ�)
	int m_moveType;				// �ړ��̎��
	int m_nParentIdx;			// �e�̃C���f�b�N�X�ԍ�
	bool m_bAddAlpha;			// ���Z�����̔���
	bool m_bZSort;				// Z�\�[�g�̃t���O
	bool m_bGravity;			// �d�͂̃t���O
	bool m_bChaseDest;			// �ڕW�̈ʒu�֌������t���O
	TYPE m_nType;				// ���
	CObject *m_pParent;			// �e�̃|�C���^
	static const char *m_apTextureFile[];			// �e�N�X�`���̃t�@�C��
	static int m_nNumAll;	// ����
	static int m_nTexIdx[TYPE_MAX];				// �e�N�X�`���̃C���f�b�N�X�ԍ�
};

#endif