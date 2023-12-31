#include <Novice.h>
#include <stdlib.h>
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>

const char kWindowTitle[] = "5142_クリ王ネの逆襲";

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
	int sample[4];//プレイヤーの描画
	sample[0] = Novice::LoadTexture("./Player1.png");
	sample[1] = Novice::LoadTexture("./Player2.png");
	sample[2] = Novice::LoadTexture("./Player3.png");
	sample[3] = Novice::LoadTexture("./Player4.png");
	int HpHandle = Novice::LoadTexture("./heart.png");//HP
	int Playertimer = 0;
	int count = 0;

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


	//はねかえしたときの弾
	typedef struct PlayerBullet {
		Vector2 position;
		float radius;
		float speed;//20

	}PlayerBullet;
	const int PBulletMax = 8;
	PlayerBullet playerBullet[PBulletMax];
	for (int i = 0; i < PBulletMax; i++) {
		playerBullet[i].position.x = -30.0f;
		playerBullet[i].position.y = -30.0f;
		playerBullet[i].radius = 10.0f;
		playerBullet[i].speed = 15.0f;
	}
	int PBullet = Novice::LoadTexture("./Player_Bullet.png");


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

	int BulletCoolTimer = 10;//スクロール時の弾のクールタイムを15に設定する



	//Boss構造体の宣言
	typedef struct Boss {
		Vector2 position;
		float radius;
	}Boss;
	Boss boss{
		{1200,400},
		14
	};



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

	int ScrollSpeedX = 5;//背景が動く速さ

	int TitleHandle = Novice::LoadTexture("./Title.png");

	int BackGround = Novice::LoadTexture("./bg.png");

	int BossHandle = Novice::LoadTexture("./boss.png");
	//int型変数BossHandleを宣言し、LoadTextureでBOSS画像を読み込む

	int BossBulletHandle = Novice::LoadTexture("./Boss_Bullet.png");

	int hipDropHandle = Novice::LoadTexture("./player_descent.png");

	//int型変数BossBulletHandleを宣言し、LoadTextureでBOSSの弾を読み込む

	int BulletHandle = Novice::LoadTexture("./Screen_Bullet.png");

	//画面スクロール
	int ClearHandle = Novice::LoadTexture("./CLEAR.png");

	int OverHandle = Novice::LoadTexture("./game_over.png");

	//音声の読み込み
	int Boss_MP3 = Novice::LoadAudio("./Stage1.mp3");
	int Title_MP3 = Novice::LoadAudio("./TiTle_1.mp3");
	int Gameover_MP3 = Novice::LoadAudio("./game_over1.mp3");
	int Clear_MP3 = Novice::LoadAudio("./clear_1.mp3");
	int Stage_MP3 = Novice::LoadAudio("./bossButtle_1.mp3");

	int bossMusic = -1;
	int titleMusic = -1;
	int gameoverMusic = -1;
	int clearMusic = -1;
	int stageMusic = -1;

	int worldPosX = 640;//ワールドから見た自機のX座標を640で初期化する
	int scrollX = 0;//ワールド座標のスクロール値を0で初期化する
	int monitorX = worldPosX - scrollX;//ワールド座標とスクロール値を引いた値をモニターから見た自分の座標に代入する



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

	float BossDistance[8];
	BossDistance[0] = 0.0f;
	BossDistance[1] = 0.0f;
	BossDistance[2] = 0.0f;
	BossDistance[3] = 0.0f;
	BossDistance[4] = 0.0f;
	BossDistance[5] = 0.0f;
	BossDistance[6] = 0.0f;
	BossDistance[7] = 0.0f;

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
	int playerHp = 15;
	int BossHp = 20;

	float amplitube = 185.0f;

	float theta = -1;

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
			BossHp = 20;

			if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == false) {
				Scene = STAGE1;
				Novice::StopAudio(titleMusic);
			}
			break;

		case STAGE1:





			Playertimer += 1;

			if (Playertimer >= 30) {
				count += 1;
				Playertimer = 0;
			}

			if (count == 4) {
				count = 0;
			}

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
				Novice::StopAudio(stageMusic);
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

				Scene = GAMEOVER;
				Novice::StopAudio(stageMusic);

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

			if (player.hipDrop) {
				if (!keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
					player.hipDrop = false;
					count = 0;
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



			newposY = (player.position.y - 500) * -1;//ここでplayerのY座標を決める
			if (player.position.y <= 5) {
				player.hipDrop = false;
			}

			break;

		case BOSS:

			Playertimer += 1;

			if (Playertimer >= 30) {
				count += 1;
				Playertimer = 0;
			}

			if (count == 4) {
				count = 0;
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

			if (player.hipDrop) {
				if (!keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
					player.hipDrop = false;
					count = 0;
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
					if (player.hipDrop == true) {//ヒップドロップしてる間なら
						isReject1 = true;
						playerBullet[0].position.x = player.position.x;//代入
						playerBullet[0].position.y = newposY;//代入
					}
					else {
						playerHp = playerHp - BossBulletAttack[0];

					}
				}
			}

			if (isReject1 == true) {
				playerBullet[0].position.x += player.speed;//右に跳ね返す
			}

			if (playerBullet[0].position.x >= 1430) {
				isReject1 = false;
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
						playerHp++;
						playerBullet[1].position.x = player.position.x;//代入
						playerBullet[1].position.y = newposY;//代入
					}
				}
			}

			if (isReject2 == true) {
				playerBullet[1].position.x += player.speed;//右に跳ね返す
			}

			if (playerBullet[1].position.x >= 1430) {
				isReject2 = false;
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
						playerHp++;
						playerBullet[2].position.x = player.position.x;//代入
						playerBullet[2].position.y = newposY;//代入
					}
				}
			}

			if (isReject3 == true) {
				playerBullet[2].position.x += player.speed;//右に跳ね返す
			}

			if (playerBullet[2].position.x >= 1430) {
				isReject3 = false;
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
						playerHp++;
						playerBullet[3].position.x = player.position.x;//代入
						playerBullet[3].position.y = newposY;//代入
					}
				}
			}

			if (isReject4 == true) {
				playerBullet[3].position.x += player.speed;//右に跳ね返す
			}

			if (playerBullet[3].position.x >= 1430) {
				isReject4 = false;
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
						playerHp++;
						playerBullet[4].position.x = player.position.x;//代入
						playerBullet[4].position.y = newposY;//代入
					}
				}
			}

			if (isReject5 == true) {
				playerBullet[4].position.x += player.speed;//右に跳ね返す
			}

			if (playerBullet[1].position.x >= 1430) {
				isReject5 = false;
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
						playerHp++;
						playerBullet[5].position.x = player.position.x;//代入
						playerBullet[5].position.y = newposY;//代入
					}
				}
			}

			if (isReject6 == true) {
				playerBullet[5].position.x += player.speed;//右に跳ね返す
			}

			if (playerBullet[5].position.x >= 1430) {
				isReject6 = false;
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
						playerHp++;
						playerBullet[6].position.x = player.position.x;//代入
						playerBullet[6].position.y = newposY;//代入
					}
				}
			}

			if (isReject7 == true) {
				playerBullet[6].position.x += player.speed;//右に跳ね返す
			}

			if (playerBullet[6].position.x >= 1430) {
				isReject7 = false;
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
						playerHp++;
						playerBullet[7].position.x = player.position.x;//代入
						playerBullet[7].position.y = newposY;//代入
					}
				}
			}

			if (isReject8 == true) {
				playerBullet[7].position.x += player.speed;//右に跳ね返す
			}

			if (playerBullet[7].position.x >= 1430) {
				isReject8 = false;
			}

			if (player.position.y <= 5) {
				player.hipDrop = false;
			}

			//BOSSの弾の当たり判定 ここまで

			//跳ね返った弾の当たり判定
			//===============================================IsBulletShot1の場合===================================================

			for (int i = 0; i < PBulletMax; i++) {
				if (isReject1 == true && IsBulletShot1 == true) {
					BossDistance[0] = sqrtf((playerBullet[i].position.x - boss.position.x) * (playerBullet[i].position.x - boss.position.x) +
						(playerBullet[i].position.y - boss.position.y) * (playerBullet[i].position.x - boss.position.y));
					if (BossDistance[0] >= playerBullet[i].radius + boss.radius + playerBullet[i].radius + boss.radius) {
						IsBulletShot1 = false;
						BossHp -= BossBulletAttack[0];
					}
				}
			}

			for (int i = 0; i < PBulletMax; i++) {
				if (isReject2 == true && IsBulletShot2 == true) {
					BossDistance[1] = sqrtf((playerBullet[i].position.x - boss.position.x) * (playerBullet[i].position.x - boss.position.x) +
						(playerBullet[i].position.y - boss.position.y) * (playerBullet[i].position.x - boss.position.y));
					if (BossDistance[1] >= playerBullet[i].radius + boss.radius + playerBullet[i].radius + boss.radius) {
						IsBulletShot2 = false;
						BossHp -= BossBulletAttack[1];
					}
				}
			}

			for (int i = 0; i < PBulletMax; i++) {
				if (isReject3 == true && IsBulletShot3 == true) {
					BossDistance[2] = sqrtf((playerBullet[i].position.x - boss.position.x) * (playerBullet[i].position.x - boss.position.x) +
						(playerBullet[i].position.y - boss.position.y) * (playerBullet[i].position.x - boss.position.y));
					if (BossDistance[2] >= playerBullet[i].radius + boss.radius + playerBullet[i].radius + boss.radius) {
						IsBulletShot3 = false;
						BossHp -= BossBulletAttack[2];
					}
				}
			}
			for (int i = 0; i < PBulletMax; i++) {
				if (isReject4 == true && IsBulletShot4 == true) {
					BossDistance[3] = sqrtf((playerBullet[i].position.x - boss.position.x) * (playerBullet[i].position.x - boss.position.x) +
						(playerBullet[i].position.y - boss.position.y) * (playerBullet[i].position.x - boss.position.y));
					if (BossDistance[3] >= playerBullet[i].radius + boss.radius + playerBullet[i].radius + boss.radius) {
						IsBulletShot4 = false;
						BossHp -= BossBulletAttack[3];
					}
				}
			}
			for (int i = 0; i < PBulletMax; i++) {
				if (isReject5 == true && IsBulletShot5 == true) {
					BossDistance[4] = sqrtf((playerBullet[i].position.x - boss.position.x) * (playerBullet[i].position.x - boss.position.x) +
						(playerBullet[i].position.y - boss.position.y) * (playerBullet[i].position.x - boss.position.y));
					if (BossDistance[4] >= playerBullet[i].radius + boss.radius + playerBullet[i].radius + boss.radius) {
						IsBulletShot5 = false;
						BossHp -= BossBulletAttack[4];
					}
				}
			}
			for (int i = 0; i < PBulletMax; i++) {
				if (isReject6 == true && IsBulletShot6 == true) {
					BossDistance[5] = sqrtf((playerBullet[i].position.x - boss.position.x) * (playerBullet[i].position.x - boss.position.x) +
						(playerBullet[i].position.y - boss.position.y) * (playerBullet[i].position.x - boss.position.y));
					if (BossDistance[5] >= playerBullet[i].radius + boss.radius + playerBullet[i].radius + boss.radius) {
						IsBulletShot6 = false;
						BossHp -= BossBulletAttack[5];
					}
				}
			}
			for (int i = 0; i < PBulletMax; i++) {
				if (isReject7 == true && IsBulletShot7 == true) {
					BossDistance[6] = sqrtf((playerBullet[i].position.x - boss.position.x) * (playerBullet[i].position.x - boss.position.x) +
						(playerBullet[i].position.y - boss.position.y) * (playerBullet[i].position.x - boss.position.y));
					if (BossDistance[6] >= playerBullet[i].radius + boss.radius + playerBullet[i].radius + boss.radius) {
						IsBulletShot7 = false;
						BossHp -= BossBulletAttack[6];
					}
				}
			}

			for (int i = 0; i < PBulletMax; i++) {
				if (isReject8 == true && IsBulletShot8 == true) {
					BossDistance[7] = sqrtf((playerBullet[i].position.x - boss.position.x) * (playerBullet[i].position.x - boss.position.x) +
						(playerBullet[i].position.y - boss.position.y) * (playerBullet[i].position.x - boss.position.y));
					if (BossDistance[7] >= playerBullet[i].radius + boss.radius + playerBullet[i].radius + boss.radius) {
						IsBulletShot8 = false;
						BossHp -= BossBulletAttack[7];
					}
				}
			}

			if (BossHp <= 0) {
				Scene = GAMECLEAR;
				Novice::StopAudio(bossMusic);
			}


			//Bossが上下に動く挙動
			boss.position.y = 220 + sinf(theta) * amplitube;

			theta += float(M_PI) / 60.0f;

			if (playerHp == 0) {

				Scene = GAMEOVER;
				Novice::StopAudio(bossMusic);

			}

			newposY = (player.position.y - 500) * -1;//ここでplayerのY座標を決める

			break;

		case GAMECLEAR:
			if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == false) {
				Scene = TITLE;
				Novice::StopAudio(clearMusic);
			}
			break;

		case GAMEOVER:
			if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == false) {
				Scene = TITLE;
				Novice::StopAudio(gameoverMusic);
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
			if (Novice::IsPlayingAudio(titleMusic) == 0 || titleMusic == -1) {
				titleMusic = Novice::PlayAudio(Title_MP3, 1, 1.0f);
			}
			Novice::DrawSprite(0, 0, TitleHandle, 1, 1, 0.0f, WHITE);

			break;

		case STAGE1:

			if (Novice::IsPlayingAudio(stageMusic) == 0 || stageMusic == -1) {
				stageMusic = Novice::PlayAudio(Stage_MP3, 1, 1.0f);
			}

			Novice::DrawSprite(0 - scrollX, 0, BackGround, 1.0f, 1.0f, 0.0f, WHITE);
			Novice::DrawSprite(1280 - scrollX, 0, BackGround, 1.0f, 1.0f, 0.0f, WHITE);
			Novice::DrawSprite(2560 - scrollX, 0, BackGround, 1.0f, 1.0f, 0.0f, WHITE);
			Novice::DrawSprite(3840 - scrollX, 0, BackGround, 1.0f, 1.0f, 0.0f, WHITE);
			Novice::DrawSprite(5120 - scrollX, 0, BackGround, 1.0f, 1.0f, 0.0f, WHITE);
			Novice::DrawSprite(6400 - scrollX, 0, BackGround, 1.0f, 1.0f, 0.0f, WHITE);

			//プレイヤーの描画
			if (count == 0 && player.hipDrop == false) {
				Novice::DrawSprite((int)player.position.x - (int)player.radius, (int)newposY - (int)player.radius, sample[0], player.scale, player.scale, 0.0f, WHITE);
			}

			if (count == 1 && player.hipDrop == false) {
				Novice::DrawSprite((int)player.position.x - (int)player.radius, (int)newposY - (int)player.radius, sample[1], player.scale, player.scale, 0.0f, WHITE);
			}

			if (count == 2 && player.hipDrop == false) {
				Novice::DrawSprite((int)player.position.x - (int)player.radius, (int)newposY - (int)player.radius, sample[2], player.scale, player.scale, 0.0f, WHITE);
			}

			if (count == 3 && player.hipDrop == false) {
				Novice::DrawSprite((int)player.position.x - (int)player.radius, (int)newposY - (int)player.radius, sample[3], player.scale, player.scale, 0.0f, WHITE);
			}

			if (player.hipDrop == true) {
				Novice::DrawSprite((int)player.position.x - (int)player.radius, (int)newposY - (int)player.radius, hipDropHandle, player.scale, player.scale, 0.0f, WHITE);
			}

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

			Novice::DrawSprite(0, 0, BackGround, 1, 1, 0.0f, WHITE);

			//プレイヤーの描画
			if (count == 0) {
				Novice::DrawSprite((int)player.position.x - (int)player.radius, (int)newposY - (int)player.radius, sample[0], player.scale, player.scale, 0.0f, WHITE);
			}

			if (count == 1) {
				Novice::DrawSprite((int)player.position.x - (int)player.radius, (int)newposY - (int)player.radius, sample[1], player.scale, player.scale, 0.0f, WHITE);
			}

			if (count == 2) {
				Novice::DrawSprite((int)player.position.x - (int)player.radius, (int)newposY - (int)player.radius, sample[2], player.scale, player.scale, 0.0f, WHITE);
			}

			if (count == 3) {
				Novice::DrawSprite((int)player.position.x - (int)player.radius, (int)newposY - (int)player.radius, sample[3], player.scale, player.scale, 0.0f, WHITE);
			}

			if (player.hipDrop == true) {
				Novice::DrawSprite((int)player.position.x - (int)player.radius, (int)newposY - (int)player.radius, hipDropHandle, player.scale, player.scale, 0.0f, WHITE);
			}

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

			//ボスに向かって返す弾
			for (int i = 0; i < PBulletMax; i++) {
				if (isReject1 == true) {
					Novice::DrawSprite((int)playerBullet[i].position.x, (int)playerBullet[i].position.y, PBullet, 1, 1, 0.0f, WHITE);

				}
			}
			for (int i = 0; i < PBulletMax; i++) {
				if (isReject2 == true) {
					Novice::DrawSprite((int)playerBullet[i].position.x, (int)playerBullet[i].position.y, PBullet, 1, 1, 0.0f, WHITE);

				}
			}
			for (int i = 0; i < PBulletMax; i++) {
				if (isReject3 == true) {
					Novice::DrawSprite((int)playerBullet[i].position.x, (int)playerBullet[i].position.y, PBullet, 1, 1, 0.0f, WHITE);

				}
			}
			for (int i = 0; i < PBulletMax; i++) {
				if (isReject4 == true) {
					Novice::DrawSprite((int)playerBullet[i].position.x, (int)playerBullet[i].position.y, PBullet, 1, 1, 0.0f, WHITE);

				}
			}
			for (int i = 0; i < PBulletMax; i++) {
				if (isReject5 == true) {
					Novice::DrawSprite((int)playerBullet[i].position.x, (int)playerBullet[i].position.y, PBullet, 1, 1, 0.0f, WHITE);

				}
			}
			for (int i = 0; i < PBulletMax; i++) {
				if (isReject6 == true) {
					Novice::DrawSprite((int)playerBullet[i].position.x, (int)playerBullet[i].position.y, PBullet, 1, 1, 0.0f, WHITE);

				}
			}
			for (int i = 0; i < PBulletMax; i++) {
				if (isReject7 == true) {
					Novice::DrawSprite((int)playerBullet[i].position.x, (int)playerBullet[i].position.y, PBullet, 1, 1, 0.0f, WHITE);

				}
			}

			for (int i = 0; i < PBulletMax; i++) {
				if (isReject8 == true) {
					Novice::DrawSprite((int)playerBullet[i].position.x, (int)playerBullet[i].position.y, PBullet, 1, 1, 0.0f, WHITE);

				}
			}



			for (int i = 0; i < playerHp; i++) {
				Novice::DrawSprite(i * 32, 0, HpHandle, 1, 1, 0.0f, WHITE);
			}

			if (Novice::IsPlayingAudio(bossMusic) == 0 || bossMusic == -1) {
				bossMusic = Novice::PlayAudio(Boss_MP3, 1, 1.0f);
			}

			Novice::ScreenPrintf(200, 100, "BossHp = %d", BossHp);

			break;

		case GAMECLEAR:
			if (Novice::IsPlayingAudio(clearMusic) == 0 || clearMusic == -1) {
				clearMusic = Novice::PlayAudio(Clear_MP3, 1, 1.0f);
			}

			Novice::DrawSprite(0, 0, ClearHandle, 1, 1, 0.0f, WHITE);
			break;

		case GAMEOVER:

			if (Novice::IsPlayingAudio(gameoverMusic) == 0 || gameoverMusic == -1) {
				gameoverMusic = Novice::PlayAudio(Gameover_MP3, 1, 1.0f);
			}

			Novice::DrawSprite(0, 0, OverHandle, 1, 1, 0.0f, WHITE);
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
