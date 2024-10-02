//============================================================
//
//	弾ヘッダー [bullet.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _BULLET_H_
#define _BULLET_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object3D.h"

//************************************************************
//	クラス定義
//************************************************************
// 弾クラス
class CBullet : public CObject3D
{
public:
	// コンストラクタ
	CBullet();

	// デストラクタ
	~CBullet();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader *pShader = nullptr) override;	// 描画

	// 静的メンバ関数
	static CBullet *Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rMove);	// 生成
	static CListManager<CBullet> *GetList(void);	// リスト取得

	// メンバ関数
	D3DXVECTOR3 GetMove(void) const { return m_move; }	// 移動量取得

private:
	// 静的メンバ変数
	static CListManager<CBullet> *m_pList;	// オブジェクトリスト

	// メンバ変数
	CListManager<CBullet>::AIterator m_iterator;	// イテレーター
	D3DXVECTOR3 m_move;	// 移動量
	float m_fCurTime;	// 経過時間
};

#endif	// _BULLET_H_
