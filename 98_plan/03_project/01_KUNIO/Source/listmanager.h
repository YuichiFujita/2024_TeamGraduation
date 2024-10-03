//==========================================================================
//
// リストマネージャヘッダー[listmanager.h]
// Author：相馬靜雅
//
//==========================================================================
#ifndef _LISTMANAGER_H_
#define _LISTMANAGER_H_	// 二重インクルード防止

//==========================================================================
// リストマネージャクラスの定義
//==========================================================================
template<class T> class CListManager
{
public:

	using Iterator = typename std::list<T*>::iterator;

	CListManager();			// コンストラクタ
	~CListManager();		// デストラクタ

	HRESULT Init();
	void Uninit();

	CListManager<T>::Iterator GetBegin();
	CListManager<T>::Iterator GetEnd();

	void Regist(T* pList);				// 割り当て
	void Delete(T* pList);				// 削除
	bool ListLoop(T** ppList);			// リストループ処理
	bool ListLoop(Iterator& itr);		// リストループ処理
	T* GetData(int nIdx);				// データ取得
	int FindIdx(T* pList);				// リストからインデックス検索
	void KillAll();					// 全て削除
	int GetNumAll();				// アイテムの総数取得
	std::list<T*>& GetList();	// アイテムのリスト取得

private:

	std::list<T*> m_ListObj;	// リスト
	int m_nNumAll;				// 総数
};



//==========================================================================
// 総数取得
//==========================================================================
template<class T>
typename CListManager<T>::Iterator CListManager<T>::GetBegin()
{
	return m_ListObj.begin();
}

template<class T>
typename CListManager<T>::Iterator CListManager<T>::GetEnd()
{
	return m_ListObj.end();
}

//==========================================================================
// コンストラクタ
//==========================================================================
template<class T> 
CListManager<T>::CListManager()
{
	// 値のクリア
	m_nNumAll = 0;	// 総数リセット
	m_ListObj.clear();	// アイテムのリスト
}

//==========================================================================
// デストラクタ
//==========================================================================
template<class T> 
CListManager<T>::~CListManager()
{
	m_ListObj.clear();
}

//==========================================================================
// 初期化処理
//==========================================================================
template<class T> 
HRESULT CListManager<T>::Init()
{
	// 総数リセット
	m_nNumAll = 0;

	return S_OK;
}

//==========================================================================
// 割り当て
//==========================================================================
template<class T> 
void CListManager<T>::Regist(T* pList)
{
	// アイテムのリストに追加
	m_ListObj.push_back(pList);

	// 総数加算
	m_nNumAll++;
}

//==========================================================================
// 削除
//==========================================================================
template<class T> 
void CListManager<T>::Delete(T* pList)
{
	// 自分自身をリストから探す
	Iterator itr = std::find(m_ListObj.begin(), m_ListObj.end(), pList);

	// 要素が見つかった場合のみ削除
	if (itr != m_ListObj.end()) 
	{
		m_ListObj.erase(itr);

		// 総数減算
		m_nNumAll--;
	}
}


//==========================================================================
// リストループ処理
//==========================================================================
template<class T> 
bool CListManager<T>::ListLoop(T** ppList)
{
	if (m_ListObj.empty())
	{// 空の場合即終了
		return false;
	}

	// 先頭のイテレーターを取得
	Iterator it = m_ListObj.begin();
	std::advance(it, 0);

	Iterator itr = it;

	if (*ppList == nullptr)
	{
		*ppList = (*itr);
	}
	else
	{
		// 自分自身をリストから探す
		itr = std::find(m_ListObj.begin(), m_ListObj.end(), *ppList);

		// 終端のイテレーターを取得
		Iterator enditr = m_ListObj.end();
		enditr = std::prev(enditr);

		if (itr == enditr)
		{// 終端だったら終了
			return false;
		}

		// 次の要素を渡す
		itr = std::next(itr, 1);
		*ppList = (*itr);
	}

	return (*ppList != nullptr);	// nullptrで終了
}


//==========================================================================
// リストループ処理
//==========================================================================
template<class T>
bool CListManager<T>::ListLoop(Iterator& itr)
{
	if (m_ListObj.empty())
	{// 空の場合即終了
		return false;
	}

#if 0
	if (*itr == nullptr)
	{// 先頭

		auto begin = m_ListObj.begin();
		(*itr) = &begin;
	}
	else
	{
		// 終端のイテレーターを取得
		Iterator enditr = m_ListObj.end();
		enditr = std::prev(enditr);

		if (*(*itr) == enditr)
		{// 終端だったら終了
			return false;
		}

		*(*itr)++;
	}

	return (itr != nullptr);	// nullptrで終了

#else

	if (itr == m_ListObj.end())
	{
		int n = 0;
		itr = m_ListObj.begin();
	}
	else
	{
		itr++;
	}

	return (itr != m_ListObj.end());

#endif
}

//==========================================================================
// データ取得
//==========================================================================
template<class T> 
T* CListManager<T>::GetData(int nIdx)
{
	if (m_ListObj.empty())
	{// 例外は即終了
		return nullptr;
	}

	if (nIdx >= static_cast<int>(m_ListObj.size()))
	{// 例外は即終了
		return nullptr;
	}

	// インデックス分進んだ要素を渡す
	Iterator it = m_ListObj.begin();
	std::advance(it, nIdx);
	return (*it);
}

//==========================================================================
// リストからインデックス検索
//==========================================================================
template<class T> 
int CListManager<T>::FindIdx(T* pList)
{
	if (m_ListObj.empty())
	{// 空の場合即終了
		return -1;
	}

	// 自分自身をリストから探す
	Iterator itr = std::find(m_ListObj.begin(), m_ListObj.end(), pList);

	return static_cast<int>(std::distance(m_ListObj.begin(), itr));
}

//==========================================================================
// 終了処理
//==========================================================================
template<class T> 
void CListManager<T>::Uninit()
{
	// クリア
	m_ListObj.clear();

	// 総数
	m_nNumAll = 0;
}

//==========================================================================
// 全て削除
//==========================================================================
template<class T> 
void CListManager<T>::KillAll()
{
	Iterator it = m_ListObj.begin();

	// 要素分繰り返し
	while (it != m_ListObj.end())
	{
		// 終了処理
		(*it)->Uninit();

		// 先頭の要素を渡す
		it = m_ListObj.begin();
		m_nNumAll--;
	}

	// クリア
	m_ListObj.clear();

	// 総数
	m_nNumAll = 0;

}

//==========================================================================
// 総数取得
//==========================================================================
template<class T> 
int CListManager<T>::GetNumAll()
{
	return m_nNumAll;
}

//==========================================================================
// リスト取得
//==========================================================================
template<class T> 
std::list<T*>& CListManager<T>::GetList()
{
	return m_ListObj;
}


#endif