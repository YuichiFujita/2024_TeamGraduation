//=============================================================================
// 
//  瓦礫ヘッダー [ballast.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _BALLAST_H_
#define _BALLAST_H_	// 二重インクルード防止

#include "main.h"
#include "object.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CObjectX;

//==========================================================================
// マクロ定義
//==========================================================================
#define MAX_BALLAST	(64)	// 瓦礫の数
#define DEFAULTMOVE_BALLAST	(MyLib::Vector3(2.0f, 12.0f, 2.0f))

//==========================================================================
// クラス定義
//==========================================================================
// 瓦礫クラス定義
class CBallast : public CObject
{
public:

	// 列挙型定義
	typedef enum
	{
		TYPE_STONE = 0,	// 石
		TYPE_ICE,		// 氷
		TYPE_MAX
	}TYPE;

	CBallast(int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CBallast();

	static CBallast *Create(MyLib::Vector3 pos, MyLib::Vector3 move, int nNum = 10, float scale = 1.0f, float fAlpha = 1.0f, TYPE type = TYPE_STONE);

	// オーバーライドされた関数
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

private:

	MyLib::Vector3 m_posOrigin;		// 原点
	MyLib::Vector3 m_moveOrigin;		// 元の移動量
	D3DXCOLOR m_col;				// 色
	D3DXCOLOR m_colOrigin;			// 原色
	TYPE m_type;					// 種類
	int m_nNumAll;					// 総数
	int m_nCntParabola;				// 瓦礫の移動カウント
	int m_nLife;					// 寿命
	int m_nMaxLife;					// 最大寿命
	float m_fScale;
	static const char *m_pFileName[TYPE_MAX];	// ファイル名
	CObjectX *m_pObjX[MAX_BALLAST];	// オブジェクトXのポインタ
	static int m_nIdxXFile[TYPE_MAX];			// Xファイルのインデックス番号
};


#endif