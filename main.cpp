#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>

struct Vector2 {
	float x;
	float y;
};
struct Ball {
	Vector2 center;
	Vector2 velocity;
	Vector2 accele;
	Vector2 direction;
	float radius;
	int shootCoolTime;
	unsigned int color;
};
struct Bullet {
	Vector2 pos;
	float width;
	float height;
	float radius;
	float speed;
	int isShoot;
};
struct circle {
	Vector2 center;
	Vector2 direction;
	int radius;
	int speed = 8;
};

const char kWindowTitle[] = "LC1C_01_アオキ_コウシ_タイトル";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };
	Ball player{
		{600.0f,50.0f},
		{0.0f,0.0f},
		{0.0f,-0.8f},
		{ 1,0 },
		30.0f,
		10,
		WHITE
	};
	Bullet bullet[32];
	for (int i = 0; i < 32; i++) {
		bullet[i].pos.x = -128.0f;
		bullet[i].pos.y = -128.0f;
		bullet[i].width = 8.0f;
		bullet[i].height = 16.0f;
		bullet[i].radius = 8.0f;
		bullet[i].speed = 8.0f;
		bullet[i].isShoot = false;
	}

	circle enemy = { {600,100},{-1,0},30 };
	float amplitude = 100.0f;
	float theta = 1.0f / 30.0f * float(M_PI);


	int dir;
	float dot;
	int posy = 0;
	int speed = 10;
	int scrollx = 0;
	int dash = 0;
	int dashcount = 30;
	int bgx[2] = { 0,1280 };
	int bgTex[2]{};
	bgTex[0] = Novice::LoadTexture("./Colosseum.png");
	bgTex[1] = Novice::LoadTexture("./Colosseum2.png");
	int glaphHandle1;
	int glaphHandle2;
	int glaphHandle3;
	glaphHandle1 = Novice::LoadTexture("./bullet.png");
	glaphHandle2 = Novice::LoadTexture("./wizardright.png");
	glaphHandle3 = Novice::LoadTexture("./wizardleft.png");
	posy -= 500;
	posy *= -1;
	int posy2 = 0;
	int posy3 = 0;
	int posy4 = 0;
	enum Direction {
		LEFT,
		RIGHT,

	};
	Direction way = RIGHT;

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
		//スクロール処理
		if (player.center.x >= 640 && player.center.x <= 1920) {
			scrollx = (int)player.center.x - 640;
		}
		//左右判定
		dir = (int)player.center.x - (int)enemy.center.x;
		if (dir >= 0) {
			enemy.direction.x = 1;
		}
		else {
			enemy.direction.x = -1;
		}
		//自機の移動処理
		if (keys[DIK_RIGHT] != 0) {
			player.center.x += speed;
			player.direction.x = 1;
			way = RIGHT;
		}
		if (keys[DIK_LEFT] != 0) {
			player.center.x -= speed;
			player.direction.x = -1;
			way = LEFT;
		}

		dot = enemy.direction.x * player.direction.x + enemy.direction.y * player.direction.y;
		//ダッシュ・ジャンプ
		if (keys[DIK_Z] != 0 && player.center.y == player.radius) {
			player.velocity.y = 20.0f;
		}
		if (keys[DIK_C] != 0 && keys[DIK_RIGHT] != 0 && dash == 0) {
			player.velocity.x = 10.0f;
			dash = 1;
		}
		if (keys[DIK_C] != 0 && keys[DIK_LEFT] != 0 && dash == 0) {
			player.velocity.x = -10.0f;
			dash = 1;
		}
		if (dash == 1) {
			;
			dashcount--;
		}
		if (dashcount == 0) {
			dash = 0;
			dashcount = 30;
			player.velocity.x = 0.0f;
		}
		player.velocity.y += player.accele.y;
		player.center.y += player.velocity.y;
		player.velocity.x += player.accele.x;
		player.center.x += player.velocity.x;

		// 弾の発射処理
		if (keys[DIK_X]) {

			// 発射間隔の調整用クールタイムの計算
			if (player.shootCoolTime > 0) {
				player.shootCoolTime--;
			}
			else {
				player.shootCoolTime = 10;
			}

			// i番目の弾が撃たれていなかったら発射する
			if (player.shootCoolTime <= 0) {
				for (int i = 0; i < 32; i++) {
					if (!bullet[i].isShoot) {
						bullet[i].isShoot = true;
						bullet[i].pos.x = player.center.x;
						bullet[i].pos.y = player.center.y;
						break;
					}
				}
			}
		}
		// 弾道計算
		for (int i = 0; i < 32; i++) {
			if (bullet[i].isShoot&&way==RIGHT) {
				// 横方向に進ませる
				bullet[i].pos.x += bullet[i].speed;

				// 画面外に出たら発射フラグをFalseに変更する
				if (bullet[i].pos.x <= 0 - bullet[i].height / 2.0f || bullet[i].pos.x >= 2530 - bullet[i].height / 2.0f) {
					bullet[i].isShoot = false;
				}
			}
			else if (bullet[i].isShoot && way == LEFT) {
				bullet[i].pos.x -= bullet[i].speed;
			}
		}
		//敵の移動処理
		enemy.center.x += enemy.speed;
		enemy.center.y = sinf(theta) * amplitude + 300;
		theta += 1.0 / 120.0f * float(M_PI);


		//地面との当たり判定
		if (player.center.y <= player.radius) {
			player.center.y = player.radius;
		}


		//自機の画面端の当たり判定
		if (player.center.x >= 2530) {
			player.center.x = 2530;
		}
		if (player.center.x <= 30) {
			player.center.x = 30;
		}


		//敵の画面端の当たり判定
		if (enemy.center.x >= 2530) {
			enemy.speed *= -1;
		}
		if (enemy.center.x <= 30) {
			enemy.speed *= -1;
		}


		//座標変換
		posy2 = ((int)player.center.y - 500) * -1;
		posy3 = ((int)enemy.center.y - 500) * -1;
		for (int i = 0; i < 32; i++) {
			posy4 = ((int)bullet[i].pos.y - 500) * -1;
		}
		///
		/// ↑更新処理ここまで
		///


		///
		/// ↓描画処理ここから
		///
		Novice::DrawSprite(0 - scrollx, 0, bgTex[0], 1, 1, 0.0f, 0xFFFFFFFF);
		Novice::DrawSprite(1280 - scrollx, 0, bgTex[1], 1, 1, 0.0f, 0xFFFFFFFF);

		if (way == RIGHT) {
			Novice::DrawSprite((int)player.center.x - scrollx, posy2, glaphHandle2, 2, 2, 0.0f, WHITE);
		}
		if (way == LEFT) {
			Novice::DrawSprite((int)player.center.x - scrollx, posy2, glaphHandle3, 2, 2, 0.0f, WHITE);
		}
		Novice::DrawEllipse((int)enemy.center.x - scrollx, (int)posy3, enemy.radius, enemy.radius, 0.0f, WHITE, kFillModeSolid);


		//Novice::DrawLine(0, posy, 1280, posy, WHITE);
		Novice::DrawLine(1 - scrollx, 1000, 1 - scrollx, -1000, RED);
		Novice::DrawLine(2529 - scrollx, 1000, 2529 - scrollx, -1000, RED);


		for (int i = 0; i < 32; i++) {
			if (bullet[i].isShoot) {
				Novice::DrawSprite((int)bullet[i].pos.x - scrollx, posy4, glaphHandle1, 1, 1, 0.0f, WHITE);
			}
		}
		Novice::ScreenPrintf(10, 10, "posy4 %d", posy4);
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