#pragma once
#ifndef DATABASE_DEFINED
#define DATABASE_DEFINED

#include<list>
#include <vector>
#include "../Data.h"

class DB
{
public:
	DB() {}
	~DB() {}
	
	static DB* GetInstans(); 

	std::vector<Player>* GetPlayerList() { return &m_playerList; }


	void PushDelete(Player _player) { m_delList.push_back(_player); }


private:
	static DB* m_instans;

	// 全プレイヤー
	std::vector<Player> m_playerList;
	std::vector<Player> m_delList;

};

#endif // !DATABASE_DEFINED
