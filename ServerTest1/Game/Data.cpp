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
			std::cout << "関数の呼び出しに割込みがありました。" << std::endl; break;
		case 10013:
			std::cout << "アクセスは拒否されました。" << std::endl; break;
		case 10014:
			std::cout << "アドレスが正しくありません。" << std::endl; break;
		case 10022:
			std::cout << "無効な引数です。" << std::endl; break;
		case 10024:
			std::cout << "開いているファイルが多すぎます。" << std::endl; break;
		case 10035:
			std::cout << "リソースが一時的に利用できなくなっています。" << std::endl; break;
		case 10036:
			std::cout << "操作は現在実行中です。" << std::endl; break;
		case 10037:
			std::cout << "操作は既に実行中です。" << std::endl; break;
		case 10038:
			std::cout << "非ソケットに対してソケット操作を試みました。" << std::endl; break;
		case 10039:
			std::cout << "送信先のアドレスが必要です。" << std::endl; break;
		case 10040:
			std::cout << "メッセージが長すぎます。" << std::endl; break;
		case 10041:
			std::cout << "プロトコルの種類がソケットに対して正しくありません。" << std::endl; break;
		case 10042:
			std::cout << "プロトコルのオプションが正しくありません。" << std::endl; break;
		case 10043:
			std::cout << "プロトコルがサポートされていません。" << std::endl; break;
		case 10044:
			std::cout << "サポートされていないソケットの種類です。" << std::endl; break;
		case 10045:
			std::cout << "操作がソケット上でサポートされていません。" << std::endl; break;
		case 10046:
			std::cout << "プロトコル ファミリがサポートされていません。" << std::endl; break;
		case 10047:
			std::cout << "プロトコル ファミリはアドレス ファミリをサポートしていません。" << std::endl; break;
		case 10048:
			std::cout << "アドレスは既に使用中です。" << std::endl; break;
		case 10049:
			std::cout << "要求されたアドレスを割り当てられません。" << std::endl; break;
		case 10050:
			std::cout << "ネットワークがダウンしています。" << std::endl; break;
		case 10051:
			std::cout << "ICMP ネットワークに到達できません。" << std::endl; break;
		case 10052:
			std::cout << "ネットワークがリセットされたため切断されました。" << std::endl; break;
		case 10053:
			std::cout << "ソフトウェアによって接続が中止されました。" << std::endl; break;
		case 10054:
			std::cout << "ピアによって接続がリセットされました。" << std::endl; break;
		case 10055:
			std::cout << "バッファ領域がサポートされていません。" << std::endl; break;
		case 10056:
			std::cout << "ソケットは既に接続されています。" << std::endl; break;
		case 10057:
			std::cout << "ソケットが接続されていません。" << std::endl; break;
		case 10058:
			std::cout << "ソケットのシャットダウン後に送信できません。" << std::endl; break;
		case 10060:
			std::cout << "接続がタイムアウトになりました。" << std::endl; break;
		case 10061:
			std::cout << "接続が拒否されました。" << std::endl; break;
		case 10064:
			std::cout << "ホストがダウンしています。" << std::endl; break;
		case 10065:
			std::cout << "ホストに到達するためのルートがありません。" << std::endl; break;
		case 10067:
			std::cout << "プロセスが多すぎます。" << std::endl; break;

		default:
			break;
		}
	}
}