//=============================================================================
// 
//  �G�t�F�N�V�A�w�b�_�[ [MyEffekseer.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _MYEFFEKSEER_H_
#define _MYEFFEKSEER_H_	// ��d�C���N���[�h�h�~

#include <EffekseerRendererDX9.h>
#include <XAudio2.h>
#include <d3d9.h>
#include <functional>
#include <wrl/client.h>
#include "listmanager.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �G�t�F�N�V�A�N���X��`
class CMyEffekseer
{
public:
	
	//=============================
	// �\���̒�`
	//=============================
	struct MyEffekseerInfo
	{
		MyLib::Vector3 pos;
		MyLib::Vector3 rot;
		MyLib::Vector3 move;		// �ړ���
		float scale;
		Effekseer::EffectRef efcRef;
		Effekseer::Handle handle;	// �G�t�F�N�g�̃I�u�W�F�N�g
		bool bAutoDeath;			// �����폜�̃t���O

		// �f�t�H���g�R���X�g���N�^
		MyEffekseerInfo() : pos(), rot(), move(), scale(), efcRef(), handle(), bAutoDeath() {}

		// �����t���R���X�g���N�^
		MyEffekseerInfo(
			const MyLib::Vector3& _pos, const MyLib::Vector3& _rot, const MyLib::Vector3& _move, float _scale,
			const Effekseer::EffectRef& _efcRef, const Effekseer::Handle _handle, bool _bAutoDeath) : 
			pos(_pos), rot(_rot), move(_move), scale(_scale), efcRef(_efcRef), handle(_handle), bAutoDeath(_bAutoDeath) {}
	};

	//=============================
	// �񋓌^��`
	//=============================
	enum EFKLABEL
	{
		EFKLABEL_SAMPLE_LASER = 0,	// �T���v���̃��[�U�[
		EFKLABEL_BRESS,				// ��
		EFKLABEL_RIVER_SAMPLE,		// ��T���v��
		EFKLABEL_IMPACT,			// �Ռ�
		EFKLABEL_SPRAYWATER,		// �����Ԃ�
		EFKLABEL_SPRAYWATER_MINI,	// �����Ԃ�
		EFKLABEL_WATERJUMP,			// ������
		EFKLABEL_PLAYERMOVE_LINE,	// �v���C���[�̈ړ���
		EFKLABEL_PHOTON,			// ���S
		EFKLABEL_BAGGAGEGET,		// �ו��l��
		EFKLABEL_AIR,				// ��C
		EFKLABEL_CROWD,				// �Q�O
		EFKLABEL_TP,				// �e���|�[�g
		EFKLABEL_HIT,				// �q�b�g
		EFKLABEL_WING,				// �H�΂���
		EFKLABEL_SMASH,				// �ׂ��
		EFKLABEL_AURA,				// �I�[��
		EFKLABEL_MAX
	};

	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	CMyEffekseer();
	~CMyEffekseer();

	void Uninit();
	void Update();
	void StopAll();	// �S�Ē�~

	Effekseer::EffectRef LoadEffect(std::string efkpath);	// �G�t�F�N�g�ǂݍ���
	Effekseer::EffectRef LoadEffect(EFKLABEL label);		// �G�t�F�N�g�ǂݍ���
	Effekseer::ManagerRef GetEfkManager() { return m_efkManager; }	// �G�t�F�N�g�̃}�l�[�W���[�擾
	::EffekseerRendererDX9::RendererRef GetEffekseerRenderer() { return efkRenderer; }	// �G�t�F�N�V�A�̃����_���[�擾

	// �ÓI�֐�
	static CMyEffekseer* GetInstance() { return m_pMyEffekseer; }	// �C���X�^���X�擾
	static CMyEffekseer* Create();								// �C���X�^���X����

private:

	//=============================
	// �����o�֐�
	//=============================
	HRESULT Init();
	void Draw();
	void UpdateAll();

	void SetupEffekseerModules(::Effekseer::ManagerRef efkManager);	// ���W���[���̃Z�b�g�A�b�v
	Effekseer::EffectRef LoadProcess(const std::u16string& efkpath);	// �ǂݍ��ݏ���

	//=============================
	// �����o�ϐ�
	//=============================
	int32_t time = 0;
	Effekseer::Handle efkHandle = 0;
	EffekseerRendererDX9::RendererRef efkRenderer;
	Effekseer::ManagerRef m_efkManager;
	Effekseer::Vector3D viewerPosition;
	Effekseer::Matrix44 projectionMatrix;
	Effekseer::Matrix44 cameraMatrix;
	std::function<void()> onLostDevice;
	std::function<void()> onResetDevice;

	// ����ϐ�
	static std::string m_EffectName[EFKLABEL_MAX];	// �G�t�F�N�g�̃t�@�C����
	static CMyEffekseer* m_pMyEffekseer;			// ���g�̃|�C���^
};



#endif