// ServerTest1.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
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
	HOSTENT* IpHost;	// �z�X�g�����i�[���Ă����\����
	IN_ADDR inAddr;		// IP�A�h���X���i�[���Ă����\����
	char szBuf[256], szIP[16];		// �z�X�g�� / IP�A�h���X���i�[���Ă����z��


	// TCP������������
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	DISABLE_C4996		
	// ���[�J���}�V���̃z�X�g�����擾����
	if (gethostname(szBuf, (int)sizeof(szBuf)) != 0)
	{
		// �G���[����
		cout << "�z�X�g�����擾�o���܂���ł����B" << endl;
		// �G���[�i���o�[���擾����
		int err = WSAGetLastError();
		cout << "ERRORNUMBER : " << err << endl;
		Data::ErrorState(err);
		return false;
	}
	cout << "HOST Name : " << szBuf << endl;

	// �z�X�g��񂩂�IP�A�h���X���̏����擾����
	IpHost = gethostbyname(szBuf);
	// �G���[����
	if (IpHost == NULL)
	{
		cout << "IP�A�h���X���擾�ł��܂���ł���" << endl;
		// �G���[�i���o�[���擾����
		int err = WSAGetLastError();
		cout << "ERRORNUMBER : " << err << endl;
		Data::ErrorState(err);
		return false;
	}	
	// IP�A�h���X���擾
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

	// �z�X�gIP���l�����\��
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

	// �I������:�C�j�V�����C�Y������Ɏ��s���ꂽ��
	// �Q�[���̏�����
	do { gameInitFlag = game->Initialize(); } while (gameInitFlag != true);

	// �X�V
	while (true)
	{
		// �v���C�V�[���̍X�V
		if (!game->Updata()) break;
		
	}

	// �I������
	game->Finish();

	delete game;

	return 0;
}
