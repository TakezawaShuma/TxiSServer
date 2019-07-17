#pragma once
#include "../IScene.h"
#include "../../Data.h"
#include "../../../Utility/Utility.h"



class ResultScene:public IScene
{
public:
	ResultScene();
	~ResultScene();

	// 開始処理
	void Start(std::list<SOCKET> _players) override;
	// 更新処理
	Data::Scenes::SCENETYPE Update() override;
	// 終了処理
	void End() override;

	// 受信時に行う処理
	void Receive(char* _data) override;
	// 全プレイヤー情報を送信する。
	//bool Send() override;

	// シーン毎のプレイヤーを追加する。
	void PushPlayers(Data::Pakets::IPaketData _player) { m_playerList.push_back(_player); }

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
	
	// シーンにいるプレイヤー情報
	std::list<Data::Pakets::ResultData> m_playerList;

	// クライアントに送る情報を全て入れる
	std::list < Data::Pakets::IPaketData> m_sendList;

	// 次のシーンに切り替えを許可＆成否用パケット
	Data::Pakets::SwitchingPakets m_swich;
};
