#pragma once


#include "../../Data.h"
#include "../IScene.h"
#include <list>


class LobbyScene:public IScene
{
public:
	LobbyScene();
	~LobbyScene();

	// �J�n����
	void Start(std::list<SOCKET> _players) override;
	// �X�V����
	Data::Scenes::SCENETYPE Update() override;
	// �I������
	void End() override;

	// ��M���ɍs������
	void Receive(char* _data) override;
	//// �S�v���C���[���𑗐M����B
	//bool Send() override;
	
	// ���r�[�ɂ���l����Ԃ��B
	int SendSize() { return m_playerList.size(); }


	// �V�[�����̃v���C���[��ǉ�����B
	void PushPlayers(Data::Pakets::IPaketData _player)
	{
		m_playerList.push_back(_player); m_sendList = ConvertListType(m_playerList);
	}
	
	// ������̃��X�g���l������
	std::list<Data::Pakets::IPaketData> GetSendList()
	{
		std::list < Data::Pakets::IPaketData> baf = m_sendList;
		m_sendList.clear();
		return baf;
	}

	// �ǂ̃V�[���ɐ؂�ւ���̂������߂�p�P�b�g���擾����
	Data::Pakets::SwitchingPakets GetSwichPakets() { return m_swich; }

private:
	// �h���N���X�̃��X�g�����N���X�̃��X�g�ɑ��������
	std::list<Data::Pakets::IPaketData> ConvertListType(std::list<Data::Pakets::LobbyData> _list);
private:

	// �V�[���ɂ���v���C���[���
	std::list<Data::Pakets::LobbyData> m_playerList;
	
	// �N���C�A���g�ɑ������S�ē����
	std::list < Data::Pakets::IPaketData> m_sendList;

	// �����Ă����f�[�^���ꎞ�I�ɕۑ�����
	std::list < Data::Pakets::LobbyData> m_dataBufferList;

	// ���̃V�[���ɐ؂�ւ����������ۗp�p�P�b�g
	Data::Pakets::SwitchingPakets m_swich;

	// �V�[����؂�ւ���Ƃ��g��
	Data::Scenes::SCENETYPE m_ret;
};

