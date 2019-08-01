#pragma once
#include<memory>
#include <list>
#include <map>

#include "Data.h"
#include "Scenes/Lobby/LobbyScene.h"
#include "Scenes/Play/PlayScene.h"
#include "Scenes/Result/ResultScene.h"

#include "../Network/ServerNet.h"


class Room
{
public:
	static const int MAX_ROOM_MEMBER_NUM = 10;

	enum ENTRYSTATE
	{
		ENTRY_PERMIT,	// ��������
		NO_ENTRY,		// �����֎~
		DELETE_ROOM,	// ���[���폜
	};

public:
	Room();
	Room(SOCKET _soc);
	~Room();

	// ���̎󂯎��
	void Receive();


	// ���[���̏�������
	void Start(ServerNet* _server);

	// ���[���̍X�V����
	void Update();

	// ���[���̏I������
	void End();

	// �V�[���̐؂�ւ�
	bool ChangeRoomScene(Data::Scenes::SCENETYPE _nextScene);

	// ���̑��M
	bool SendRoomData(char* _data);

	// �V�[�����瑗��������Ă���
	void GetSendData();

	// �v���C���[�̍폜
	ENTRYSTATE DeletePlayers();

	// ���[���̃��X�g�ɒǉ�
	void PushList(SOCKET _soc);

	// ���[���̃V�[����ԍX�V
	void SceneChange(Data::Scenes::Scene _nextScene) { m_roomScene = _nextScene; }

	// �������Ă���v���C���[�̐�
	int GetPlayerNum() { return m_playerSocketList.size(); }

	// ���������o���Ă��邩�H
	int GetEntry() { return m_entry; }

	// ���݂̃V�[�����擾
	Data::Scenes::Scene GetNowScene() { return m_roomScene; }


private:
	// �T�[�o�[�ւ̃|�C���^
	ServerNet* m_server;

	// ���[���̃V�[�����Ǘ�����
	Data::Scenes::Scene m_roomScene;

	// �Q�[���̃V�[�������Ǘ�����|�C���^
	std::unique_ptr<IScene> m_scene;


	// ���[���ɓ����Ă���v���C���[
	std::list<SOCKET> m_playerSocketList;

	// ����v���C���[���
	std::list<Data::Pakets::IPaketData> m_sendList;

	// �؂�ւ����������l�����J�E���g
	std::list<SOCKET> m_swithCount;

	// �V�[���؂�ւ��t���O
	bool m_changeSceneFlag;

	// �폜����v���C���[�̏��
	std::list<SOCKET> m_delList;


	// �����̉s��
	// 0 = �\�E1 = �s�\�E 2 = ���[���폜 
	ENTRYSTATE m_entry;
};

