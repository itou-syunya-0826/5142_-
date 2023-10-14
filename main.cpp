#include <Novice.h>
#include <stdlib.h>
#include <time.h>

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
		Vector2 position;//X,100 Y,0
		Vector2 velocity;//速度0
		Vector2 acceleration;//加速度-0.8
		float scale;//1
		float speed;//20
	}Player;
	
	Player player{
		{100.0f,0.0f},
		{0.0f,0.0f},
		{0.0f,-0.8f},
		1.0f,
		20.0f
	};

	int JampSystem = 0;
	int IsJump = 0;
	int IsJampTimer = 0;
	int JampTimer = 25;//ジャンプのラグ
	float newposY = 0;

	


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


	//弾

	typedef struct Bullet1 {
		Vector2 position;
		float speed;
		float scale;
	}Bullet1;
	
	typedef struct Bullet2 {
		Vector2 position;
		float speed;
		float scale;
	}Bullet2;

	typedef struct Bullet3 {
		Vector2 position;
		float speed;
		float scale;
	}Bullet3;

	Bullet1 bullet1{
		{0.0f,0.0f},
		10.0f,
		1.0f
	};

	Bullet2 bullet2{
		{0.0f,0.0f},
		10.0f,
		1.0f
	};

	Bullet3 bullet3{
		{0.0f,0.0f},
		10.0f,
		1.0f
	};

	bool IsBullet1 = false;
	bool IsBullet2 = false;
	bool IsBullet3 = false;

	int tama1 = Novice::LoadTexture("./tama.png");
	int tama2 = Novice::LoadTexture("./tama.png");
	int tama3 = Novice::LoadTexture("./tama.png");


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
		
		
		//position
		newposY = (player.position.y - 480) * -1;//ここでplayerのY座標を決める
		//Boss
		NewBossPosY = (boss.position.y - 415) * -1;


		//背景のスクロール

		worldPosX += int(Speed);//ワールド座標を右方向に動かす
		scrollX += int(Speed);//スクロール値も更新する

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

		//BOSSの登場処理
		if (boss.position.x == 1150) {
			boss.speed = 0;
		}

		boss.position.x -= int(boss.speed);


		



		//playerの挙動

		//二段ジャンプ
		if (IsJump == 0 || IsJump == 1) {//ジャンプ０回と１回の時に
			if (IsJampTimer == 0) {//ジャンプラグ
				if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {//ジャンプ押したら
					JampSystem = 1;//ジャンプ
					player.velocity.y = 19.0;
					IsJump += 1;
					IsJampTimer = 1;//ジャンプのラグ計算開始
				}
			}
		}

		//ジャンプのラグ
		if (IsJampTimer == 1) {
			JampTimer--;//25フレーム
		}

		if (JampTimer == 0) {//ラグタイマーが終わったら
			IsJampTimer = 0;//ラグタイマー変数を0に戻す
			JampTimer = 25;//ラグタイマーを25フレームに戻す
		}

		//2回目のジャンプ
		if (IsJump == 2) {
			if (IsJampTimer == 0) {
				if (keys[DIK_SPACE]) {//長押しでヒップドロップ
					player.position.y -= player.speed;
				}
			}

		}


		//ジャンプしたら
		if (JampSystem == 1) {//ジャンプの仕組み
			player.velocity.y += player.acceleration.y;
			player.position.y += player.velocity.y;
		}

		//謎---ジャンプして元の位置に戻すためのだった
		if (player.position.y < player.scale) {
			player.position.y = player.scale;
			IsJump = 0;//元の位置に戻ったらジャンプ０
		}

		//弾の発射

		if (keys[DIK_UP]&&!preKeys[DIK_UP]) {
			IsBullet1 = true;
			IsBullet2 = true;
			IsBullet3 = true;
			bullet1.position.x = 1280;
			bullet2.position.x = 1280;
			bullet3.position.x = 1280;
			bullet1.position.y = 500;
			bullet2.position.y = 300;
			bullet3.position.y = 100;
		}

		if (IsBullet1 == true) {
			bullet1.position.x -= bullet1.speed;
		}

		if (IsBullet2 == true) {
			bullet2.position.x -= bullet2.speed;
		}

		if (IsBullet3 == true) {
			bullet3.position.x -= bullet3.speed;
		}

		if (bullet1.position.x == -10) {
			IsBullet1 = false;
		}

		if (bullet2.position.x == -10) {
			IsBullet2 = false;
		}

		if (bullet3.position.x == -10) {
			IsBullet3 = false;
		}


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		
		//値確認
		Novice::ScreenPrintf(100, 100, "isJump=%d", IsJump);
		Novice::ScreenPrintf(100, 130, "isjampTimer=%d", IsJampTimer);
		Novice::ScreenPrintf(100, 160, "jampTimer=%d", JampTimer);
		Novice::ScreenPrintf(100, 190, "IsBullet=%d", IsBullet1);

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
		//playerの描画
		
		Novice::DrawSprite((int)monitorX - 512, (int)newposY, sample, player.scale, player.scale, 0.0f, WHITE);//32*32

		//Bossの描画
		Novice::DrawSprite((int)boss.position.x, (int)NewBossPosY, BossHandle, 1.0f, 1.0f, 0.0f, boss.color);

		
		//弾の描画

		if (IsBullet1 == true) {
			Novice::DrawSprite((int)bullet1.position.x, (int)bullet1.position.y, tama1, bullet1.scale, bullet1.scale, 0.0f, WHITE);
		}

		if (IsBullet2 == true) {
			Novice::DrawSprite((int)bullet2.position.x, (int)bullet2.position.y, tama2, bullet2.scale, bullet2.scale, 0.0f, WHITE);
		}

		if (IsBullet3 == true) {
			Novice::DrawSprite((int)bullet3.position.x, (int)bullet3.position.y, tama3, bullet3.scale, bullet3.scale, 0.0f, WHITE);
		}

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
