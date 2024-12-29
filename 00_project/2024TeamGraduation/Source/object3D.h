//=============================================================================
// 
//  �I�u�W�F�N�g3D�w�b�_�[ [object3D.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_	// ��d�C���N���[�h�h�~

#include "object.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �I�u�W�F�N�g3D�N���X��`
class CObject3D : public CObject
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum EAnchorPoint
	{
		LEFT = 0,		// ��
		TOP_LEFT,		// ����
		TOP_CENTER,		// ��
		TOP_RIGHT,		// �E��
		RIGHT,			// �E
		UNDER_RIGHT,	// �E��
		UNDER_CENTER,	// ��
		UNDER_LEFT,		// ����
		CENTER,			// ����
		MAX
	};

	CObject3D(int nPriority, const LAYER layer = LAYER::LAYER_DEFAULT);
	virtual ~CObject3D();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	virtual void Kill() override;	// ���I�폜����

	virtual void SetVtx();
	virtual void BindTexture(int nIdx);

	void SetWorldMtx(const MyLib::Matrix& mtx) { m_mtxWorld = mtx; }		// �}�g���b�N�X�ݒ�
	MyLib::Matrix GetWorldMtx() const { return m_mtxWorld; }				// �}�g���b�N�X�擾
	void SetColor(const D3DXCOLOR& col) { m_col = col; }				// �F�ݒ�
	D3DXCOLOR GetColor() const { return m_col; }						// �F�擾
	void SetColorOrigin(const D3DXCOLOR& col) { m_colOrigin = col; }				// �F�ݒ�
	D3DXCOLOR GetColorOrigin() const { return m_colOrigin; }						// �F�擾
	void SetAlpha(float a) { m_col.a = a; }			// �����x�ݒ�
	float GetAlpha() { return m_col.a; }			// �����x�擾

	void SetSize(const MyLib::Vector2& size);								// �T�C�Y�ݒ�
	MyLib::Vector2 GetSize() const { return m_size; }						// �T�C�Y�擾
	void SetSizeOrigin(const MyLib::Vector2& size) { m_sizeOrigin = size; }	// ���̃T�C�Y�ݒ�
	MyLib::Vector2 GetSizeOrigin() const { return m_sizeOrigin; }			// ���̃T�C�Y�擾
	

	//--------------------------
	// ���_���
	//--------------------------
	// UV
	void SetUV(const std::vector<D3DXVECTOR2>& uv) { m_vecUV = uv; }		// �e�N�X�`�����W�ݒ�
	std::vector<D3DXVECTOR2> GetUV() { return m_vecUV; }					// �e�N�X�`�����W�擾

	// �A���J�[�|�C���g
	void SetAnchorType(const EAnchorPoint& type) { m_AnchorType = type; }	// �A���J�[�|�C���g�̎�ސݒ�
	EAnchorPoint GetAnchorType() { return m_AnchorType; }					// �A���J�[�|�C���g�̎�ގ擾
	void SetAnchorRate(float rate) { m_fAnchorRate = UtilFunc::Transformation::Clamp(rate, 0.0f, 1.0f); }	// �A���J�[�̊����ݒ�
	float GetAnchorRate() { return m_fAnchorRate; }															// �A���J�[�̊����擾

	static CObject3D *Create(int nPriority = 5);
	static CObject3D *Create(const MyLib::Vector3& pos, const MyLib::Vector3& rot);
	CObject3D* GetObject3D() { return this; }
	virtual CObject3DMesh* GetObject3DMesh() { return nullptr; }

protected:

	// �����o�ϐ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@

	// ���z�֐�
	virtual void CalWorldMtx();	// ���[���h�}�g���b�N�X�̌v�Z����

private:

	//=============================
	// �����o�֐�
	//=============================
	MyLib::Vector3 RotateVtx(const MyLib::Vector3& vtx, const MyLib::Vector3& center);

	//=============================
	// �����o�ϐ�
	//=============================
	MyLib::Matrix m_mtxWorld;	// ���[���h�}�g���b�N�X
	std::vector<D3DXVECTOR2> m_vecUV;		// �e�N�X�`�����W
	D3DXCOLOR m_col;				// �F
	D3DXCOLOR m_colOrigin;			// ���̐F
	MyLib::Vector2 m_size;			// �T�C�Y
	MyLib::Vector2 m_sizeOrigin;	// ���̃T�C�Y
	EAnchorPoint m_AnchorType;		// �A���J�[�|�C���g�̎��
	float m_fAnchorRate;			// �A���J�[�̊���
	float m_fLength;				// �Ίp���̒���
	float m_fAngle;					// �Ίp���̌���
	int m_nTexIdx;					// �e�N�X�`���̃C���f�b�N�X�ԍ�
};



#endif