#pragma once

#include <list>
#include "../../Network/ServerNet.h"

class IScene
{
public:
	// ����������
	virtual void Start(std::list<SOCKET> _players) = 0;
	/// <summary>
	/// �X�V����
	/// </summary>
	/// <returns>���̃t���[���̃V�[����Ԃ�����</returns>
	virtual Data::Scenes::SCENETYPE Update() = 0;
	virtual void End() = 0;

	// ��M���ɍs������
	virtual void Receive(char* _data) = 0;
	// ���M����B
	virtual bool Send() = 0;

	// �V�[�����̃v���C���[��ǉ�����B
	virtual void PushPlayers(Data::Pakets::IPaketData _player) = 0;
	
	// ������̃��X�g���l������
	virtual std::list<Data::Pakets::IPaketData> GetSendList() = 0;

	// �ǂ̃V�[���ɐ؂�ւ���̂������߂�p�P�b�g���擾����
	virtual Data::Pakets::SwitchingPakets GetSwichPakets() = 0;

protected:

};

