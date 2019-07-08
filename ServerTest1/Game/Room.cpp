#include "../stdafx.h"
#include "Room.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Room::Room() :m_entry(ENTRYSTATE::ENTRY_PERMIT),m_server(nullptr), m_roomScene(Data::Scenes::Scene()),m_changeSceneFlag(false)
{
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="_soc"></param>
Room::Room(SOCKET _soc) : m_entry(ENTRYSTATE::ENTRY_PERMIT),m_roomScene(Data::Scenes::Scene()),m_server(nullptr), m_changeSceneFlag(false)
{
    m_playerSocketList.push_back(_soc);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Room::~Room()
{
 
}

/// <summary>
/// ����������
/// </summary>
/// <param name="_server"></param>
void Room::Start(ServerNet* _server)
{
	m_server = _server;
	// ���r�[�V�[�����쐬���J��
	ChangeRoomScene(Data::Scenes::LOBBY);
}

/// <summary>
/// �X�V����
/// </summary>
void Room::Update()
{
	// �V�[���̍X�V
	if (m_scene != nullptr) { m_roomScene.nextScene = m_scene->Update(); }

	// �V�[���̐؂�ւ�
	if (m_changeSceneFlag &&
		m_roomScene.nextScene != Data::Scenes::SCENETYPE::NON)
	{
		std::cout << "�V�[����؂�ւ��܂� -> " << m_roomScene.nextScene << std::endl;
		m_changeSceneFlag = ChangeRoomScene(m_roomScene.nextScene);
	}
}

/// <summary>
/// �I������
/// </summary>
void Room::End()
{
	// �V�[�����J��
	m_scene->End();
	IScene* p = m_scene.release();
	delete p;

	// ���X�g�̊J��
	m_sendList.clear();
	m_delList.clear();
	m_playerSocketList.clear();
	m_swithCount.clear();
}

/// <summary>
/// ���[�������s����V�[����؂�ւ���
/// </summary>
/// <param name="_nextScene"></param>
/// <returns>�V�[���؂�ւ� ���� = false/���s= true</returns>
bool Room::ChangeRoomScene(Data::Scenes::SCENETYPE _nextScene)
{
	bool ret = true;
	// �V�[���̏I�����������\�[�X�̉��
	if (m_scene != nullptr)
	{
		// �I������
		m_scene->End();
		// ���\�[�X�̊J��
		m_scene.reset();
	}
	//���\�[�X�̍Đݒ�
	switch (_nextScene)
	{
	case Data::Scenes::LOGIN:
		break;
	case Data::Scenes::TITLE:
		break;
	case Data::Scenes::LOBBY:
		m_scene.reset(new LobbyScene());
		m_roomScene.nowScene = Data::Scenes::SCENETYPE::LOBBY;
		m_roomScene.nextScene = Data::Scenes::SCENETYPE::NON;
		ret = false;
		break;
	case Data::Scenes::PLAY:
		m_scene.reset(new PlayScene());
		m_roomScene.nowScene = Data::Scenes::SCENETYPE::PLAY;
		m_roomScene.nextScene = Data::Scenes::SCENETYPE::NON;
		ret = false;
		break;
	case Data::Scenes::RESULT:
		m_scene.reset(new ResultScene());
		m_roomScene.nowScene = Data::Scenes::SCENETYPE::RESULT;
		m_roomScene.nextScene = Data::Scenes::SCENETYPE::NON;
		ret = false;
		break;
	case Data::Scenes::BACK:
		break;
	case Data::Scenes::NON:
		break;
	default:
		break;
	}

	if (m_scene != nullptr) { m_scene->Start(m_playerSocketList); }
	return ret;
}

/// <summary>
/// ��M�����Ƃ��̏���(�f�[�^�̍X�V���o�^�̂�)
/// </summary>
void Room::Receive()
{
	// ������ł��낤�l�̕�������
	for (std::list<SOCKET>::iterator itr = m_playerSocketList.begin(); itr != m_playerSocketList.end(); itr++)
	{
		int recvSize = 0;
		char data[1024];

		//	��M����
		RECV_CONNECTION state = m_server->Recv(*itr, data, sizeof(data), &recvSize);

		// �ʐM������������
		if (state == RECV_CONNECTION::RECV_SUCCESS)
		{
			// �ǂ̃p�P�b�g�ő����Ă������̊m�F
			//Data::Pakets::IPaketData *base = reinterpret_cast<Data::Pakets::IPaketData*> (new char[1024]);
			Data::Pakets::IPaketData *base = (Data::Pakets::IPaketData*)malloc(1024);
			memcpy(base, data, sizeof(Data::Pakets::IPaketData));

			if (base->paketId != Data::Pakets::PaketID::SWITCH_PAKET)
			{
				// ����ꂽ�f�[�^���V�[����Receive�֐��Ŏ󂯂�
				m_scene->Receive(data);
			}
			// �V�[����؂�ւ��鎞�̏���
			else
			{
				// �؂�ւ�OK�̃��X�g�̒��ɓ����Ă��Ȃ��Ȃ�
				std::list<SOCKET>::iterator tmp = std::find(m_swithCount.begin(), m_swithCount.end(), *itr);
				if (tmp == m_swithCount.end())
				{
					// �o�^
					m_swithCount.push_back(*itr);
				}
				// ���[���ɓ����Ă���l��1�l�ȏ�
				if (m_playerSocketList.size() < 1) 
				{
					if (m_swithCount.size() == m_playerSocketList.size())
					{
						// �S�����V�[����؂�ւ�����V�[����؂�ւ���t���O�𗧂Ă�B
						m_changeSceneFlag = true;
					}
				}
			}
			free(base);
		}
		// �ڑ����؂ꂽ�l���폜���X�g�ɓo�^
		else if (state == RECV_CONNECTION::RECV_FAILURE)
		{
			m_delList.push_back(*itr);
		}
	}
}


/// <summary>
/// �f�[�^�𑗂�
/// </summary>
/// <returns>�ʐM���� true</returns>
bool Room::SendRoomData()
{
	// �V�[�����瑗���񃊃X�g�����Ă���
	m_sendList = m_scene->GetSendList();
	if (m_sendList.size() != 0)
	{
		char data[1024];

		// �����m_sendList�̏����N���C�A���g�ɑ����Ă���
		for (std::list<Data::Pakets::IPaketData>::iterator itr = m_sendList.begin(); itr != m_sendList.end(); itr++)
		{
			memcpy(data, &*itr, sizeof(data));
			// m_sendList�̒��g�����[���̒��ɂ���l�ɑ���
			for each (SOCKET var in m_playerSocketList) { m_server->Send(var, data, sizeof(data)); }
		}
		m_sendList.clear();
		return true;
	}
	return false;
}

/// <summary>
/// ���[������o���v���C���[�̍폜
/// </summary>
/// <returns></returns>
Room::ENTRYSTATE Room::DeletePlayers()
{
	// ���[������̍폜����
	if (m_delList.size() != 0)
	{
		for each (SOCKET var in m_delList)
		{
			m_playerSocketList.remove(var);
			m_swithCount.remove(var);
		}
		m_delList.clear();
	}
	// ���[�����ɐl�����Ȃ��Ȃ�����
	if (m_entry == ENTRYSTATE::NO_ENTRY && m_playerSocketList.size() < 1)
	{
		m_entry = ENTRYSTATE::DELETE_ROOM;
	}
	return m_entry;
}

/// <summary>
/// ���[���ɓ��������違�V�[���ւ̓o�^������
/// </summary>
/// <param name="_soc"></param>
void Room::PushList(SOCKET _soc)
{
	m_playerSocketList.push_back(_soc); 
	m_scene->PushPlayers(Data::Pakets::LobbyData(_soc));
}
