//==========================================================================
//
// ���X�g�}�l�[�W���w�b�_�[[listmanager.h]
// Author�F���n�Ή�
//
//==========================================================================
#ifndef _LISTMANAGER_H_
#define _LISTMANAGER_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// ���X�g�}�l�[�W���N���X�̒�`
//==========================================================================
template<class T> class CListManager
{
public:

	using Iterator = typename std::list<T*>::iterator;

	CListManager();			// �R���X�g���N�^
	~CListManager();		// �f�X�g���N�^

	HRESULT Init();
	void Uninit();

	CListManager<T>::Iterator GetBegin();
	CListManager<T>::Iterator GetEnd();

	void Regist(T* pList);				// ���蓖��
	void Delete(T* pList);				// �폜
	bool ListLoop(T** ppList);			// ���X�g���[�v����
	bool ListLoop(Iterator& itr);		// ���X�g���[�v����
	T* GetData(int nIdx);				// �f�[�^�擾
	int FindIdx(T* pList);				// ���X�g����C���f�b�N�X����
	void KillAll();					// �S�č폜
	int GetNumAll();				// �A�C�e���̑����擾
	std::list<T*>& GetList();	// �A�C�e���̃��X�g�擾

private:

	std::list<T*> m_ListObj;	// ���X�g
	int m_nNumAll;				// ����
};



//==========================================================================
// �����擾
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
// �R���X�g���N�^
//==========================================================================
template<class T> 
CListManager<T>::CListManager()
{
	// �l�̃N���A
	m_nNumAll = 0;	// �������Z�b�g
	m_ListObj.clear();	// �A�C�e���̃��X�g
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
template<class T> 
CListManager<T>::~CListManager()
{
	m_ListObj.clear();
}

//==========================================================================
// ����������
//==========================================================================
template<class T> 
HRESULT CListManager<T>::Init()
{
	// �������Z�b�g
	m_nNumAll = 0;

	return S_OK;
}

//==========================================================================
// ���蓖��
//==========================================================================
template<class T> 
void CListManager<T>::Regist(T* pList)
{
	// �A�C�e���̃��X�g�ɒǉ�
	m_ListObj.push_back(pList);

	// �������Z
	m_nNumAll++;
}

//==========================================================================
// �폜
//==========================================================================
template<class T> 
void CListManager<T>::Delete(T* pList)
{
	// �������g�����X�g����T��
	Iterator itr = std::find(m_ListObj.begin(), m_ListObj.end(), pList);

	// �v�f�����������ꍇ�̂ݍ폜
	if (itr != m_ListObj.end()) 
	{
		m_ListObj.erase(itr);

		// �������Z
		m_nNumAll--;
	}
}


//==========================================================================
// ���X�g���[�v����
//==========================================================================
template<class T> 
bool CListManager<T>::ListLoop(T** ppList)
{
	if (m_ListObj.empty())
	{// ��̏ꍇ���I��
		return false;
	}

	// �擪�̃C�e���[�^�[���擾
	Iterator it = m_ListObj.begin();
	std::advance(it, 0);

	Iterator itr = it;

	if (*ppList == nullptr)
	{
		*ppList = (*itr);
	}
	else
	{
		// �������g�����X�g����T��
		itr = std::find(m_ListObj.begin(), m_ListObj.end(), *ppList);

		// �I�[�̃C�e���[�^�[���擾
		Iterator enditr = m_ListObj.end();
		enditr = std::prev(enditr);

		if (itr == enditr)
		{// �I�[��������I��
			return false;
		}

		// ���̗v�f��n��
		itr = std::next(itr, 1);
		*ppList = (*itr);
	}

	return (*ppList != nullptr);	// nullptr�ŏI��
}


//==========================================================================
// ���X�g���[�v����
//==========================================================================
template<class T>
bool CListManager<T>::ListLoop(Iterator& itr)
{
	if (m_ListObj.empty())
	{// ��̏ꍇ���I��
		return false;
	}

#if 0
	if (*itr == nullptr)
	{// �擪

		auto begin = m_ListObj.begin();
		(*itr) = &begin;
	}
	else
	{
		// �I�[�̃C�e���[�^�[���擾
		Iterator enditr = m_ListObj.end();
		enditr = std::prev(enditr);

		if (*(*itr) == enditr)
		{// �I�[��������I��
			return false;
		}

		*(*itr)++;
	}

	return (itr != nullptr);	// nullptr�ŏI��

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
// �f�[�^�擾
//==========================================================================
template<class T> 
T* CListManager<T>::GetData(int nIdx)
{
	if (m_ListObj.empty())
	{// ��O�͑��I��
		return nullptr;
	}

	if (nIdx >= static_cast<int>(m_ListObj.size()))
	{// ��O�͑��I��
		return nullptr;
	}

	// �C���f�b�N�X���i�񂾗v�f��n��
	Iterator it = m_ListObj.begin();
	std::advance(it, nIdx);
	return (*it);
}

//==========================================================================
// ���X�g����C���f�b�N�X����
//==========================================================================
template<class T> 
int CListManager<T>::FindIdx(T* pList)
{
	if (m_ListObj.empty())
	{// ��̏ꍇ���I��
		return -1;
	}

	// �������g�����X�g����T��
	Iterator itr = std::find(m_ListObj.begin(), m_ListObj.end(), pList);

	return static_cast<int>(std::distance(m_ListObj.begin(), itr));
}

//==========================================================================
// �I������
//==========================================================================
template<class T> 
void CListManager<T>::Uninit()
{
	// �N���A
	m_ListObj.clear();

	// ����
	m_nNumAll = 0;
}

//==========================================================================
// �S�č폜
//==========================================================================
template<class T> 
void CListManager<T>::KillAll()
{
	Iterator it = m_ListObj.begin();

	// �v�f���J��Ԃ�
	while (it != m_ListObj.end())
	{
		// �I������
		(*it)->Uninit();

		// �擪�̗v�f��n��
		it = m_ListObj.begin();
		m_nNumAll--;
	}

	// �N���A
	m_ListObj.clear();

	// ����
	m_nNumAll = 0;

}

//==========================================================================
// �����擾
//==========================================================================
template<class T> 
int CListManager<T>::GetNumAll()
{
	return m_nNumAll;
}

//==========================================================================
// ���X�g�擾
//==========================================================================
template<class T> 
std::list<T*>& CListManager<T>::GetList()
{
	return m_ListObj;
}


#endif