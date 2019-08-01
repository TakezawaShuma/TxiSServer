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
/// 初期化&ソケットの作成
/// </summary>
/// <returns></returns>
bool Game::Initialize()
{
	m_server->CreateSocket();
	m_login->Start();

	// サーバーの受け入れ準備
	if (m_server->Listen() == false)
	{
		return false;
	}
	return true;
}

/// <summary>
/// ログイン処理&ルームへの振り分け
/// </summary>
bool Game::Login()
{
	// クライアントのソケット
	SOCKET soc;
	// クライアントアドレスを獲得する。
	sockaddr_in m_addr;

	// 初期の接続確認
	if (m_server->Accept(&soc, &m_addr) == true)
	{
		// 接続が確認取れて送信まで確立したら
		if (m_login->Connect(soc))
		{
			// ルームに入室
			AssignToRoom(soc);
			return true;
		}
	}
	return false;
}


/// <summary>
/// ゲームの更新処理
/// </summary>
bool Game::Updata()
{
	// ずっと回し続ける
	Login();
	// ルームの数が0でなかったらルームの更新
	if (m_roomList.size() != 0)
	{
		// ルームの数だけ更新
		for (std::map<int, Room*>::iterator itr = m_roomList.begin(); itr != m_roomList.end(); ++itr)
		{
			// 受信
			(itr)->second->Receive();
			// プレイヤーの削除
			(itr)->second->DeletePlayers();

			// ルームの更新
			itr->second->Update();

			// 送信
			(itr)->second->GetSendData();

			// ルームの削除
			if ((itr)->second->GetEntry() == Room::ENTRYSTATE::DELETE_ROOM)
			{
				itr = m_roomList.erase(itr);
			}
		}
	}


	return true;
}

/// <summary>
/// 終了処理
/// </summary>
void Game::Finish()
{
	m_login->End();
	m_server->Close();
	delete m_server;
	delete m_login;
}


/// <summary>
///  ルームへの割り当て
/// </summary>
/// <param name="_soc">入室したい人のソケット</param>
/// <returns>正常にルームを作成＆割り当てれたら true</returns>
void Game::AssignToRoom(SOCKET _soc)
{
	// ルームがない
	if (m_roomList.size() == 0)
	{
		// ルームの作成 & プレイヤーの入室
		m_roomList.insert(std::make_pair(0, new Room(_soc)));
		m_roomList[0]->Start(m_server);
		// ログを流す
		std::cout << "ルーム0を作成" << std::endl;
		std::cout << "ルーム0に " << _soc << " が入室" << std::endl;
	}
	// ルームがある
	else
	{
		bool join = false;
		// ルームリストを回して入れるところを検索
		for (std::map<int, Room*>::iterator itr = m_roomList.begin(); itr != m_roomList.end(); itr++)
		{
			// ルーム内の空きを検索
			if ((itr)->second->GetPlayerNum() < (itr)->second->MAX_ROOM_MEMBER_NUM)
			{
				// 入室許可を出しているか
				if ((itr)->second->GetEntry() == 0)
				{
					// 開いていたら入室
					(itr)->second->PushList(_soc);
					// 入室ログ
					std::cout << "ルーム" << (itr)->first << "に " << _soc << " が入室" << std::endl;
					join = true;
					break;
				}
				// 入室許可を出していないなら
				else { continue; }
			}
		}
		// 入れるところがなかったら新しくルームを作る
		if (!join)
		{
			std::map<int, Room*>::iterator itr = std::map<int, Room*>::iterator();

			unsigned int roomNum = 0;
			for (roomNum = 0; roomNum < m_roomList.size(); roomNum++)
			{
				itr = m_roomList.find(roomNum);
				if (itr == m_roomList.end()) { break; }
			}
			// 空いた部屋があるなら使用する
			if (itr == m_roomList.end())
			{
				m_roomList.insert(std::make_pair(roomNum, new Room(_soc)));
				m_roomList[roomNum]->Start(m_server);
			}
			// ないなら新しくつくる。
			else
			{
				m_roomList.insert(std::make_pair(m_roomList.size(), new Room(_soc)));
				m_roomList[roomNum]->Start(m_server);
			}
		}
	}
}
