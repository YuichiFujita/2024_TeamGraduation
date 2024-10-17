//=============================================================================
//
// パーティクル処理 [particle.h]
// Author : 相馬靜雅
//
//=============================================================================

#ifndef _PARTICLE_H_		//このマクロ定義がされていなかったら
#define _PARTICLE_H_		//二重インクルード防止のマクロを定義する

//==========================================================================
// クラス定義
//==========================================================================
// パーティクルクラス定義
class CParticle
{
public:

	// 列挙型定義
	typedef enum
	{
		TYPE_NONE = 0,		// 煙のパーティクル
		TYPE_SMOKE,			// 煙のパーティクル
		TYPE_SMOKE_RED,		// 煙のパーティクル赤
		TYPE_SMOKE_YEL,		// 煙のパーティクル黄
		TYPE_EXPLOSION,		// 爆発のパーティクル
		TYPE_MANYTXT_MINI,	// 大量発生のUI(ミニ)
		TYPE_RESULTMESSAGE,	// リザルトメッセージ
		TYPE_OFFSETTING,	// 弾打ち消し
		TYPE_SUPERATTACK,	// 超攻撃
		TYPE_ICE_SPAWN,		// 氷出現
		TYPE_CHECK,			// 確認用
		TYPE_CANCELABLE,	// キャンセル可能
		TYPE_MAX
	}TYPE;

	CParticle(int nPriority = 1);
	~CParticle();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Create(D3DXVECTOR3 pos, TYPE nType);

	static int GetNumAll(void);

private:
	void Smoke(void);
	void SmokeRed(void);
	void SmokeYel(void);
	void ManyUI_Mini(void);
	void ResultMessage(void);
	void OffSetting(void);
	void SuperAttack(void);
	void IceSpawn(void);
	void Check(void);
	void Cancelable(void);

	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_move;		// 移動量
	D3DXCOLOR m_col;		// 色
	float m_fRadius;		// 半径
	float m_fAngle;			// 向き
	float m_fMove;			// 移動量
	int m_nLife;			// 寿命
	TYPE m_nType;			// 種類
	static int m_nNumAll;	// 総数
};

#endif