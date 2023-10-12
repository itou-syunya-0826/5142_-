#include <Novice.h>

const char kWindowTitle[] = "GC1D_03_イトウシュンヤ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 640);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	typedef struct Vector2 {
		float x;
		float y;
	}Vecter2;

	typedef struct Player {
		Vector2 position;
		Vector2 velocity;//速度
		Vector2 acceleration;//加速度
		float scale;
		float speed;
	}Player;

	Player player{
		{100.0f,0.0f},
		{0.0f,0.0f},
		{0.0f,-0.8f},
		1.0f,
		20.0f
	};

	bool IsPlayer = false;
	float newposY = 0;
	float JumpNum = 0;
	float CoolDown = 180;
	float HipDrop = 0;

	


	int sample = Novice::LoadTexture("./sample.png");

	//Boss


	typedef struct Boss {
		Vector2 position;
		float speed;
		unsigned int color;
	}Boss;

	Boss boss{
		{7550.0f,0.0f},
		5.0f,
		WHITE
	};

	float NewBossPosY = 0;

	int BossHandle = Novice::LoadTexture("./Boss1.png");

	//マップチップと背景

	float Speed = 5.0f;
	


	int BackGround[6];
	BackGround[0] = Novice::LoadTexture("./bg1.png");
	BackGround[1] = Novice::LoadTexture("./bg2.png");
	BackGround[2] = Novice::LoadTexture("./bg3.png");
	BackGround[3] = Novice::LoadTexture("./bg4.png");
	BackGround[4] = Novice::LoadTexture("./bg5.png");
	BackGround[5] = Novice::LoadTexture("./bg6.png");


	int TileHandle = Novice::LoadTexture("./block.png");

	int BlockSize = 32; //int型変数BlockSizeを宣言し,32で初期化する

	int worldPosX = 640;//ワールドから見た自機のX座標を640で初期化する

	int scrollX = 0;//ワールド座標のスクロール値を0で初期化する

	int monitorX = worldPosX - scrollX;//ワールド座標とスクロール値を引いた値をモニターから見た自分の座標に代入する





	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		//マップチップ処理

		int Map[20][40] =
		{
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},

		};






		//背景のスクロール

		worldPosX += int(Speed);//ワールド座標を右方向に動かす
		scrollX += int(Speed);//スクロール値も更新する

		boss.position.x -= int(boss.speed);

		//BOSSの登場処理
		if (boss.position.x == 1150) {
			boss.speed = 0;
		}

		if (worldPosX > 7623) {
			scrollX -= int(Speed);
		}

		if (worldPosX > 7040 || worldPosX > 7680) {//ワールド座標が3200かつ3840の場合
			scrollX = 6400;//スクロール値を6400で固定する
		}

		monitorX = worldPosX - scrollX;

		if (scrollX == 6400) {
			Speed = 0;
		}



		//playerの挙動

		//二段ジャンプ

		if (JumpNum == 0 || JumpNum == 1) {
			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				IsPlayer = true;
				player.velocity.y = 19.0;
				JumpNum += 1;
				
			}
		}

		
		

		if (JumpNum == 2) {
			CoolDown--;
		}

		if (CoolDown == 179) {
			HipDrop += 1;
		}

		if (CoolDown == 0) {
			JumpNum = 0;
			CoolDown = 180;
			HipDrop = 0;
		}

		if (IsPlayer == true) {
			player.velocity.y += player.acceleration.y;
			player.position.y += player.velocity.y;
		}

		if (player.position.y < player.scale) {
			player.position.y = player.scale;
		}

		//長押し
		if (keys[DIK_SPACE]&&HipDrop==1) {
			player.position.y -= player.speed;
		}






		newposY = (player.position.y - 480) * -1;//ここでplayerのY座標を決める




		//Boss

		NewBossPosY = (boss.position.y - 415) * -1;



		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		
		//値確認
		Novice::ScreenPrintf(100, 100, "JumpNum=%f", JumpNum);
		Novice::ScreenPrintf(100,130,"Cooldown=%f",CoolDown);
		Novice::ScreenPrintf(100,160,"HipDrop=%f",HipDrop);
		


		//背景
		Novice::DrawSprite(0 - scrollX, 0, BackGround[0], 1.0f, 1.0f, 0.0f, WHITE);
		Novice::DrawSprite(1280 - scrollX, 0, BackGround[1], 1.0f, 1.0f, 0.0f, WHITE);
		Novice::DrawSprite(2560 - scrollX, 0, BackGround[2], 1.0f, 1.0f, 0.0f, WHITE);
		Novice::DrawSprite(3840 - scrollX, 0, BackGround[3], 1.0f, 1.0f, 0.0f, WHITE);
		Novice::DrawSprite(5120 - scrollX, 0, BackGround[4], 1.0f, 1.0f, 0.0f, WHITE);
		Novice::DrawSprite(6400 - scrollX, 0, BackGround[5], 1.0f, 1.0f, 0.0f, WHITE);






		//マップチップ描画
		
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 40; x++) {
				if (Map[y][x] == 1) {
					Novice::DrawSprite(x * BlockSize, y * BlockSize, TileHandle, 1.0f, 1.0f, 0.0f, WHITE);
				}
			}
		}
		
		





		
		//player描画
		//Novice::DrawSprite((int)player.position.x, (int)newposY, sample, player.scale, player.scale, 0.0f, WHITE);
		
		Novice::DrawSprite((int)monitorX - 512, (int)newposY, sample, player.scale, player.scale, 0.0f, WHITE);//32*32

		Novice::DrawSprite((int)boss.position.x, (int)NewBossPosY, BossHandle, 1.0f, 1.0f, 0.0f, boss.color);

		




		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
