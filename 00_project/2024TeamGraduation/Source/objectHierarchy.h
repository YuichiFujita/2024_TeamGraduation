//=============================================================================
// 
//  �K�w�I�u�W�F�N�g�w�b�_�[ [objectHierarchy.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _OBJECTHIERARCHY_H_
#define _OBJECTHIERARCHY_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "object.h"
#include "characterStatus.h"
#include "BallStatus.h"

//==========================================================================
// �O���錾
//==========================================================================
class CModel;

//==========================================================================
// �N���X��`
//==========================================================================
// �K�w�I�u�W�F�N�g�N���X��`
class CObjectHierarchy : public CObject
{
public:

	//=============================
	// �\���̒�`
	//=============================
	// �ǂݍ��ގq�f�[�^
	struct LoadData
	{
		std::string pModelFile;	// ���f���t�@�C����
		int nType;				// ���f�����
		int nParent;			// �e�̃C���f�b�N�X
		int nStart;				// �ŏ����炠�邩�ǂ���
		int nSwitchType;		// �؂�ւ��̎��
		int nIDSwitchModel;		// �؂�ւ����f����ID
		MyLib::Vector3 pos;		// �ʒu
		MyLib::Vector3 rot;		// ����
	};

	// �ǂݍ��ސe�f�[�^
	struct Load
	{
		std::string sTextFile;						// �e�L�X�g�t�@�C����
		int nCenterIdx;								// ���S�ɂ���p�[�c�C���f�b�N�X
		MyLib::Vector3 centerOffSet;				// ���S�ʒu�̃I�t�Z�b�g
		int nNumModel;								// ���f����
		MyLib::Vector3 posOrigin;					// �ŏ��̈ʒu
		float scale;								// �X�P�[��
		CCharacterStatus::CharParameter parameter;	// �p�����[�^�[
		CBallStatus::SBallParameter parameterBall;	// �p�����[�^�[(�{�[��)
		std::vector<LoadData> LoadData;	// �ǂݍ��ގq�f�[�^

		// �R���X�g���N�^
		Load() : scale(1.0f) {}
	};

	// �񋓌^��`
	enum EState
	{
		STATE_NONE = 0,	// �Ȃɂ��Ȃ�
		STATE_MAX
	};

	CObjectHierarchy(int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CObjectHierarchy();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	virtual void Draw(D3DXCOLOR col);
	virtual void Draw(float fAlpha);
	virtual void Kill() override;	// ���I�폜����

	void SetmtxWorld(const MyLib::Matrix mtxWorld);
	MyLib::Matrix GetWorldMtx() const { return m_mtxWorld; }			// �}�g���b�N�X�擾
	MyLib::Matrix GetmtxWorld() const;			// ���[���h�}�g���b�N�X�擾
	MyLib::Vector3 GetCenterPosition() const;			// ���S�̈ʒu�擾
	void SetOriginPosition(const MyLib::Vector3& pos) { m_posOrigin = pos; }	// �ŏ��̈ʒu�ݒ�
	MyLib::Vector3 GetOriginPosition() const { return m_posOrigin; }			// �ŏ��̈ʒu�擾
	void SetRadius(const float fRadius) { m_fRadius = fRadius; }	// ���a�ݒ�
	float GetRadius() const { return m_fRadius; }					// ���a�擾
	void SetScale(const float scale) { m_fScale = scale; }			// �X�P�[���ݒ�
	float GetScale() { return m_fScale; }							// �X�P�[���擾
	void CalWorldMtx();		// ���[���h�}�g���b�N�X�̌v�Z����

	HRESULT ReadText(const std::string& file);	// �O���t�@�C���ǂݍ��ݏ���
	virtual HRESULT SetCharacter(const std::string& file);	// �L�����N�^�[�ݒ�

	void ChangeObject(int nSwitchIdx, const std::string& file);		// �؂�ւ��̎��
	void SetObject(int nNewParts);			// ���f���̐ݒ�
	void DeleteObject(int nDeleteIdx);		// ���f���폜

	int GetNumModel();					// ���f�����擾
	void SetNumModel(int nNumModel);		// ���f�����ݒ�
	int GetIdxFile();					// �t�@�C���̃C���f�b�N�X�ԍ��擾
	Load GetLoadData(int nIdx);				// �ǂݍ��ݏ��擾
	CModel** GetModel();				// ���f���擾
	CModel* GetModel(int idx);			// ���f���擾
	CObjectHierarchy* GetObjectHierarchy();	// �K�w�I�u�W�F�N�g�擾
	static CObjectHierarchy *Create(const std::string& pTextFile);

protected:

	virtual void LoadObjectData(FILE* pFile, const std::string& file);	// �I�u�W�F�N�g���̃f�[�^�ǂݍ���
	virtual void LoadPartsData(FILE* pFile, const std::string& file, int *pCntParts);	// �p�[�c���̃f�[�^�ǂݍ���
	virtual void BindObjectData(int nCntData);	// �I�u�W�F�N�g���̃f�[�^���蓖��

	std::vector<CModel*> m_apModel;		// ���f��(�p�[�c)�̃|�C���^
	int m_nIdxFile;					// �t�@�C���̃C���f�b�N�X�ԍ�
	static std::vector<Load> m_aLoadData;
	static int m_nNumLoad;	// �ǂݍ��񂾐�
private:

	//=============================
	// �����o�֐�
	//=============================
	void AdjustPositionByScale();	// �X�P�[�������ƂɈʒu����

	//=============================
	// �����o�ϐ�
	//=============================
	MyLib::Matrix m_mtxWorld;	// ���[���h�}�g���b�N�X
	MyLib::Vector3 m_posOrigin;	// �ŏ��̈ʒu
	MyLib::Vector3 m_posInit;	// �����̈ʒu
	MyLib::Vector3 m_posCenter;	// ���S�ʒu
	float m_fScale;				// �X�P�[��

	int m_nCenterPartsIdx;			// ���S�ɂ���p�[�c�̃C���f�b�N�X
	MyLib::Vector3 m_CenterOffset;	// ���S�̃I�t�Z�b�g
	float m_fRadius;				// ���a
	int m_nNumModel;				// ���f���̐�

};



#endif