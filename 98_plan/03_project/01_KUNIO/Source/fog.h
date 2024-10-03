//=============================================================================
// 
// フォグ [fog.h]
// Author : 相馬靜雅
// 
//=============================================================================

#ifndef _FOG_H_
#define _FOG_H_		// 二重インクルード防止

#include "manager.h"
#include "renderer.h"

/**
	@brief	フォグの機能
*/
namespace MyFog
{

    extern D3DXCOLOR Color;
    extern float Start;
    extern float End;
    extern D3DFOGMODE Mode;
    extern bool bOnScreen;

    /**
    @brief	フォグのパラメータを設定
    @param	color	[in]	フォグの色
    @param	start	[in]	フォグの開始距離
    @param	end 	[in]	フォグの終了距離
    @param	mode 	[in]	フォグの種類
    */
    inline void SetFogparam(const D3DXCOLOR& color, float start, float end, D3DFOGMODE mode)
    {
        Color = color;
        Start = start;
        End = end;
        Mode = mode;
    }

    /**
    @brief	フォグのフラグ切り替え
    @param	onscreen	[in]	フォグのフラグ
    */
    inline void ToggleFogFrag(bool onscreen)
    {
        bOnScreen = onscreen;
    }

    /**
    @brief	フォグのフラグ切り替え
    */
    inline void ToggleFogFrag()
    {
        bOnScreen = bOnScreen ? false : true;
    }

    /**
    @brief	フォグを設定
    @param	device	[in]	使用されてるデバイス
    @param	color	[in]	フォグの色
    @param	start	[in]	フォグの開始距離
    @param	end 	[in]	フォグの終了距離
    @param	mode 	[in]	フォグの種類
    */
    inline void SetFog()
    {
        LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
        if (!bOnScreen)
        {
            pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
            return;
        }

        // フォグ有効
        pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);

        // 情報設定
        pDevice->SetRenderState(D3DRS_FOGCOLOR, Color);
        pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&Start));
        pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&End));
        pDevice->SetRenderState(D3DRS_FOGTABLEMODE, Mode);


        //霧の状態を設定
        pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
        pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
    }

    /**
    @brief	フォグを無効
    */
    inline void DisableFog(LPDIRECT3DDEVICE9 device)
    {
        device->SetRenderState(D3DRS_FOGENABLE, FALSE);
    }
}

#endif