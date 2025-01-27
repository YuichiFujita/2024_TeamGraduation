//==========================================================================
// 
//  �����_���[�w�b�_�[ [renderer.h]
//  Author : ���n�Ή�
//  Adder  : ���c�E��
// 
//==========================================================================

#ifndef _RENDERER_H_
#define _RENDERER_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �O���錾
//==========================================================================
class CRenderTextureManager;
class CRenderTexture;
class CScreen;

//==========================================================================
// �N���X��`
//==========================================================================
// �����_���[�N���X��`
class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	// �G�C���A�X��`
	using ACameraFunc = std::function<void()>;	// �J�����ݒ�֐��^
	using ADrawFunc = std::function<void()>;	// �`��֐��^

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void Draw();
	void DrawRenderTexture(LPDIRECT3DSURFACE9* pSurface, ADrawFunc pDrawFunc, ACameraFunc pSetCameraFunc);	// �����_�[�e�N�X�`���`��
	HRESULT CreateRenderTexture();				// �����_�[�e�N�X�`���[����
	void SetEnableShader(const bool bShader, const float fTime = 0.0f);	// �X�N���[���V�F�[�_�[�t���O�ݒ�
	bool IsShader() const;						// �X�N���[���V�F�[�_�[�t���O�擾
	int GetRenderTextureIndex() const;			// �����_�[�e�N�X�`���C���f�b�N�X�擾
	inline LPDIRECT3DDEVICE9 GetDevice() const	{ return m_pD3DDevice; }
	inline LPDIRECT3D9 GetD3D() const			{ return m_pD3D; }

private:
	// �����o�ϐ�
	CRenderTextureManager*	m_pRenderTextureManager;	// �����_�[�e�N�X�`���}�l�[�W���[
	CRenderTexture*			m_pRenderScene;		// �V�[�������_�[�e�N�X�`��
	CScreen*				m_pDrawScreen;		// �X�N���[���`��|���S��
	LPDIRECT3DSURFACE9		m_pDefSurScreen;	// ���̕`��T�[�t�F�C�X�ۑ��p
	LPDIRECT3D9				m_pD3D;				// Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9		m_pD3DDevice;		// Direct3D�f�o�C�X
};

#endif
