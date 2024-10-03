//=============================================================================
// 
//  �I�u�W�F�N�gX�w�b�_�[ [objectX.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _OBJECTX_H_
#define _OBJECTX_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object.h"
#include "Xload.h"
#include "listmanager.h"

// �O���錾
class CShadow;
class CCollisionLine_Box;
class CHandle_Move;

//==========================================================================
// �N���X��`
//==========================================================================
// �I�u�W�F�N�gX�N���X��`
class CObjectX : public CObject
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum STATE
	{
		STATE_NONE = 0,	// �Ȃɂ��Ȃ�
		STATE_EDIT,		// �G�f�B�b�g
		STATE_MAX
	};

	// �}�N����`
#define MAX_MAT				(512)		// �}�e���A���T�C�Y
#define MAX_TX				(96)		// �e�N�X�`���T�C�Y

	CObjectX(int nPriority = mylib_const::PRIORITY_DEFAULT,
		CObject::LAYER layer = CObject::LAYER::LAYER_DEFAULT);
	~CObjectX();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init();
	HRESULT Init(const std::string& file);
	HRESULT Init(int nIdxXFile);
	void Uninit();
	void Update();
	void Draw();
	void Draw(D3DXCOLOR col);
	void Draw(float fAlpha);

	void DrawOnly();	// �`��̂�

	void BindTexture(int *nIdx);
	void BindXData(int nIdxXFile);

	void CalWorldMtx();							// ���[���h�}�g���b�N�X�̌v�Z����
	void SetWorldMtx(const MyLib::Matrix mtx);	// �}�g���b�N�X�ݒ�
	MyLib::Matrix GetWorldMtx() const;			// �}�g���b�N�X�擾
	void SetScale(const MyLib::Vector3 scale);	// �X�P�[���ݒ�
	MyLib::Vector3 GetScale() const;			// �X�P�[���擾
	void SetColor(const D3DXCOLOR col);			// �F�ݒ�
	D3DXCOLOR GetColor() const;					// �F�擾
	void SetSize(const MyLib::Vector3 size);	// �T�C�Y�̐ݒ�
	void SetAABB(const MyLib::AABB& aabb) { m_AABB = aabb; }
	MyLib::Vector3 GetSize() const;				// �T�C�Y�̎擾
	MyLib::Vector3 GetVtxMax() const;			// ���_�̍ő�l�擾
	MyLib::Vector3 GetVtxMin() const;			// ���_�̍ŏ��l�擾
	MyLib::AABB GetAABB() const;				// AABB���擾
	int GetIdxXFile() const;					// X�t�@�C���̃C���f�b�N�X�擾
	std::string GetFileName() const { return m_fileName; }					// X�t�@�C���̃t�@�C�����擾
	bool GetUseShadow() const;					// �e���g���Ă��邩�ǂ���

	void SetState(STATE state) { m_state = state; }		// ��Ԑݒ�
	STATE GetState() { return m_state; }				// ��Ԏ擾

	float GetHeight(MyLib::Vector3 pos, bool &bLand);	// �����擾

	void Kill();
	void CreateCollisionBox();	// �����蔻��{�b�N�X����
	CCollisionLine_Box* GetCollisionLineBox() { return m_pCollisionLineBox; }	// �����蔻��{�b�N�X�擾
	static CListManager<CObjectX> GetListObj() { return m_List; }				// ���X�g�擾

	static CObjectX *Create();
	static CObjectX *Create(const std::string& file);
	static CObjectX *Create(const std::string& file, const MyLib::Vector3& pos, const MyLib::Vector3& rot = 0.0f, bool bShadow = false);
	static CObjectX* Create(
		int nIdxXFile,
		const MyLib::Vector3& pos,
		const MyLib::Vector3& rot = 0.0f,
		bool bShadow = false,
		int nPriority = mylib_const::PRIORITY_DEFAULT,
		CObject::LAYER layer = CObject::LAYER::LAYER_DEFAULT);
	CObjectX *GetObjectX();
	static int GetNumAll();

private:


	struct VertexFormat
	{
		D3DXVECTOR3 position; // ���_�̈ʒu
		D3DXVECTOR3 normal;   // ���_�̖@��
		D3DXVECTOR2 texCoord; // �e�N�X�`�����W
	};

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CObjectX::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];	// ��Ԋ֐����X�g

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԋ֐�
	void StateNone();	// �Ȃɂ��Ȃ�
	void StateEdit();	// �G�f�B�b�g

	// ���̑�
	void SetCollisionBoxData();	// �����蔻��{�b�N�X�f�[�^�ݒ�

	//=============================
	// �����o�ϐ�
	//=============================
	MyLib::Matrix m_mtxWorld;	// ���[���h�}�g���b�N�X
	MyLib::Vector3 m_scale;		// �X�P�[��
	D3DXCOLOR m_col;			// �F
	MyLib::Vector3 m_fSize;		// �T�C�Y
	MyLib::AABB m_AABB;			// AABB���
	MyLib::AABB m_OriginAABB;	// ����AABB���
	STATE m_state;				// ���
	bool m_bShadow;				// �e���g�����ǂ���
	int m_nIdxTexure;			// �e�N�X�`���̃C���f�b�N�X�ԍ�
	int m_nIdxXFile;			// X�t�@�C���̃C���f�b�N�X�ԍ�
	std::string m_fileName;
	static int m_nNumAll;		// ����
	CShadow *m_pShadow;			// �e�̏��
	CCollisionLine_Box* m_pCollisionLineBox;	// �����蔻��{�b�N�X
	static CListManager<CObjectX> m_List;		// ���X�g

	LPD3DXMESH m_pMesh;			// ���b�V��(���_���)�ւ̃|�C���^

};



#endif