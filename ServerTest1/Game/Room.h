#pragma once
#include<memory>
#include <list>
#include <map>

#include "Data.h"
#include "Scenes/Lobby/LobbyScene.h"
#include "Scenes/Play/PlayScene.h"
#include "Scenes/Result/ResultScene.h"

#include "../Network/ServerNet.h"


class Room
{
public:
	static const int MAX_ROOM_MEMBER_NUM = 10;

	enum ENTRYSTATE
	{
		ENTRY_PERMIT,	// 入室許可
		NO_ENTRY,		// 入室禁止
		DELETE_ROOM,	// ルーム削除
	};

public:
	Room();
	Room(SOCKET _soc);
	~Room();

	// 情報の受け取り
	void Receive();


	// ルームの初期処理
	void Start(ServerNet* _server);

	// ルームの更新処理
	void Update();

	// ルームの終了処理
	void End();

	// シーンの切り替え
	bool ChangeRoomScene(Data::Scenes::SCENETYPE _nextScene);

	// 情報の送信
	bool SendRoomData(char* _data);

	// シーンから送る情報を貰ってくる
	void GetSendData();

	// プレイヤーの削除
	ENTRYSTATE DeletePlayers();

	// ルームのリストに追加
	void PushList(SOCKET _soc);

	// ルームのシーン状態更新
	void SceneChange(Data::Scenes::Scene _nextScene) { m_roomScene = _nextScene; }

	// 入室しているプレイヤーの数
	int GetPlayerNum() { return m_playerSocketList.size(); }

	// 入室許可を出しているか？
	int GetEntry() { return m_entry; }

	// 現在のシーンを取得
	Data::Scenes::Scene GetNowScene() { return m_roomScene; }


private:
	// サーバーへのポインタ
	ServerNet* m_server;

	// ルームのシーンを管理する
	Data::Scenes::Scene m_roomScene;

	// ゲームのシーンをを管理するポインタ
	std::unique_ptr<IScene> m_scene;


	// ルームに入っているプレイヤー
	std::list<SOCKET> m_playerSocketList;

	// 送るプレイヤー情報
	std::list<Data::Pakets::IPaketData> m_sendList;

	// 切り替え完了した人数をカウント
	std::list<SOCKET> m_swithCount;

	// シーン切り替えフラグ
	bool m_changeSceneFlag;

	// 削除するプレイヤーの情報
	std::list<SOCKET> m_delList;


	// 入室の可不可
	// 0 = 可能・1 = 不可能・ 2 = ルーム削除 
	ENTRYSTATE m_entry;
};

