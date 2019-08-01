#include "../stdafx.h"
#include "ServerNet.h"
#include <stdio.h>
#include <iostream>

#define PORT 10000


/// <summary>
/// �R���X�g���N�^
/// WinSock2�̏�����
/// </summary>
ServerNet::ServerNet()
{
	WSADATA wsaData;

	// WinSock�̃o�[�W�������聕������
	if (WSAStartup(MAKEWORD(2, 0), &wsaData))
	{
		std::cout << "WinSock �̏������Ɏ��s���܂���" << std::endl;
	}
}

ServerNet::~ServerNet()
{

}



/// <summary>
/// �\�P�b�g�̍쐬���o�C���h
/// </summary>
/// <returns>���� = 0 / ���s = -1</returns>
int ServerNet::CreateSocket()
{
	// �\�P�b�g�̍쐬
	m_soc = socket(AF_INET, SOCK_STREAM, 0);
	if (m_soc == INVALID_SOCKET)
	{
		std::cout << "�\�P�b�g�쐬���ɃG���[" << std::endl;
		return -1;
	}

	// �\�P�b�g�̐ݒ�
	memset(&m_addr, 0, sizeof(m_addr));
	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons(PORT); 
	m_addr.sin_addr.s_addr = INADDR_ANY;

	// �\�P�b�g�̃o�C���h
	if (bind(m_soc, (sockaddr*)&m_addr, sizeof(m_addr)) != 0)
	{
		std::cout << "�\�P�b�g�̃o�C���h�Ɏ��s���܂���" << std::endl;
		return -1;
	}
	return 0;
}

/// <summary>
/// �ڑ��󂯓���
/// </summary>
/// <returns> ���� = 0 / ���s = -1 </returns>
bool ServerNet::Listen()
{
	if (listen(m_soc, 5))
	{
		std::cout << "�ڑ������Ɏ��s���܂���" << std::endl;
		return false;
	}
	u_long val = 1;
	ioctlsocket(m_soc, FIONBIO, &val);

	return true;
}

/// <summary>
/// �ڑ�����
/// </summary>
/// <param name="_soc">�ڑ���̃\�P�b�g�����Ēu��</param>
/// <param name="_destAddr">�ڑ�����A�h���X</param>
/// <returns> ���� = 0 / ���s = -1</returns>
bool ServerNet::Accept(SOCKET* _soc, sockaddr_in* _destAddr)
{
	int clientAddrSize = sizeof(sockaddr_in);

	*_soc = accept(m_soc, (sockaddr*)_destAddr, &clientAddrSize);
	if (*_soc != INVALID_SOCKET)
	{
		u_long val = 1;
		ioctlsocket(*_soc, FIONBIO, &val);
		return true;
	}
	return false;
}

/// <summary>
/// ��M
/// </summary>
/// <param name="_soc">�N���C�A���g�\�P�b�g</param>
/// <param name="_data">�����Ă���f�[�^������</param>
/// <param name="_dataSize">�����Ă���f�[�^�̃T�C�Y</param>
/// <param name="_recvSize">�����Ă����f�[�^�̃T�C�Y�����Ă���</param>
/// <returns></returns>
RECV_CONNECTION ServerNet::Recv(SOCKET _soc, char * _data, int _dataSize, int * _recvSize)
{
	RECV_CONNECTION state = RECV_CONNECTION::RECV_NON_DATA;
	int n;
	while (true)
	{	
		n = recv(_soc, _data, _dataSize, 0);

		if (n < 1)
		{
			// �f�[�^�Ȃ�
			if (WSAGetLastError() == WSAEWOULDBLOCK)	break;
			// �ؒf
			else { state = RECV_CONNECTION::RECV_FAILURE;		break; }
		}
		else { state = RECV_CONNECTION::RECV_SUCCESS;	*_recvSize = n;
		}
	}
	return state;
}

/// <summary>
/// ���M
/// </summary>
/// <param name="_sendSoc">���鑊��̃\�P�b�g</param>
/// <param name="_sendData">����f�[�^</param>
/// <param name="_sendDataSize">����f�[�^�T�C�Y</param>
/// <returns>���M����</returns>
bool ServerNet::Send(SOCKET _sendSoc,const char& _sendData, int _sendDataSize)
{
	int a = send(_sendSoc, _sendData, _sendDataSize, 0);

	if (a == SOCKET_ERROR)
	{
		std::cout << "���M�Ɏ��s���܂��� : ERRORNo." << GetLastError()<<"	" << std::ends;
		Data::ErrorState(GetLastError());
		return false;
	}
	return true;
}

/// <summary>
/// �ؒf
/// </summary>
/// <param name="_destSocket">����\�P�b�g</param>
/// <returns></returns>
bool ServerNet::Close()
{
	if (closesocket(m_soc) != 0)
	{
		std::cout << "�ؒf���s" << std::endl;
		return false;
	}
		return true;
}

