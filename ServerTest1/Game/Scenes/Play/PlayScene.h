#pragma once
#include <memory>
#include "../../Data.h"
#include "../../../Utility/Utility.h"
#include "../IScene.h"


class PlayScene:public IScene
{
public:
	PlayScene();
	~PlayScene();

	// 開始処理
	void Start(std::list<SOCKET> _players) override;
	// 更新処理
	Data::Scenes::SCENETYPE Update() override;
	// 終了処理
	void End() override;

	// 受信時に行う処理
	void Receive(char* _data) override;
	// 全プレイヤー情報を送信する。
	bool Send() override;

	// 自分と他の人の弾
	bool HitChack(Utility::Vector2 _v1, Utility::Vector2 _v2);


	bool Execution(Data::Pakets::PlayData* _p1, Data::Pakets::PlayData* _p2);


	// シーン毎のプレイヤーを追加する。
	void PushPlayers(Data::Pakets::IPaketData _player) { m_playerList.push_back(_player); }

	void DeleteList() override;

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
	Data::Pakets::PlayData SetStartPosition(SOCKET _soc, Utility::Vector2 _vec);

	Utility::Vector2 RandomPosition();

private:



	// シーンにいるプレイヤー情報
	std::list<Data::Pakets::PlayData> m_playerList;

	// クライアントに送る情報を全て入れる
	std::list < Data::Pakets::IPaketData> m_sendList;

	// 次のシーンに切り替えを許可＆成否用パケット
	Data::Pakets::SwitchingPakets m_swich;

	// 現在の時間
	u_int m_time;
};