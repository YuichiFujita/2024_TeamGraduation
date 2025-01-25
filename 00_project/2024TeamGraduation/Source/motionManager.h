//=============================================================================
// 
//  ���[�V�����}�l�[�W���[�w�b�_�[ [motionManager.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _MOTIONMANAGER_H_
#define _MOTIONMANAGER_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "gamemanager.h"
#include "constans.h"

//==========================================================================
// �O���錾
//==========================================================================
class CModel;
class CObjectChara;

//==========================================================================
// �N���X��`
//==========================================================================
// ���[�V�����}�l�[�W���[�N���X
class CMotionManager
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum LOOP
	{
		LOOP_OFF = 0,	// ���[�v����
		LOOP_ON,		// ���[�v����
		LOOP_MAX
	};

	//=============================
	// �\���̒�`
	//=============================
	// �������
	struct AlignInfo
	{
		int nFrame;				// ���̃t���[��
		int nExtensionFrame;	// �P�\�t���[��
		bool bSet;				// �ݒ莞

		// �R���X�g���N�^
		AlignInfo() : nFrame(0), nExtensionFrame(0), bSet(false) {}
	};

	// ����̍\����
	struct AttackInfo
	{
		int nCollisionNum;		// �����蔻��̃p�[�c�ԍ�
		float fRangeSize;		// ����̃T�C�Y
		MyLib::Vector3 Offset;	// ����̃I�t�Z�b�g
		int nMinCnt;			// ����̍Œ�J�E���g
		int nMaxCnt;			// ����̍ő�J�E���g
		float fCntRatio;		// ����J�E���g�̊���
		int nDamage;			// �_���[�W
		int nInpactCnt;			// �Ռ��̃J�E���g
		bool bInpactAct;		// �Ռ��J�E���g�̍s����������
		bool bInpactActSet;		// �Ռ��J�E���g�̍s���ݒ肵����
		bool bAtkking;			// �U���t���O
		bool bEndAtk;			// �U���̏I���t���O
		AlignInfo AlignInfo;	// �������
	};

	// �p�[�c���
	struct Parts
	{
		MyLib::Vector3 rot;			// ����
		MyLib::Vector3 rotDest;		// �ڕW�̌���
		MyLib::Vector3 pos;			// �ʒu
		MyLib::Vector3 posDest;		// �ڕW�̈ʒu
		MyLib::Vector3 posOrigin;	// �ʒu�̌��_
		MyLib::Vector3 scale;		// �X�P�[��

		Parts() : scale(1.0f) {}
	};

	// �L�[���
	struct Key
	{
		std::vector<Parts> aParts;	// �p�[�c���
		int nFrame;					// �Đ��t���[��
		float fRotMove;				// �ړ��̌���
	};

	// �S�̂̏��
	struct Info
	{
		std::vector<Key> aKey;	// �L�[���
		int nNumKey;			// �L�[�̐�
		int nLoop;				// ���[�v����
		int nMove;				// �ړ�����
		bool bSpecial;			// �X�y�V��������
		int nNumAttackInfo;		// �U�����̐�
		int nCancelableFrame;	// �L�����Z���\�t���[��
		int nCombolableFrame;	// �R���{�\�t���[��
		std::vector<AttackInfo> AttackInfo;	// �����蔻��p

		// �R���X�g���N�^
		Info() : bSpecial(false), nCancelableFrame(-1), nCombolableFrame(-1) {}
	};

	// �ǂݍ��ݏ��ۑ����
	struct SLoadInfo
	{
		std::vector<int> vecDefaultIdx;					// �f�t�H���g�̃C���f�b�N�X
		std::string sTextFile;							// �e�L�X�g�t�@�C����
		int nNumLoadData;								// ���[�V�������̃f�[�^��
		std::vector<CMotionManager::Info> vecLoadData;	// ���[�V�����̓ǂݍ��ݏ��
	};


	//=============================
	// �R���X�g���N�^, �f�X�g���N�^
	//=============================
	CMotionManager();
	~CMotionManager();

	//=============================
	// �����o�֐�
	//=============================
	HRESULT Init();	// ����������
	void Uninit();	// �I������
	void Reset();	// ���Z�b�g

	//--------------------------
	// �Đ������
	//--------------------------
	SLoadInfo Load(const std::string& setupfile);	// �ǂݍ��ݏ���

	//=============================
	// �ÓI�����o�֐�
	//=============================
	static CMotionManager* Create();	// ����
	static CMotionManager* GetInstance() { return m_pThisPtr; }	// �C���X�^���X�擾

private:

	Info LoadMotion(const std::string& file, int nMotion);

	// �����o�ϐ�
	std::vector<SLoadInfo> m_LoadInfo;	// �ǂݍ��݃f�[�^
	static CMotionManager* m_pThisPtr;	// ���g�̃|�C���^
};

#endif