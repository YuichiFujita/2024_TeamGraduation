//=============================================================================
// 
//  �I�u�W�F�N�gX�w�b�_�[ [objectX.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _OBJECTX_H_
#define _OBJECTX_H_	// ��d�C���N���[�h�h�~

#include "object.h"
#include "Xload.h"
#include "listmanager.h"

// �O���錾
class CShadow;
class CCollisionLine_Box;

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

	CObjectX(int nPriority = mylib_const::PRIORITY_DEFAULT,	CObject::LAYER layer = CObject::LAYER::LAYER_DEFAULT);
	~CObjectX();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init() override;
	HRESULT Init(const std::string& file);
	HRESULT Init(int nIdxXFile);
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;
	void Draw(const D3DXCOLOR& col);
	void Draw(float fAlpha);

	//--------------------------
	// ����
	//--------------------------
	void Kill();		// �폜
	void DrawOnly();	// �`��̂�

	//--------------------------
	// �v�f
	//--------------------------
	void BindTexture(int *nIdx);								// �e�N�X�`�����蓖��
	void BindXData(int nIdxXFile) { m_nIdxXFile = nIdxXFile; }	// X�t�@�C���f�[�^���蓖��
	int GetIdxXFile() const { return m_nIdxXFile; }				// X�t�@�C���̃C���f�b�N�X�擾
	std::string GetFileName() const { return m_fileName; }		// X�t�@�C���̃t�@�C�����擾

	//--------------------------
	// �p�����[�^�[
	//--------------------------
	void SetWorldMtx(const MyLib::Matrix& mtx) { m_mtxWorld = mtx; }	// �}�g���b�N�X�ݒ�
	MyLib::Matrix GetWorldMtx() const { return m_mtxWorld; }			// �}�g���b�N�X�擾
	void SetScale(const MyLib::Vector3& scale) { m_scale = scale; }		// �X�P�[���ݒ�
	MyLib::Vector3 GetScale() const { return m_scale; }					// �X�P�[���擾
	void SetColor(const D3DXCOLOR& col) { m_col = col; }				// �F�ݒ�
	D3DXCOLOR GetColor() const { return m_col; }						// �F�擾
	void SetAABB(const MyLib::AABB& aabb) { m_AABB = aabb; }			// AABB�ݒ�
	MyLib::AABB GetAABB() const { return m_AABB; }						// AABB���擾
	MyLib::Vector3 GetVtxMax() const { return m_AABB.vtxMax; }			// ���_�̍ő�l�擾
	MyLib::Vector3 GetVtxMin() const { return m_AABB.vtxMin; }			// ���_�̍ŏ��l�擾
	
	//--------------------------
	// ���
	//--------------------------
	void SetState(STATE state) { m_state = state; }		// ��Ԑݒ�
	STATE GetState() { return m_state; }				// ��Ԏ擾

	//--------------------------
	// �����蔻��{�b�N�X
	//--------------------------
	void CreateCollisionBox();	// �����蔻��{�b�N�X����
	CCollisionLine_Box* GetCollisionLineBox() { return m_pCollisionLineBox; }	// �����蔻��{�b�N�X�擾
	static CListManager<CObjectX> GetListObj() { return m_List; }				// ���X�g�擾

	//--------------------------
	// ����
	//--------------------------
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

	//--------------------------
	// ���̑�
	//--------------------------
	float GetHeight(const MyLib::Vector3& pos, bool& bLand);	// �����擾
	bool IsUseShadow() const { return m_bShadow; }				// �e���g���Ă��邩�ǂ���
	CObjectX* GetObjectX() { return this; }						// �I�u�W�F�N�gX�̗v�f�擾

protected:
	// ���z�֐�
	virtual void CalWorldMtx();	// ���[���h�}�g���b�N�X�̌v�Z����

private:

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
	int m_nIdxTexure;			// �e�N�X�`���̃C���f�b�N�X�ԍ�
	int m_nIdxXFile;			// X�t�@�C���̃C���f�b�N�X�ԍ�
	std::string m_fileName;		// �t�@�C����
	CShadow *m_pShadow;			// �e�̏��
	bool m_bShadow;				// �e���g�����ǂ���
	LPD3DXMESH m_pMesh;			// ���b�V��(���_���)�ւ̃|�C���^
	CCollisionLine_Box* m_pCollisionLineBox;	// �����蔻��{�b�N�X
	static CListManager<CObjectX> m_List;		// ���X�g


};



#endif