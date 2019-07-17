#include "../../../stdafx.h"
#include "PlayScene.h"
#include <random>


PlayScene::PlayScene():m_time(0)
{
}

PlayScene::~PlayScene()
{
}

/// <summary>
/// 初期化
/// </summary>
void PlayScene::Start(std::list<SOCKET> _players)
{
	m_swich = Data::Pakets::SwitchingPakets(Data::Scenes::PLAY, Data::Scenes::RESULT);

	int count = 0;

	// 初期のプレイヤーの初期位置を決める
	for each (SOCKET var in _players)
	{
		Data::Pakets::PlayData tmp = Data::Pakets::PlayData(var);

		m_playerList.push_back(tmp);
	}
	m_time = (u_int)time(0);
	srand(m_time);

}

/// <summary>
/// データの更新
/// </summary>
Data::Scenes::SCENETYPE PlayScene::Update()
{
	// あたり判定


	return Data::Scenes::SCENETYPE::NON;
}

/// <summary>
/// 終了
/// </summary>
void PlayScene::End()
{
}

/// <summary>
/// 受信(データを更新)
/// </summary>
/// <param name="_data"></param>
void PlayScene::Receive(char * _data)
{
	Data::Pakets::PlayData playData;
	memcpy(&playData, _data, sizeof(Data::Pakets::PlayData));


	for (std::list<Data::Pakets::PlayData>::iterator itr = m_playerList.begin(); itr != m_playerList.end(); itr++)
	{
		if (itr->socket = playData.socket) { *itr = playData; break; }
	}
}

//bool PlayScene::Send()
//{
//	return false;
//}

// あたり判定
bool PlayScene::HitChack(Utility::Vector2 _v1, Utility::Vector2 _v2)
{
	// 点 _v2と中心 _v1半径16とのあたり判定
	if (Utility::Collistions::HitCircleAndPoint(_v2, _v1, 16))
	{
		return true;
	}
	return false;
}

/// <summary>
/// Play の更新(あたり判定＆ダメージ)
/// </summary>
/// <param name="_p1"></param>
/// <param name="_p2"></param>
/// <returns></returns>
bool PlayScene::Execution(Data::Pakets::PlayData* _p1, Data::Pakets::PlayData* _p2)
{
	bool flag = false;
	for (int i = 0; i < BULLET_NUM; i++)
	{
		// あたり判定
		// p1自身とp2の弾とのあたり判定
		if (_p2->bulletActive[i] == true)
		{
			if (this->HitChack(Utility::Vector2(_p1->posX, _p1->posY), Utility::Vector2(_p2->bulletPosX[i], _p2->bulletPosY[i])))
			{
  				_p2->bulletActive[i] = false;
				_p1->hitPoint -= 1;
				flag = true;
			}
		}
	}

	return flag;
}

Data::Pakets::PlayData PlayScene::SetStartPosition(SOCKET _soc, Utility::Vector2 _vec)
{
	Data::Pakets::PlayData tmp = Data::Pakets::PlayData(_soc);

	return tmp;
}

Utility::Vector2 PlayScene::RandomPosition()
{
	Utility::Vector2 ret;
	int buf = (rand() % 2000) - 1000;
	ret.x = buf;
	buf = (rand() % 2000) - 1000;
	ret.y = buf;

	return ret;
}

