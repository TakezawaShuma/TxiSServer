#include "../stdafx.h"
#include "Data.h"
#include <iostream>


namespace Data
{
	void ErrorState(int errorNumber)
	{
		switch (errorNumber)
		{
		case 10004:
			std::cout << "�֐��̌Ăяo���Ɋ����݂�����܂����B" << std::endl; break;
		case 10013:
			std::cout << "�A�N�Z�X�͋��ۂ���܂����B" << std::endl; break;
		case 10014:
			std::cout << "�A�h���X������������܂���B" << std::endl; break;
		case 10022:
			std::cout << "�����Ȉ����ł��B" << std::endl; break;
		case 10024:
			std::cout << "�J���Ă���t�@�C�����������܂��B" << std::endl; break;
		case 10035:
			std::cout << "���\�[�X���ꎞ�I�ɗ��p�ł��Ȃ��Ȃ��Ă��܂��B" << std::endl; break;
		case 10036:
			std::cout << "����͌��ݎ��s���ł��B" << std::endl; break;
		case 10037:
			std::cout << "����͊��Ɏ��s���ł��B" << std::endl; break;
		case 10038:
			std::cout << "��\�P�b�g�ɑ΂��ă\�P�b�g��������݂܂����B" << std::endl; break;
		case 10039:
			std::cout << "���M��̃A�h���X���K�v�ł��B" << std::endl; break;
		case 10040:
			std::cout << "���b�Z�[�W���������܂��B" << std::endl; break;
		case 10041:
			std::cout << "�v���g�R���̎�ނ��\�P�b�g�ɑ΂��Đ���������܂���B" << std::endl; break;
		case 10042:
			std::cout << "�v���g�R���̃I�v�V����������������܂���B" << std::endl; break;
		case 10043:
			std::cout << "�v���g�R�����T�|�[�g����Ă��܂���B" << std::endl; break;
		case 10044:
			std::cout << "�T�|�[�g����Ă��Ȃ��\�P�b�g�̎�ނł��B" << std::endl; break;
		case 10045:
			std::cout << "���삪�\�P�b�g��ŃT�|�[�g����Ă��܂���B" << std::endl; break;
		case 10046:
			std::cout << "�v���g�R�� �t�@�~�����T�|�[�g����Ă��܂���B" << std::endl; break;
		case 10047:
			std::cout << "�v���g�R�� �t�@�~���̓A�h���X �t�@�~�����T�|�[�g���Ă��܂���B" << std::endl; break;
		case 10048:
			std::cout << "�A�h���X�͊��Ɏg�p���ł��B" << std::endl; break;
		case 10049:
			std::cout << "�v�����ꂽ�A�h���X�����蓖�Ă��܂���B" << std::endl; break;
		case 10050:
			std::cout << "�l�b�g���[�N���_�E�����Ă��܂��B" << std::endl; break;
		case 10051:
			std::cout << "ICMP �l�b�g���[�N�ɓ��B�ł��܂���B" << std::endl; break;
		case 10052:
			std::cout << "�l�b�g���[�N�����Z�b�g���ꂽ���ߐؒf����܂����B" << std::endl; break;
		case 10053:
			std::cout << "�\�t�g�E�F�A�ɂ���Đڑ������~����܂����B" << std::endl; break;
		case 10054:
			std::cout << "�s�A�ɂ���Đڑ������Z�b�g����܂����B" << std::endl; break;
		case 10055:
			std::cout << "�o�b�t�@�̈悪�T�|�[�g����Ă��܂���B" << std::endl; break;
		case 10056:
			std::cout << "�\�P�b�g�͊��ɐڑ�����Ă��܂��B" << std::endl; break;
		case 10057:
			std::cout << "�\�P�b�g���ڑ�����Ă��܂���B" << std::endl; break;
		case 10058:
			std::cout << "�\�P�b�g�̃V���b�g�_�E����ɑ��M�ł��܂���B" << std::endl; break;
		case 10060:
			std::cout << "�ڑ����^�C���A�E�g�ɂȂ�܂����B" << std::endl; break;
		case 10061:
			std::cout << "�ڑ������ۂ���܂����B" << std::endl; break;
		case 10064:
			std::cout << "�z�X�g���_�E�����Ă��܂��B" << std::endl; break;
		case 10065:
			std::cout << "�z�X�g�ɓ��B���邽�߂̃��[�g������܂���B" << std::endl; break;
		case 10067:
			std::cout << "�v���Z�X���������܂��B" << std::endl; break;

		default:
			break;
		}
	}
}