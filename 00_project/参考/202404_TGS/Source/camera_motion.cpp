//=============================================================================
// 
// カメラモーション処理 [camera_motion.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "camera_motion.h"
#include "camera.h"
#include "debugproc.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "calculation.h"
#include "particle.h"


//==========================================================================
// マクロ定義
//==========================================================================
namespace
{
	const std::string FILENAME = "data\\TEXT\\camera\\motion_manager.txt";	// 読み込むファイル名
}


//==========================================================================
// 関数ポインタ
//==========================================================================
CCameraMotion::MOTION_FUNC CCameraMotion::m_MotionFunc[] =
{
	&CCameraMotion::MotionPass,		// パス
	&CCameraMotion::MotionGoal,		// ゴール
	&CCameraMotion::MotionGoalBag,	// ゴールフット橋
};

//==========================================================================
// コンストラクタ
//==========================================================================
CCameraMotion::CCameraMotion()
{
	m_vecMotionInfo.clear();	// モーション情報
	m_MotionFileName.clear();	// モーションファイル名
	m_pos = 0.0f;				// 位置
	m_nNowMotionIdx = 0;		// 現在のモーションインデックス
	m_nNowKeyIdx = 0;			// 現在のキーインデックス
	m_nNowTriggerIdx = 0;		// 現在のトリガーインデックス
	m_fMotionTimer = 0.0f;		// モーションタイマー
	m_fTriggerTimer = 0.0f;		// トリガータイマー
	m_bFinish = false;			// 終了判定
	m_bEdit = false;			// エディターフラグ
	m_bPause = false;			// ポーズ判定
	m_bSystemPause = false;		// システムポーズ判定
}

//==========================================================================
// デストラクタ
//==========================================================================
CCameraMotion::~CCameraMotion()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CCameraMotion* CCameraMotion::Create()
{
	// メモリの確保
	CCameraMotion* pObj = DEBUG_NEW CCameraMotion;

	if (pObj == nullptr)
	{
		return nullptr;
	}

	// 初期化処理
	pObj->Init();

	return pObj;
}

//==========================================================================
// カメラの初期化処理
//==========================================================================
HRESULT CCameraMotion::Init()
{
	// 各種変数初期化
	m_bFinish = true;	// 終了判定

	// テキスト読み込み
	LoadText();

	m_EditInfo.motionInfo = m_vecMotionInfo[0];

	for (int i = 0; i < MOTION_MAX; i++)
	{
		m_pCameraMotion_Trigger[i] = CCameraMotion_Trigger::Create(i);
	}

	return S_OK;
}

//==========================================================================
// テキスト読み込み
//==========================================================================
void CCameraMotion::LoadText()
{
	// ファイルを開く
	std::ifstream File(FILENAME);
	if (!File.is_open()) {
		return;
	}

	// コメント用
	std::string hoge;

	// ファイル名
	std::string pathname, filename;

	// モーションファイル名リセット
	m_MotionFileName.clear();
	m_PathName.clear();	// パス名

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

		// ストリーム作成
		std::istringstream lineStream(line);

		if (line.find("PATH") != std::string::npos)
		{// PATHで読み込むパス読み込み

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>		// ＝
				m_PathName;	// パス名

			continue;
		}

		if (line.find("MOTION_FILENAME") != std::string::npos)
		{// MOTION_FILENAMEでモーションファイル読み込み

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>		// ＝
				filename;	// モーションファイル名

			// モーションファイル名追加
			m_MotionFileName.push_back(filename);

			// モーション読み込み
			LoadMotion(filename);

			continue;
		}

		if (line.find("END_SCRIPT") != std::string::npos)
		{
			break;
		}
	}

	// ファイルを閉じる
	File.close();
}

//==========================================================================
// モーション書き込み
//==========================================================================
void CCameraMotion::SaveMotion(const std::string& filename, const MotionInfo& info)
{
	// ファイルを開く
	std::ofstream File((m_PathName + filename), std::ios::binary);
	if (!File.is_open()) {
		return;
	}

	// コンテナ以外のセーブ
	//File.write(reinterpret_cast<const char*>(&info.playTime), sizeof(info.playTime));

	// データの個数を計算
	size_t vecSize = info.Key.size();

	// コンテナのサイズをセーブ
	File.write(reinterpret_cast<const char*>(&vecSize), sizeof(vecSize));

	// コンテナ内の要素をセーブ
	File.write(reinterpret_cast<const char*>(info.Key.data()), vecSize * sizeof(MotionKey));




#if 1
	// トリガーのセーブ
	{
		//MotionInfo_Save info_Save;

		// データの個数を計算
		size_t vecSize = info.trigger.size();

		// コンテナのサイズをセーブ
		File.write(reinterpret_cast<const char*>(&vecSize), sizeof(vecSize));

		// コンテナ内の要素をセーブ
		File.write(reinterpret_cast<const char*>(info.trigger.data()), vecSize * sizeof(float));
	}
#endif





	// ファイルを閉じる
	File.close();
}

//==========================================================================
// モーション読み込み
//==========================================================================
void CCameraMotion::LoadMotion(const std::string& filename)
{
	// ファイルを開く
	std::ifstream File((m_PathName + filename), std::ios::binary);
	if (!File.is_open()) 
	{
		// 例外処理
		m_vecMotionInfo.push_back(MotionInfo());
		m_vecMotionInfo.back().Key.push_back(MotionKey());
		return;
	}

	MotionInfo loadData;	// 読み込みデータ

	// コンテナ以外のロード
	//File.read(reinterpret_cast<char*>(&loadData.playTime), sizeof(loadData.playTime));

	// キーのサイズをロード
	size_t size;
	File.read(reinterpret_cast<char*>(&size), sizeof(size));

	// サイズ分確保
	loadData.Key.resize(size);

	// キーデータロード
	File.read(reinterpret_cast<char*>(loadData.Key.data()), size * sizeof(MotionKey));


	// トリガーのロード
	{
		// トリガーのサイズをロード
		File.read(reinterpret_cast<char*>(&size), sizeof(size));

		// サイズ分確保
		loadData.trigger.resize(size);

		// キーデータロード
		File.read(reinterpret_cast<char*>(loadData.trigger.data()), size * sizeof(float));
	}


	// ファイルを閉じる
	File.close();


	// モーション情報追加
	m_vecMotionInfo.push_back(loadData);
}

//==========================================================================
// カメラの終了処理
//==========================================================================
void CCameraMotion::Uninit()
{
	for (int i = 0; i < MOTION_MAX; i++)
	{
		if (m_pCameraMotion_Trigger[i] == nullptr)
		{
			continue;
		}

		delete m_pCameraMotion_Trigger[i];
		m_pCameraMotion_Trigger[i] = nullptr;
	}

	m_vecMotionInfo.clear();
	m_MotionFileName.clear();

	delete this;
}

//==========================================================================
// カメラの更新処理
//==========================================================================
void CCameraMotion::Update()
{
#ifdef _DEBUG
	// エディット更新
	UpdateEdit();
#endif // DEBUG

	// カメラ取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	pCamera->SetEnableMotion(!m_bFinish);

	if (m_bFinish)
	{// モーション中のみ
		return;
	}

	// キーボード情報取得
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();

	// 現在のモーション情報
	MotionInfo nowInfo = m_vecMotionInfo[m_nNowMotionIdx];
	int keySize = static_cast<int>(nowInfo.Key.size());


	// モーションタイマー加算
	if (!m_bPause && !m_bSystemPause)
	{
		m_fMotionTimer += CManager::GetInstance()->GetDeltaTime();
		m_fTriggerTimer += CManager::GetInstance()->GetDeltaTime();
	}
	else
	{
		// トリガー判定リセット
		m_bTrigger = false;
		return;
	}


	//=============================
	// トリガー判定
	//=============================
	{
		// トリガー判定リセット
		m_bTrigger = false;

		if (!m_bTrigger &&
			m_nNowTriggerIdx < static_cast<int>(nowInfo.trigger.size()) &&	// トリガーのサイズ以下
			m_fTriggerTimer >= nowInfo.trigger[m_nNowTriggerIdx])
		{
			// トリガー時処理
			TriggerMoment();
			
			// トリガー判定ON
			m_bTrigger = true;
			m_nNowTriggerIdx++;

			

#if _DEBUG
			my_particle::Create(MyLib::Vector3(640.0f, 360.0f, 0.0f), my_particle::TYPE_OFFSETTING_2D);
#endif

		}
	}

	if (m_fMotionTimer >= nowInfo.Key[m_nNowKeyIdx].playTime)
	{
		// キー更新
		m_fMotionTimer = 0.0f;
		m_nNowKeyIdx = (m_nNowKeyIdx + 1) % keySize;

		if (m_nNowKeyIdx == 0)
		{// 一周

			// 終了判定ON
			m_bFinish = true;
			pCamera->SetEnableMotion(false);

			// トリガーリセット
			m_nNowTriggerIdx = 0;
			m_fTriggerTimer = 0.0f;
			return;
		}
	}


	// 次のキーインデックス
	int nextKeyID = (m_nNowKeyIdx + 1) % keySize;
	if (nextKeyID == 0)
	{// 終端
		nextKeyID = keySize - 1;
	}

	// キー情報
	MotionKey nowKey = nowInfo.Key[m_nNowKeyIdx];
	MotionKey nextKey = nowInfo.Key[nextKeyID];

	// カメラ情報取得
	MyLib::Vector3 posR, rot;
	float distance = pCamera->GetDistance();

	// 線形補正
	switch (m_EasingType)
	{
	case CCameraMotion::Linear:
		posR = UtilFunc::Correction::EasingLinear(nowKey.posRDest, nextKey.posRDest, 0.0f, nowKey.playTime, m_fMotionTimer);
		rot = UtilFunc::Correction::EasingLinear(nowKey.rotDest, nextKey.rotDest, 0.0f, nowKey.playTime, m_fMotionTimer);
		distance = UtilFunc::Correction::EasingLinear(nowKey.distance, nextKey.distance, 0.0f, nowKey.playTime, m_fMotionTimer);
		break;

	case CCameraMotion::EaseIn:
		posR = UtilFunc::Correction::EasingEaseIn(nowKey.posRDest, nextKey.posRDest, 0.0f, nowKey.playTime, m_fMotionTimer);
		rot = UtilFunc::Correction::EasingEaseIn(nowKey.rotDest, nextKey.rotDest, 0.0f, nowKey.playTime, m_fMotionTimer);
		distance = UtilFunc::Correction::EasingEaseIn(nowKey.distance, nextKey.distance, 0.0f, nowKey.playTime, m_fMotionTimer);
		break;

	case CCameraMotion::EaseOut:
		posR = UtilFunc::Correction::EasingEaseOut(nowKey.posRDest, nextKey.posRDest, 0.0f, nowKey.playTime, m_fMotionTimer);
		rot = UtilFunc::Correction::EasingEaseOut(nowKey.rotDest, nextKey.rotDest, 0.0f, nowKey.playTime, m_fMotionTimer);
		distance = UtilFunc::Correction::EasingEaseOut(nowKey.distance, nextKey.distance, 0.0f, nowKey.playTime, m_fMotionTimer);
		break;

	case CCameraMotion::EaseInOut:
		posR = UtilFunc::Correction::EasingEaseInOut(nowKey.posRDest, nextKey.posRDest, 0.0f, nowKey.playTime, m_fMotionTimer);
		rot = UtilFunc::Correction::EasingEaseInOut(nowKey.rotDest, nextKey.rotDest, 0.0f, nowKey.playTime, m_fMotionTimer);
		distance = UtilFunc::Correction::EasingEaseInOut(nowKey.distance, nextKey.distance, 0.0f, nowKey.playTime, m_fMotionTimer);
		break;
	}

	// カメラ情報設定
	pCamera->SetPositionR(m_pos + posR);
	pCamera->SetRotation(rot);
	pCamera->SetDistance(distance);
}

//==========================================================================
// トリガー判定の瞬間
//==========================================================================
void CCameraMotion::TriggerMoment()
{
	// 状態更新
	//(this->*(m_MotionFunc[m_nNowMotionIdx]))();

	if (m_pCameraMotion_Trigger[m_nNowMotionIdx] == nullptr)
	{
		return;
	}

	// トリガーの瞬間
	m_pCameraMotion_Trigger[m_nNowMotionIdx]->TriggerMoment(m_nNowTriggerIdx);
}

//==========================================================================
// パス
//==========================================================================
void CCameraMotion::MotionPass()
{

}

//==========================================================================
// ゴール
//==========================================================================
void CCameraMotion::MotionGoal()
{

}

//==========================================================================
// ゴールフット橋
//==========================================================================
void CCameraMotion::MotionGoalBag()
{

}

//==========================================================================
// エディット更新
//==========================================================================
void CCameraMotion::UpdateEdit()
{
	if (ImGui::CollapsingHeader("CameraMotion"))
	{
		// エディット中判定
		m_bEdit = true;

		// 再生
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::SetNextItemWidth(150.0f);
		if (ImGui::Button("Play / RePlay"))
		{
			SetMotion(m_EditInfo.motionIdx, EASING::Linear);
		}
		ImGui::SameLine();

		ImGui::SetNextItemWidth(150.0f);
		if (ImGui::Button("Reset"))
		{
			m_nNowKeyIdx = 0;
			m_fMotionTimer = 0.0f;
			m_bFinish = true;	// 終了判定
		}


		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		if (ImGui::Button("Save", ImVec2(80, 50)))
		{
			SaveMotion(m_MotionFileName[m_EditInfo.motionIdx], m_EditInfo.motionInfo);
		}
		ImGui::Dummy(ImVec2(0.0f, 5.0f));

		// スライド再生
		SliderPlay();

		// オフセット
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::SetNextItemWidth(150.0f);
		if (ImGui::Button("Offset"))
		{
			m_EditInfo.offset = CManager::GetInstance()->GetCamera()->GetPositionR();
			m_pos = m_EditInfo.offset;
		}
		ImGui::SameLine();
		ImGui::Text("x:%.2f y:%.2f z:%.2f", m_EditInfo.offset.x, m_EditInfo.offset.y, m_EditInfo.offset.z);

		// モーション切り替え
		ChangeMotion();

		// キー切替
		ChangeKey();

		// トリガー編集
		EditTrigger();

	}
	else m_bEdit = false;
}

//==========================================================================
// スライド再生
//==========================================================================
void CCameraMotion::SliderPlay()
{
	m_EditInfo.bSlide = ImGui::TreeNode("SlidePlay");

	if (m_EditInfo.bSlide)
	{
		ImGui::SliderFloat("Play Ratio", &m_EditInfo.playRatio, 0.0f, 1.0f);

		// カメラ情報取得
		CCamera* pCamera = CManager::GetInstance()->GetCamera();
		MyLib::Vector3 posR, rot;
		float distance = pCamera->GetDistance();

		// 現在のモーション情報
		MotionInfo nowInfo = m_EditInfo.motionInfo;

		// 次のキーインデックス
		int keySize = nowInfo.Key.size();
		int nextKeyID = (m_EditInfo.keyIdx + 1) % keySize;
		if (nextKeyID == 0)
		{// 終端
			nextKeyID = keySize - 1;
		}

		// キー情報
		MotionKey nowKey = nowInfo.Key[m_EditInfo.keyIdx];
		MotionKey nextKey = nowInfo.Key[nextKeyID];

		posR = UtilFunc::Correction::EasingLinear(nowKey.posRDest, nextKey.posRDest, m_EditInfo.playRatio);
		rot = UtilFunc::Correction::EasingLinear(nowKey.rotDest, nextKey.rotDest, m_EditInfo.playRatio);
		distance = UtilFunc::Correction::EasingLinear(nowKey.distance, nextKey.distance, m_EditInfo.playRatio);

		// カメラ情報設定
		pCamera->SetPositionR(m_pos + posR);
		pCamera->SetRotation(rot);
		pCamera->SetDistance(distance);

		ImGui::TreePop();
	}
}

//==========================================================================
// モーション切り替え
//==========================================================================
void CCameraMotion::ChangeMotion()
{
	// [グループ]モーション切り替え
	if (ImGui::TreeNode("Change Motion"))
	{
		// [ラジオボタン]モーション切り替え
		for (int i = 0; i < m_MotionFileName.size(); i++)
		{
			if (ImGui::RadioButton(m_MotionFileName[i].c_str(), &m_EditInfo.motionIdx, i))
			{
				// エディット情報切替
				m_EditInfo.motionInfo = m_vecMotionInfo[i];
			}
		}

		//=============================
		// モーションエディット
		//=============================
		EditMotion();

		ImGui::TreePop();
	}
}

//==========================================================================
// キー切り替え
//==========================================================================
void CCameraMotion::ChangeKey()
{
	// リサイズ
	if (ImGui::TreeNode("Key"))
	{
		ImGui::SeparatorText("Change Key");

		//=============================
		// コピー
		//=============================
		if (ImGui::Button("Copy Key"))
		{
			m_EditInfo.Key_copyData = m_vecMotionInfo[m_EditInfo.motionIdx].Key[m_EditInfo.keyIdx];
		}
		ImGui::SameLine();

		//=============================
		// ペースト
		//=============================
		if (ImGui::Button("Paste Key"))
		{
			m_EditInfo.motionInfo.Key[m_EditInfo.keyIdx] = m_EditInfo.Key_copyData;

			// カメラ情報取得
			CCamera* pCamera = CManager::GetInstance()->GetCamera();

			// カメラ情報設定
			pCamera->SetPositionR(m_pos + m_EditInfo.motionInfo.Key[m_EditInfo.keyIdx].posRDest);
			pCamera->SetRotation(m_EditInfo.motionInfo.Key[m_EditInfo.keyIdx].rotDest);
			pCamera->SetDistance(m_EditInfo.motionInfo.Key[m_EditInfo.keyIdx].distance);
		}
		ImGui::Separator();

		//=============================
		// 総数変更
		//=============================
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Change Coolider Num:");
		ImGui::SameLine();
		if (ImGui::ArrowButton("##left", ImGuiDir_Left) &&
			m_EditInfo.motionInfo.Key.size() > 1)
		{
			m_EditInfo.motionInfo.Key.pop_back();
		}
		ImGui::SameLine(0.0f);
		if (ImGui::ArrowButton("##right", ImGuiDir_Right))
		{
			m_EditInfo.motionInfo.Key.push_back(MotionKey());
		}
		ImGui::SameLine();

		// サイズ
		int segmentSize = static_cast<int>(m_EditInfo.motionInfo.Key.size());
		ImGui::Text("%d", segmentSize);



		ImGui::SetNextItemWidth(140.0f);
		if (ImGui::SliderInt("Key Idx", &m_EditInfo.keyIdx, 0, m_EditInfo.motionInfo.Key.size() - 1))
		{
			// 元のデータ
			if (m_vecMotionInfo[m_EditInfo.motionIdx].Key.size() > m_EditInfo.keyIdx)
			{
				m_EditInfo.motionInfo.Key[m_EditInfo.keyIdx] = m_vecMotionInfo[m_EditInfo.motionIdx].Key[m_EditInfo.keyIdx];

				// カメラ情報取得
				CCamera* pCamera = CManager::GetInstance()->GetCamera();\

				// カメラ情報設定
				pCamera->SetPositionR(m_pos + m_EditInfo.motionInfo.Key[m_EditInfo.keyIdx].posRDest);
				pCamera->SetRotation(m_EditInfo.motionInfo.Key[m_EditInfo.keyIdx].rotDest);
				pCamera->SetDistance(m_EditInfo.motionInfo.Key[m_EditInfo.keyIdx].distance);
			}
			else
			{
				m_EditInfo.motionInfo.Key[m_EditInfo.keyIdx] = MotionKey();
			}


		}
		ImGui::Dummy(ImVec2(0.0f, 10.0f));


		//=============================
		// キーエディット
		//=============================
		EditKey();


		ImGui::TreePop();
	}
}

//==========================================================================
// モーションエディット
//==========================================================================
void CCameraMotion::EditMotion()
{
	// カメラ情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// キー情報
	MotionInfo* pInfo = &m_EditInfo.motionInfo;

	if (ImGui::Button("Regist Motion"))
	{
		m_vecMotionInfo[m_EditInfo.motionIdx] = m_EditInfo.motionInfo;
	}
}

//==========================================================================
// キーエディット
//==========================================================================
void CCameraMotion::EditKey()
{
	if (m_EditInfo.bSlide)	// スライド中終了
		return;

	// カメラ情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 rot = pCamera->GetRotation();
	MyLib::Vector3 posR = pCamera->GetPositionR();

	// キー情報
	MotionKey* pKey = &m_EditInfo.motionInfo.Key[m_EditInfo.keyIdx];

	pKey->rotDest = rot;
	pKey->posRDest = posR - m_EditInfo.offset;

	// 位置入力
	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::DragFloat3("posR", (float*)&posR, 1.0f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat3("rot", (float*)&rot, 0.1f, 0.0f, 0.0f, "%.2f");
		pCamera->SetPositionR(posR);
		pCamera->SetRotation(rot);

		pKey->rotDest = rot;
		pKey->posRDest = posR - m_EditInfo.offset;
	}

	if (ImGui::Button("Regist Key"))
	{
		if (m_vecMotionInfo[m_EditInfo.motionIdx].Key.size() <= m_EditInfo.keyIdx)
		{// 編集するキーが元の要素より大きいとき
			m_vecMotionInfo[m_EditInfo.motionIdx].Key.push_back(*pKey);
		}
		else
		{
			m_vecMotionInfo[m_EditInfo.motionIdx].Key[m_EditInfo.keyIdx] = *pKey;
		}
	}

	// 距離
	pKey->distance = pCamera->GetDistance();
	ImGui::Text("distance : %f", pKey->distance);

	// 再生時間
	ImGui::DragFloat("playTime", &pKey->playTime, 0.01f, 0.0f, 0.0f, "%.2f");
}

//==========================================================================
// トリガーエディット
//==========================================================================
void CCameraMotion::EditTrigger()
{
	if (ImGui::TreeNode("Trigger"))
	{
		std::vector<float>* pTrigger = &m_EditInfo.motionInfo.trigger;

		//=============================
		// 総数変更
		//=============================
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Change Trigger Num:");
		ImGui::SameLine();
		if (ImGui::ArrowButton("##left", ImGuiDir_Left) &&
			pTrigger->size() > 1)
		{
			pTrigger->pop_back();

			m_EditInfo.triggerIdx = UtilFunc::Transformation::Clamp(m_EditInfo.triggerIdx, 0, static_cast<int>(pTrigger->size()) - 1);
		}
		ImGui::SameLine(0.0f);
		if (ImGui::ArrowButton("##right", ImGuiDir_Right))
		{
			pTrigger->push_back(0.0f);
		}
		ImGui::SameLine();

		// サイズ
		int segmentSize = static_cast<int>(pTrigger->size());
		ImGui::Text("%d", segmentSize);

		//=============================
		// 例外処理
		//=============================
		if (pTrigger->size() <= 0)
		{
			ImGui::Text("※None Data※");
			ImGui::TreePop();
			return;
		}
		float* pThisTrigger = &m_EditInfo.motionInfo.trigger[m_EditInfo.triggerIdx];

		ImGui::SetNextItemWidth(140.0f);
		if (ImGui::SliderInt("Trigger Idx", &m_EditInfo.triggerIdx, 0, pTrigger->size() - 1))
		{
			// 元のデータ
			if (m_vecMotionInfo[m_EditInfo.motionIdx].trigger.size() > m_EditInfo.triggerIdx)
			{
				m_EditInfo.motionInfo.trigger[m_EditInfo.triggerIdx] = m_vecMotionInfo[m_EditInfo.motionIdx].trigger[m_EditInfo.triggerIdx];
			}
			else
			{
				m_EditInfo.motionInfo.trigger[m_EditInfo.keyIdx] = 0.0f;
			}

		}
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		//=============================
		// 元データへ割り当て
		//=============================
		if (ImGui::Button("Regist Trigger"))
		{
			m_vecMotionInfo[m_EditInfo.motionIdx].trigger = *pTrigger;
		}

		// トリガー発生時間
		ImGui::DragFloat("trigger Time", &(*pThisTrigger), 0.01f, 0.0f, 0.0f, "%.2f");

		ImGui::TreePop();
	}
}

//==========================================================================
// モーション設定
//==========================================================================
void CCameraMotion::SetMotion(int motion, EASING EasingType)
{
	m_nNowMotionIdx = motion;
	m_nNowKeyIdx = 0;
	m_nNowTriggerIdx = 0;
	m_fMotionTimer = 0.0f;
	m_fTriggerTimer = 0.0f;
	m_bFinish = false;
	m_bPause = false;
	m_EasingType = EasingType;
}

//==========================================================================
// トリガー判定取得
//==========================================================================
CCameraMotion::TriggerInfo CCameraMotion::GetTrigger()
{
	return TriggerInfo(m_bTrigger, m_nNowTriggerIdx);
}
