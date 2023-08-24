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
	unsigned int color;
};
struct circle {
	Vector2 center;
	Vector2 direction;
	int radius;
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
		WHITE
	};
	circle enemy = { {600,100},{-1,0},30 };



	int dir;
	int flag = 0;
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
	posy -= 500;
	posy *= -1;
	int posy2 = 0;
	int posy3 = 0;
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

		///
		/// ↑更新処理ここまで
		///
		if (player.center.x > 2530) {
			player.center.x = 2530;
		}
		if (player.center.x < 30) {
			player.center.x = 30;
		}
		if (player.center.x >= 640 && player.center.x <= 1920) {
			scrollx = (int)player.center.x - 640;
		}

		dir = (int)player.center.x - (int)enemy.center.x;
		if (dir >= 0) {
			enemy.direction.x = 1;
		}
		else {
			enemy.direction.x = -1;
		}
		if (keys[DIK_A] != 0) {
			player.center.x -= speed;
			player.direction.x = -1;
		}
		if (keys[DIK_D] != 0) {
			player.center.x += speed;
			player.direction.x = 1;
		}
		dot = enemy.direction.x * player.direction.x + enemy.direction.y * player.direction.y;
		if (dot < 0.0f) {
			flag = 1;
		}
		else {
			flag = 0;
		}

		if (keys[DIK_SPACE] != 0 && player.center.y == player.radius) {
			player.velocity.y = 20.0f;
		}
		if (keys[DIK_C] != 0 && keys[DIK_D] != 0) {
			player.velocity.x = 20.0f;
			dash = 1;
		}
		 if (keys[DIK_C] != 0 && keys[DIK_A] != 0) {
			 player.velocity.x = -20.0f;
			 dash = 1;
		}
		if (dash == 1) {;
			dashcount--;
		}
		if (dashcount == 0) {
			dash = 0;
			player.velocity.x = 0.0f;
		}
		player.velocity.y += player.accele.y;
		player.center.y += player.velocity.y;
		player.velocity.x += player.accele.x;
		player.center.x += player.velocity.x;

		if (player.center.y <= player.radius) {
			player.center.y = player.radius;
		}
		posy2 = ((int)player.center.y - 500) * -1;
		posy3 = ((int)enemy.center.y - 500) * -1;
		///
		/// ↓描画処理ここから
		///
		Novice::DrawSprite(0 - scrollx, 0, bgTex[0], 1, 1, 0.0f, 0xFFFFFFFF);
		Novice::DrawSprite(1280 - scrollx, 0, bgTex[1], 1, 1, 0.0f, 0xFFFFFFFF);
		Novice::DrawEllipse((int)player.center.x - scrollx, posy2, (int)player.radius, (int)player.radius, 0.0f, WHITE, kFillModeSolid);
		Novice::DrawEllipse((int)enemy.center.x, (int)posy3, enemy.radius, enemy.radius, 0.0f, WHITE, kFillModeSolid);
		if (flag == 1) {
			Novice::DrawEllipse((int)enemy.center.x, (int)posy2, enemy.radius, enemy.radius, 0.0f, BLUE, kFillModeSolid);
		}
		Novice::DrawLine((int)enemy.center.x, (int)posy2, (int)enemy.center.x + (int)enemy.direction.x * 200, (int)posy, WHITE);
		Novice::DrawLine((int)player.center.x, (int)posy2, (int)player.center.x + (int)player.direction.x * 200, (int)posy, WHITE);
		Novice::DrawLine(0, posy, 1280, posy, WHITE);
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