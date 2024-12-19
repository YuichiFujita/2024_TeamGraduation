//=============================================================================
// 
//  �I�u�W�F�N�g2D�w�b�_�[ [object2D.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_	// ��d�C���N���[�h�h�~

#include "object.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �I�u�W�F�N�g2D�N���X��`
class CObject2D : public CObject
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum AnchorPoint
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

	CObject2D(int nPriority, const LAYER layer = LAYER::LAYER_2D);
	virtual ~CObject2D();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init() override;
	HRESULT Init(int nNumVertex);
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;
	virtual void Kill() override;	// ���I�폜����


	virtual void SetVtx();
	virtual void SetVtx(int nNumVertex);
	void BindTexture(int nIdx) { m_nTexIdx = nIdx; }	// �e�N�X�`���C���f�b�N�X�ݒ�
	int GetIdxTexture() { return m_nTexIdx; }			// �e�N�X�`���C���f�b�N�X�擾
	
	//--------------------------
	// �F
	//--------------------------
	virtual void SetColor(const D3DXCOLOR& col) { m_col = col; }					// �F�ݒ�
	virtual D3DXCOLOR GetColor() const { return m_col; }							// �F�擾
	virtual void SetOriginColor(const D3DXCOLOR& col) { m_colOrigin = col; }		// ���̐F�ݒ�
	virtual D3DXCOLOR GetOriginColor() const { return m_colOrigin; }				// ���̐F�擾
	virtual void SetAlpha(const float alpha) { m_col.a = alpha; }					// �s�����x�ݒ�
	virtual float GetAlpha() const { return m_col.a; }								// �s�����x�擾

	//--------------------------
	// �T�C�Y�֘A
	//--------------------------
	virtual void SetSize(const D3DXVECTOR2& size);									// �T�C�Y�̐ݒ�
	virtual D3DXVECTOR2 GetSize() const { return m_size; }							// �T�C�Y�̎擾
	virtual void SetSizeOrigin(const D3DXVECTOR2& size) { m_sizeOrigin = size; }	// ���̃T�C�Y�̐ݒ�
	virtual D3DXVECTOR2 GetSizeOrigin() const { return m_sizeOrigin; }				// ���̃T�C�Y�̎擾
	
	// �Ίp��
	void SetLength(const float fLength) { m_fLength = fLength; }					// �Ίp���̒����ݒ�
	float GetLength() const { return m_fLength; }									// �Ίp���̒����擾
	void SetAngle(const float fAngle) { m_fAngle = fAngle; }						// �Ίp���̌����ݒ�
	float GetAngle() const { return m_fAngle; }										// �Ίp���̌����擾

	//--------------------------
	// ���_���
	//--------------------------
	// UV
	void SetTex(D3DXVECTOR2* tex) { memcpy(&m_vecUV[0], tex, sizeof(m_vecUV)); }	// �e�N�X�`�����W�̐ݒ�
	void SetTexUV(const std::vector<D3DXVECTOR2>& uv);								// �e�N�X�`�����W�̐ݒ�
	D3DXVECTOR2* GetTex() { return &m_vecUV[0]; }									// �e�N�X�`�����W�̃|�C���^�擾
	std::vector<D3DXVECTOR2> GetVecTexUV() { return m_vecUV; }						// �e�N�X�`�����W�̎擾
	
	// ���_���W
	void SetVecVtxPos(const std::vector<MyLib::Vector3>& vecpos) { m_vecVtxPos = vecpos; }	// ���_���W�ݒ�
	MyLib::Vector3* GetVtxPos() { return &m_vecVtxPos[0]; }									// ���_���W�̃|�C���^�擾
	std::vector<MyLib::Vector3> GetVecVtxPos() { return m_vecVtxPos; }						// ���_���W�擾
	int GetNumVertex() { return m_nNumVertex; }							// ���_��

	// ���_�o�b�t�@
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff() const { return m_pVtxBuff; }	// ���_�o�b�t�@�擾

	//--------------------------
	// �A���J�[�|�C���g
	//--------------------------
	void SetAnchorType(const AnchorPoint& type) { m_AnchorType = type; }	// �A���J�[�|�C���g�̎�ސݒ�
	AnchorPoint GetAnchorType() { return m_AnchorType; }					// �A���J�[�|�C���g�̎�ގ擾

	static CObject2D *Create(int nPriority = mylib_const::PRIORITY_DEF2D, int nNumVtx = 4);	// ��������

private:

	//=============================
	// �����o�֐�
	//=============================
	MyLib::Vector3 RotateVtx(const MyLib::Vector3& vtx, const MyLib::Vector3& center);	// ���_��]����

	//=============================
	// �����o�ϐ�
	//=============================
	D3DXCOLOR m_col;							// �F
	D3DXCOLOR m_colOrigin;						// ���̐F
	D3DXVECTOR2 m_size;							// �T�C�Y
	D3DXVECTOR2 m_sizeOrigin;					// ���̃T�C�Y
	AnchorPoint m_AnchorType;					// �A���J�[�|�C���g�̎��
	float m_fLength;							// �Ίp���̒���
	float m_fAngle;								// �Ίp���̌���
	int m_nTexIdx;								// �e�N�X�`���̃C���f�b�N�X�ԍ�
	int m_nNumVertex;							// ���_��
	std::vector<D3DXVECTOR2> m_vecUV;			// �e�N�X�`�����W
	std::vector<MyLib::Vector3> m_vecVtxPos;	// ���_���W
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// ���_�o�b�t�@
};

#endif