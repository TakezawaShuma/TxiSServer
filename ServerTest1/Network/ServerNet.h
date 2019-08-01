#pragma once
#include "../Game/Data.h"
// �}���`�X���b�h�쐬�ɕK�v
#include <thread>
// �����X���b�h�����̏�����G�������ɖ�肪�N���Ȃ��悤�ɂ��邽�߂̂���
#include <mutex>



#pragma comment (lib,"ws2_32.lib")


// ��M���
enum RECV_CONNECTION
{
	// �ڑ�����
	RECV_SUCCESS,
	// �ڑ����s/�G���[
	RECV_FAILURE,
	// �f�[�^�����Ă��Ȃ�
	RECV_NON_DATA
};

class ServerNet
{
public:
	ServerNet();
	~ServerNet();
	
	// �\�P�b�g�̍쐬���o�C���h
	int CreateSocket();
	// �ڑ��p��
	bool Listen();
	// �ڑ��m�F
	bool Accept(SOCKET* _soc, sockaddr_in* _destAddr);
	// ��M
	RECV_CONNECTION Recv(SOCKET _soc, char* _data, int _dataSize, int* _recvSize);
	// ���M
	bool Send(SOCKET _soc, const char& _data, int _dataSize);
	// �ؒf
	bool Close();
private:

	SOCKET m_soc;

	sockaddr_in m_addr;

};


