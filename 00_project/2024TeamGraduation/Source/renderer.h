//=============================================================================
// 
//  レンダラーヘッダー [renderer.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _RENDERER_H_
#define _RENDERER_H_	// 二重インクルード防止

#include "main.h"

//==========================================================================
// クラス定義
//==========================================================================
// レンダラークラス定義
class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void Draw();
	LPDIRECT3DDEVICE9 GetDevice() const;
	LPDIRECT3D9 GetD3D() const { return m_pD3D; }


private:
	LPDIRECT3D9 m_pD3D;					// Direct3Dオブジェクトへのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice;		// Direct3Dデバイスへのポインタ
};


#endif