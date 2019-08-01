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
	m_callFlag = false;
}

/// <summary>
/// ロビー時の更新処理
/// </summary>
/// <returns>シーン切り替え許可</returns>
Data::Scenes::SCENETYPE LobbyScene::Update()
{
	// 変更点がある場合はm_sendListに追加して情報を更新する
	// ない場合はそのまま情報を更新する
	for each(Data::Pakets::LobbyData var in m_dataBufferList)
	{
		for (std::list<Data::Pakets::LobbyData>::iterator itr = m_playerList.begin(); itr != m_playerList.end(); itr++)
		{
			if (itr->socket != var.socket) { continue; }
			else { *itr = var; }
		}
	}
	// クライアントから何かコールがあったら全てを更新する
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
	
	// バッファを削除する
	m_dataBufferList.clear();


	// シーンを切り替えるかどうかのリターン用変数
	Data::Scenes::SCENETYPE ret = Data::Scenes::SCENETYPE::NON;
	



	// 全員の準備が出来たらシーン切り替え用のパケットを送る
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
	// リストの中に送られてきた人がいるなら追加させるフラグ
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
	m_callFlag = true;
}

/// <summary>
/// パケットの型を変更する
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
/// 切断された人を削除リストに追加
/// </summary>
/// <param name="_soc"></param>
void LobbyScene::OffTheCutter(SOCKET _soc)
{
	m_delList.push_back(Data::Pakets::LobbyData(_soc));
}

/// <summary>
/// リストから削除
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
				std::cout << "ロビーから " << itr->socket << " が退出した。" << std::endl;
				break;
			}
		}
	}
}

/// <summary>
/// 指定されたリストの中身を返す
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

