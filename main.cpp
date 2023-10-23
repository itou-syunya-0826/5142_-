#include <Novice.h>
#include<stdlib.h>
#include <time.h>
#include <math.h>

const char kWindowTitle[] = "5142_飛べや";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {




	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 640);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};


	//SWITCH
	enum OneButton {
		TITLE,
		STAGE1,
		BOSS,
		GAMECLEAR,
		GAMEOVER,
	};

	OneButton Scene = TITLE;

	//x,y
	typedef struct Vector2 {
		float x;
		float y;
	}Vecter2;


	typedef struct Player {
		Vector2 position;//X,100 Y,0
		Vector2 velocity;//速度0
		Vector2 acceleration;//加速度-0.8
		Vector2 radius;
		float scale;//1
		float speed;//20
		float LeftX;
		float RightX;
		float FrontY;
		float BackY;
	}Player;
	Player player{
		{100.0f,0.0f},
		{0.0f,0.0f},
		{0.0f,-0.8f},
		{14.0f,14.0f},
		1.0f,
		20.0f,
		0.0f,0.0f,0.0f,0.0f
	};
	int sample = Novice::LoadTexture("./sample.png");//プレイヤーの描画

	int playerHp = 5;
	int jampSystem = 0;
	int isJump = 0;
	int hipDrop = 0;
	int isjampTimer = 0;
	int jampTimer = 25;//ジャンプのラグ
	float newposY = 0;
	
	


	//Boss
	typedef struct Boss {
		Vector2 position;
		float speed;
		unsigned int color;
	}Boss;
	Boss boss{
		{0.0f,0.0f},
		5.0f,
		WHITE
	};
	int BossHandle = Novice::LoadTexture("./Boss1.png");
	int HpHandle = Novice::LoadTexture("./heart.png");
	float NewBossPosY = 0;

	
	//スクロール中の敵の弾============================
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
	int isPlayerHit = 0;

	//Bossの弾の構造体の宣言
	typedef struct BossBullet {
		Vector2 position;
		float speed;
		float radius;
		float LeftX;
		float RightX;
		float FrontY;
		float BackY;
		unsigned int color;
		int isShot;
	}BossBullet;

	const int Max2 = 5;
	BossBullet bossBullet[Max2];
	for (int i = 0; i < Max2; i++) {
		bossBullet[i].position.x = 0.0f;
		bossBullet[i].position.y = -10.0f;
		bossBullet[i].speed = 7.0f;
		bossBullet[i].radius = 10.0f;
		bossBullet[i].LeftX = 0.0f;
		bossBullet[i].RightX = 0.0f;
		bossBullet[i].FrontY = 0.0f;
		bossBullet[i].BackY = 0.0f;
		bossBullet[i].color = GREEN;
		bossBullet[i].isShot = 0;
	}
	int BossBulletHandle = Novice::LoadTexture("./tama.png");
	int isBossHit = 0;

	//背景
	int scrollSpeedX = 5;


	int BackGround[6];
	BackGround[0] = Novice::LoadTexture("./bg1.png");
	BackGround[1] = Novice::LoadTexture("./bg2.png");
	BackGround[2] = Novice::LoadTexture("./bg3.png");
	BackGround[3] = Novice::LoadTexture("./bg4.png");
	BackGround[4] = Novice::LoadTexture("./bg5.png");
	BackGround[5] = Novice::LoadTexture("./bg6.png");



	int worldPosX = 640;//ワールドから見た自機のX座標を640で初期化する

	int scrollX = 0;//ワールド座標のスクロール値を0で初期化する

	int monitorX = worldPosX - scrollX;//ワールド座標とスクロール値を引いた値をモニターから見た自分の座標に代入する


	int BossBulletCoolTimer = 15;
	int BulletCoolTimer = 10;

	float Distance = 0.0f;//スクロール時の弾の当たり判定用の変数を用意

	int BulletAttack = 1;



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




			//プレイヤーの当たり判定初期化
			player.LeftX = player.position.x - player.radius.x;
			player.RightX = player.position.x + player.radius.x;
			player.FrontY = player.position.y - player.radius.y;
			player.BackY = player.position.y + player.radius.y;

			//スクロール中の敵の弾の当たり判定初期化
			for (int i = 0; i < Max2; i++) {
				bullet[i].LeftX = bullet[i].position.x - bullet[i].radius;
				bullet[i].RightX = bullet[i].position.x + bullet[i].radius;
				bullet[i].FrontY = bullet[i].position.y - bullet[i].radius;
				bullet[i].BackY = bullet[i].position.y + bullet[i].radius;
			}
			// スクロール中の敵の弾の当たり判定
			for (int i = 0; i < Max; i++) {
				if (bullet[i].LeftX <= player.RightX && player.LeftX <= bullet[i].RightX && bullet[i].FrontY <= player.BackY && player.FrontY <= bullet[i].BackY)
				{
					isPlayerHit = 1;
				}
				else {
					isPlayerHit = 0;
				}
			}
				
			//敵の弾の当たり判定初期化
			for (int i = 0; i < Max2; i++) {
				bossBullet[i].LeftX = bossBullet[i].position.x - bossBullet[i].radius;
				bossBullet[i].RightX = bossBullet[i].position.x + bossBullet[i].radius;
				bossBullet[i].FrontY = bossBullet[i].position.y - bossBullet[i].radius;
				bossBullet[i].BackY = bossBullet[i].position.y + bossBullet[i].radius;
			}
			for (int i = 0; i < Max2; i++) {
				if (bossBullet[i].LeftX <= player.RightX && player.LeftX <= bossBullet[i].RightX && bossBullet[i].FrontY <= player.BackY && player.FrontY <= bossBullet[i].BackY)
				{
					isBossHit = 1;
					if (hipDrop == 1) {//ヒップドロップの時に跳ね返す
						bossBullet[i].speed = -7.0f;
					}
					else {
						bossBullet[i].speed = 7.0f;
					}
				}
				else {
						isBossHit = 0;
				}
			}
			
	
	

		
		
	






			//背景のスクロール================================

	
			worldPosX += scrollSpeedX;//ワールド座標を右方向に動かす
			scrollX += scrollSpeedX;//スクロール値も更新する
	
			//座標のスクロール
			monitorX = worldPosX - scrollX;
	
			boss.position.x -= int(boss.speed);


			if (worldPosX > 7623) {
				scrollX -= int(scrollSpeedX);
			}

			if (worldPosX > 7040 || worldPosX > 7680) {//ワールド座標が3200かつ3840の場合
				scrollX = 6400;//スクロール値を6400で固定する
			}


			//ボスの場所に行ったら止まる
			if (scrollX == 6400) {
				scrollSpeedX = 0;
			}
			//===============================================-
	
				//BOSSの登場処理
			if (boss.position.x == 1150) {
				boss.speed = 0;
				if (BossBulletCoolTimer > 0) {
					BossBulletCoolTimer--;
				}
				else {
					BossBulletCoolTimer = 15;
				}
	
				/*if (BossBulletCoolTimer <= 0) {
					for (int i = 0; i < Max2; i++) {
						if (bossBullet[i].isShot == false) {
							bossBullet[i].isShot = true;
							bossBullet[i].position.x = boss.position.x;
							bossBullet[i].position.y = boss.position.y + 485.0f + rand() % 400 - 350;
							break;
						}
					}
				}*/
			}
	
			/*for (int i = 0; i < Max2; i++) {
				if (bossBullet[i].isShot == true) {
					bossBullet[i].position.x -= bossBullet[i].speed;
	
					if (bossBullet[i].position.x <= -54) {
						bossBullet[i].isShot = false;
						break;
					}
				}
			}*/
	
	
			


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
				if (bullet[i].isShot == true) {
					bullet[i].position.x -= bullet[i].speed;
						
					if (bullet[i].position.x <= -20) {
						bullet[i].isShot = false;
						break;
					}
				}
			}

			if (worldPosX >= 7040) {
				for (int i = 0; i < Max; i++) {
					bullet[i].isShot = false;
				}
			}
	

			for (int i = 0; i < Max; i++) {
				if (bullet[i].isShot == true) {
					Distance = sqrtf((bullet[i].position.x - player.position.x) * (bullet[i].position.x - player.position.x) +
						(bullet[i].position.y - newposY) * (bullet[i].position.y - newposY));

					if (Distance <= player.radius.x + bullet[i].radius + player.radius.x + bullet[i].radius) {
						bullet[i].isShot = false;
						playerHp = playerHp - BulletAttack;

					}
				}
			}


			//===================================<プレイヤーのジャンプ処理>=================================
			
		
			//二段ジャンプ
			if (isJump == 0 || isJump == 1) {//ジャンプ０回と１回の時に
				if (isjampTimer== 0) {//ジャンプラグ
					if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {//ジャンプ押したら
						jampSystem = 1;//ジャンプ
						player.velocity.y = 19.0;
						isJump += 1;
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
			if (isJump == 2) {
				if (isjampTimer == 0) {
					if (keys[DIK_SPACE]) {//長押しでヒップドロップ
						player.position.y -= player.speed;
						hipDrop = 1;
					}
				}
			}


			//ジャンプしたら
			if (jampSystem == 1) {//ジャンプの仕組み
				player.velocity.y += player.acceleration.y;
				player.position.y += player.velocity.y;
			}

			//謎---ジャンプして元の位置に戻すためのだった
			if (player.position.y < player.scale) {
				player.position.y = player.scale;
				isJump = 0;//元の位置に戻ったらジャンプ０
				hipDrop = 0;
			}



			//position
			newposY = (player.position.y - 480) * -1;//ここでplayerのY座標を決める
			//Boss
			NewBossPosY = (boss.position.y - 415) * -1;


	
		
			break;

		case BOSS:
	
			//===================================<プレイヤーのジャンプ処理>=================================

		//二段ジャンプ
			if (isJump == 0 || isJump == 1) {//ジャンプ０回と１回の時に
				if (isjampTimer == 0) {//ジャンプラグ
					if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {//ジャンプ押したら
						jampSystem = 1;//ジャンプ
						player.velocity.y = 19.0;
						isJump += 1;
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
			if (isJump == 2) {
				if (isjampTimer == 0) {
					if (keys[DIK_SPACE]) {//長押しでヒップドロップ
						player.position.y -= player.speed;
					}
				}

			}

			//ジャンプしたら
			if (jampSystem == 1) {//ジャンプの仕組み
				player.velocity.y += player.acceleration.y;
				player.position.y += player.velocity.y;
			}

			//謎---ジャンプして元の位置に戻すためのだった
			if (player.position.y < player.scale) {
				player.position.y = player.scale;
				isJump = 0;//元の位置に戻ったらジャンプ０
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

			Novice::DrawSprite((int)player.position.x - (int)player.radius.x, (int)newposY - (int)player.radius.y, sample, player.scale, player.scale, 0.0f, WHITE);

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

			Novice::DrawSprite((int)player.position.x - (int)player.radius.x, (int)newposY - (int)player.radius.y, sample, player.scale, player.scale, 0.0f, WHITE);

			if (IsBulletShot1 == true) {
				Novice::DrawSprite((int)bullet1.position.x, (int)bullet1.position.y, BossBulletHandle, 1, 1, 0.0f, WHITE);
			}

			if (IsBulletShot2 == true) {
				Novice::DrawSprite((int)bullet2.position.x, (int)bullet2.position.y + 32, BossBulletHandle, 1, 1, 0.0f, WHITE);
			}

			if (IsBulletShot3 == true) {
				Novice::DrawSprite((int)bullet3.position.x, (int)bullet3.position.y + 32, BossBulletHandle, 1, 1, 0.0f, WHITE);
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




		////値確認
		//Novice::ScreenPrintf(100, 100, "isJump=%d", isJump);
		//Novice::ScreenPrintf(100, 130, "isjampTimer=%d", isjampTimer);
		//Novice::ScreenPrintf(100, 160, "jampTimer=%d", jampTimer);
		//Novice::ScreenPrintf(230, 160, "hipdrop=%d", hipDrop);
		//Novice::ScreenPrintf(100, 250, "isBossHit=%d", isBossHit);
		//Novice::ScreenPrintf(100, 280, "isPlayerHit=%d", isPlayerHit);

		//Novice::ScreenPrintf(100, 190, "worldPosX = %d", worldPosX);
		//Novice::ScreenPrintf(100, 220, "monitorX = %d", monitorX);

		////背景
		//Novice::DrawSprite(0 - scrollX, 0, BackGround[0], 1.0f, 1.0f, 0.0f, WHITE);
		//Novice::DrawSprite(1280 - scrollX, 0, BackGround[1], 1.0f, 1.0f, 0.0f, WHITE);
		//Novice::DrawSprite(2560 - scrollX, 0, BackGround[2], 1.0f, 1.0f, 0.0f, WHITE);
		//Novice::DrawSprite(3840 - scrollX, 0, BackGround[3], 1.0f, 1.0f, 0.0f, WHITE);
		//Novice::DrawSprite(5120 - scrollX, 0, BackGround[4], 1.0f, 1.0f, 0.0f, WHITE);
		//Novice::DrawSprite(6400 - scrollX, 0, BackGround[5], 1.0f, 1.0f, 0.0f, WHITE);

		////敵の弾
		//for (int i = 0; i < Max2; i++) {
		//	if (bossBullet[i].isShot == true) {
		//		Novice::DrawEllipse((int)bossBullet[i].position.x-(int)bossBullet[i].radius, (int)bossBullet[i].position.y-(int)bossBullet[i].radius,
		//			(int)bossBullet[i].radius, (int)bossBullet[i].radius,
		//			0.0f, (int)bossBullet[i].color, kFillModeSolid);
		//	}
		//}
		////スクロール中の敵の弾
		//for (int i = 0; i < Max; i++) {
		//	if (bullet[i].isShot == true) {
		//		Novice::DrawEllipse(int(bullet[i].position.x)-int(bullet[i].radius), int(bullet[i].position.y) - int(bullet[i].radius),
		//			int(bullet[i].radius), int(bullet[i].radius),
		//			0.0f, bullet[i].color, kFillModeSolid);
		//	}
		//}

		//
		////monitorX - 512


		////player描画
		//Novice::DrawSprite((int)player.position.x-(int)player.radius.x, (int)newposY - (int)player.radius.y, sample, player.scale, player.scale, 0.0f, WHITE);//32*32

		//Novice::DrawSprite((int)boss.position.x, (int)NewBossPosY, BossHandle, 1.0f, 1.0f, 0.0f, boss.color);





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
