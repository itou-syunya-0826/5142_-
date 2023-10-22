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
	char keys[256] = {0};
	char preKeys[256] = {0};	

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
		bool isReject1;//跳ね返すフラグ
	}Player;

	//Player構造体の初期化
	Player player{
		{100.0f,0.0f},
		{0.0f,0.0f},
		{0.0f,-0.8f},
		14.0f,
		1.0f,
		20.0f,
		false
	};
	int sample = Novice::LoadTexture("./sample.png");//プレイヤーの描画

	int Jampsystem = 0;
	int IsJump = 0;
	int isjampTimer = 0;
	int jampTimer = 25;//ジャンプのラグ
	float newposY = 0;

	//スクロール時のBullet構造体の宣言===================================================================
	typedef struct Bullet {

		Vector2 position;
		float radius;
		float speed;
		float Attack;
		unsigned int color;
		bool isShot;

	}Bullet;

	//スクロール時のBullet構造体の初期化================================================================

	const int Max = 7;//int型変数Maxを宣言し,値を7で初期化する
	Bullet bullet[Max];
	for (int i = 0; i < Max; i++) {
		bullet[i].position.x = 0.0f;
		bullet[i].position.y = -10.0f;
		bullet[i].radius = 10.0f;
		bullet[i].speed = 7.0f;
		bullet[i].color = WHITE;
		bullet[i].isShot = false;
	}

	//Boss構造体の宣言==================================================================================
	typedef struct Boss {
		Vector2 position;
		unsigned int color;
	}Boss;

	//Boss構造体の初期化================================================================================
	Boss boss{
		{0.0f,0.0f},
		WHITE
	};

	typedef struct BossBullet {
		Vector2 position;
		float scale;
		float speed;
	}BossBullet;

	float NewBossPosY = 0;

	int ScrollSpeedX = 5;//背景が動く速さ

	int BackGround[6];//int型配列BackGroundを要素数6で宣言し、LoadTextureで背景画像を6つ読み込む
	BackGround[0] = Novice::LoadTexture("./bg1.png");
	BackGround[1] = Novice::LoadTexture("./bg2.png");
	BackGround[2] = Novice::LoadTexture("./bg3.png");
	BackGround[3] = Novice::LoadTexture("./bg4.png");
	BackGround[4] = Novice::LoadTexture("./bg5.png");
	BackGround[5] = Novice::LoadTexture("./bg6.png");

	int BossHandle = Novice::LoadTexture("./Boss1.png");
	//int型変数BossHandleを宣言し、LoadTextureでBOSS画像を読み込む

	int BossBulletHandle = Novice::LoadTexture("./tama.png");
	
	//int型変数BossBulletHandleを宣言し、LoadTextureでBOSSの弾を読み込む

	int BulletHandle = Novice::LoadTexture("./tama_sample_green.png");

	int HpHandle = Novice::LoadTexture("./heart.png");

	int worldPosX = 640;//ワールドから見た自機のX座標を640で初期化する

	int scrollX = 0;//ワールド座標のスクロール値を0で初期化する

	int monitorX = worldPosX - scrollX;//ワールド座標とスクロール値を引いた値をモニターから見た自分の座標に代入する

	int BulletCoolTimer = 10;
	//スクロール時の弾のクールタイムを15に設定する

	float Distance = 0.0f;//スクロール時の弾の当たり判定用の変数を用意

	int BulletAttack = 1;
	int playerHp = 5;



	//==================================================<Bossの弾の宣言と初期化>===================================================

	BossBullet bullet1{
		{0,0},
		1.0f,
		10.0f,
	};

	BossBullet bullet2{
		{0,0},
		1.0f,
		10.0f
	};

	BossBullet bullet3{
		{0,0},
		1.0f,
		10.0f
	};

	bool IsBulletShot1 = false;
	bool IsBulletShot2 = false;
	bool IsBulletShot3 = false;

	bool IsBoss1 = false;
	bool IsBoss2 = false;
	bool IsBoss3 = false;

	int EnemyCount1 = 60;
	int EnemyCount2 = 60;
	int EnemyCount3 = 60;

	int BossShotCount1 = 30;
	int BossShotCount2 = 30;
	int BossShotCount3 = 30;

	bool BossAction1 = true;
	bool BossAction2 = true;
	bool BossAction3 = true;

	//ためし
	time_t Time = time(nullptr);

	srand((unsigned int)Time);

	int randnum = 0;

	int timer = 50;


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

			if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == false) {
				Scene = STAGE1;
			}
			break;

		case STAGE1:

			//playerの挙動

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

			for (int i = 0; i < Max; i++) {//for文処理
				if (bullet[i].isShot == true) {//bullet[i].isShotがtrueだったら
					bullet[i].position.x -= bullet[i].speed;
					//スクロール時の弾を左方向に動かす

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
				for (int i = 0; i < Max; i++) {
					//int型変数の初期化
					playerHp = 5;
					scrollX = 0;
					worldPosX = 640;

					//配列型変数の初期化
					bullet[i].isShot = false;

					Scene = GAMEOVER;
					
				}
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

				if (timer > 0) {
					timer--;
				}

				if (timer <= 0) {
					if (randnum == 0) {
						randnum = rand() % 3 + 1;
					}
				}

				//一個目のアクション

				if (randnum == 1) {
					BossAction1 = true;
					if (BossAction1 == true) {
						EnemyCount1--;
						timer = 30;

						if (EnemyCount1 == 0) {
							IsBoss1 = true;
							boss.position.x = 1100;
							boss.position.y = 10;
						}

						if (IsBoss1 == true) {
							BossShotCount1--;
						}


						if (BossShotCount1 == 0) {
							IsBulletShot1 = true;
							IsBulletShot2 = true;
							IsBulletShot3 = true;
							bullet1.position.x = boss.position.x;
							bullet1.position.y = boss.position.y + 0;

							bullet2.position.x = boss.position.x;
							bullet2.position.y = boss.position.y + 16;

							bullet3.position.x = boss.position.x;
							bullet3.position.y = boss.position.y + 64;
						}


						if (IsBulletShot1 && IsBulletShot2 && IsBulletShot3 == true) {
							bullet1.position.x -= bullet1.speed;
							bullet2.position.x -= bullet2.speed;
							bullet3.position.x -= bullet3.speed;
						}

						if (bullet1.position.x && bullet2.position.x && bullet3.position.x < 0) {
							IsBulletShot1 = false;
							IsBulletShot2 = false;
							IsBulletShot3 = false;
							IsBoss1 = false;
							EnemyCount1 = 60;
							BossShotCount1 = 30;
							BossAction1 = false;
							bullet1.position.x = boss.position.x;
							bullet1.position.y = boss.position.y + 0;

							bullet2.position.x = boss.position.x;
							bullet2.position.y = boss.position.y + 16;

							bullet3.position.x = boss.position.x;
							bullet3.position.y = boss.position.y + 64;
							randnum = 0;
						}
					}
				}

				//二個目のアクション

				if (randnum == 2) {
					BossAction2 = true;
					if (BossAction2 == true) {
						EnemyCount2--;
						timer = 30;

						if (EnemyCount2 == 0) {
							IsBoss2 = true;
							boss.position.x = 1100;
							boss.position.y = 200;
						}

						if (IsBoss2 == true) {
							BossShotCount2--;
						}

						if (BossShotCount2 == 0) {
							IsBulletShot1 = true;
							IsBulletShot2 = true;
							IsBulletShot3 = true;
							bullet1.position.x = boss.position.x;
							bullet1.position.y = boss.position.y + 0;

							bullet2.position.x = boss.position.x;
							bullet2.position.y = boss.position.y + 16;

							bullet3.position.x = boss.position.x;
							bullet3.position.y = boss.position.y + 64;
						}

						if (IsBulletShot1 && IsBulletShot2 && IsBulletShot3 == true) {
							bullet1.position.x -= bullet1.speed;
							bullet2.position.x -= bullet2.speed;
							bullet3.position.x -= bullet3.speed;
						}

						if (bullet1.position.x && bullet2.position.x && bullet3.position.x < 0) {
							IsBulletShot1 = false;
							IsBulletShot2 = false;
							IsBulletShot3 = false;
							IsBoss2 = false;
							EnemyCount2 = 60;
							BossShotCount2 = 30;
							BossAction2 = false;
							bullet1.position.x = boss.position.x;
							bullet1.position.y = boss.position.y + 0;

							bullet2.position.x = boss.position.x;
							bullet2.position.y = boss.position.y + 16;

							bullet3.position.x = boss.position.x;
							bullet3.position.y = boss.position.y + 64;
							randnum = 0;
						}
					}
				}

				//三個目のアクション

				if (randnum == 3) {
					BossAction3 = true;
					if (BossAction3 == true) {
						EnemyCount3--;
						timer = 30;

						if (EnemyCount3 == 0) {
							IsBoss3 = true;
							boss.position.x = 1100;
							boss.position.y = 400;
						}

						if (IsBoss3 == true) {
							BossShotCount3--;
						}

						if (BossShotCount3 == 0) {
							IsBulletShot1 = true;
							IsBulletShot2 = true;
							IsBulletShot3 = true;
							bullet1.position.x = boss.position.x;
							bullet1.position.y = boss.position.y + 0;

							bullet2.position.x = boss.position.x;
							bullet2.position.y = boss.position.y + 16;

							bullet3.position.x = boss.position.x;
							bullet3.position.y = boss.position.y + 64;
						}

						if (IsBulletShot1 && IsBulletShot2 && IsBulletShot3 == true) {
							bullet1.position.x -= bullet1.speed;
							bullet2.position.x -= bullet2.speed;
							bullet3.position.x -= bullet3.speed;
						}

						if (bullet1.position.x && bullet2.position.x && bullet3.position.x < 0) {
							IsBulletShot1 = false;
							IsBulletShot2 = false;
							IsBulletShot3 = false;
							IsBoss3 = false;
							EnemyCount3 = 60;
							BossShotCount3 = 30;
							BossAction3 = false;
							bullet1.position.x = boss.position.x;
							bullet1.position.y = boss.position.y + 0;

							bullet2.position.x = boss.position.x;
							bullet2.position.y = boss.position.y + 16;

							bullet3.position.x = boss.position.x;
							bullet3.position.y = boss.position.y + 64;
							randnum = 0;
						}
					}
				}

				//==========================================<Bossの弾の更新処理 ここまで>==========================================


				newposY = (player.position.y - 480) * -1;//ここでplayerのY座標を決める
				NewBossPosY = (boss.position.y - 415) * -1;//ここでplayerのY座標を決める

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

			if (IsBoss1 == true) {
				Novice::DrawSprite((int)boss.position.x, (int)boss.position.y, BossHandle, 1.0f, 1.0f, 0.0f, WHITE);
			}

			if (IsBoss2 == true) {
				Novice::DrawSprite((int)boss.position.x, (int)boss.position.y, BossHandle, 1.0f, 1.0f, 0.0f, WHITE);
			}

			if (IsBoss3 == true) {
				Novice::DrawSprite((int)boss.position.x, (int)boss.position.y, BossHandle, 1.0f, 1.0f, 0.0f, WHITE);
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
