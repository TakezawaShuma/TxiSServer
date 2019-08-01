#pragma once
#include "../Game/Data.h"
// マルチスレッド作成に必要
#include <thread>
// 複数スレッドから一つの処理を触った時に問題が起きないようにするためのもの
#include <mutex>



#pragma comment (lib,"ws2_32.lib")


// 受信状態
enum RECV_CONNECTION
{
	// 接続成功
	RECV_SUCCESS,
	// 接続失敗/エラー
	RECV_FAILURE,
	// データが来ていない
	RECV_NON_DATA
};

class ServerNet
{
public:
	ServerNet();
	~ServerNet();
	
	// ソケットの作成＆バインド
	int CreateSocket();
	// 接続用意
	bool Listen();
	// 接続確認
	bool Accept(SOCKET* _soc, sockaddr_in* _destAddr);
	// 受信
	RECV_CONNECTION Recv(SOCKET _soc, char* _data, int _dataSize, int* _recvSize);
	// 送信
	bool Send(SOCKET _soc, const char& _data, int _dataSize);
	// 切断
	bool Close();
private:

	SOCKET m_soc;

	sockaddr_in m_addr;

};


