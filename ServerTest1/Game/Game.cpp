#include "../stdafx.h"
#include "Game.h"

Game::Game():m_server(nullptr), m_login(nullptr)
{
	m_server = new ServerNet();
	m_login = new LoginScene(m_server);
	m_roomList = std::map<int, Room*>();
}

Game::~Game()
{

}

/// <summary>
/// ������&�\�P�b�g�̍쐬
/// </summary>
/// <returns></returns>
bool Game::Initialize()
{
	m_server->CreateSocket();
	m_login->Start();

	// �T�[�o�[�̎󂯓��ꏀ��
	if (m_server->Listen() == false)
	{
		return false;
	}
	return true;
}

/// <summary>
/// ���O�C������&���[���ւ̐U�蕪��
/// </summary>
bool Game::Login()
{
	// �N���C�A���g�̃\�P�b�g
	SOCKET soc;
	// �N���C�A���g�A�h���X���l������B
	sockaddr_in m_addr;

	// �����̐ڑ��m�F
	if (m_server->Accept(&soc, &m_addr) == true)
	{
		// �ڑ����m�F���đ��M�܂Ŋm��������
		if (m_login->Connect(soc))
		{
			// ���[���ɓ���
			AssignToRoom(soc);
			return true;
		}
	}
	return false;
}


/// <summary>
/// �Q�[���̍X�V����
/// </summary>
bool Game::Updata()
{
	// �����Ɖ񂵑�����
	Login();
	// ���[���̐���0�łȂ������烋�[���̍X�V
	if (m_roomList.size() != 0)
	{
		// ���[���̐������X�V
		for (std::map<int, Room*>::iterator itr = m_roomList.begin(); itr != m_roomList.end(); ++itr)
		{
			// ��M
			(itr)->second->Receive();
			// �v���C���[�̍폜
			(itr)->second->DeletePlayers();

			// ���[���̍X�V
			itr->second->Update();

			// ���M
			(itr)->second->GetSendData();

			// ���[���̍폜
			if ((itr)->second->GetEntry() == Room::ENTRYSTATE::DELETE_ROOM)
			{
				itr = m_roomList.erase(itr);
			}
		}
	}


	return true;
}

/// <summary>
/// �I������
/// </summary>
void Game::Finish()
{
	m_login->End();
	m_server->Close();
	delete m_server;
	delete m_login;
}


/// <summary>
///  ���[���ւ̊��蓖��
/// </summary>
/// <param name="_soc">�����������l�̃\�P�b�g</param>
/// <returns>����Ƀ��[�����쐬�����蓖�Ăꂽ�� true</returns>
void Game::AssignToRoom(SOCKET _soc)
{
	// ���[�����Ȃ�
	if (m_roomList.size() == 0)
	{
		// ���[���̍쐬 & �v���C���[�̓���
		m_roomList.insert(std::make_pair(0, new Room(_soc)));
		m_roomList[0]->Start(m_server);
		// ���O�𗬂�
		std::cout << "���[��0���쐬" << std::endl;
		std::cout << "���[��0�� " << _soc << " ������" << std::endl;
	}
	// ���[��������
	else
	{
		bool join = false;
		// ���[�����X�g���񂵂ē����Ƃ��������
		for (std::map<int, Room*>::iterator itr = m_roomList.begin(); itr != m_roomList.end(); itr++)
		{
			// ���[�����̋󂫂�����
			if ((itr)->second->GetPlayerNum() < (itr)->second->MAX_ROOM_MEMBER_NUM)
			{
				// ���������o���Ă��邩
				if ((itr)->second->GetEntry() == 0)
				{
					// �J���Ă��������
					(itr)->second->PushList(_soc);
					// �������O
					std::cout << "���[��" << (itr)->first << "�� " << _soc << " ������" << std::endl;
					join = true;
					break;
				}
				// ���������o���Ă��Ȃ��Ȃ�
				else { continue; }
			}
		}
		// �����Ƃ��낪�Ȃ�������V�������[�������
		if (!join)
		{
			std::map<int, Room*>::iterator itr = std::map<int, Room*>::iterator();

			unsigned int roomNum = 0;
			for (roomNum = 0; roomNum < m_roomList.size(); roomNum++)
			{
				itr = m_roomList.find(roomNum);
				if (itr == m_roomList.end()) { break; }
			}
			// �󂢂�����������Ȃ�g�p����
			if (itr == m_roomList.end())
			{
				m_roomList.insert(std::make_pair(roomNum, new Room(_soc)));
				m_roomList[roomNum]->Start(m_server);
			}
			// �Ȃ��Ȃ�V��������B
			else
			{
				m_roomList.insert(std::make_pair(m_roomList.size(), new Room(_soc)));
				m_roomList[roomNum]->Start(m_server);
			}
		}
	}
}
