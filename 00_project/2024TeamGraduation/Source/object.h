//=============================================================================
// 
//  �I�u�W�F�N�g�w�b�_�[ [object.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _OBJECT_H_
#define _OBJECT_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "constans.h"
#include "renderer.h"
#include "texture.h"

class CObject2D;
class CObject3D;
class CObject3DMesh;
class CObjectX;
class CObjectChara;
class CEffect3D;

//==========================================================================
// �N���X��`
//==========================================================================
// �I�u�W�F�N�g�N���X��`
class CObject
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	// ���
	enum TYPE
	{
		TYPE_NONE = 0,			// ����
		TYPE_PLAYER,			// �v���C���[
		TYPE_ENEMY,				// �G
		TYPE_EDIT,				// �G�f�B�b�g
		TYPE_XFILE,				// X�t�@�C��
		TYPE_STAGE,				// �X�e�[�W(��)
		TYPE_OBJECT2D,			// �I�u�W�F�N�g2D
		TYPE_OBJECT3D,			// �I�u�W�F�N�g3D
		TYPE_OBJECTMESH,		// �I�u�W�F�N�g3D
		TYPE_OBJECTX,			// �I�u�W�F�N�gX
		TYPE_OBJECTBILLBOARD,	// �I�u�W�F�N�g�r���{�[�h
		TYPE_MESHFIELD,			// ���b�V���t�B�[���h
		TYPE_MESHWALL,			// ���b�V���E�H�[��
		TYPE_MESHCYLINDER,		// ���b�V���V�����_�[
		TYPE_MESHDOME,			// ���b�V���h�[��
		TYPE_MESHDONUTS,		// ���b�V���h�[�i�c
		TYPE_MESHSPHERE,		// ���b�V���X�t�B�A
		TYPE_MANAGER,			// �}�l�[�W���[
		TYPE_NUMBER,			// ����
		TYPE_LIGHT,				// ���C�g
		TYPE_TIMER,				// �^�C�}�[
		TYPE_UI,				// UI
		TYPE_MAX
	};

	// ���C���[
	enum LAYER
	{
		LAYER_TIMER = 0,
		LAYER_MAP,
		LAYER_LINE,
		LAYER_DEFAULT,
		LAYER_2D,
		LAYER_MAX
	};

	CObject(int nPriority = mylib_const::PRIORITY_DEFAULT, const LAYER layer = LAYER::LAYER_DEFAULT);
	virtual ~CObject();

	//--------------------------
	// �������z�֐�
	//--------------------------
	virtual HRESULT Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;
	virtual void Draw() = 0;
	virtual void Kill() = 0;	// ���I�폜����

	//--------------------------
	// ���z�֐�
	//--------------------------
	virtual void Release();	// �J������
	// TODO�F������Kill
	virtual inline void SetVtx() { assert(false); }	// ���_���ݒ�

	//--------------------------
	// �@�\
	//--------------------------
	static void ReleaseAll();						// �S�J��
	static void UpdateAll(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �S�X�V
	static void DrawAll();							// �S�`��

	//--------------------------
	// ���ʂ���p�����[�^
	//--------------------------
	virtual inline void SetPosition(const MyLib::Vector3& pos)			{ m_pos = pos; }		// �ʒu�ݒ�
	virtual inline MyLib::Vector3 GetPosition() const					{ return m_pos; }		// �ʒu�擾
	virtual inline void SetOldPosition(const MyLib::Vector3& posOld)	{ m_posOld = posOld; }	// �ߋ��̈ʒu�ݒ�
	virtual inline MyLib::Vector3 GetOldPosition() const				{ return m_posOld; }	// �ߋ��̈ʒu�擾
	virtual inline void SetOriginPosition(const MyLib::Vector3& pos)	{ m_posOrigin = pos; }	// ���̈ʒu�ݒ�
	virtual inline MyLib::Vector3 GetOriginPosition() const				{ return m_posOrigin; }	// ���̈ʒu�擾
	virtual inline void SetMove(const MyLib::Vector3& move)				{ m_move = move; }		// �ړ��ʐݒ�
	virtual inline MyLib::Vector3 GetMove() const						{ return m_move; }		// �ړ��ʎ擾
	virtual inline void SetRotation(const MyLib::Vector3& rot)			{ m_rot = rot; }		// �����ݒ�
	virtual inline MyLib::Vector3 GetRotation() const					{ return m_rot; }		// �����擾
	virtual inline void SetOldRotation(const MyLib::Vector3& rot)		{ m_rotOld = rot; }		// �O��̌����ݒ�
	virtual inline MyLib::Vector3 GetOldRotation() const				{ return m_rotOld; }	// �O��̌����擾
	virtual inline void SetOriginRotation(const MyLib::Vector3& rot)	{ m_rotOrigin = rot; }	// ���̌����ݒ�
	virtual inline MyLib::Vector3 GetOriginRotation() const				{ return m_rotOrigin; }	// ���̌����擾

	// �ύX����
	virtual inline void AddPosition(const MyLib::Vector3& pos)	{ m_pos += pos; }	// �ʒu���Z
	virtual inline void AddMove(const MyLib::Vector3& move)		{ m_move += move; }	// �ړ��ʉ��Z
	virtual inline void AddRotation(const MyLib::Vector3& rot)	{ m_rot += rot; }	// �������Z

	//--------------------------
	// �`��E�X�N���[���֘A
	//--------------------------
	inline int GetPriority() { return m_nPriority; }					// �D�揇�ʎ擾
	static bool ZSort(const CObject* obj1, const CObject* obj2);		// Z�\�[�g�̔�r�֐�
	static bool ZSortInverse(const CObject *obj1, const CObject *obj2);	// �tZ�\�[�g�̔�r�֐�
	virtual inline void SetEnableDisp(bool bDisp) { m_bDisp = bDisp; }	// �`��󋵐ݒ�
	bool IsDisp() { return m_bDisp; }									// �`��󋵎擾
	void SetEnablePosibleMove_WorldPause(bool bMove) { m_bPosibleMove_WorldPause = bMove; }	// ���E��~���ɓ�����t���O
	bool IsPosibleMove_WorldPause() { return m_bPosibleMove_WorldPause; }					// ���E��~���ɓ�����t���O

	//--------------------------
	// ���̑�
	//--------------------------
	void SetEnableDeath(bool bDeath) { m_bDeath = bDeath; }				// ���S�t���O�ݒ�
	bool IsDeath() { return m_bDeath; }									// ���S�t���O�擾
	TYPE GetType() const { return m_type; }								// ��ގ擾
	virtual inline void SetType(const TYPE type) { m_type = type; }		// ��ސݒ�
	static const int GetNumAll() { return m_nNumAll; }					// �����擾
	static std::map<LAYER, std::map<int, std::vector<CObject*>>> GetTop() { return m_pObj; }	// �擪�擾

private:

	//=============================
	// �����o�֐�
	//=============================
	static void DrawNone(const LAYER layer, int nPriority);		// �ʏ�`��
	static void DrawZSort(const LAYER layer, int nPriority);	// Z�\�[�g�`��
	float ScreenZ() const;										// �X�N���[����Z���W�擾

	//=============================
	// �����o�ϐ�
	//=============================
	//--------------------------
	// ���ʂ���p�����[�^
	//--------------------------
	MyLib::Vector3 m_pos;			// �ʒu
	MyLib::Vector3 m_posOld;		// �O��̈ʒu
	MyLib::Vector3 m_posOrigin;		// ���̈ʒu
	MyLib::Vector3 m_rot;			// ����
	MyLib::Vector3 m_rotOld;		// �O��̌���
	MyLib::Vector3 m_rotOrigin;		// ���̌���
	MyLib::Vector3 m_move;			// �ړ���

	//--------------------------
	// �I�u�W�F�N�g�Ǘ��p
	//--------------------------
	int m_nPriority;		// �D�揇��
	LAYER m_Layer;			// ���C���[��
	TYPE m_type;			// ���
	static std::map<LAYER, std::map<int, std::vector<CObject*>>> m_pObj;	// �I�u�W�F�N�g�i�[�p

	//--------------------------
	// ���̑�
	//--------------------------
	bool m_bDeath;				// ���S�t���O
	bool m_bDisp;				// �`��t���O
	bool m_bPosibleMove_WorldPause;		// ���E��~���ɓ�����t���O
	static int m_nNumAll;		// �I�u�W�F�N�g�̑���

};

#endif