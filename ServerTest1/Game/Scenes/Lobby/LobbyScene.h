#pragma once


#include "../../Data.h"
#include "../IScene.h"
#include <list>


class LobbyScene:public IScene
{
public:
	LobbyScene();
	~LobbyScene();

	// 開始処理
	void Start(std::list<SOCKET> _players) override;
	// 更新処理
	Data::Scenes::SCENETYPE Update() override;
	// 終了処理
	void End() override;

	// 受信時に行う処理
	void Receive(char* _data) override;
	//// 全プレイヤー情報を送信する。
	//bool Send() override;
	
	// ロビーにいる人数を返す。
	int SendSize() { return m_playerList.size(); }


	// シーン毎のプレイヤーを追加する。
	void PushPlayers(Data::Pakets::IPaketData _player)
	{
		m_playerList.push_back(_player); m_sendList = ConvertListType(m_playerList);
	}
	
	// 送る情報のリストを獲得する
	std::list<Data::Pakets::IPaketData> GetSendList()
	{
		std::list < Data::Pakets::IPaketData> baf = m_sendList;
		m_sendList.clear();
		return baf;
	}

	// どのシーンに切り替えるのかを決めるパケットを取得する
	Data::Pakets::SwitchingPakets GetSwichPakets() { return m_swich; }

private:
	// 派生クラスのリストを基底クラスのリストに代入させる
	std::list<Data::Pakets::IPaketData> ConvertListType(std::list<Data::Pakets::LobbyData> _list);
private:

	// シーンにいるプレイヤー情報
	std::list<Data::Pakets::LobbyData> m_playerList;
	
	// クライアントに送る情報を全て入れる
	std::list < Data::Pakets::IPaketData> m_sendList;

	// 送られてきたデータを一時的に保存する
	std::list < Data::Pakets::LobbyData> m_dataBufferList;

	// 次のシーンに切り替えを許可＆成否用パケット
	Data::Pakets::SwitchingPakets m_swich;

	// シーンを切り替えるとき使う
	Data::Scenes::SCENETYPE m_ret;
};

