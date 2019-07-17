#pragma once
#include "../IScene.h"
#include "../../Data.h"
#include "../../../Utility/Utility.h"



class ResultScene:public IScene
{
public:
	ResultScene();
	~ResultScene();

	// �J�n����
	void Start(std::list<SOCKET> _players) override;
	// �X�V����
	Data::Scenes::SCENETYPE Update() override;
	// �I������
	void End() override;

	// ��M���ɍs������
	void Receive(char* _data) override;
	// �S�v���C���[���𑗐M����B
	//bool Send() override;

	// �V�[�����̃v���C���[��ǉ�����B
	void PushPlayers(Data::Pakets::IPaketData _player) { m_playerList.push_back(_player); }

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
	
	// �V�[���ɂ���v���C���[���
	std::list<Data::Pakets::ResultData> m_playerList;

	// �N���C�A���g�ɑ������S�ē����
	std::list < Data::Pakets::IPaketData> m_sendList;

	// ���̃V�[���ɐ؂�ւ����������ۗp�p�P�b�g
	Data::Pakets::SwitchingPakets m_swich;
};
