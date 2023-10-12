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
		10.0f
	};


	bool IsPlayer = false;
	float newposY = 0;
	float JumpNum = 0;
	float CoolDown = 180;
	float HipDrop = 0;


	int sample = Novice::LoadTexture("./sample.png");


	//マップチップ



	int TileHandle = Novice::LoadTexture("./block.png");

	int BlockSize = 32; //int型変数BlockSizeを宣言し,32で初期化する







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














		//playerの挙動

		//二段ジャンプ

		if (JumpNum == 0 || JumpNum == 1) {
			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				IsPlayer = true;
				player.velocity.y = 20.0;
				JumpNum += 1;
			}
		}
		

		if (JumpNum == 2) {
			CoolDown--;
			HipDrop += 1;
		}

		if (CoolDown == 0) {
			JumpNum = 0;
			CoolDown = 180;
		}

		if (IsPlayer == true) {
			player.velocity.y += player.acceleration.y;
			player.position.y += player.velocity.y;
		}

		if (player.position.y <= player.scale) {
			player.position.y = player.scale;
		}

		//長押し
		if (keys[DIK_SPACE]/*&&HipDrop==1*/) {
			player.position.y -= player.velocity.y;
		}








		newposY = (player.position.y - 480) * -1;//ここでplayerのY座標を決める

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		
		//値確認
		Novice::ScreenPrintf(100, 100, "JumpNum=%f", JumpNum);
		Novice::ScreenPrintf(100,130,"Cooldown=%f",CoolDown);









		//マップチップ描画
		
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 40; x++) {
				if (Map[y][x] == 1) {
					Novice::DrawSprite(x * BlockSize, y * BlockSize, TileHandle, 1.0f, 1.0f, 0.0f, WHITE);
				}
			}
		}
		
		
		
		
		
		Novice::DrawSprite((int)player.position.x, (int)newposY, sample, player.scale, player.scale, 0.0f, WHITE);
		/*Novice::DrawLine(0, 500, 1280, 500,WHITE);*/
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
