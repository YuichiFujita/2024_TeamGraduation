//=============================================================================
// 
//  �G�t�F�N�V�A�w�b�_�[ [MyEffekseer.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _MYEFFEKSEER_H_
#define _MYEFFEKSEER_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include <EffekseerRendererDX9.h>
#include <XAudio2.h>
#include <d3d9.h>
#include <functional>
#include <wrl/client.h>
#include "listmanager.h"

//==========================================================================
// �O���錾
//==========================================================================
class CEffekseerObj;

//==========================================================================
// �N���X��`
//==========================================================================
// �G�t�F�N�V�A�N���X��`
class CMyEffekseer
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum EEfkLabel
	{
		EFKLABEL_SAMPLE_LASER = 0,		// �T���v���̃��[�U�[
		EFKLABEL_THROWLINE_NORMAL,		// ���������̐�(�ʏ�)
		EFKLABEL_THROWLINE_FAST,		// ���������̐�(����)
		EFKLABEL_CAMEHAME_ENERGY,		// ���߂͂ߔg�G�l���M�[����
		EFKLABEL_CAMEHAME_CHARGE,		// ���߂͂ߔg(�`���[�W)
		EFKLABEL_CAMEHAME_CHARGE_WIND,	// ���߂͂ߔg(�`���[�W)(��)
		EFKLABEL_CAMEHAME_CHARGE_THUNDER,	// ���߂͂ߔg(�`���[�W)(��)
		EFKLABEL_CAMEHAME_ATMOSPHERE,	// ���߂͂ߔg(��ԃI�[��)
		EFKLABEL_CAMEHAME_BRUST,		// ���߂͂ߔg(����)
		EFKLABEL_CAMEHAME_FLASH,		// ���߂͂ߔg(�t���b�V��)
		EFKLABEL_CAMEHAME_BALLAST,		// ���߂͂ߔg(��Ԃ��ꂫ)
		EFKLABEL_HIT,					// �q�b�g
		EFKLABEL_SWEAT,					// ��
		EFKLABEL_BLINK,					// �u�����N
		EFKLABEL_MAX
	};
	
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

	struct SDebugInfo	// �f�o�b�O���
	{
		EEfkLabel label;		// �G�t�F�N�g���x��
		MyLib::Vector3 pos;		// �ʒu
		MyLib::Vector3 rot;		// ����
		float scale;			// �X�P�[��
		CEffekseerObj* pEfkObj;	// �G�t�F�N�V�A�I�u�W�F�N�g

		SDebugInfo() : label(EEfkLabel::EFKLABEL_SAMPLE_LASER), scale(10.0f), pEfkObj(nullptr) {}
	};

	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	CMyEffekseer();
	~CMyEffekseer();

	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void StopAll();	// �S�Ē�~

	Effekseer::EffectRef LoadEffect(std::string efkpath);	// �G�t�F�N�g�ǂݍ���
	Effekseer::EffectRef LoadEffect(EEfkLabel label);		// �G�t�F�N�g�ǂݍ���
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
	void UpdateAll(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	void SetupEffekseerModules(::Effekseer::ManagerRef efkManager);		// ���W���[���̃Z�b�g�A�b�v
	Effekseer::EffectRef LoadProcess(const std::u16string& efkpath);	// �ǂݍ��ݏ���
	void Debug();	// �f�o�b�O

	//=============================
	// �����o�ϐ�
	//=============================
	float fTime = 0.0f;
	Effekseer::Handle efkHandle = 0;
	EffekseerRendererDX9::RendererRef efkRenderer;
	Effekseer::ManagerRef m_efkManager;
	Effekseer::Vector3D viewerPosition;
	Effekseer::Matrix44 projectionMatrix;
	Effekseer::Matrix44 cameraMatrix;
	std::function<void()> onLostDevice;
	std::function<void()> onResetDevice;

	// ����ϐ�
	SDebugInfo m_debugInfo;							// �f�o�b�O���
	static std::string m_EffectName[EFKLABEL_MAX];	// �G�t�F�N�g�̃t�@�C����
	static CMyEffekseer* m_pMyEffekseer;			// ���g�̃|�C���^
};



#endif