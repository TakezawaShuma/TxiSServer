#pragma once
#include "../Network\ServerNet.h"

#include "Scenes\Login\LoginScene.h"
#include "Data.h"
#include "Room.h"
#include <map>

class Game
{
public:
	Game();
	~Game();

	// ����������
	bool Initialize();
	// �V�[�����̏���
	bool Login();
	// �Q�[�����̍X�V����
	bool Updata();
	// �Q�[���̏I������
	void Finish();

	// ���[���̐U�蕪��&�쐬
	void AssignToRoom(SOCKET _soc);

private:
	// �T�[�o�[�ւ̃|�C���^�[
	ServerNet* m_server;

	// ���O�C���V�[���̃|�C���^�[
	LoginScene* m_login;

	// ���[���̍ő吔
	int maxRoomSize;

	// ���[���̃��X�g<���[���ԍ�,���[��>
	std::map<int, Room*> m_roomList;

};

