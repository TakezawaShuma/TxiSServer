#include "../../../stdafx.h"
#include "LoginScene.h"
#include "../../../Network/ServerNet.h"


LoginScene::LoginScene(ServerNet *_server):m_server(nullptr)
{
	m_server = _server;
	m_swich = Data::Pakets::SwitchingPakets(Data::Scenes::NON, Data::Scenes::LOBBY);
}

LoginScene::~LoginScene()
{
}

/// <summary>
/// スタート処理
/// </summary>
/// <param name="_players"></param>
void LoginScene:: Start()
{

}

/// <summary>
/// ログインシーンのスタート
/// </summary>
/// <param name="_soc"></param>
/// <param name="_addr"></param>
/// <returns></returns>
bool LoginScene::Connect(SOCKET _soc)
{
	if (Send(_soc)) { return true; }
	return false;
}


/// <summary>
/// 終了処理
/// </summary>
void LoginScene::End()
{
}


/// <summary>
/// 送信処理
/// </summary>
/// <returns></returns>
bool LoginScene::Send(SOCKET _soc)
{
	// 送るデータのバッファ
	char data[1024];

	Data::Pakets::LoginData login = Data::Pakets::LoginData(_soc);
	memcpy(data, &login, sizeof(Data::Pakets::LoginData));
	if (m_server->Send(_soc, data, sizeof(data)))
	{
		// 正しく送れたら
		return true;
	}
	return false;
}

