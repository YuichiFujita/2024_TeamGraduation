//=============================================================================
// 
// �t�H�O [fog.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _FOG_H_
#define _FOG_H_		// ��d�C���N���[�h�h�~

#include "manager.h"
#include "renderer.h"

/**
	@brief	�t�H�O�̋@�\
*/
namespace MyFog
{

    extern D3DXCOLOR Color;
    extern float Start;
    extern float End;
    extern D3DFOGMODE Mode;
    extern bool bOnScreen;

    /**
    @brief	�t�H�O�̃p�����[�^��ݒ�
    @param	color	[in]	�t�H�O�̐F
    @param	start	[in]	�t�H�O�̊J�n����
    @param	end 	[in]	�t�H�O�̏I������
    @param	mode 	[in]	�t�H�O�̎��
    */
    inline void SetFogparam(const D3DXCOLOR& color, float start, float end, D3DFOGMODE mode)
    {
        Color = color;
        Start = start;
        End = end;
        Mode = mode;
    }

    /**
    @brief	�t�H�O�̃t���O�؂�ւ�
    @param	onscreen	[in]	�t�H�O�̃t���O
    */
    inline void ToggleFogFrag(bool onscreen)
    {
        bOnScreen = onscreen;
    }

    /**
    @brief	�t�H�O�̃t���O�؂�ւ�
    */
    inline void ToggleFogFrag()
    {
        bOnScreen = bOnScreen ? false : true;
    }

    /**
    @brief	�t�H�O��ݒ�
    @param	device	[in]	�g�p����Ă�f�o�C�X
    @param	color	[in]	�t�H�O�̐F
    @param	start	[in]	�t�H�O�̊J�n����
    @param	end 	[in]	�t�H�O�̏I������
    @param	mode 	[in]	�t�H�O�̎��
    */
    inline void SetFog()
    {
        LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
        if (!bOnScreen)
        {
            pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
            return;
        }

        // �t�H�O�L��
        pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);

        // ���ݒ�
        pDevice->SetRenderState(D3DRS_FOGCOLOR, Color);
        pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&Start));
        pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&End));
        pDevice->SetRenderState(D3DRS_FOGTABLEMODE, Mode);


        //���̏�Ԃ�ݒ�
        pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
        pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
    }

    /**
    @brief	�t�H�O�𖳌�
    */
    inline void DisableFog(LPDIRECT3DDEVICE9 device)
    {
        device->SetRenderState(D3DRS_FOGENABLE, FALSE);
    }
}

#endif