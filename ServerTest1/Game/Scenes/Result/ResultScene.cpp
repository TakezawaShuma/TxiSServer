#include "../../../stdafx.h"
#include "ResultScene.h"

ResultScene::ResultScene()
{
}

ResultScene::~ResultScene()
{
}

void ResultScene::Start(std::list<SOCKET> _players)
{
	m_swich = Data::Pakets::SwitchingPakets(Data::Scenes::RESULT, Data::Scenes::NON);
}

Data::Scenes::SCENETYPE ResultScene::Update()
{
	return Data::Scenes::SCENETYPE();
}

void ResultScene::End()
{
}

void ResultScene::Receive(char * _data)
{
}

bool ResultScene::Send()
{
	return false;
}

void ResultScene::DeleteList()
{
}
