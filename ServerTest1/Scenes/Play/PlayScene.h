#pragma once
#include <memory>
#include "../../Game/DataBase.h"
#include "../../Utility/Utility.h"



class Play
{
public:
	Play();
	~Play();


	// 自分と他の人の弾
	bool HitChack(Utility::Vector2 _v1, Utility::Vector2 _v2);

	bool Execution(Player* _p1, Player* _p2);
	int SceneFlag(Player p);

private:


private:

	int m_marginalTime;
};