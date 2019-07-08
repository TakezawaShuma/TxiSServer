#pragma once
#include "../Network\ServerNet.h"

#include "Scenes\Login\LoginScene.h"
#include "Data.h"
#include "Room.h"
#include <map>

class Game
{
public:
	Game();
	~Game();

	// 初期化処理
	bool Initialize();
	// シーン毎の処理
	bool Login();
	// ゲーム中の更新処理
	bool Updata();
	// ゲームの終了処理
	void Finish();

	// ルームの振り分け&作成
	void AssignToRoom(SOCKET _soc);

private:
	// サーバーへのポインター
	ServerNet* m_server;

	// ログインシーンのポインター
	LoginScene* m_login;

	// ルームの最大数
	int maxRoomSize;

	// ルームのリスト<ルーム番号,ルーム>
	std::map<int, Room*> m_roomList;

};

