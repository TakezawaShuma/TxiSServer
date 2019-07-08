#include "../../../stdafx.h"
#include "LobbyScene.h"


LobbyScene::LobbyScene():m_ret(Data::Scenes::SCENETYPE::NON)
{
}

LobbyScene::~LobbyScene()
{
}

/// <summary>
/// ロビー初期処理
/// </summary>
void LobbyScene::Start(std::list<SOCKET> _players)
{
	m_swich = Data::Pakets::SwitchingPakets(Data::Scenes::LOBBY, Data::Scenes::PLAY);
}

/// <summary>
/// ロビー時の更新処理
/// </summary>
/// <returns>シーン切り替え許可</returns>
Data::Scenes::SCENETYPE LobbyScene::Update()
{
	// 変更点がある場合はm_sendListに追加して情報を更新する
	// ない場合はそのまま情報を更新する
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
	// バッファを削除する
	m_dataBufferList.clear();

	Data::Scenes::SCENETYPE ret = Data::Scenes::SCENETYPE::NON;
	
	// 全員の準備が出来たら
	for (std::list<Data::Pakets::LobbyData>::iterator itr = m_playerList.begin(); itr != m_playerList.end(); itr++)
	{
		if (itr->ready) { ret = Data::Scenes::SCENETYPE::PLAY; }
		else { ret = Data::Scenes::SCENETYPE::NON;	break; }
	}
	if (m_playerList.size() > 1)
	{
		// 切り替え用のパケットをm_sendListに追加する。
		if (m_ret != ret)
		{
			m_ret = ret;
			if (m_ret != Data::Scenes::SCENETYPE::NON) { m_sendList.push_back(m_swich); }
		}
	}
	return m_ret;
}

/// <summary>
/// 終了処理
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
/// ロビーの受信時の処理
/// </summary>
/// <param name="_data">受信したデータの先頭ポインタ</param>
/// <param name="_soc"></param>
void LobbyScene::Receive(char * _data)
{
	bool uptflag = false;
	Data::Pakets::LobbyData lobbyData;
	memcpy(&lobbyData, _data, sizeof(Data::Pakets::LobbyData));

	// リストの中にいるなら
	for (std::list<Data::Pakets::LobbyData>::iterator itr = m_playerList.begin(); itr != m_playerList.end(); itr++)
	{
		m_dataBufferList.push_back(lobbyData);

		if (itr->socket != lobbyData.socket) { continue; }
		// データの更新
		else { uptflag = true;	break; }
		
	}
	if (!uptflag)
	{
		//リストの中にいないなら追加する。
		m_playerList.push_back(lobbyData);
	}
}

bool LobbyScene::Send()
{
	return false;
}

