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
	m_callFlag = false;
}

/// <summary>
/// ���r�[���̍X�V����
/// </summary>
/// <returns>�V�[���؂�ւ�����</returns>
Data::Scenes::SCENETYPE LobbyScene::Update()
{
	// �ύX�_������ꍇ��m_sendList�ɒǉ����ď����X�V����
	// �Ȃ��ꍇ�͂��̂܂܏����X�V����
	for each(Data::Pakets::LobbyData var in m_dataBufferList)
	{
		for (std::list<Data::Pakets::LobbyData>::iterator itr = m_playerList.begin(); itr != m_playerList.end(); itr++)
		{
			if (itr->socket != var.socket) { continue; }
			else { *itr = var; }
		}
	}
	// �N���C�A���g���牽���R�[������������S�Ă��X�V����
	if (m_callFlag)
	{
		for each (Data::Pakets::LobbyData var in m_playerList) 
		{
			m_sendList.push_back(var); 
			if (var.scene.nextScene == Data::Scenes::SCENETYPE::BACK)
			{
				m_delList.push_back(var);
			}
		}
		m_callFlag = false;
	}
	
	// �o�b�t�@���폜����
	m_dataBufferList.clear();


	// �V�[����؂�ւ��邩�ǂ����̃��^�[���p�ϐ�
	Data::Scenes::SCENETYPE ret = Data::Scenes::SCENETYPE::NON;
	



	// �S���̏������o������V�[���؂�ւ��p�̃p�P�b�g�𑗂�
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
	// ���X�g�̒��ɑ����Ă����l������Ȃ�ǉ�������t���O
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
	m_callFlag = true;
}

/// <summary>
/// �p�P�b�g�̌^��ύX����
/// </summary>
/// <param name="_list"></param>
/// <returns></returns>
std::list<Data::Pakets::IPaketData> LobbyScene::ConvertListType(std::list<Data::Pakets::LobbyData> _list)
{
	std::list<Data::Pakets::IPaketData> tmp;
	for each(auto var in _list)
	{
		tmp.push_back(var);
	}
	return tmp;
}

/// <summary>
/// �ؒf���ꂽ�l���폜���X�g�ɒǉ�
/// </summary>
/// <param name="_soc"></param>
void LobbyScene::OffTheCutter(SOCKET _soc)
{
	m_delList.push_back(Data::Pakets::LobbyData(_soc));
}

/// <summary>
/// ���X�g����폜
/// </summary>
void LobbyScene::DeleteList()
{
	for each (Data::Pakets::LobbyData var in m_delList)
	{
		for (std::list<Data::Pakets::LobbyData>::iterator itr = m_playerList.begin(); itr != m_playerList.end(); itr++)
		{
			if (var.socket == itr->socket)
			{
				itr = m_playerList.erase(itr);
				std::cout << "���r�[���� " << itr->socket << " ���ޏo�����B" << std::endl;
				break;
			}
		}
	}
}

/// <summary>
/// �w�肳�ꂽ���X�g�̒��g��Ԃ�
/// </summary>
/// <param name="_i"></param>
/// <returns></returns>
Data::Pakets::IPaketData * LobbyScene::GetSendList(int _i)
{
	if (m_sendList.size() > _i)
	{
		return &m_sendList[_i];
	}
	m_sendList.clear();
	return nullptr;
}

