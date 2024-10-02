//============================================================
//
//	プレイヤーヘッダー [player.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object3D.h"

//************************************************************
//	前方宣言
//************************************************************
class CGun;	// 銃クラス

//************************************************************
//	クラス定義
//************************************************************
// プレイヤークラス
class CPlayer : public CObject3D
{
public:
	// コンストラクタ
	CPlayer();

	// デストラクタ
	~CPlayer();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader *pShader = nullptr) override;	// 描画

	// 静的メンバ関数
	static CPlayer *Create(void);	// 生成
	static CListManager<CPlayer> *GetList(void);	// リスト取得

	// メンバ関数
	void MoveGunRecoil(const float fRecScalar);	// 反動移動
	D3DXVECTOR3 GetMove(void) const { return m_move; }	// 移動量取得
	D3DXVECTOR3 GetGunRotation(void) const { return m_rotGun; }	// 銃の向き取得

private:
	// 静的メンバ変数
	static CListManager<CPlayer> *m_pList;	// オブジェクトリスト

	// メンバ変数
	CListManager<CPlayer>::AIterator m_iterator;	// イテレーター
	CGun* m_pGun;			// 銃の情報
	D3DXVECTOR3 m_rotGun;	// 銃の向き
	D3DXVECTOR3 m_move;		// 移動量
};

#endif	// _PLAYER_H_
