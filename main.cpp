#include <Novice.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

const char kWindowTitle[] = "5142";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 640);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	enum OneButton {
		TITLE,
		STAGE1,
		BOSS,
		GAMECLEAR,
		GAMEOVER,
	};


	OneButton Scene = TITLE;

	//Vector2構造体の宣言
	typedef struct Vector2 {
		float x;
		float y;
	}Vecter2;



	//Player構造体の宣言
	typedef struct Player {
		Vector2 position;//X,100 Y,0
		Vector2 velocity;//速度0
		Vector2 acceleration;//加速度-0.8
		float radius;
		float scale;//1
		float speed;//20
		bool hipDrop;
	}Player;
	Player player{
		{100.0f,0.0f},
		{0.0f,0.0f},
		{0.0f,-0.8f},
		14.0f,
		1.0f,
		20.0f,
		false
	};
	int HpHandle = Novice::LoadTexture("./heart.png");//HP
	int sample = Novice::LoadTexture("./sample.png");//プレイヤーの描画
	int playerHp = 5;
	int Jampsystem = 0;
	int IsJump = 0;//ジャンプしたら1
	int isjampTimer = 0;//ジャンプタイマー始動変数
	int jampTimer = 25;//ジャンプのラグ
	float newposY = 0;
	bool isReject1 = false;
	bool isReject2 = false;
	bool isReject3 = false;
	bool isReject4 = false;
	bool isReject5 = false;
	bool isReject6 = false;
	bool isReject7 = false;
	bool isReject8 = false;



	//スクロール時のBullet構造体の宣言
	typedef struct Bullet {

		Vector2 position;
		float radius;
		float speed;
		float LeftX;
		float RightX;
		float FrontY;
		float BackY;
		unsigned int color;
		bool isShot;

	}Bullet;
	const int Max = 7;
	Bullet bullet[Max];
	for (int i = 0; i < Max; i++) {
		bullet[i].position.x = 0.0f;
		bullet[i].position.y = -10.0f;
		bullet[i].radius = 10.0f;
		bullet[i].speed = 7.0f;
		bullet[i].LeftX = 0.0f;
		bullet[i].RightX = 0.0f;
		bullet[i].FrontY = 0.0f;
		bullet[i].BackY = 0.0f;
		bullet[i].color = WHITE;
		bullet[i].isShot = false;
	}
	int BulletHandle = Novice::LoadTexture("./tama_sample_green.png");
	int BulletCoolTimer = 10;//スクロール時の弾のクールタイムを15に設定する



	//Boss構造体の宣言
	typedef struct Boss {
		Vector2 position;
		unsigned int color;
	}Boss;
	Boss boss{
		{1200,220},
		10
	};
	int BossHandle = Novice::LoadTexture("./Boss1.png");


	//bossの弾
	typedef struct BossBullet {
		Vector2 position;
		float scale;
		float speed;
		float radius;
		float LeftX;
		float RightX;
		float FrontY;
		float BackY;
		bool isBulletShot;
	}BossBullet;
	const int Max2 = 8;
	BossBullet bossBullet[Max2];
	for (int i = 0; i < Max2; i++) {
		bossBullet[i].position.x = 0.0f;
		bossBullet[i].position.y = 0.0f;
		bossBullet[i].scale = 1.0f;
		bossBullet[i].speed = 10.0f;
		bossBullet[i].LeftX = 0.0f;
		bossBullet[i].RightX = 0.0f;
		bossBullet[i].FrontY = 0.0f;
		bossBullet[i].BackY = 0.0f;
		bossBullet[i].isBulletShot= false;
	}
	float NewBossPosY = 0;
	int BossBulletHandle = Novice::LoadTexture("./tama.png");

	
	int BackGround[6];//int型配列BackGroundを要素数6で宣言し、LoadTextureで背景画像を6つ読み込む
	BackGround[0] = Novice::LoadTexture("./bg1.png");
	BackGround[1] = Novice::LoadTexture("./bg2.png");
	BackGround[2] = Novice::LoadTexture("./bg3.png");
	BackGround[3] = Novice::LoadTexture("./bg4.png");
	BackGround[4] = Novice::LoadTexture("./bg5.png");
	BackGround[5] = Novice::LoadTexture("./bg6.png");


	
	//画面スクロール
	int worldPosX = 640;//ワールドから見た自機のX座標を640で初期化する
	int scrollX = 0;//ワールド座標のスクロール値を0で初期化する
	int monitorX = worldPosX - scrollX;//ワールド座標とスクロール値を引いた値をモニターから見た自分の座標に代入する
	int ScrollSpeedX = 5;



	float Distance = 0.0f;//弾の当たり判定用の変数を用意


	float PlayerDistance[8];
	PlayerDistance[0] = 0.0f;
	PlayerDistance[1] = 0.0f;
	PlayerDistance[2] = 0.0f;
	PlayerDistance[3] = 0.0f;
	PlayerDistance[4] = 0.0f;
	PlayerDistance[5] = 0.0f;
	PlayerDistance[6] = 0.0f;
	PlayerDistance[7] = 0.0f;
	
	int BossBulletAttack[8];
	BossBulletAttack[0] = 1;
	BossBulletAttack[1] = 1;
	BossBulletAttack[2] = 1;
	BossBulletAttack[3] = 1;
	BossBulletAttack[4] = 1;
	BossBulletAttack[5] = 1;
	BossBulletAttack[6] = 1;
	BossBulletAttack[7] = 1;

	int BulletAttack = 1;
	

	//double homing=sqrt()

	

	//==================================================<Bossの弾の宣言と初期化>===================================================

	BossBullet bullet1{
		{0,0},
		1.0f,
		10.0f,
		10.0f
	};

	BossBullet bullet2{
		{0,0},
		1.0f,
		10.0f,
		10.0f
	};

	BossBullet bullet3{
		{0,0},
		1.0f,
		10.0f,
		10.0f
	};

	BossBullet bullet4{
		{0,0},
		1.0f,
		10.0f,
		10.0f
	};

	BossBullet bullet5{
		{0,0},
		1.0f,
		10.0f,
		10.0f
	};

	BossBullet bullet6{
		{0,0},
		1.0f,
		10.0f,
		10.0f
	};

	BossBullet bullet7{
		{0,0},
		1.0f,
		10.0f,
		10.0f
	};

	BossBullet bullet8{
		{0,0},
		1.0f,
		10.0f,
		10.0f
	};

	bool IsBulletShot1 = false;
	bool IsBulletShot2 = false;
	bool IsBulletShot3 = false;
	bool IsBulletShot4 = false;
	bool IsBulletShot5 = false;
	bool IsBulletShot6 = false;
	bool IsBulletShot7 = false;
	bool IsBulletShot8 = false;

	bool IsBoss = true;

	int BossShotCount1 = 30;
	int BossShotCount2 = 30;
	int BossShotCount3 = 30;
	int BossShotCount4 = 30;

	bool BossAction1 = true;
	bool BossAction2 = true;
	bool BossAction3 = true;
	bool BossAction4 = true;

	time_t Time = time(nullptr);

	srand((unsigned int)Time);

	int randnum = 0;

	int bossBulletTimer = 50;

	

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

		switch (Scene) {

		case TITLE:

			IsBulletShot1 = false;
			IsBulletShot2 = false;
			IsBulletShot3 = false;
			IsBulletShot4 = false;
			IsBulletShot5 = false;
			IsBulletShot6 = false;
			IsBulletShot7 = false;
			IsBulletShot8 = false;

			IsBoss = true;

			BossShotCount1 = 30;
			BossShotCount2 = 30;
			BossShotCount3 = 30;
			BossShotCount4 = 30;

			BossAction1 = true;
			BossAction2 = true;
			BossAction3 = true;
			BossAction4 = true;

			worldPosX = 640;
			scrollX = 0;
			ScrollSpeedX = 5;
			playerHp = 15;

			if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == false) {
				Scene = STAGE1;
			}
			break;

		case STAGE1:

			



			//背景のスクロール

			worldPosX += ScrollSpeedX;//ワールド座標を右方向に動かす
			scrollX += ScrollSpeedX;//スクロール値も更新する

			//背景のスクロール処理==========================================================================
			if (worldPosX > 7623) {
				scrollX -= ScrollSpeedX;
			}

			if (worldPosX > 7040 || worldPosX > 7680) {//ワールド座標が3200かつ3840の場合
				scrollX = 6400;//スクロール値を6400で固定する
			}

			monitorX = worldPosX - scrollX;

			if (scrollX == 6400) {//スクロール値が6400まで到達したら
				ScrollSpeedX = 0;//スクロールのスピードを0にする
				Scene = BOSS;
			}

			//スクロール時の複数弾の処理====================================================================
			if (worldPosX <= 7040) {
				if (BulletCoolTimer > 0) {
					BulletCoolTimer--;
				}
				else {
					BulletCoolTimer = 10;
				}

				if (BulletCoolTimer <= 0) {
					for (int i = 0; i < Max; i++) {
						if (bullet[i].isShot == false) {
							bullet[i].isShot = true;

							bullet[i].position.x = 1270.0f;
							bullet[i].position.y = 480.0f + rand() % 400 - 350;
							break;
						}
					}
				}
			}

			for (int i = 0; i < Max; i++) {
				if (bullet[i].isShot == true) {//弾を打ったら
					bullet[i].position.x -= bullet[i].speed;//スクロール時の弾を左方向に動かす

					if (bullet[i].position.x <= -20) {//スクロール時の弾のX座標が0まで到達したら
						bullet[i].isShot = false;//スクロール時の弾のフラグをfalseにする
						break;
					}
				}
			}

			if (worldPosX >= 7040) {//WorldPosXが7040以上だったら
				for (int i = 0; i < Max; i++) {
					bullet[i].isShot = false;
					//スクロール時の弾のフラグをfalseに設定する
				}
			}

			for (int i = 0; i < Max; i++) {
				if (bullet[i].isShot == true) {
					Distance = sqrtf((bullet[i].position.x - player.position.x) * (bullet[i].position.x - player.position.x) +
						(bullet[i].position.y - newposY) * (bullet[i].position.y - newposY));

					if (Distance <= player.radius + bullet[i].radius + player.radius + bullet[i].radius) {
						bullet[i].isShot = false;
						playerHp = playerHp - BulletAttack;

					}
				}
			}

			//ゲームオーバーに向かう処理
			if (playerHp == 0) {

				playerHp = 15;
				scrollX = 0;
				worldPosX = 640;
				ScrollSpeedX = 5;

				Scene = GAMEOVER;

			}

			//===================================<プレイヤーのジャンプ処理>=================================

			//二段ジャンプ
			if (IsJump == 0 || IsJump == 1) {//ジャンプ０回と１回の時に
				if (isjampTimer == 0) {//ジャンプラグ
					if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {//ジャンプ押したら
						Jampsystem = 1;//ジャンプ
						player.velocity.y = 19.0;
						IsJump += 1;
						isjampTimer = 1;//ジャンプのラグ計算開始
					}
				}
			}

			//ジャンプのラグ
			if (isjampTimer == 1) {
				jampTimer--;//25フレーム
			}

			if (jampTimer == 0) {//ラグタイマーが終わったら
				isjampTimer = 0;//ラグタイマー変数を0に戻す
				jampTimer = 25;//ラグタイマーを25フレームに戻す
			}

			//2回目のジャンプ
			if (IsJump == 2) {
				if (isjampTimer == 0) {
					if (keys[DIK_SPACE]) {//長押しでヒップドロップ

						player.position.y -= player.speed;

						player.hipDrop = true;
						if (player.hipDrop == true) {
							player.position.y -= player.speed;
						}

					}
				}

			}

			//ジャンプしたら
			if (Jampsystem == 1) {//ジャンプの仕組み
				player.velocity.y += player.acceleration.y;
				player.position.y += player.velocity.y;
			}

			//謎---ジャンプして元の位置に戻すためのだった
			if (player.position.y < player.scale) {
				player.position.y = player.scale;
				IsJump = 0;//元の位置に戻ったらジャンプ０
			}
			newposY = (player.position.y - 480) * -1;//ここでplayerのY座標を決める

			//値確認
			//Novice::ScreenPrintf(100, 100, "isJump=%d", IsJump);
			//Novice::ScreenPrintf(100, 130, "isjampTimer=%d", isjampTimer);
			//Novice::ScreenPrintf(100, 160, "jampTimer=%d", jampTimer);

			Novice::ScreenPrintf(100, 190, "playerHp = %d", playerHp);

			break;

		case BOSS:

			//===================================<プレイヤーのジャンプ処理>=================================

		//二段ジャンプ
			if (IsJump == 0 || IsJump == 1) {//ジャンプ０回と１回の時に
				if (isjampTimer == 0) {//ジャンプラグ
					if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {//ジャンプ押したら
						Jampsystem = 1;//ジャンプ
						player.velocity.y = 19.0;
						IsJump += 1;
						isjampTimer = 1;//ジャンプのラグ計算開始
					}
				}
			}

			//ジャンプのラグ
			if (isjampTimer == 1) {
				jampTimer--;//25フレーム
			}

			if (jampTimer == 0) {//ラグタイマーが終わったら
				isjampTimer = 0;//ラグタイマー変数を0に戻す
				jampTimer = 25;//ラグタイマーを25フレームに戻す
			}

			//2回目のジャンプ
			if (IsJump == 2) {
				if (isjampTimer == 0) {
					if (keys[DIK_SPACE]) {//長押しでヒップドロップ


						player.position.y -= player.speed;


						player.hipDrop = true;
						if (player.hipDrop == true) {
							player.position.y -= player.speed;
						}
					}
				}
			}

			//ジャンプしたら
			if (Jampsystem == 1) {//ジャンプの仕組み
				player.velocity.y += player.acceleration.y;
				player.position.y += player.velocity.y;
			}

			//謎---ジャンプして元の位置に戻すためのだった
			if (player.position.y < player.scale) {
				player.position.y = player.scale;
				IsJump = 0;//元の位置に戻ったらジャンプ０
			}

			//==========================================<Bossの弾の更新処理 ここから>==========================================

			if (bossBulletTimer > 0) {
				bossBulletTimer--;//タイマーが０になるまでマイナスする
			}

			if (bossBulletTimer <= 0) {
				if (randnum == 0) {
					randnum = rand() % 4 + 1;//タイマーが０以下になった時にrandnumが０だったらrandnum1.2.3.4のどれかになる
				}
			}



			//一個目のアクション

			if (randnum == 1) {//rand1が呼ばれたときにこの動作をする
				BossAction1 = true;
				if (BossAction1 == true) {//一つ目の
					bossBulletTimer = 30;

					if (IsBoss == true) {
						BossShotCount1--;
					}

					if (BossShotCount1 == 0) {
						IsBulletShot1 = true;
						IsBulletShot2 = true;
						IsBulletShot3 = true;
						IsBulletShot4 = true;
						IsBulletShot5 = true;
						IsBulletShot6 = true;
						IsBulletShot7 = true;
						IsBulletShot8 = true;
						bullet1.position.x = 1100;
						bullet1.position.y = 0;

						bullet2.position.x = 1100;
						bullet2.position.y = 10;

						bullet3.position.x = 1100;
						bullet3.position.y = 50;

						bullet4.position.x = 1100;
						bullet4.position.y = 90;




						bullet5.position.x = 1100;
						bullet5.position.y = 360;

						bullet6.position.x = 1100;
						bullet6.position.y = 400;

						bullet7.position.x = 1100;
						bullet7.position.y = 440;

						bullet8.position.x = 1100;
						bullet8.position.y = 480;
					}

					if (IsBulletShot1 == true) {
						bullet1.position.x -= bullet1.speed;
					}

					if (IsBulletShot2 == true) {
						bullet2.position.x -= bullet2.speed;
					}

					if (IsBulletShot3 == true) {
						bullet3.position.x -= bullet3.speed;
					}

					if (IsBulletShot4 == true) {
						bullet4.position.x -= bullet4.speed;
					}

					if (IsBulletShot5 == true) {
						bullet5.position.x -= bullet5.speed;
					}

					if (IsBulletShot6 == true) {
						bullet6.position.x -= bullet6.speed;
					}

					if (IsBulletShot7 == true) {
						bullet7.position.x -= bullet7.speed;
					}

					if (IsBulletShot8 == true) {
						bullet8.position.x -= bullet8.speed;
					}

					if (bullet1.position.x < 0) {
						IsBulletShot1 = false;
						bullet1.position.x = 1100;
						bullet1.position.y = 0;
						if (IsBulletShot1 == false) {
							BossShotCount1 = 30;
							BossAction1 = false;
							randnum = 0;
						}
					}

					if (bullet2.position.x < 0) {
						IsBulletShot2 = false;
						bullet2.position.x = 1100;
						bullet2.position.y = 10;
						if (IsBulletShot2 == false) {
							BossShotCount1 = 30;
							BossAction1 = false;
							randnum = 0;
						}
					}

					if (bullet3.position.x < 0) {
						IsBulletShot3 = false;
						bullet3.position.x = 1100;
						bullet3.position.y = 50;
						if (IsBulletShot3 == false) {
							BossShotCount1 = 30;
							BossAction1 = false;
							randnum = 0;
						}
					}

					if (bullet4.position.x < 0) {
						IsBulletShot4 = false;
						bullet4.position.x = 1100;
						bullet4.position.y = 90;
						if (IsBulletShot4 == false) {
							BossShotCount1 = 30;
							BossAction1 = false;
							randnum = 0;
						}
					}

					if (bullet5.position.x < 0) {
						IsBulletShot5 = false;
						bullet5.position.x = 1100;
						bullet5.position.y = 360;
						if (IsBulletShot5 == false) {
							BossShotCount1 = 30;
							BossAction1 = false;
							randnum = 0;
						}
					}

					if (bullet6.position.x < 0) {
						IsBulletShot6 = false;
						bullet6.position.x = 1100;
						bullet6.position.y = 400;
						if (IsBulletShot6 == false) {
							BossShotCount1 = 30;
							BossAction1 = false;
							randnum = 0;
						}
					}


					if (bullet7.position.x < 0) {
						IsBulletShot7 = false;
						bullet7.position.x = 1100;
						bullet7.position.y = 440;
						if (IsBulletShot7 == false) {
							BossShotCount1 = 30;
							BossAction1 = false;
							randnum = 0;
						}
					}

					if (bullet8.position.x < 0) {
						IsBulletShot8 = false;
						bullet8.position.x = 1100;
						bullet8.position.y = 480;
						if (IsBulletShot8 == false) {
							BossShotCount1 = 30;
							BossAction1 = false;
							randnum = 0;
						}
					}
				}
			}

			//二個目のアクション

			if (randnum == 2) {//rand2が呼ばれたときにこの動作をする
				BossAction2 = true;
				if (BossAction2 == true) {//二つ目の
					bossBulletTimer = 30;

					if (IsBoss == true) {
						BossShotCount2--;
					}

					if (BossShotCount2 == 0) {
						IsBulletShot1 = true;
						IsBulletShot2 = true;
						IsBulletShot3 = true;
						IsBulletShot4 = true;
						IsBulletShot5 = true;
						IsBulletShot6 = true;
						IsBulletShot7 = true;
						IsBulletShot8 = true;

						bullet1.position.x = 1100;
						bullet1.position.y = 90;

						bullet2.position.x = 1100;
						bullet2.position.y = 100;

						bullet3.position.x = 1100;
						bullet3.position.y = 140;

						bullet4.position.x = 1100;
						bullet4.position.y = 180;

						bullet5.position.x = 1100;
						bullet5.position.y = 220;

						bullet6.position.x = 1100;
						bullet6.position.y = 260;

						bullet7.position.x = 1100;
						bullet7.position.y = 300;

						bullet8.position.x = 1100;
						bullet8.position.y = 340;

					}

					if (IsBulletShot1 == true) {
						bullet1.position.x -= bullet1.speed;
					}

					if (IsBulletShot2 == true) {
						bullet2.position.x -= bullet2.speed;
					}

					if (IsBulletShot3 == true) {
						bullet3.position.x -= bullet3.speed;
					}

					if (IsBulletShot4 == true) {
						bullet4.position.x -= bullet4.speed;
					}

					if (IsBulletShot5 == true) {
						bullet5.position.x -= bullet5.speed;
					}

					if (IsBulletShot6 == true) {
						bullet6.position.x -= bullet6.speed;
					}

					if (IsBulletShot7 == true) {
						bullet7.position.x -= bullet7.speed;
					}

					if (IsBulletShot8 == true) {
						bullet8.position.x -= bullet8.speed;
					}

					if (bullet1.position.x < 0) {
						IsBulletShot1 = false;
						bullet1.position.x = 1100;
						bullet1.position.y = 90;
						if (IsBulletShot1 == false) {
							BossShotCount2 = 30;
							BossAction2 = false;
							randnum = 0;
						}
					}

					if (bullet2.position.x < 0) {
						IsBulletShot2 = false;
						bullet2.position.x = 1100;
						bullet2.position.y = 100;
						if (IsBulletShot2 == false) {
							BossShotCount2 = 30;
							BossAction2 = false;
							randnum = 0;
						}
					}

					if (bullet3.position.x < 0) {
						IsBulletShot3 = false;
						bullet3.position.x = 1100;
						bullet3.position.y = 140;
						if (IsBulletShot3 == false) {
							BossShotCount2 = 30;
							BossAction2 = false;
							randnum = 0;
						}
					}

					if (bullet4.position.x < 0) {
						IsBulletShot4 = false;
						bullet4.position.x = 1100;
						bullet4.position.y = 180;
						if (IsBulletShot4 == false) {
							BossShotCount2 = 30;
							BossAction2 = false;
							randnum = 0;
						}
					}

					if (bullet5.position.x < 0) {
						IsBulletShot5 = false;
						bullet5.position.x = 1100;
						bullet5.position.y = 220;
						if (IsBulletShot5 == false) {
							BossShotCount2 = 30;
							BossAction2 = false;
							randnum = 0;
						}
					}

					if (bullet6.position.x < 0) {
						IsBulletShot6 = false;
						bullet6.position.x = 1100;
						bullet6.position.y = 260;
						if (IsBulletShot6 == false) {
							BossShotCount2 = 30;
							BossAction2 = false;
							randnum = 0;
						}
					}


					if (bullet7.position.x < 0) {
						IsBulletShot7 = false;
						bullet7.position.x = 1100;
						bullet7.position.y = 300;
						if (IsBulletShot7 == false) {
							BossShotCount2 = 30;
							BossAction2 = false;
							randnum = 0;
						}
					}

					if (bullet8.position.x < 0) {
						IsBulletShot8 = false;
						bullet8.position.x = 1100;
						bullet8.position.y = 340;
						if (IsBulletShot8 == false) {
							BossShotCount2 = 30;
							BossAction2 = false;
							randnum = 0;
						}
					}
				}
			}

			//三個目のアクション

			if (randnum == 3) {//rand3が呼ばれたときにこの動作をする
				BossAction3 = true;
				if (BossAction3 == true) {//三つ目の
					bossBulletTimer = 30;

					if (IsBoss == true) {
						BossShotCount3--;
					}

					if (BossShotCount3 == 0) {
						IsBulletShot1 = true;
						IsBulletShot2 = true;
						IsBulletShot3 = true;
						IsBulletShot4 = true;
						IsBulletShot5 = true;
						IsBulletShot6 = true;
						IsBulletShot7 = true;
						IsBulletShot8 = true;

						bullet1.position.x = 1100;
						bullet1.position.y = 0;

						bullet2.position.x = 1100;
						bullet2.position.y = 10;





						bullet3.position.x = 1100;
						bullet3.position.y = 280;

						bullet4.position.x = 1100;
						bullet4.position.y = 320;

						bullet5.position.x = 1100;
						bullet5.position.y = 360;

						bullet6.position.x = 1100;
						bullet6.position.y = 400;

						bullet7.position.x = 1100;
						bullet7.position.y = 440;

						bullet8.position.x = 1100;
						bullet8.position.y = 480;
					}

					if (IsBulletShot1 == true) {
						bullet1.position.x -= bullet1.speed;
					}

					if (IsBulletShot2 == true) {
						bullet2.position.x -= bullet2.speed;
					}

					if (IsBulletShot3 == true) {
						bullet3.position.x -= bullet3.speed;
					}

					if (IsBulletShot4 == true) {
						bullet4.position.x -= bullet4.speed;
					}

					if (IsBulletShot5 == true) {
						bullet5.position.x -= bullet5.speed;
					}

					if (IsBulletShot6 == true) {
						bullet6.position.x -= bullet6.speed;
					}

					if (IsBulletShot7 == true) {
						bullet7.position.x -= bullet7.speed;
					}

					if (IsBulletShot8 == true) {
						bullet8.position.x -= bullet8.speed;
					}

					if (bullet1.position.x < 0) {
						IsBulletShot1 = false;
						bullet1.position.x = 1100;
						bullet1.position.y = 0;
						if (IsBulletShot1 == false) {
							BossShotCount3 = 30;
							BossAction3 = false;
							randnum = 0;
						}
					}

					if (bullet2.position.x < 0) {
						IsBulletShot2 = false;
						bullet2.position.x = 1100;
						bullet2.position.y = 10;
						if (IsBulletShot2 == false) {
							BossShotCount3 = 30;
							BossAction3 = false;
							randnum = 0;
						}
					}

					if (bullet3.position.x < 0) {
						IsBulletShot3 = false;
						bullet3.position.x = 1100;
						bullet3.position.y = 280;
						if (IsBulletShot3 == false) {
							BossShotCount3 = 30;
							BossAction3 = false;
							randnum = 0;
						}
					}

					if (bullet4.position.x < 0) {
						IsBulletShot4 = false;
						bullet4.position.x = 1100;
						bullet4.position.y = 320;
						if (IsBulletShot4 == false) {
							BossShotCount3 = 30;
							BossAction3 = false;
							randnum = 0;
						}
					}

					if (bullet5.position.x < 0) {
						IsBulletShot5 = false;
						bullet5.position.x = 1100;
						bullet5.position.y = 360;
						if (IsBulletShot5 == false) {
							BossShotCount3 = 30;
							BossAction3 = false;
							randnum = 0;
						}
					}

					if (bullet6.position.x < 0) {
						IsBulletShot6 = false;
						bullet6.position.x = 1100;
						bullet6.position.y = 400;
						if (IsBulletShot6 == false) {
							BossShotCount3 = 30;
							BossAction3 = false;
							randnum = 0;
						}
					}


					if (bullet7.position.x < 0) {
						IsBulletShot7 = false;
						bullet7.position.x = 1100;
						bullet7.position.y = 440;
						if (IsBulletShot7 == false) {
							BossShotCount3 = 30;
							BossAction3 = false;
							randnum = 0;
						}
					}

					if (bullet8.position.x < 0) {
						IsBulletShot8 = false;
						bullet8.position.x = 1100;
						bullet8.position.y = 480;
						if (IsBulletShot8 == false) {
							BossShotCount3 = 30;
							BossAction3 = false;
							randnum = 0;
						}
					}
				}
			}

			//四個目のアクション

			if (randnum == 4) {//rand4が呼ばれたときにこの動作をする
				BossAction4 = true;
				if (BossAction4 == true) {//四つ目の
					bossBulletTimer = 30;

					if (IsBoss == true) {
						BossShotCount4--;
					}

					if (BossShotCount4 == 0) {
						IsBulletShot1 = true;
						IsBulletShot2 = true;
						IsBulletShot3 = true;
						IsBulletShot4 = true;
						IsBulletShot5 = true;
						IsBulletShot6 = true;
						IsBulletShot7 = true;
						IsBulletShot8 = true;

						bullet1.position.x = 1100;
						bullet1.position.y = 0;

						bullet2.position.x = 1100;
						bullet2.position.y = 10;

						bullet3.position.x = 1100;
						bullet3.position.y = 50;

						bullet4.position.x = 1100;
						bullet4.position.y = 90;

						bullet5.position.x = 1100;
						bullet5.position.y = 130;

						bullet6.position.x = 1100;
						bullet6.position.y = 170;

						bullet7.position.x = 1100;
						bullet7.position.y = 440;

						bullet8.position.x = 1100;
						bullet8.position.y = 480;
					}

					if (IsBulletShot1 == true) {
						bullet1.position.x -= bullet1.speed;
					}

					if (IsBulletShot2 == true) {
						bullet2.position.x -= bullet2.speed;
					}

					if (IsBulletShot3 == true) {
						bullet3.position.x -= bullet3.speed;
					}

					if (IsBulletShot4 == true) {
						bullet4.position.x -= bullet4.speed;
					}

					if (IsBulletShot5 == true) {
						bullet5.position.x -= bullet5.speed;
					}

					if (IsBulletShot6 == true) {
						bullet6.position.x -= bullet6.speed;
					}

					if (IsBulletShot7 == true) {
						bullet7.position.x -= bullet7.speed;
					}

					if (IsBulletShot8 == true) {
						bullet8.position.x -= bullet8.speed;
					}

					if (bullet1.position.x < 0) {
						IsBulletShot1 = false;
						bullet1.position.x = 1100;
						bullet1.position.y = 0;
						if (IsBulletShot1 == false) {
							BossShotCount4 = 30;
							BossAction4 = false;
							randnum = 0;
						}
					}

					if (bullet2.position.x < 0) {
						IsBulletShot2 = false;
						bullet2.position.x = 1100;
						bullet2.position.y = 10;
						if (IsBulletShot2 == false) {
							BossShotCount4 = 30;
							BossAction4 = false;
							randnum = 0;
						}
					}

					if (bullet3.position.x < 0) {
						IsBulletShot3 = false;
						bullet3.position.x = 1100;
						bullet3.position.y = 50;
						if (IsBulletShot3 == false) {
							BossShotCount4 = 30;
							BossAction4 = false;
							randnum = 0;
						}
					}

					if (bullet4.position.x < 0) {
						IsBulletShot4 = false;
						bullet4.position.x = 1100;
						bullet4.position.y = 90;
						if (IsBulletShot4 == false) {

							BossShotCount4 = 30;
							BossAction4 = false;
							randnum = 0;
						}
					}
					

					if (bullet5.position.x < 0) {
						IsBulletShot5 = false;
						bullet5.position.x = 1100;
						bullet5.position.y = 130;
						if (IsBulletShot5 == false) {
							BossShotCount4 = 30;
							BossAction4 = false;
							randnum = 0;
						}
					}

					if (bullet6.position.x < 0) {
						IsBulletShot6 = false;
						bullet6.position.x = 1100;
						bullet6.position.y = 170;
						if (IsBulletShot6 == false) {
							BossShotCount4 = 30;
							BossAction4 = false;
							randnum = 0;
						}
					}


					if (bullet7.position.x < 0) {
						IsBulletShot7 = false;
						bullet7.position.x = 1100;
						bullet7.position.y = 440;
						if (IsBulletShot7 == false) {
							BossShotCount4 = 30;
							BossAction4 = false;
							randnum = 0;
						}
					}

					if (bullet8.position.x < 0) {
						IsBulletShot8 = false;
						bullet8.position.x = 1100;
						bullet8.position.y = 480;
						if (IsBulletShot8 == false) {
							BossShotCount4 = 30;
							BossAction4 = false;
							randnum = 0;
						}
					}
				}
			}

			//BOSSの弾の当たり判定 ここから

			//===============================================IsBulletShot1の場合===================================================

			if (IsBulletShot1 == true) {
				PlayerDistance[0] = sqrtf((bullet1.position.x - player.position.x) * (bullet1.position.x - player.position.x) +
					(bullet1.position.y - newposY) * (bullet1.position.y - newposY));
				if (PlayerDistance[0] <= bullet1.radius + player.radius + bullet1.radius + player.radius) {//当たったら
					IsBulletShot1 = false;
					playerHp = playerHp - BossBulletAttack[0];
					if (player.hipDrop == true) {//ヒップドロップしてる間なら
						isReject1 = true;
					}
					else {
						isReject1 = false;
					}
				}
			}

			if (isReject1 == true) {

			}


			//===============================================IsBulletShot2の場合===================================================
			if (IsBulletShot2 == true) {
				PlayerDistance[1] = sqrtf((bullet2.position.x - player.position.x) * (bullet2.position.x - player.position.x) +
					(bullet2.position.y - newposY) * (bullet2.position.y - newposY));
				if (PlayerDistance[1] <= bullet2.radius + player.radius + bullet2.radius + player.radius) {
					IsBulletShot2 = false;
					playerHp = playerHp - BossBulletAttack[1];
					if (player.hipDrop == true) {//ヒップドロップしてる間なら
						isReject2 = true;
					}
					else {
						isReject2 = false;
					}
				}
			}

			//===============================================IsBulletShot3の場合===================================================
			if (IsBulletShot3 == true) {
				PlayerDistance[2] = sqrtf((bullet3.position.x - player.position.x) * (bullet3.position.x - player.position.x) +
					(bullet3.position.y - newposY) * (bullet3.position.y - newposY));
				if (PlayerDistance[2] <= bullet3.radius + player.radius + bullet3.radius + player.radius) {
					IsBulletShot3 = false;
					playerHp = playerHp - BossBulletAttack[2];
					if (player.hipDrop == true) {//ヒップドロップしてる間なら
						isReject3 = true;
					}
					else {
						isReject3 = false;
					}
				}	
			}

			//===============================================IsBulletShot4の場合===================================================
			if (IsBulletShot4 == true) {
				PlayerDistance[3] = sqrtf((bullet4.position.x - player.position.x) * (bullet4.position.x - player.position.x) +
					(bullet4.position.y - newposY) * (bullet4.position.y - newposY));
				if (PlayerDistance[3] <= bullet4.radius + player.radius + bullet4.radius + player.radius) {
					IsBulletShot4 = false;
					playerHp = playerHp - BossBulletAttack[3];
					if (player.hipDrop == true) {//ヒップドロップしてる間なら
						isReject4 = true;
					}
					else {
						isReject4 = false;
					}
				}
			}

			//===============================================IsBulletShot5の場合===================================================
			if (IsBulletShot5 == true) {
				PlayerDistance[4] = sqrtf((bullet5.position.x - player.position.x) * (bullet5.position.x - player.position.x) +
					(bullet5.position.y - newposY) * (bullet5.position.y - newposY));
				if (PlayerDistance[4] <= bullet5.radius + player.radius + bullet5.radius + player.radius) {
					IsBulletShot5 = false;
					playerHp = playerHp - BossBulletAttack[4];
					if (player.hipDrop == true) {//ヒップドロップしてる間なら
						isReject5 = true;
					}
					else {
						isReject5 = false;
					}
				}
				
			}

			//===============================================IsBulletShot6の場合===================================================
			if (IsBulletShot6 == true) {
				PlayerDistance[5] = sqrtf((bullet6.position.x - player.position.x) * (bullet6.position.x - player.position.x) +
					(bullet6.position.y - newposY) * (bullet6.position.y - newposY));
				if (PlayerDistance[5] <= bullet6.radius + player.radius + bullet6.radius + player.radius) {
					IsBulletShot6 = false;
					playerHp = playerHp - BossBulletAttack[5];
					if (player.hipDrop == true) {//ヒップドロップしてる間なら
						isReject6 = true;
					}
					else {
						isReject7 = false;
					}
				}
			}


			//===============================================IsBulletShot7の場合===================================================

			if (IsBulletShot7 == true) {
				PlayerDistance[6] = sqrtf((bullet7.position.x - player.position.x) * (bullet7.position.x - player.position.x) +
					(bullet7.position.y - newposY) * (bullet7.position.y - newposY));
				if (PlayerDistance[6] <= bullet7.radius + player.radius + bullet7.radius + player.radius) {
					IsBulletShot7 = false;
					playerHp = playerHp - BossBulletAttack[6];
					if (player.hipDrop == true) {//ヒップドロップしてる間なら
						isReject7 = true;
					}
					else {
						isReject7 = false;
					}
				}
			}

			//===============================================IsBulletShot8の場合===================================================
			if (IsBulletShot8 == true) {
				PlayerDistance[7] = sqrtf((bullet8.position.x - player.position.x) * (bullet8.position.x - player.position.x) +
					(bullet8.position.y - newposY) * (bullet8.position.y - newposY));
				if (PlayerDistance[7] <= bullet8.radius + player.radius + bullet8.radius + player.radius) {
					IsBulletShot8 = false;
					playerHp = playerHp - BossBulletAttack[7];
					if (player.hipDrop == true) {//ヒップドロップしてる間なら
						isReject8 = true;
					}
					else {
						isReject8 = false;
					}
				}
			}

			//BOSSの弾の当たり判定 ここまで


			if (playerHp == 0) {

				Scene = GAMEOVER;

			}

			newposY = (player.position.y - 480) * -1;//ここでplayerのY座標を決める
			NewBossPosY = (boss.position.y - 415) * -1;//ここでplayerのY座標を決める

			Novice::ScreenPrintf(0, 400, "timer = %d", bossBulletTimer);
			Novice::ScreenPrintf(0, 420, "randnum = %d", randnum);


			break;

		case GAMECLEAR:
			if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == false) {
				Scene = GAMEOVER;
			}
			break;

		case GAMEOVER:
			if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == false) {
				Scene = TITLE;
			}
			break;


		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		switch (Scene) {

		case TITLE:
			Novice::DrawBox(0, 0, 1280, 640, 0.0f, BLUE, kFillModeSolid);

			break;

		case STAGE1:
			Novice::DrawSprite(0 - scrollX, 0, BackGround[0], 1.0f, 1.0f, 0.0f, WHITE);
			Novice::DrawSprite(1280 - scrollX, 0, BackGround[1], 1.0f, 1.0f, 0.0f, WHITE);
			Novice::DrawSprite(2560 - scrollX, 0, BackGround[2], 1.0f, 1.0f, 0.0f, WHITE);
			Novice::DrawSprite(3840 - scrollX, 0, BackGround[3], 1.0f, 1.0f, 0.0f, WHITE);
			Novice::DrawSprite(5120 - scrollX, 0, BackGround[4], 1.0f, 1.0f, 0.0f, WHITE);
			Novice::DrawSprite(6400 - scrollX, 0, BackGround[5], 1.0f, 1.0f, 0.0f, WHITE);

			Novice::DrawSprite((int)player.position.x - (int)player.radius, (int)newposY - (int)player.radius, sample, player.scale, player.scale, 0.0f, WHITE);

			for (int i = 0; i < Max; i++) {
				if (bullet[i].isShot == true) {
					Novice::DrawSprite(int(bullet[i].position.x), int(bullet[i].position.y), BulletHandle, 1, 1, 0.0f, bullet[i].color);
					//for文を使用し、bullet[i].isShotがTrueだった場合、DrawSpriteで画像を描画する
				}
			}

			for (int i = 0; i < playerHp; i++) {
				Novice::DrawSprite(i * 32, 0, HpHandle, 1, 1, 0.0f, WHITE);
			}

			break;

		case BOSS:

			Novice::DrawSprite(6400 - scrollX, 0, BackGround[5], 1.0f, 1.0f, 0.0f, WHITE);

			Novice::DrawSprite((int)player.position.x - (int)player.radius, (int)newposY - (int)player.radius, sample, player.scale, player.scale, 0.0f, WHITE);

			if (IsBulletShot1 == true) {
				Novice::DrawSprite((int)bullet1.position.x, (int)bullet1.position.y, BossBulletHandle, bullet1.scale, bullet1.scale, 0.0f, WHITE);
			}

			if (IsBulletShot2 == true) {
				Novice::DrawSprite((int)bullet2.position.x, (int)bullet2.position.y + 32, BossBulletHandle, bullet2.scale, bullet2.scale, 0.0f, WHITE);
			}

			if (IsBulletShot3 == true) {
				Novice::DrawSprite((int)bullet3.position.x, (int)bullet3.position.y + 32, BossBulletHandle, bullet3.scale, bullet3.scale, 0.0f, WHITE);
			}

			if (IsBulletShot4 == true) {
				Novice::DrawSprite((int)bullet4.position.x, (int)bullet4.position.y + 32, BossBulletHandle, bullet4.scale, bullet4.scale, 0.0f, WHITE);
			}

			if (IsBulletShot5 == true) {
				Novice::DrawSprite((int)bullet5.position.x, (int)bullet5.position.y + 32, BossBulletHandle, bullet5.scale, bullet5.scale, 0.0f, WHITE);
			}

			if (IsBulletShot6 == true) {
				Novice::DrawSprite((int)bullet6.position.x, (int)bullet6.position.y + 32, BossBulletHandle, bullet6.scale, bullet6.scale, 0.0f, WHITE);
			}

			if (IsBulletShot7 == true) {
				Novice::DrawSprite((int)bullet7.position.x, (int)bullet7.position.y + 32, BossBulletHandle, bullet7.scale, bullet7.scale, 0.0f, WHITE);
			}

			if (IsBulletShot8 == true) {
				Novice::DrawSprite((int)bullet8.position.x, (int)bullet8.position.y + 32, BossBulletHandle, bullet8.scale, bullet8.scale, 0.0f, WHITE);
			}

			if (IsBoss == true) {
				Novice::DrawSprite((int)boss.position.x, (int)boss.position.y, BossHandle, 1.0f, 1.0f, 0.0f, WHITE);
			}

			for (int i = 0; i < playerHp; i++) {
				Novice::DrawSprite(i * 32, 0, HpHandle, 1, 1, 0.0f, WHITE);
			}

			break;

		case GAMECLEAR:
			Novice::DrawBox(0, 0, 1280, 640, 0.0f, RED, kFillModeSolid);
			break;

		case GAMEOVER:
			Novice::DrawBox(0, 0, 1280, 640, 0.0f, BLACK, kFillModeSolid);
			break;

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
