// ServerTest1.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "Game\Data.h"
#include "Game\Game.h"

#include <WinSock2.h>
#include <iostream>
#include <list>
#include <vector>
#pragma comment(lib,"ws2_32.lib")

#ifdef _MSC_VER
#define DISABLE_C4996   __pragma(warning(push)) __pragma(warning(disable:4996))
#define ENABLE_C4996    __pragma(warning(pop))
#else
#define DISABLE_C4996
#define ENABLE_C4996
#endif

using namespace std;

bool GetHostIp(void)
{
	int i;
	HOSTENT* IpHost;	// ホスト情報を格納しておく構造体
	IN_ADDR inAddr;		// IPアドレスを格納しておく構造体
	char szBuf[256], szIP[16];		// ホスト名 / IPアドレスを格納しておく配列


	// TCPを初期化する
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	DISABLE_C4996		
	// ローカルマシンのホスト名を取得する
	if (gethostname(szBuf, (int)sizeof(szBuf)) != 0)
	{
		// エラー処理
		cout << "ホスト名を取得出来ませんでした。" << endl;
		// エラーナンバーを取得する
		int err = WSAGetLastError();
		cout << "ERRORNUMBER : " << err << endl;
		Data::ErrorState(err);
		return false;
	}
	cout << "HOST Name : " << szBuf << endl;

	// ホスト情報からIPアドレス等の情報を取得する
	IpHost = gethostbyname(szBuf);
	// エラー処理
	if (IpHost == NULL)
	{
		cout << "IPアドレスを取得できませんでした" << endl;
		// エラーナンバーを取得する
		int err = WSAGetLastError();
		cout << "ERRORNUMBER : " << err << endl;
		Data::ErrorState(err);
		return false;
	}	
	// IPアドレスを取得
	for (i = 0; IpHost->h_addr_list[i]; i++)
	{
		memcpy(&inAddr, IpHost->h_addr_list[i], 4);
		strcpy_s(szIP, sizeof(szIP), inet_ntoa(inAddr));
		cout << "IP Adress : " << szIP << endl;
	}
	ENABLE_C4996
	return true;
}

int main()
{

	// ホストIPを獲得＆表示
	for (int i = 0; i < 5; i++)
	{
		if (GetHostIp() == false)
		{
			continue;
		}
		break;
	}
	Game* game = new Game();
	bool gameInitFlag = false;

	// 終了条件:イニシャライズが正常に実行されたら
	// ゲームの初期化
	do { gameInitFlag = game->Initialize(); } while (gameInitFlag != true);

	// 更新
	while (true)
	{
		// プレイシーンの更新
		if (!game->Updata()) break;
		
	}

	// 終了する
	game->Finish();

	delete game;

	return 0;
}
