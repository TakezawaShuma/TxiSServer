#pragma once
#include <memory>
#include "../../Data.h"
#include "../../../Utility/Utility.h"
#include "../IScene.h"


class PlayScene:public IScene
{
public:
	PlayScene();
	~PlayScene();

	// �J�n����
	void Start(std::list<SOCKET> _players) override;
	// �X�V����
	Data::Scenes::SCENETYPE Update() override;
	// �I������
	void End() override;

	// ��M���ɍs������
	void Receive(char* _data) override;
	// �S�v���C���[���𑗐M����B
	bool Send() override;

	// �����Ƒ��̐l�̒e
	bool HitChack(Utility::Vector2 _v1, Utility::Vector2 _v2);


	bool Execution(Data::Pakets::PlayData* _p1, Data::Pakets::PlayData* _p2);


	// �V�[�����̃v���C���[��ǉ�����B
	void PushPlayers(Data::Pakets::IPaketData _player) { m_playerList.push_back(_player); }

	void DeleteList() override;

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
	Data::Pakets::PlayData SetStartPosition(SOCKET _soc, Utility::Vector2 _vec);

	Utility::Vector2 RandomPosition();

private:



	// �V�[���ɂ���v���C���[���
	std::list<Data::Pakets::PlayData> m_playerList;

	// �N���C�A���g�ɑ������S�ē����
	std::list < Data::Pakets::IPaketData> m_sendList;

	// ���̃V�[���ɐ؂�ւ����������ۗp�p�P�b�g
	Data::Pakets::SwitchingPakets m_swich;

	// ���݂̎���
	u_int m_time;
};