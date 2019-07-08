#pragma once
//// �}���`�X���b�h�쐬�ɕK�v
//#include <thread>
//// �����X���b�h�����̏�����G�������ɖ�肪�N���Ȃ��悤�ɂ��邽�߂̂���
//#include <mutex>

#include "../../Data.h"
#include "../IScene.h"
class ServerNet;

class LoginScene
{
public:
	LoginScene(ServerNet *_server);
	~LoginScene();

	// ����������
	void Start();

	// �I������
	void End();

	// �ڑ�����
	bool Connect(SOCKET _soc);


	// ���M����B
	bool Send(SOCKET _soc);


	// �ǂ̃V�[���ɐ؂�ւ���̂������߂�p�P�b�g���擾����
	Data::Pakets::SwitchingPakets GetSwichPakets() { return m_swich; }

private:
	// �ʐM�N���X
	ServerNet* m_server;
	//// �ڑ����Ă����N���C�A���g�̃\�P�b�g���
	//SOCKET m_soc;
	// ���̃V�[���ɐ؂�ւ����������ۗp�p�P�b�g
	Data::Pakets::SwitchingPakets m_swich;

};

