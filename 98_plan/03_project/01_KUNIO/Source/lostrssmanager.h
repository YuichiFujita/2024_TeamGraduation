//======================================================
//
//�f�o�C�X���X�g�ȂǂŔj�����K�v�Ȃ��̃}�l�[�W���[�̃w�b�_[lostrssmanager.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _LOSTRSS_MANAGER_H_	//���d�C���N���[�h�΍�
#define _LOSTRSS_MANAGER_H_

#include <list>

//���X�g������̂Ɍp��������C���^�[�t�F�[�X
__interface ILostResource
{
	void Backup() = 0;
	void Restore() = 0;
};

//�I�u�W�F�N�g�N���X
class CLostResourceManager
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CLostResourceManager();
	~CLostResourceManager();

	//�}�l�[�W���[�Ǘ�
	static CLostResourceManager* GetInstance();
	static void Release();

	//���X�g������̓o�^�E�폜
	void Regist(ILostResource* lostrss) { m_lostResources.emplace_back(lostrss); }
	void Remove(ILostResource* lostrss);

	//���X�g������̃o�b�N�A�b�v�E�Đ���
	void Backup();
	void Restore();
private:
	static CLostResourceManager* m_pInstance;	//�C���X�^���X�i�V���O���g���ɂ��邽�߁j
	std::list<ILostResource*> m_lostResources;
};

#endif	//���d