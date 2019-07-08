#pragma once
#ifndef DATA_DEFINED
#define DATA_DEFINED

#include <WinSock2.h>
#include "../Utility/Utility.h"

#define BULLET_NUM 25

namespace Data
{
	namespace Scenes
	{
		enum SCENETYPE
		{
			//接続時
			LOGIN,
			// タイトルシーン
			TITLE,
			// ロビーシーン
			LOBBY,
			// プレイシーン
			PLAY,
			// リザルトシーン
			RESULT,
			// 戻るor切断
			BACK,

			// シーンゼロ
			NON,
		};


		struct Scene
		{
			Scene() { nowScene = SCENETYPE::NON; nextScene = SCENETYPE::NON; }
			SCENETYPE nowScene;
			SCENETYPE nextScene;
		};

	}

	namespace Pakets
	{
		enum PaketID
		{
			PAKET_BASE = (int)0,
			LOGIN_PAKET = (int)100,
			LOBBY_PAKET = (int)200,
			PLAY_PAKET = (int)300,
			RESULT_PAKET = (int)400,

			SWITCH_PAKET = (int)500,
		};



		// 基底
		struct IPaketData
		{
			// パケットのID
			PaketID paketId;
			// シーン管理
			Scenes::Scene scene;
			// プレイヤーID
			SOCKET socket;
		};

		/// <summary>
		/// ログインシーンのデータ
		/// </summary>
		/// パケットID = 100
		struct LoginData : IPaketData
		{
			LoginData() { scene.nowScene = Scenes::SCENETYPE::LOGIN; scene.nextScene = Scenes::SCENETYPE::LOBBY; }
			LoginData(SOCKET _soc) { scene.nowScene = Scenes::SCENETYPE::LOGIN; scene.nextScene = Scenes::SCENETYPE::LOBBY; socket = _soc; }
		};

		/// <summary>
		/// ロビーでやり取りするデータ
		/// </summary>
		/// パケットID = 200
		struct LobbyData :IPaketData
		{

			LobbyData() { scene.nowScene = Scenes::SCENETYPE::LOBBY; scene.nextScene = Scenes::SCENETYPE::NON; paketId = PaketID::LOBBY_PAKET; ready = false; }
			LobbyData(SOCKET _soc) { scene.nowScene = Scenes::SCENETYPE::LOBBY; scene.nextScene = Scenes::SCENETYPE::NON; paketId = PaketID::LOBBY_PAKET; socket = _soc; ready = false; }
			LobbyData(IPaketData _i) { scene.nowScene = _i.scene.nowScene; scene.nextScene = _i.scene.nextScene; paketId = _i.paketId; socket = _i.socket; ready = false; }

			// 準備完了
			bool ready;
		};

		/// <summary>
		/// プレイでやり取りするデータ
		/// </summary>
		/// パケットID = 300
		struct PlayData :IPaketData
		{
			PlayData()
			{
				scene.nowScene = Scenes::SCENETYPE::PLAY; 
				scene.nextScene = Scenes::SCENETYPE::NON; 
				paketId = PaketID::PLAY_PAKET; 
			}

			PlayData(SOCKET _soc)
			{ 
				scene.nowScene = Scenes::SCENETYPE::PLAY;
				scene.nextScene = Scenes::SCENETYPE::NON;
				paketId = PaketID::PLAY_PAKET;
				socket = _soc;
			}
			PlayData(IPaketData _i) 
			{ 
				scene.nowScene = _i.scene.nowScene;
				scene.nextScene = _i.scene.nextScene;
				paketId = _i.paketId;
				socket = _i.socket; 
			}
			void Start(Utility::Vector2 _vec)
			{
				hitPoint = 5;
				posX = _vec.x;
				posY = _vec.y;
				angle = 0;
			}
			// 体力
			int hitPoint;
			// 位置
			float posX;
			float posY;

			// 向いている角度
			float angle;

			// 弾の数＆位置＆有無効
			float bulletPosX[BULLET_NUM];
			float bulletPosY[BULLET_NUM];
			bool bulletActive[BULLET_NUM];
		};

		
		/// <summary>
		/// リザルトでやり取りするデータ
		/// </summary>
		struct ResultData:IPaketData
		{
			ResultData() { scene.nowScene = Scenes::SCENETYPE::PLAY; scene.nextScene = Scenes::SCENETYPE::NON; paketId = PaketID::RESULT_PAKET; }
			ResultData(SOCKET _soc) { scene.nowScene = Scenes::SCENETYPE::PLAY; scene.nextScene = Scenes::SCENETYPE::NON; paketId = PaketID::RESULT_PAKET; socket = _soc; }
			ResultData(IPaketData _i){ scene.nowScene = _i.scene.nowScene; scene.nextScene = _i.scene.nextScene; paketId = PaketID::RESULT_PAKET; socket = _i.socket; }

			// 取り合えず
			bool ready;

		};


		/// <summary>
		/// 切り替えを要請又は完了時に送る(受ける)データ
		/// </summary>
		/// パケットID = 500
		struct SwitchingPakets :IPaketData
		{
			SwitchingPakets() { scene.nowScene = Scenes::SCENETYPE::NON; scene.nextScene = Scenes::SCENETYPE::NON; paketId = PaketID::SWITCH_PAKET; socket = (SOCKET)-1; }
			SwitchingPakets(Scenes::SCENETYPE _now, Scenes::SCENETYPE _next) { scene.nowScene = _now; scene.nextScene = _next; paketId = PaketID::SWITCH_PAKET; socket = (SOCKET)-1; }
			// 切り替え(1=切り替え成功/0=切り替え要請/-1=切り替え失敗)
			int switchFlag;
		};
	}

	// ネットワークのエラー状態
	void ErrorState(int errorNumber);
}
#endif // !DATA_DEFINED