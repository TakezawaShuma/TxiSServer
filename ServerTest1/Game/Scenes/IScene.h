#pragma once

#include <list>
#include "../../Network/ServerNet.h"

class IScene
{
public:
	// 初期化処理
	virtual void Start(std::list<SOCKET> _players) = 0;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <returns>次のフレームのシーンを返したい</returns>
	virtual Data::Scenes::SCENETYPE Update() = 0;

	// 終了処理
	virtual void End() = 0;

	// 受信時に行う処理
	virtual void Receive(char* _data) = 0;

	virtual void Send() = 0;


protected:

	// シーン毎のプレイヤーを追加する。
	virtual void PushPlayers(Data::Pakets::IPaketData _player) = 0;

	virtual void OffTheCutter(SOCKET _soc) = 0;

	// シーン毎のプレイヤーを削除する。
	virtual void DeleteList() = 0;
	
	// 送る情報のリストを獲得する
	virtual Data::Pakets::IPaketData* GetSendList(int _i) = 0;

	// どのシーンに切り替えるのかを決めるパケットを取得する
	virtual Data::Pakets::SwitchingPakets GetSwichPakets() = 0;
};

