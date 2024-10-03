//=============================================================================
// 
//  �R�[�X�w�b�_�[ [course.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _COURSE_H_
#define _COURSE_H_	// ��d�C���N���[�h�h�~

#include "object3DMesh.h"
#include "collisionLine_Box.h"
#include "scene.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �R�[�X�N���X
class CCourse : public CObject3DMesh
{

public:

	//=============================
	// �\���̒�`
	//=============================
	// �ӂ̓������
	struct LineInfo
	{
		MyLib::Vector3 pos;	// �ʒu
		MyLib::Vector3 rot;	// ����
		float width;		// ��

		LineInfo() : pos(0.0f), rot(0.0f), width(0.0f) {}
		LineInfo(const MyLib::Vector3& _pos, const MyLib::Vector3& _rot, float _width) : pos(_pos), rot(_rot), width(_width) {}
	};

	// �ӏ��
	struct Line
	{
		LineInfo lineInfo;	// �ӂ̓������
	};

	// ���_���
	struct VtxInfo
	{
		MyLib::Vector3 pos;	// �ʒu
		MyLib::Vector3 rot;	// ����

		VtxInfo() : pos(), rot() {}
		VtxInfo(const MyLib::Vector3& _pos, const MyLib::Vector3& _rot) : pos(_pos), rot(_rot) {}
	};

	CCourse(int nPriority = 2, const LAYER layer = LAYER::LAYER_MAP);
	~CCourse();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
	virtual void SetVtx();

	void Reset();	// ���Z�b�g
	void ReCreateVtx();	// ���Z�b�g
	void CalVtxPosition();	// �e���_�v�Z

	// ���擾�E�ݒ�
	CCollisionLine_Box* GetCollisionLineBox(int idx);								// �����蔻��{�b�N�X�擾
	std::vector<MyLib::Vector3> GetVecPosition() { return m_vecSegmentPosition; }	// ��_�̈ʒu�擾
	MyLib::Vector3 GetVecPosition(int idx);											// ��_�̈ʒu�擾
	void SetVecPosition(const std::vector<MyLib::Vector3>& vecpos) { m_vecSegmentPosition = vecpos; }
	void SetVecPosition(int idx, const MyLib::Vector3& pos);						// ��_�̈ʒu�ݒ�
	float GetCourceLength() { return m_courceLength; }								// �R�[�X�̒���
	static float GetCreateDistance() { return m_fCreateDistance; }					// �����Ԋu�擾

	std::vector<VtxInfo> GetVecVtxinfo() { return m_vecVtxInfo; }						// �e���_�̏��擾
	VtxInfo GetVecVtxinfo(int idx);														// �e���_�̏��擾
	void SetVecVtxinfo(const std::vector<VtxInfo>& vecinfo) { m_vecVtxInfo = vecinfo; }	// �e���_�̏��ݒ�
	void SetVecVtxinfo(int idx, const VtxInfo& vecinfo);								// �e���_�̏��ݒ�

	// ����
	float GetHeight(const MyLib::Vector3& pos, bool* pLand);	// �����擾
	int GetMyVtxIndex(const MyLib::Vector3& _pos);				// �����̂��钸�_�̃C���f�b�N�X�擾
	bool CollisionVtxQuad(int idx, MyLib::Vector3 rayPos, MyLib::Vector3* colPos);		// ���_����Ȃ镽�ʂƂ̓����蔻��

	// �t�@�C������
	void Save();	// �Z�[�u
	HRESULT Load(const std::string& file);	// ���[�h

	static CCourse *Create(const std::string& file, CScene::MODE mode = CScene::MODE::MODE_GAME);	// ����

protected:

	//=============================
	// �����o�֐�
	//=============================
	std::vector<MyLib::Vector3> CalSegmentDivision(const std::vector<MyLib::Vector3>& segment);	// �Z�O�����g�����v�Z
	void CalBothVtxPosition();	// �����̒��_���W�v�Z
	void SetVtxPosition();	// ���_���W

	//=============================
	// �����o�ϐ�
	//=============================
	std::string m_FileName;	// �t�@�C����
	std::vector<MyLib::Vector3> m_vecSegmentPosition;	// ��_�̈ʒu
	std::vector<VtxInfo> m_vecVtxInfo;		// �e���_�̏��
	std::vector<CCollisionLine_Box*> m_pCollisionLineBox;	// �����蔻��{�b�N�X
	float m_courceLength;
	float m_fTexU;		// U�X�N���[���p
	float m_fTexV;		// V�X�N���[���p
	float m_fSinCurve;	// �T�C���J�[�u�̈ړ���
	float m_fWaveHeight;	// �T�C���J�[�u�̍���
	bool m_bEnableWave;	// �g�̗L������
	static const float m_fCreateDistance;	// �����Ԋu
};



#endif