//=============================================================================
// 
//  判定ゾーンマネージャ [judgezoneManager.cpp]
//  Author : 石原颯馬
// 
//============================================================================
#include "judgezoneManager.h"
#include "judgezone.h"
#include "judge.h"
#include "judgeobj.h"
#include "game.h"
#include "course.h"
#include "texture.h"
#include "object2D.h"
#include "spline.h"
#include "objectBillboard.h"
#include <map>
#include <direct.h>

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	std::map<CJudge::JUDGE, std::string> TEXTURE =
	{
		{CJudge::JUDGE::JUDGE_AAA,"data\\TEXTURE\\judge_test_01.png"},
		{CJudge::JUDGE::JUDGE_BBB,"data\\TEXTURE\\judge_test_02.png"},
		{CJudge::JUDGE::JUDGE_CCC,"data\\TEXTURE\\judge_test_03.png"},
		{CJudge::JUDGE::JUDGE_DDD,"data\\TEXTURE\\judge_test_04.png"},
	};
	const std::string TEXT_LINE = "#------------------------------------------------------------------------------";	// テキストのライン
	const int ADDPOINT[] =
	{
		15,	// AAA
		5,	// BBB
		-5,	// CCC
		-10,// DDD
	};
}
CJudgeZoneManager* CJudgeZoneManager::m_ThisPtr = nullptr;

//==========================================================================
// コンストラクタ
//==========================================================================
CJudgeZoneManager::CJudgeZoneManager()
{
	
}

//==========================================================================
// デストラクタ
//==========================================================================
CJudgeZoneManager::~CJudgeZoneManager()
{
	
}

//==========================================================================
// 生成処理
//==========================================================================
CJudgeZoneManager *CJudgeZoneManager::Create()
{
	if (m_ThisPtr == nullptr)
	{
		// メモリの確保
		m_ThisPtr = DEBUG_NEW CJudgeZoneManager;

		if (m_ThisPtr != nullptr)
		{
			// 初期化処理
			m_ThisPtr->Init();
		}
	}

	return m_ThisPtr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CJudgeZoneManager::Init()
{
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CJudgeZoneManager::Uninit()
{
	ReleaseAll();
	delete m_ThisPtr;
	m_ThisPtr = nullptr;
}

//==========================================================================
// 判定確認処理
//==========================================================================
void CJudgeZoneManager::Check(float progress)
{
	CListManager<CJudgeZone> list = CJudgeZone::GetListObj();
	CListManager<CJudgeZone>::Iterator itr = list.GetEnd();
	while (list.ListLoop(itr))
	{
		if ((*itr)->IsEnable())
		{
			CJudgeZone::SJudgeZone zone = (*itr)->GetZone();
			if (progress >= zone.start && progress <= zone.end)
			{//範囲内
				(*itr)->Check();
			}
			else if (progress > zone.end)
			{//終了（判定）
				CJudge::JUDGE judge = (*itr)->Judge();	//ここに判定が入ってる

				// ポイント加算
				CGame::GetInstance()->GetGameManager()->AddEvaluationPoint(ADDPOINT[judge]);

				// 評価ごとのUI生成
				CJudgeObj* pObj	= CJudgeObj::Create(MyLib::Vector3(400.0f, 100.0f, 0.0f), judge);
				pObj->SetSize(D3DXVECTOR2(256.0f, 72.0f));
				pObj->SetSizeOrigin(D3DXVECTOR2(192.0f, 54.0f));

				(*itr)->Uninit();
			}

#ifdef _DEBUG
			CCourse* pCource = CGame::GetInstance()->GetCourse();
			MyLib::Vector3 pos;
			float length = pCource->GetCourceLength();

			//スタート
			pos = MySpline::GetSplinePosition_NonLoop(pCource->GetVecPosition(), length * (*itr)->GetZone().start);
			pos.y = (*itr)->GetZone().borderHeight;
			CEffect3D::Create(
				pos,
				MyLib::Vector3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
				40.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

			//終了
			pos = MySpline::GetSplinePosition_NonLoop(pCource->GetVecPosition(), length * (*itr)->GetZone().end);
			pos.y = (*itr)->GetZone().borderHeight;
			CEffect3D::Create(
				pos,
				MyLib::Vector3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
				40.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
#endif // _DEBUG
		}
	}

#ifdef _DEBUG
	// 表示用ボタン
	if (ImGui::Button("DispObj_AAA"))
	{
		CJudgeObj* pObj = CJudgeObj::Create(MyLib::Vector3(400.0f, 100.0f, 0.0f), CJudge::JUDGE::JUDGE_AAA);
		pObj->SetSize(D3DXVECTOR2(256.0f, 72.0f));
		pObj->SetSizeOrigin(D3DXVECTOR2(192.0f, 54.0f));

		// ポイント加算
		CGame::GetInstance()->GetGameManager()->AddEvaluationPoint(ADDPOINT[CJudge::JUDGE::JUDGE_AAA]);
	}
	if (ImGui::Button("DispObj_BBB"))
	{
		CJudgeObj* pObj = CJudgeObj::Create(MyLib::Vector3(400.0f, 100.0f, 0.0f), CJudge::JUDGE::JUDGE_BBB);
		pObj->SetSize(D3DXVECTOR2(256.0f, 72.0f));
		pObj->SetSizeOrigin(D3DXVECTOR2(192.0f, 54.0f));

		// ポイント加算
		CGame::GetInstance()->GetGameManager()->AddEvaluationPoint(ADDPOINT[CJudge::JUDGE::JUDGE_BBB]);
	}
	if (ImGui::Button("DispObj_CCC"))
	{
		CJudgeObj* pObj = CJudgeObj::Create(MyLib::Vector3(400.0f, 100.0f, 0.0f), CJudge::JUDGE::JUDGE_CCC);
		pObj->SetSize(D3DXVECTOR2(256.0f, 72.0f));
		pObj->SetSizeOrigin(D3DXVECTOR2(192.0f, 54.0f));

		// ポイント加算
		CGame::GetInstance()->GetGameManager()->AddEvaluationPoint(ADDPOINT[CJudge::JUDGE::JUDGE_CCC]);
	}
	if (ImGui::Button("DispObj_DDD"))
	{
		CJudgeObj* pObj = CJudgeObj::Create(MyLib::Vector3(400.0f, 100.0f, 0.0f), CJudge::JUDGE::JUDGE_DDD);
		pObj->SetSize(D3DXVECTOR2(256.0f, 72.0f));
		pObj->SetSizeOrigin(D3DXVECTOR2(192.0f, 54.0f));

		// ポイント加算
		CGame::GetInstance()->GetGameManager()->AddEvaluationPoint(ADDPOINT[CJudge::JUDGE::JUDGE_DDD]);
	}
#endif
	
	// 無効化したゾーン破棄
	Release();
}

//==========================================================================
// 無効ゾーン破棄処理
//==========================================================================
void CJudgeZoneManager::Release()
{
	CListManager<CJudgeZone> list = CJudgeZone::GetListObj();
	std::list<CJudgeZone*> removeList;
	std::list<CJudgeZone*>::iterator itr;
	itr = list.GetEnd();
	CJudgeZone* pObj = nullptr;
	while (list.ListLoop(itr))
	{
		if (!(*itr)->IsEnable())
		{
			removeList.push_back((*itr));
		}
	}
	
	for (itr = removeList.begin(); itr != removeList.end(); itr++)
	{
		(*itr)->Uninit();
		list.Delete((*itr));
	}
}

//==========================================================================
// 全ゾーン破棄処理
//==========================================================================
void CJudgeZoneManager::ReleaseAll()
{
	CListManager<CJudgeZone> list = CJudgeZone::GetListObj();
	CJudgeZone* pZone = nullptr;
	while (list.ListLoop(&pZone))
	{
		pZone->Uninit();			// 終了
	}
}

//==========================================================================
// 判定ゾーン類読み込み処理
//==========================================================================
void CJudgeZoneManager::Load(std::string path)
{
	CJudgeZone* pJudgeZone = nullptr;
	std::string aPath[2];

	// ファイルを開く
	std::ifstream File(path);
	if (File.is_open()) {
		// コメント用
		std::string hoge;

		// データ読み込み
		std::string line;
		while (std::getline(File, line))
		{
			// コメントはスキップ
			if (line.empty() ||
				line[0] == '#')
			{
				continue;
			}

			if (line.find("USEZONE") != std::string::npos)
			{// TYPEで配置物の種類

				// ストリーム作成
				std::istringstream lineStream(line);

				// 情報渡す
				std::string path;
				lineStream >>
					hoge >>
					hoge >>		// ＝
					path;		// 数値
				path = UtilFunc::Transformation::ReplaceBackslash(path);

				//判定ゾーン読み込み
				LoadZone(path);
			}

			if (line.find("END_SCRIPT") != std::string::npos)
			{
				break;
			}
		}

		// ファイルを閉じる
		File.close();
	}
}

//==========================================================================
// 判定ゾーン読み込み処理
//==========================================================================
void CJudgeZoneManager::LoadZone(std::string path)
{
	CJudgeZone* pJudgeZone = nullptr;
	std::string aPath[2];

	// ファイルを開く
	std::ifstream File(path);
	if (File.is_open()) {
		// コメント用
		std::string hoge;

		// データ読み込み
		std::string line;
		while (std::getline(File, line))
		{
			// コメントはスキップ
			if (line.empty() ||
				line[0] == '#')
			{
				continue;
			}

			if (line.find("ZONESET") != std::string::npos)
			{// MODELSETで配置情報読み込み

				// 読み込み情報
				CJudgeZone::SJudgeZone zone = CJudgeZone::SJudgeZone(0.0f, 1.0f, 0.0f);
				float border = 0.0f;

				while (line.find("END_ZONESET") == std::string::npos)
				{
					std::getline(File, line);
					if (line.find("START") != std::string::npos)
					{// TYPEで配置物の種類

						// ストリーム作成
						std::istringstream lineStream(line);

						// 情報渡す
						lineStream >>
							hoge >>
							hoge >>		// ＝
							zone.start;	// 数値
					}
					if (line.find("END") != std::string::npos)
					{// TYPEで配置物の種類

						// ストリーム作成
						std::istringstream lineStream(line);

						// 情報渡す
						lineStream >>
							hoge >>
							hoge >>		// ＝
							zone.end;	// 数値
					}
					if (line.find("BORDER") != std::string::npos)
					{// TYPEで配置物の種類

						// ストリーム作成
						std::istringstream lineStream(line);

						// 情報渡す
						lineStream >>
							hoge >>
							hoge >>		// ＝
							border;		// 数値
					}
					else if (line.find("CONDITION_TOP") != std::string::npos)
					{// POSで位置

						// ストリーム作成
						std::istringstream lineStream(line);

						// 情報渡す
						lineStream >>
							hoge >>
							hoge >>		// ＝
							aPath[0];	// パス
						aPath[0] = UtilFunc::Transformation::ReplaceBackslash(aPath[0]);
					}
					else if (line.find("CONDITION_UNDER") != std::string::npos)
					{// ROTで向き

						// ストリーム作成
						std::istringstream lineStream(line);

						// 情報渡す
						lineStream >>
							hoge >>
							hoge >>		// ＝
							aPath[1];	// パス
						aPath[1] = UtilFunc::Transformation::ReplaceBackslash(aPath[1]);
					}
				}

				// 取り込み
				if (pJudgeZone == nullptr)
				{
					pJudgeZone = CJudgeZone::Create(zone.start, zone.end, border);
				}
			}

			if (line.find("END_SCRIPT") != std::string::npos)
			{
				break;
			}
		}

		// ファイルを閉じる
		File.close();
	}

	// 条件読み込み
	if (pJudgeZone != nullptr)
	{
		CListManager<CJudgeZone> list = CJudgeZone::GetListObj();

		pJudgeZone->SetPath(path);
		pJudgeZone->SetInfo(CJudge::BORDER::TOP, LoadCondition(aPath[CJudge::BORDER::TOP]));
		pJudgeZone->SetInfo(CJudge::BORDER::UNDER, LoadCondition(aPath[CJudge::BORDER::UNDER]));
		list.Regist(pJudgeZone);
	}
}

//==========================================================================
// 条件読み込み処理
//==========================================================================
CJudge::SJudgeCondition CJudgeZoneManager::LoadCondition(std::string path)
{
	CJudge::SJudgeCondition info;
	info.type = CJudge::CONDITIONTYPE::TYPE_NONE;
	info.judgeParam = 
	{
		{CJudge::JUDGE::JUDGE_AAA,-1},
		{CJudge::JUDGE::JUDGE_BBB,-1},
		{CJudge::JUDGE::JUDGE_CCC,-1},
		{CJudge::JUDGE::JUDGE_DDD,-1},
	};

	// ファイルを開く
	std::ifstream File(path);
	if (File.is_open()) {
		// コメント用
		std::string hoge;

		// データ読み込み
		std::string line;
		while (std::getline(File, line))
		{
			// コメントはスキップ
			if (line.empty() ||
				line[0] == '#')
			{
				continue;
			}

			if (line.find("CONDITIONSET") != std::string::npos)
			{// MODELSETで配置情報読み込み
				while (line.find("END_CONDITIONSET") == std::string::npos)
				{
					std::getline(File, line);
					if (line.find("TYPE") != std::string::npos)
					{// TYPEで配置物の種類
						int num = 0;
						
						// ストリーム作成
						std::istringstream lineStream(line);

						// 情報渡す
						lineStream >>
							hoge >>
							hoge >>		// ＝
							num;		// 数値

						info.type = static_cast<CJudge::CONDITIONTYPE>(num);
					}
					if (line.find("JUDGE_AAA") != std::string::npos)
					{// TYPEで配置物の種類

						// ストリーム作成
						std::istringstream lineStream(line);

						// 情報渡す
						lineStream >>
							hoge >>
							hoge >>		// ＝
							info.judgeParam[CJudge::JUDGE::JUDGE_AAA];	// 数値
					}
					if (line.find("JUDGE_BBB") != std::string::npos)
					{// TYPEで配置物の種類

						// ストリーム作成
						std::istringstream lineStream(line);

						// 情報渡す
						lineStream >>
							hoge >>
							hoge >>		// ＝
							info.judgeParam[CJudge::JUDGE::JUDGE_BBB];	// 数値
					}
					if (line.find("JUDGE_CCC") != std::string::npos)
					{// TYPEで配置物の種類

						// ストリーム作成
						std::istringstream lineStream(line);

						// 情報渡す
						lineStream >>
							hoge >>
							hoge >>		// ＝
							info.judgeParam[CJudge::JUDGE::JUDGE_CCC];	// 数値
					}
					if (line.find("JUDGE_DDD") != std::string::npos)
					{// TYPEで配置物の種類

						// ストリーム作成
						std::istringstream lineStream(line);

						// 情報渡す
						lineStream >>
							hoge >>
							hoge >>		// ＝
							info.judgeParam[CJudge::JUDGE::JUDGE_DDD];	// 数値
					}
				}
			}

			if (line.find("END_SCRIPT") != std::string::npos)
			{
				break;
			}
		}

		// ファイルを閉じる
		File.close();
	}
	return info;
}

//==========================================================================
// 判定ゾーンリスト書き込み処理
//==========================================================================
void CJudgeZoneManager::Save(std::string path)
{
	// ファイルを開く
	std::ofstream File(path, std::ios_base::out);
	if (!File.is_open()) {
		return;
	}

	int path_Length = path.find_last_of("\\");
	int name_Length = path.size() - path.find_last_of("\\");
	std::string fileName_ext = path.substr(path_Length + 1, name_Length + 1);
	std::string filename = fileName_ext.substr(0, fileName_ext.find_last_of("."));
	File << "#====================================================================================================" << std::endl;
	File << "#" << std::endl;
	File << "# 判定ゾーンリストスクリプトファイル [" << fileName_ext << "]" << std::endl;
	File << "# Author : エディタ書き出し" << std::endl;
	File << "#" << std::endl;
	File << "#====================================================================================================" << std::endl;
	File << "SCRIPT		# この行は絶対消さないこと！\n" << std::endl;
	File << std::endl;
	File << TEXT_LINE << std::endl;
	File << "# 判定ゾーンパス情報" << std::endl;
	File << "# USEZONE	: 使用する判定ゾーンのファイルパス" << std::endl;
	File << TEXT_LINE << std::endl;

	CListManager<CJudgeZone> list = CJudgeZone::GetListObj();
	auto itr = list.GetEnd();
	while(list.ListLoop(itr))
	{
		// ゾーン単体のパス書き出し
		File << "USEZONE = " << (*itr)->GetPath() << std::endl;
	}

	// ファイルを閉じる
	File << "\nEND_SCRIPT\t\t# この行は絶対消さないこと！" << std::endl;
	File.close();
}

//==========================================================================
// 判定ゾーン書き込み処理
//==========================================================================
void CJudgeZoneManager::SaveZone(std::string path, CJudgeZone::SJudgeZone zoneinfo, CJudge::SJudgeCondition conditionUp, CJudge::SJudgeCondition conditionUnder)
{
	// ゾーンファイルパス分解
	int path_Length = path.find_last_of("\\");
	int name_Length = path.size() - path.find_last_of("\\");
	std::string dirName = path.substr(0, path_Length + 1);
	std::string fileName_ext = path.substr(path_Length + 1, name_Length + 1);
	std::string filename = fileName_ext.substr(0, fileName_ext.find_last_of("."));

	// 条件ファイルパス作成
	std::string conditionPath_base = "data\\TEXT\\judgecondition\\" + filename;
	std::string pathConditionUp, pathConditionUnder;
	pathConditionUp = conditionPath_base + "_condition_Top.txt";
	pathConditionUnder = conditionPath_base + "_condition_Under.txt";

	// 条件ファイル書き出し
	_mkdir("data\\TEXT\\judgecondition");
	if (!SaveCondition(pathConditionUp, conditionUp))
	{
		pathConditionUp = "<<< FAILED >>>";
	}
	if (!SaveCondition(pathConditionUnder, conditionUnder))
	{
		pathConditionUnder = "<<< FAILED >>>";
	}

	// ファイルを開く
	std::ofstream File(path, std::ios_base::out);
	if (!File.is_open()) {
		return;
	}

	File << "#====================================================================================================" << std::endl;
	File << "#" << std::endl;
	File << "# 判定ゾーンスクリプトファイル [" << fileName_ext << "]" << std::endl;
	File << "# Author : エディタ書き出し" << std::endl;
	File << "#" << std::endl;
	File << "#====================================================================================================" << std::endl;
	File << "SCRIPT		# この行は絶対消さないこと！\n" << std::endl;
	File << std::endl;
	File << TEXT_LINE << std::endl;
	File << "# 判定ゾーン情報" << std::endl;
	File << "# START				: 判定開始地点（進行度割合表記, ここでどっちの条件にするか決定）" << std::endl;
	File << "# END : 判定終了地点（進行度割合表記, ここで判定が出る）" << std::endl;
	File << "# BORDER : 上と下の境界線の高さ" << std::endl;
	File << "# CONDITION_TOP : 上の条件ファイルパス" << std::endl;
	File << "# CONDITION_UNDER : 下の条件ファイルパス" << std::endl;
	File << TEXT_LINE << std::endl;

	{
		// ゾーン情報設定
		File << "ZONESET" << std::endl;

		// 変数類
		File << "START = " << zoneinfo.start << std::endl;
		File << "END = " << zoneinfo.end << std::endl;
		File << "BORDER = " << zoneinfo.borderHeight << std::endl;

		// 条件ファイルパス
		File << "CONDITION_TOP = " << pathConditionUp << std::endl;
		File << "CONDITION_UNDER = " << pathConditionUnder << std::endl;

		// 終わり
		File << "END_ZONESET" << std::endl;
	}

	// ファイルを閉じる
	File << "\nEND_SCRIPT\t\t# この行は絶対消さないこと！" << std::endl;
	File.close();
}

//==========================================================================
// 判定条件書き込み処理
//==========================================================================
bool CJudgeZoneManager::SaveCondition(std::string path, CJudge::SJudgeCondition condition)
{
	// ファイルを開く
	std::ofstream File(path, std::ios_base::out);
	if (!File.is_open()) {
		return false;
	}

	int path_Length = path.find_last_of("\\");
	int name_Length = path.size() - path.find_last_of("\\");
	std::string fileName_ext = path.substr(path_Length + 1, name_Length + 1);
	std::string filename = fileName_ext.substr(0, fileName_ext.find_last_of("."));
	File << "#====================================================================================================" << std::endl;
	File << "#" << std::endl;
	File << "# 判定条件スクリプトファイル [" << fileName_ext << "]" << std::endl;
	File << "# Author : エディタ書き出し" << std::endl;
	File << "#" << std::endl;
	File << "#====================================================================================================" << std::endl;
	File << "SCRIPT		# この行は絶対消さないこと！\n" << std::endl;
	File << std::endl;
	File << TEXT_LINE << std::endl;
	File << "# 判定ゾーン情報" << std::endl;
	File << "# TYPE		: 種類（0: 無条件, 1 : ぶつかった回数）" << std::endl;
	File << "# JUDGE_xxx(AAA/BBB/CCC/DDD) : 条件を満たす数値（ -1以下で判定を使用しない）" << std::endl;
	File << "#	TYPE = 0 : 何かしらの数値が入っている判定が適用。複数ある場合数値が大きい方が適用）" << std::endl;
	File << "#	TYPE = 1 : ぶつかった回数（指定した数値以下でその判定になる）" << std::endl;
	File << TEXT_LINE << std::endl;

	{
		// ゾーン情報設定
		File << "CONDITIONSET" << std::endl;

		// 種類
		File << "TYPE = " << static_cast<int>(condition.type) << std::endl;

		// 条件回数
		File << "JUDGE_AAA = " << condition.judgeParam[CJudge::JUDGE::JUDGE_AAA] << std::endl;
		File << "JUDGE_BBB = " << condition.judgeParam[CJudge::JUDGE::JUDGE_BBB] << std::endl;
		File << "JUDGE_CCC = " << condition.judgeParam[CJudge::JUDGE::JUDGE_CCC] << std::endl;
		File << "JUDGE_DDD = " << condition.judgeParam[CJudge::JUDGE::JUDGE_DDD] << std::endl;

		// 終わり
		File << "END_CONDITIONSET" << std::endl;
	}

	// ファイルを閉じる
	File << "\nEND_SCRIPT\t\t# この行は絶対消さないこと！" << std::endl;
	File.close();

	return true;
}

