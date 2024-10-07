//=============================================================================
// 
//  �}�b�v���b�V���w�b�_�[ [mapmesh.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _MAPMESH_H_
#define _MAPMESH_H_	// ��d�C���N���[�h�h�~

#include "object3DMesh.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �}�b�v���b�V���N���X
class CMapMesh : public CObject3DMesh
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum MeshType
	{
		TYPE_STONEWALL_LEFT = 0,	// �Ί_��
		TYPE_TOWNFIELD_SINUOUS,		// �X�t�B�[���h(���˂�)
		TYPE_TOWNFIELD_SINUOUS_FRONT,		// �X�t�B�[���h(���˂�)(��O)
		TYPE_TOWNFIELD_FIXEDPLANE,	// �X�t�B�[���h(�Œ蕽��)
		TYPE_TOWNFIELD_FIXEDPLANE_RESULT,	// �X�t�B�[���h(�Œ蕽��/���U���g)
		TYPE_TOWNFIELD_FIXEDPLANE_RANKING,	// �X�t�B�[���h(�Œ蕽��/�����L���O)
		TYPE_MAX
	};

	CMapMesh(int nPriority = 1, const LAYER layer = LAYER::LAYER_MAP);
	~CMapMesh();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void SetVtx() override;

	void Reset();	// ���Z�b�g
	void CalVtxPosition();	// �e���_�v�Z
	virtual void BindVtxPosition() = 0;	// ���_���W���蓖��

	// ���擾�E�ݒ�
	std::vector<MyLib::Vector3> GetVecPosition() { return m_vecSegmentPosition; }	// ��_�̈ʒu�擾
	MyLib::Vector3 GetVecPosition(int idx);											// ��_�̈ʒu�擾
	void SetVecPosition(const std::vector<MyLib::Vector3>& vecpos) { m_vecSegmentPosition = vecpos; }
	void SetVecPosition(int idx, const MyLib::Vector3& pos);						// ��_�̈ʒu�ݒ�
	std::vector<MyLib::Vector3> GetVecVtxPosition() { return m_vecVtxPosition; }	// �e���_�̈ʒu�擾
	MyLib::Vector3 GetVecVtxPosition(int idx);										// �e���_�̈ʒu�擾
	void SetVecVtxPosition(const std::vector<MyLib::Vector3>& vecpos) { m_vecVtxPosition = vecpos; }
	void SetVecVtxPosition(int idx, const MyLib::Vector3& pos);						// �e���_�̈ʒu�ݒ�

	static CMapMesh *Create(MeshType type);	// ����

protected:

	//=============================
	// �����o�ϐ�
	//=============================
	std::vector<MyLib::Vector3> m_vecSegmentPosition;	// ��_�̈ʒu
	std::vector<MyLib::Vector3> m_vecVtxPosition;		// �e���_�̈ʒu
	float m_courceLength;

private:

	virtual void SetVtxTexUV() = 0;	// �e���_UV���W�ݒ�
	 
	
};



#endif