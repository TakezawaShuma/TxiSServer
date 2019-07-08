#include "../../../stdafx.h"
#include "LobbyScene.h"


LobbyScene::LobbyScene():m_ret(Data::Scenes::SCENETYPE::NON)
{
}

LobbyScene::~LobbyScene()
{
}

/// <summary>
/// ���r�[��������
/// </summary>
void LobbyScene::Start(std::list<SOCKET> _players)
{
	m_swich = Data::Pakets::SwitchingPakets(Data::Scenes::LOBBY, Data::Scenes::PLAY);
}

/// <summary>
/// ���r�[���̍X�V����
/// </summary>
/// <returns>�V�[���؂�ւ�����</returns>
Data::Scenes::SCENETYPE LobbyScene::Update()
{
	// �ύX�_������ꍇ��m_sendList�ɒǉ����ď����X�V����
	// �Ȃ��ꍇ�͂��̂܂܏����X�V����
	for (std::list<Data::Pakets::LobbyData>::iterator itr = m_playerList.begin(); itr != m_playerList.end(); itr++)
	{
		for each(Data::Pakets::LobbyData var in m_dataBufferList)
		{
			if (itr->socket != var.socket) { continue; }
			else 
			{
				if (var.ready != itr->ready)
				{
					m_sendList.push_back(var);
				}
				*itr = var;
			}
		}
	}
	// �o�b�t�@���폜����
	m_dataBufferList.clear();

	Data::Scenes::SCENETYPE ret = Data::Scenes::SCENETYPE::NON;
	
	// �S���̏������o������
	for (std::list<Data::Pakets::LobbyData>::iterator itr = m_playerList.begin(); itr != m_playerList.end(); itr++)
	{
		if (itr->ready) { ret = Data::Scenes::SCENETYPE::PLAY; }
		else { ret = Data::Scenes::SCENETYPE::NON;	break; }
	}
	if (m_playerList.size() > 1)
	{
		// �؂�ւ��p�̃p�P�b�g��m_sendList�ɒǉ�����B
		if (m_ret != ret)
		{
			m_ret = ret;
			if (m_ret != Data::Scenes::SCENETYPE::NON) { m_sendList.push_back(m_swich); }
		}
	}
	return m_ret;
}

/// <summary>
/// �I������
/// </summary>
/// <param name="_data"></param>
/// <param name="_soc"></param>
void LobbyScene::End()
{
	m_sendList.clear();
	m_playerList.clear();
	m_dataBufferList.clear();
}

/// <summary>
/// ���r�[�̎�M���̏���
/// </summary>
/// <param name="_data">��M�����f�[�^�̐擪�|�C���^</param>
/// <param name="_soc"></param>
void LobbyScene::Receive(char * _data)
{
	bool uptflag = false;
	Data::Pakets::LobbyData lobbyData;
	memcpy(&lobbyData, _data, sizeof(Data::Pakets::LobbyData));

	// ���X�g�̒��ɂ���Ȃ�
	for (std::list<Data::Pakets::LobbyData>::iterator itr = m_playerList.begin(); itr != m_playerList.end(); itr++)
	{
		m_dataBufferList.push_back(lobbyData);

		if (itr->socket != lobbyData.socket) { continue; }
		// �f�[�^�̍X�V
		else { uptflag = true;	break; }
		
	}
	if (!uptflag)
	{
		//���X�g�̒��ɂ��Ȃ��Ȃ�ǉ�����B
		m_playerList.push_back(lobbyData);
	}
}

bool LobbyScene::Send()
{
	return false;
}

