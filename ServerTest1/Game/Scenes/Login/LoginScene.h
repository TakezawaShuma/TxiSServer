#pragma once
//// マルチスレッド作成に必要
//#include <thread>
//// 複数スレッドから一つの処理を触った時に問題が起きないようにするためのもの
//#include <mutex>

#include "../../Data.h"
#include "../IScene.h"
class ServerNet;

class LoginScene
{
public:
	LoginScene(ServerNet *_server);
	~LoginScene();

	// 初期化処理
	void Start();

	// 終了処理
	void End();

	// 接続処理
	bool Connect(SOCKET _soc);


	// 送信する。
	bool Send(SOCKET _soc);


	// どのシーンに切り替えるのかを決めるパケットを取得する
	Data::Pakets::SwitchingPakets GetSwichPakets() { return m_swich; }

private:
	// 通信クラス
	ServerNet* m_server;
	//// 接続してきたクライアントのソケット情報
	//SOCKET m_soc;
	// 次のシーンに切り替えを許可＆成否用パケット
	Data::Pakets::SwitchingPakets m_swich;

};

