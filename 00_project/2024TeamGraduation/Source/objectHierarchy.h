//=============================================================================
// 
//  階層オブジェクトヘッダー [objectHierarchy.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _OBJECTHIERARCHY_H_
#define _OBJECTHIERARCHY_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include "object.h"
#include "characterStatus.h"
#include "BallStatus.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CModel;

//==========================================================================
// クラス定義
//==========================================================================
// 階層オブジェクトクラス定義
class CObjectHierarchy : public CObject
{
public:

	//=============================
	// 構造体定義
	//=============================
	// 読み込む子データ
	struct LoadData
	{
		std::string pModelFile;	// モデルファイル名
		int nType;				// モデル種類
		int nParent;			// 親のインデックス
		int nStart;				// 最初からあるかどうか
		int nSwitchType;		// 切り替えの種類
		int nIDSwitchModel;		// 切り替えモデルのID
		MyLib::Vector3 pos;		// 位置
		MyLib::Vector3 rot;		// 向き
	};

	// 読み込む親データ
	struct Load
	{
		std::string sTextFile;						// テキストファイル名
		int nCenterIdx;								// 中心にするパーツインデックス
		MyLib::Vector3 centerOffSet;				// 中心位置のオフセット
		int nNumModel;								// モデル数
		MyLib::Vector3 posOrigin;					// 最初の位置
		float scale;								// スケール
		CCharacterStatus::CharParameter parameter;	// パラメーター
		CBallStatus::SBallParameter parameterBall;	// パラメーター(ボール)
		std::vector<LoadData> LoadData;	// 読み込む子データ

		// コンストラクタ
		Load() : scale(1.0f) {}
	};

	// 列挙型定義
	enum EState
	{
		STATE_NONE = 0,	// なにもない
		STATE_MAX
	};

	CObjectHierarchy(int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CObjectHierarchy();

	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	virtual void Draw(D3DXCOLOR col);
	virtual void Draw(float fAlpha);
	virtual void Kill() override;	// 動的削除処理

	void SetmtxWorld(const MyLib::Matrix mtxWorld);
	MyLib::Matrix GetWorldMtx() const { return m_mtxWorld; }			// マトリックス取得
	MyLib::Matrix GetmtxWorld() const;			// ワールドマトリックス取得
	MyLib::Vector3 GetCenterPosition() const;			// 中心の位置取得
	void SetOriginPosition(const MyLib::Vector3& pos) { m_posOrigin = pos; }	// 最初の位置設定
	MyLib::Vector3 GetOriginPosition() const { return m_posOrigin; }			// 最初の位置取得
	void SetRadius(const float fRadius) { m_fRadius = fRadius; }	// 半径設定
	float GetRadius() const { return m_fRadius; }					// 半径取得
	void SetScale(const float scale) { m_fScale = scale; }			// スケール設定
	float GetScale() { return m_fScale; }							// スケール取得
	void CalWorldMtx();		// ワールドマトリックスの計算処理

	HRESULT ReadText(const std::string& file);	// 外部ファイル読み込み処理
	virtual HRESULT SetCharacter(const std::string& file);	// キャラクター設定

	void ChangeObject(int nSwitchIdx, const std::string& file);		// 切り替えの種類
	void SetObject(int nNewParts);			// モデルの設定
	void DeleteObject(int nDeleteIdx);		// モデル削除

	int GetNumModel();					// モデル数取得
	void SetNumModel(int nNumModel);		// モデル数設定
	int GetIdxFile();					// ファイルのインデックス番号取得
	Load GetLoadData(int nIdx);				// 読み込み情報取得
	CModel** GetModel();				// モデル取得
	CModel* GetModel(int idx);			// モデル取得
	CObjectHierarchy* GetObjectHierarchy();	// 階層オブジェクト取得
	static CObjectHierarchy *Create(const std::string& pTextFile);

protected:

	virtual void LoadObjectData(FILE* pFile, const std::string& file);	// オブジェクト毎のデータ読み込み
	virtual void LoadPartsData(FILE* pFile, const std::string& file, int *pCntParts);	// パーツ毎のデータ読み込み
	virtual void BindObjectData(int nCntData);	// オブジェクト毎のデータ割り当て

	std::vector<CModel*> m_apModel;		// モデル(パーツ)のポインタ
	int m_nIdxFile;					// ファイルのインデックス番号
	static std::vector<Load> m_aLoadData;
	static int m_nNumLoad;	// 読み込んだ数
private:

	//=============================
	// メンバ関数
	//=============================
	void AdjustPositionByScale();	// スケールをもとに位置調整

	//=============================
	// メンバ変数
	//=============================
	MyLib::Matrix m_mtxWorld;	// ワールドマトリックス
	MyLib::Vector3 m_posOrigin;	// 最初の位置
	MyLib::Vector3 m_posInit;	// 初期の位置
	MyLib::Vector3 m_posCenter;	// 中心位置
	float m_fScale;				// スケール

	int m_nCenterPartsIdx;			// 中心にするパーツのインデックス
	MyLib::Vector3 m_CenterOffset;	// 中心のオフセット
	float m_fRadius;				// 半径
	int m_nNumModel;				// モデルの数

};



#endif