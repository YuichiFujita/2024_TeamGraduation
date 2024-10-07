////=============================================================================
//// 
////  �T���v��_�I�u�W�F�N�g2D���� [movie.cpp]
////  Author : ���n�Ή�
//// 
////=============================================================================
//#include "movie.h"
//#include "manager.h"
//#include "calculation.h"
//#include "input.h"
//
//
//#include <dshow.h>
//#include <atlbase.h>
//
//// ���̑��ϐ�
//IGraphBuilder* pGraph = nullptr;
//IMediaControl* pControl = nullptr;
//IMediaSeeking* pSeeking = nullptr;
//IBaseFilter* pSourceFilter = nullptr;
//IBasicVideo* pBasicVideo = nullptr;
//CComPtr<IVideoWindow> pVideoWindow;
//
//BYTE* pFrameBuffer = nullptr;
//
//LPDIRECT3DTEXTURE9 m_ptexture = nullptr;
//long lWidth = 0, lHeight = 0;
//
////==========================================================================
//// �萔��`
////==========================================================================
//namespace
//{
//	const std::string TEXTURE_SAMPLE = "data\\TEXTURE\\key\\A.png";	// �e�N�X�`���̃t�@�C��
//}
//
//HRESULT CMovie::RemoveVideoRenderer()
//{
//	CComPtr<IEnumFilters> pEnum;
//	HRESULT hr = pGraph->EnumFilters(&pEnum);
//	if (FAILED(hr)) return hr;
//
//	IBaseFilter* pFilter = nullptr;
//	while (pEnum->Next(1, &pFilter, NULL) == S_OK)
//	{
//		CComPtr<IBaseFilter> pBaseFilter = pFilter;
//		CComPtr<IFileSourceFilter> pFileSource;
//
//		// �t�B���^�̎�ނ��`�F�b�N
//		CLSID clsid;
//		hr = pBaseFilter->GetClassID(&clsid);
//		if (FAILED(hr)) continue;
//
//		// Video Renderer �� CLSID �� CLSID_VideoRenderer �ł�
//		if (clsid == CLSID_VideoRenderer)
//		{
//			// Video Renderer �t�B���^���폜
//			hr = pGraph->RemoveFilter(pBaseFilter);
//			if (FAILED(hr)) return hr;
//			break;
//		}
//	}
//
//	return S_OK;
//}
//
//
////==========================================================================
//// �R���X�g���N�^
////==========================================================================
//CMovie::CMovie(int nPriority) : CObject2D(nPriority)
//{
//	HRESULT hr;
//
//	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&pGraph);
//	if (FAILED(hr)) return;
//
//	hr = pGraph->RenderFile(L"data\\MOVIE\\TKD.mp4", NULL);
//	if (FAILED(hr)) return;
//
//	// Video Renderer �t�B���^�̍폜
//	hr = RemoveVideoRenderer();
//	if (FAILED(hr)) return;
//
//	hr = pGraph->QueryInterface(IID_IMediaControl, (void**)&pControl);
//	if (FAILED(hr)) return;
//
//	hr = pGraph->QueryInterface(IID_IMediaSeeking, (void**)&pSeeking);
//	if (FAILED(hr)) return;
//
//	hr = pGraph->QueryInterface(IID_IBasicVideo, (void**)&pBasicVideo);
//	if (FAILED(hr)) return;
//
//	hr = pGraph->QueryInterface(IID_IVideoWindow, (void**)&pVideoWindow);
//	if (SUCCEEDED(hr))
//	{
//		pVideoWindow->put_Visible(OAFALSE); // �E�B���h�E���\���ɐݒ�
//		pVideoWindow->put_AutoShow(OAFALSE); // �����I�ɕ\�����Ȃ��ݒ�
//	}
//
//	// �r�f�I�̕��ƍ������擾
//	hr = pBasicVideo->GetVideoSize(&lWidth, &lHeight);
//	if (FAILED(hr)) return;
//
//	// �t���[���o�b�t�@�̊m��
//	pFrameBuffer = new BYTE[lWidth * lHeight * 4]; // RGB32 �t�H�[�}�b�g
//
//}
//
//// �r�f�I�t���[�����e�N�X�`���ɓ]������֐�
//void CMovie::UpdateTextureFromVideoFrame(IBasicVideo* pBasicVideo)
//{
//	// �r�f�I�t���[���̎擾
//	long bufferSize = lWidth * lHeight * 4; // RGB32 �t�H�[�}�b�g
//	HRESULT hr = pBasicVideo->GetCurrentImage(&bufferSize, reinterpret_cast<long*>(pFrameBuffer));
//	if (SUCCEEDED(hr))
//	{
//		if (m_ptexture != nullptr)
//		{
//			m_ptexture->Release();
//			m_ptexture = nullptr;
//		}
//
//		if (m_ptexture == nullptr)
//		{
//			// �f�o�C�X�̎擾
//			LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
//
//			// �e�N�X�`���̐���
//			hr = pDevice->CreateTexture(
//				lWidth,          // �e�N�X�`���̕�
//				lHeight,         // �e�N�X�`���̍���
//				0,               // �~�b�v�}�b�v���x���i1 �Ń~�b�v�}�b�v�Ȃ��j
//				D3DUSAGE_DYNAMIC, // �g�p���@�i���I�e�N�X�`���j
//				D3DFMT_A8R8G8B8, // �s�N�Z���t�H�[�}�b�g�i��FA8R8G8B8�j
//				D3DPOOL_DEFAULT, // �������v�[��
//				&m_ptexture,     // �e�N�X�`���̃|�C���^
//				NULL             // �n���h���i�g�p���Ȃ��j
//			);
//		}
//
//		// �e�N�X�`���̃��b�N�ƃf�[�^�̃R�s�[
//		D3DLOCKED_RECT lockedRect;
//		hr = m_ptexture->LockRect(0, &lockedRect, NULL, D3DLOCK_DISCARD);
//		if (SUCCEEDED(hr))
//		{
//			memcpy(lockedRect.pBits, pFrameBuffer, bufferSize);
//			m_ptexture->UnlockRect(0);
//		}
//	}
//
//
//
//
//
//
//
//	//hr = pGraph->QueryInterface(IID_IBasicVideo, (void**)&pBasicVideo);
//	//if (SUCCEEDED(hr))
//	//{
//	//	hr = pBasicVideo->GetCurrentImage(&bufferSize, reinterpret_cast<long*>(pFrameBuffer));
//	//	pBasicVideo->Release();
//
//	//	if (SUCCEEDED(hr))
//	//	{
//	//		if (m_ptexture == nullptr)
//	//		{
//	//			// �f�o�C�X�̎擾
//	//			LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
//
//	//			// �e�N�X�`���̐���
//	//			HRESULT hr = pDevice->CreateTexture(
//	//				lWidth,          // �e�N�X�`���̕�
//	//				lHeight,         // �e�N�X�`���̍���
//	//				1,               // �~�b�v�}�b�v���x���i1 �Ń~�b�v�}�b�v�Ȃ��j
//	//				D3DUSAGE_DYNAMIC, // �g�p���@�i�����ł͓��I�e�N�X�`���j
//	//				D3DFMT_A8R8G8B8, // �s�N�Z���t�H�[�}�b�g�i��FA8R8G8B8�j
//	//				D3DPOOL_DEFAULT, // �������v�[��
//	//				&m_ptexture,       // �e�N�X�`���̃|�C���^
//	//				NULL             // �n���h���i�g�p���Ȃ��j
//	//			);
//	//		}
//
//	//		// �e�N�X�`���̃��b�N�ƃf�[�^�̃R�s�[
//	//		D3DLOCKED_RECT lockedRect;
//	//		hr = m_ptexture->LockRect(0, &lockedRect, NULL, D3DLOCK_DISCARD);
//	//		if (SUCCEEDED(hr))
//	//		{
//	//			memcpy(lockedRect.pBits, pFrameBuffer, bufferSize);
//	//			m_ptexture->UnlockRect(0);
//	//		}
//	//	}
//	//}
//
//	//pControl->Run(); // �Đ����ĊJ
//}
//
//
////==========================================================================
//// �f�X�g���N�^
////==========================================================================
//CMovie::~CMovie()
//{
//	// ���\�[�X�̉��
//	pBasicVideo->Release();
//	pControl->Release();
//	pSeeking->Release();
//	pGraph->Release();
//	CoUninitialize();
//}
//
////==========================================================================
//// ��������
////==========================================================================
//CMovie* CMovie::Create()
//{
//	// �������̊m��
//	CMovie* pObj = DEBUG_NEW CMovie;
//
//	if (pObj != nullptr)
//	{
//		// ����������
//		pObj->Init();
//	}
//
//	return pObj;
//}
//
////==========================================================================
//// ����������
////==========================================================================
//HRESULT CMovie::Init()
//{
//
//	// �I�u�W�F�N�g2D�̏�����
//	CObject2D::Init();
//
//	// �e�N�X�`���ݒ�
//	int texID = CTexture::GetInstance()->Regist(TEXTURE_SAMPLE);
//	BindTexture(texID);
//
//	// �T�C�Y�ݒ�
//	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);
//
//#if 0	// ���������ɃT�C�Y�ݒ�
//	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);
//
//#else	// �c�������ɃT�C�Y�ݒ�
//	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);
//#endif
//	SetSize(D3DXVECTOR2(854 / 2, 480 / 2));
//	SetSizeOrigin(D3DXVECTOR2(854 / 2, 480 / 2));
//
//
//	// �ʒu�A�����ݒ�͕K�v������Βǉ�
//
//	SetPosition(MyLib::Vector3(640.0f, 360.0f, 0.0f));
//
//	// ��ނ̐ݒ�
//	SetType(CObject::TYPE::TYPE_OBJECT2D);
//
//	return S_OK;
//}
//
//
//
////==========================================================================
//// �I������
////==========================================================================
//void CMovie::Uninit()
//{
//	// �I������
//	CObject2D::Uninit();
//}
//
////==========================================================================
//// �X�V����
////==========================================================================
//void CMovie::Update()
//{
//	if (CInputKeyboard::GetInstance()->GetTrigger(DIK_1))
//	{
//		UpdateTextureFromVideoFrame(pBasicVideo);
//	}
//
//	// �X�V����
//	CObject2D::Update();
//}
//
////==========================================================================
//// �`�揈��
////==========================================================================
//void CMovie::Draw()
//{
//	// �f�o�C�X�̎擾
//	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
//
//	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
//	pDevice->SetStreamSource(0, GetVtxBuff(), 0, sizeof(VERTEX_2D));
//
//	// ���_�t�H�[�}�b�g�̐ݒ�
//	pDevice->SetFVF(FVF_VERTEX_2D);
//
//	// �e�N�X�`���̐ݒ�
//	pDevice->SetTexture(0, m_ptexture);
//
//	// �|���S���̕`��
//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
//}
