//=============================================================================
// 
//  水紋ヘッダー [waterripple.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _WATERRIPPLE_H_
#define _WATERRIPPLE_H_	// 二重インクルード防止

#include "object3DMesh.h"

//==========================================================================
// クラス定義
//==========================================================================
// 水紋クラス
class CWaterRipple : public CObject3DMesh
{

public:

	struct Info
	{
		float height;		// 高さ
		float velocity;		// 速度
		float length;		// 長さ
		float thickness;	// 波の太さ
		int life;			// 寿命
		int maxLife;		// 最大寿命

		Info() : height(0.0f), velocity(0.0f), length(0.0f), thickness(0.0f), life(0), maxLife(0) {}
		Info(float _height, float _velocity, float _length, float _thickness, int _life, int _maxLife) :
			height(_height), velocity(_velocity), length(_length), thickness(_thickness), life(_life), maxLife(_maxLife) {}
	};

	CWaterRipple(const int block, const float blockSize, int nPriority = 3, const LAYER layer = LAYER::LAYER_MAP);
	~CWaterRipple();

	// オーバーライドされた関数
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
	virtual void SetVtx();

	static CWaterRipple *Create(
		const int block, const float blockSize, 
		const MyLib::Vector3& pos, float height, float velocity, float thickness, int life);	// 生成

private:

	// メンバ関数
	void SetVtxPosition();	// 頂点座標

	Info m_Info;
	const int m_Block;	// 分割
	const float m_BlockSize;	// ブロックサイズ
};



#endif