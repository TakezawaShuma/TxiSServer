#include "../stdafx.h"
#include "Room.h"

/// <summary>
/// コンストラクタ
/// </summary>
Room::Room() :m_entry(ENTRYSTATE::ENTRY_PERMIT),m_server(nullptr), m_roomScene(Data::Scenes::Scene()),m_changeSceneFlag(false)
{
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="_soc"></param>
Room::Room(SOCKET _soc) : m_entry(ENTRYSTATE::ENTRY_PERMIT),m_roomScene(Data::Scenes::Scene()),m_server(nullptr), m_changeSceneFlag(false)
{
    m_playerSocketList.push_back(_soc);
}

/// <summary>
/// デストラクタ
/// </summary>
Room::~Room()
{
 
}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="_server"></param>
void Room::Start(ServerNet* _server)
{
	m_server = _server;
	// ロビーシーンを作成し開く
	ChangeRoomScene(Data::Scenes::LOBBY);

}

/// <summary>
/// 更新処理
/// </summary>
void Room::Update()
{
	// シーンの更新
	if (m_scene != nullptr) { m_roomScene.nextScene = m_scene->Update(); }

	// シーンの切り替え
	if (m_changeSceneFlag &&
		m_roomScene.nextScene != Data::Scenes::SCENETYPE::NON)
	{
		std::cout << "シーンを切り替えます -> " << m_roomScene.nextScene << std::endl;
		m_changeSceneFlag = ChangeRoomScene(m_roomScene.nextScene);
	}
}

/// <summary>
/// 終了処理
/// </summary>
void Room::End()
{
	// シーンを開放
	m_scene->End();
	IScene* p = m_scene.release();
	delete p;

	// リストの開放
	m_sendList.clear();
	m_delList.clear();
	m_playerSocketList.clear();
	m_swithCount.clear();
}

/// <summary>
/// ルームが実行するシーンを切り替える
/// </summary>
/// <param name="_nextScene"></param>
/// <returns>シーン切り替え 成功 = false/失敗= true</returns>
bool Room::ChangeRoomScene(Data::Scenes::SCENETYPE _nextScene)
{
	bool ret = true;
	// シーンの終了処理＆リソースの解放
	if (m_scene != nullptr)
	{
		// 終了処理
		m_scene->End();
		// リソースの開放
		m_scene.reset();
	}
	//リソースの再設定
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
		//m_scene.reset(new PlayScene());
		//m_roomScene.nowScene = Data::Scenes::SCENETYPE::PLAY;
		//m_roomScene.nextScene = Data::Scenes::SCENETYPE::NON;
		ret = false;
		break;
	case Data::Scenes::RESULT:
		//m_scene.reset(new ResultScene());
		//m_roomScene.nowScene = Data::Scenes::SCENETYPE::RESULT;
		//m_roomScene.nextScene = Data::Scenes::SCENETYPE::NON;
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
/// 受信したときの処理(データの更新＆登録のみ)
/// </summary>
void Room::Receive()
{
	// 送られるであろう人の分だけ回す
	for (std::list<SOCKET>::iterator itr = m_playerSocketList.begin(); itr != m_playerSocketList.end(); itr++)
	{
		int recvSize = 0;
		char data[1024] = { 0 };

		//	受信する
		RECV_CONNECTION state = m_server->Recv(*itr, data, sizeof(data), &recvSize);

		// 通信が成功したら
		if (state == RECV_CONNECTION::RECV_SUCCESS)
		{
			// どのパケットで送られてきたかの確認
			//Data::Pakets::IPaketData *base = reinterpret_cast<Data::Pakets::IPaketData*> (new char[1024]);
			Data::Pakets::IPaketData *base = (Data::Pakets::IPaketData*)malloc(1024);
			memcpy(base, data, sizeof(Data::Pakets::IPaketData));

			if (base->paketId != Data::Pakets::PaketID::SWITCH_PAKET)
			{
				// 送られたデータをシーンのReceive関数で受ける
				m_scene->Receive(data);
			}
			// シーンを切り替える時の処理
			else
			{
				// 切り替えOKのリストの中に入っていないなら
				std::list<SOCKET>::iterator tmp = std::find(m_swithCount.begin(), m_swithCount.end(), *itr);
				if (tmp == m_swithCount.end())
				{
					// 登録
					m_swithCount.push_back(*itr);
				}
				// ルームに入っている人が1人以上
				if (m_playerSocketList.size() < 1) 
				{
					if (m_swithCount.size() == m_playerSocketList.size())
					{
						// 全員がシーンを切り替えたらシーンを切り替えるフラグを立てる。
						m_changeSceneFlag = true;
					}
				}
			}
			free(base);
		}
		// 接続が切れた人を削除リストに登録
		else if (state == RECV_CONNECTION::RECV_FAILURE)
		{
			m_delList.push_back(*itr);
			m_scene->OffTheCutter(*itr);
		}
	}
}


/// <summary>
/// データを送る
/// </summary>
/// <returns>通信成功 true</returns>
bool Room::SendRoomData(char* _data)
{
	for each (SOCKET var in m_playerSocketList)
	{
		Data::Pakets::LobbyData l;
		memcpy(&l, _data, sizeof(Data::Pakets::LobbyData));
		m_server->Send(var, _data, sizeof(_data));
	}
	return false;
}

/// <summary>
/// 
/// </summary>
void Room::GetSendData()
{
	// シーンから送る情報リストを貰ってくる
	switch (m_roomScene.nowScene)
	{
	case Data::Scenes::SCENETYPE::LOBBY:
	{
		std::list<Data::Pakets::IPaketData*> sendList;
		int i = 0;
		while (true)
		{
			Data::Pakets::IPaketData*tmp = m_scene->GetSendList(i);
			if (tmp == nullptr) { break; }
			else
			{
				sendList.push_back(tmp);
				i++;
			}
		}
		for each (Data::Pakets::IPaketData* var in sendList)
		{
			char data[1024] = { 0 };
			memcpy(data, var, sizeof(data));
			SendRoomData(data);
		}
		break;
	}
	default:
		break;
	}
}

/// <summary>
/// ルームから出たプレイヤーの削除
/// </summary>
/// <returns></returns>
Room::ENTRYSTATE Room::DeletePlayers()
{
	m_scene->DeleteList();
	// ルームからの削除処理
	if (m_delList.size() != 0)
	{
		for each (SOCKET var in m_delList)
		{
			m_playerSocketList.remove(var);
			std::cout << var << " が退出しました。" << std::endl;
			m_swithCount.remove(var);
		}
		m_delList.clear();
	}
	// ルーム内に人がいなくなったら
	if (m_entry == ENTRYSTATE::NO_ENTRY && m_playerSocketList.size() < 1)
	{
		m_entry = ENTRYSTATE::DELETE_ROOM;
	}
	return m_entry;
}

/// <summary>
/// ルームに入室させる＆シーンへの登録をする
/// </summary>
/// <param name="_soc"></param>
void Room::PushList(SOCKET _soc)
{
	m_playerSocketList.push_back(_soc); 
	m_scene->PushPlayers(Data::Pakets::LobbyData(_soc));
}
