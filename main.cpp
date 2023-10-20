#include <Novice.h>
#include <stdlib.h>
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
		float Width;
		float Height;
		float radius;
		float scale;//1
		float speed;//20
		
	}Player;

	//Player構造体の初期化
	Player player{
		{100.0f,0.0f},
		{0.0f,0.0f},
		{0.0f,-0.8f},
		8.0f,
		16.0f,
		20.0f,
		1.0f,
		20.0f,
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
		float Width;
		float Height;
		float radius;
		float speed;
		unsigned int color;
		bool isShot;

	}Bullet;

	//スクロール時のBullet構造体の初期化================================================================

	const int Max = 7;//int型変数Maxを宣言し,値を7で初期化する
	Bullet bullet[Max];
	for (int i = 0; i < Max; i++) {
		bullet[i].position.x = 0.0f;
		bullet[i].position.y = -10.0f;
		bullet[i].Width = 8.0f;
		bullet[i].Height = 16.0f;
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
		{1150.0f,0.0f},
		WHITE
	};

	//Bossの弾の構造体の宣言============================================================================
	typedef struct BossBullet {
		Vector2 position;
		float speed;
		float radius;
		unsigned int color;
		bool isShot;
	}BossBullet;

	//Bossの弾の構造体の初期化==========================================================================

	const int Max2 = 5;//int型変数Max2を宣言し,値を5で固定する
	BossBullet bossBullet[Max2];
	for (int i = 0; i < Max2; i++) {
		bossBullet[i].position.x = 0.0f;
		bossBullet[i].position.y = -10.0f;
		bossBullet[i].speed = 7.0f;
		bossBullet[i].radius = 10.0f;
		bossBullet[i].color = WHITE;
		bossBullet[i].isShot = false;
	}

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

	int worldPosX = 640;//ワールドから見た自機のX座標を640で初期化する

	int scrollX = 0;//ワールド座標のスクロール値を0で初期化する

	int monitorX = worldPosX - scrollX;//ワールド座標とスクロール値を引いた値をモニターから見た自分の座標に代入する

	int BossBulletCoolTimer = 15;
	//Bossの弾のクールタイムを15に設定する

	int BulletCoolTimer = 10;
	//スクロール時の弾のクールタイムを15に設定する

	int Scene = 0;
	//シーン切り替え用の変数Sceneを宣言し,0で初期化する

	float Distance = 0.0f;

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
							bullet[i].position.y = 500.0f + rand() % 400 - 350;
							break;
						}
					}
				}
			}

			for (int i = 0; i < Max; i++) {//for文処理
				if (bullet[i].isShot == true) {//bullet[i].isShotがtrueだったら
					bullet[i].position.x -= bullet[i].speed;
					//スクロール時の弾を左方向に動かす

					if (bullet[i].position.x <= 0) {//スクロール時の弾のX座標が0まで到達したら
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

					if (Distance <= player.Width + bullet[i].Height + player.Height + bullet[i].Width) {
						bullet[i].color = BLACK;
					}
					else {
						bullet[i].color = WHITE;
					}
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
			Novice::ScreenPrintf(100, 100, "isJump=%d", IsJump);
			Novice::ScreenPrintf(100, 130, "isjampTimer=%d", isjampTimer);
			Novice::ScreenPrintf(100, 160, "jampTimer=%d", jampTimer);


			Novice::ScreenPrintf(100, 190, "worldPosX = %d", worldPosX);
			Novice::ScreenPrintf(100, 220, "scrollX = %d", scrollX);
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

				//ボスの複数弾処理==============================================================================
				if (boss.position.x == 1150) {
					if (BossBulletCoolTimer > 0) {
						BossBulletCoolTimer--;
					}
					else {
						BossBulletCoolTimer = 15;
					}

					if (BossBulletCoolTimer <= 0) {
						for (int i = 0; i < Max2; i++) {
							if (bossBullet[i].isShot == false) {
								bossBullet[i].isShot = true;

								bossBullet[i].position.x = boss.position.x - 15.0f;
								bossBullet[i].position.y = boss.position.y + 468.0f + rand() % 400 - 350;
								break;
							}
						}
					}
				}


				for (int i = 0; i < Max2; i++) {
					if (bossBullet[i].isShot == true) {
						bossBullet[i].position.x -= bossBullet[i].speed;

						if (bossBullet[i].position.x <= -54) {
							bossBullet[i].isShot = false;
							break;
						}
					}
				}

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
					Novice::DrawEllipse(int(bullet[i].position.x), int(bullet[i].position.y),
						int(bullet[i].radius) , int(bullet[i].radius),
						0.0f, bullet[i].color, kFillModeSolid);
					//for文を使用し、bullet[i].isShotがTrueだった場合、DrawSpriteで画像を描画する
				}
			}

			break;

		case BOSS:

			Novice::DrawSprite(6400 - scrollX, 0, BackGround[5], 1.0f, 1.0f, 0.0f, WHITE);

			Novice::DrawSprite((int)player.position.x - (int)player.radius, (int)newposY - (int)player.radius, sample, player.scale, player.scale, 0.0f, WHITE);

			Novice::DrawSprite((int)boss.position.x, (int)NewBossPosY, BossHandle, 1.0f, 1.0f, 0.0f, boss.color);

			for (int i = 0; i < Max2; i++) {
				if (bossBullet[i].isShot == true) {
					Novice::DrawSprite((int)bossBullet[i].position.x, (int)bossBullet[i].position.y,
						BossBulletHandle, 1, 1, 0.0f, (int)bossBullet[i].color);
					//for文を使用し、bossBullet[i].isShotがTrueだった場合、DrawSpriteで画像を描画する
				}
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
