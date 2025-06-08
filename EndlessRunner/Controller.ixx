/**
 * @file Controller.ixx
 * @brief Modu� definiuj�cy klas� Controller, zarz�dzaj�c� g��wn� p�tl� gry.
 *
 * Klasa Controller odpowiada za renderowanie t�a, interfejsu u�ytkownika i obs�ug� stanu gry.
 */
module;
#include "raylib.h"
#include <iostream>
#include <fstream>
#include <random>
export module ControllerModule;

import BoardModule;
import ResourcesModule;
import ObstacleFactoryModule;
import ConfigModule;
import <string>;

/**
 * @class Controller
 * @brief Klasa zarz�dzaj�ca g��wn� p�tl� gry i renderowaniem.
 *
 * Odpowiada za inicjalizacj� gry, aktualizacj� stanu, renderowanie t�a i UI oraz zapis wynik�w.
 */
export class Controller {
private:
	/** @brief Referencja do zasob�w gry. */
	Resources& resources;
	/** @brief Referencja do planszy gry. */
	Board& board;
	/** @brief Nazwa u�ytkownika. */
	std::string username;
	/** @brief Szeroko�� okna gry. */
	int windowWidth{};
	/** @brief Wysoko�� okna gry. */
	int windowHeight{};
	/** @brief Skala element�w interfejsu u�ytkownika. */
	float numbScale = Config::UI_NUMB_SCALE;

	/** @brief Pozycja X t�a (background). */
	float bgX{};
	/** @brief Pozycja X warstwy �rodkowej (midground). */
	float mgX{};
	/** @brief Pozycja X warstwy przedniej (foreground). */
	float fgX{};
	/** @brief Pozycja X ziemi. */
	float groundX{};
	/** @brief Flaga wskazuj�ca na przegran�. */
	bool gameOver{ false };
	/** @brief Liczba �y� gracza. */
	int lives = Config::PLAYER_LIVES; 
	/** @brief Aktualny wynik gracza. */
	float score = 0.0f;
	/** @brief Flaga wskazuj�ca, czy wynik zosta� zapisany. */
	bool savedScore{ false };

	/** @brief Tekstura t�a. */
	Texture2D bgTexture{};
	/** @brief Tekstura warstwy �rodkowej. */
	Texture2D mgTexture{};
	/** @brief Tekstura warstwy przedniej. */
	Texture2D fgTexture{};
	/** @brief Tekstura ziemi. */
	Texture2D groundTexture{};

	/**
	 * @brief Zapisuje wynik gracza do pliku.
	 */
	void saveScore() {
		if (username.empty() || savedScore) return;
		std::ofstream file("scores.txt", std::ios::app);
		if (file.is_open()) {
			file << username << ":" << static_cast<int>(score) << "\n";
			file.close();
			savedScore = true;
		}
		else {
			std::cout << "Failed to open scores.txt for writing\n";
		}
	}

public:
	/**
	 * @brief Konstruktor klasy Controller.
	 * @param res Referencja do zasob�w gry.
	 * @param b Referencja do planszy gry.
	 * @param user Nazwa u�ytkownika.
	 * @param width Szeroko�� okna.
	 * @param height Wysoko�� okna.
	 */
	Controller(Resources& res, Board& b, const std::string& user, int width, int height)
		: resources(res), board(b), username(user), windowWidth(width), windowHeight(height) {
	}

	/**
	 * @brief Uruchamia g��wn� p�tl� gry.
	 */
	void run() {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, 3); // 0-3, bo mamy 4 t�a
		int backgroundChoice = dis(gen);

		BackgroundType bgType;
		switch (backgroundChoice) {
		case 0: // Pustynia dzienna
			bgTexture = resources.getDesertBackground();
			mgTexture = resources.getDesertMidground();
			fgTexture = resources.getDesertForeground();
			groundTexture = resources.getDesertGround();
			bgType = BackgroundType::DESERT_DAY;
			break;
		case 1: // Pustynia noc�
			bgTexture = resources.getDesertNightBackground();
			mgTexture = resources.getDesertNightMidground();
			fgTexture = resources.getDesertNightForeground();
			groundTexture = resources.getDesertNightGround();
			bgType = BackgroundType::DESERT_NIGHT;
			break;
		case 2: // Las za dnia
			bgTexture = resources.getForestBackground();
			mgTexture = resources.getForestMidground();
			fgTexture = resources.getForestForeground();
			groundTexture = resources.getForestGround();
			bgType = BackgroundType::FOREST_DAY;
			break;
		case 3: // Las noc�
			bgTexture = resources.getForestNightBackground();
			mgTexture = resources.getForestNightMidground();
			fgTexture = resources.getForestNightForeground();
			groundTexture = resources.getForestNightGround();
			bgType = BackgroundType::FOREST_NIGHT;
			break;
		default:
			bgTexture = resources.getDesertBackground();
			mgTexture = resources.getDesertMidground();
			fgTexture = resources.getDesertForeground();
			bgType = BackgroundType::DESERT_DAY;
			break;
		}

		board.setBackgroundType(bgType);
		board.init(resources.getGreenDinoRun(), GetScreenWidth(), GetScreenHeight());
		SetTargetFPS(60);

		while (!WindowShouldClose()) {
			float dt = GetFrameTime();

			BeginDrawing();
			ClearBackground(BLACK);

			scrollBackground(dt, GetScreenWidth(), GetScreenHeight());

			if (!gameOver) {
				board.update(dt, windowHeight);
				gameOver = board.checkLoss();
				score += dt;
			}

			if (gameOver) {
				std::string gameOverText = "Game Over! Score: " + std::to_string(static_cast<int>(score));
				DrawText(gameOverText.c_str(), windowWidth / 2 - MeasureText(gameOverText.c_str(), 40) / 2, windowHeight / 2, 40, RED);
				if (!savedScore) {
					saveScore();
				}
				DrawText("Press ENTER to return to menu", windowWidth / 2 - MeasureText("Press ENTER to return to menu", 20) / 2, windowHeight / 2 + 50, 20, WHITE);
				if (IsKeyPressed(KEY_ENTER)) {
					gameOver = false;
					break;
				}
			}
			else {
				board.draw();
			}

			drawUI();

			EndDrawing();
		}
	}

private:
	/**
	* @brief Przewija t�o gry (paralaksa).
	* @param dt Czas od ostatniej klatki (w sekundach).
	* @param windowWidth Szeroko�� okna.
	* @param windowHeight Wysoko�� okna.
	*/
	void scrollBackground(float dt, int windowWidth, int windowHeight) {
		float avgHeight = (bgTexture.height + mgTexture.height + fgTexture.height) / 3.0f;
		float universalScale = static_cast<float>(windowHeight) / avgHeight;

		float bgScaledWidth = bgTexture.width * universalScale;
		float mgScaledWidth = mgTexture.width * universalScale;
		float fgScaledWidth = fgTexture.width * universalScale;
		float groundScaledWidth = groundTexture.width * universalScale;

		// Przesuwanie z r�nymi pr�dko�ciami
		bgX -= 20 * dt;
		mgX -= 40 * dt;
		fgX -= 60 * dt;
		groundX -= 200 * dt;

		// Resetowanie pozycji, gdy tekstura ca�kowicie wyjdzie poza ekran
		// U�ywamy modulo, aby zapewni� ci�g�o��
		bgX = fmod(bgX, bgScaledWidth);
		if (bgX > 0) bgX -= bgScaledWidth; // Zapewniamy, �e pierwsza instancja zaczyna si� od lewej kraw�dzi
		mgX = fmod(mgX, mgScaledWidth);
		if (mgX > 0) mgX -= mgScaledWidth;
		fgX = fmod(fgX, fgScaledWidth);
		if (fgX > 0) fgX -= fgScaledWidth;
		groundX = fmod(groundX, groundScaledWidth);
		if (groundX > 0) groundX -= groundScaledWidth;

		auto drawTileableLayer = [&](Texture2D texture, float xPos, float scaledWidth) {
			// Obliczamy, ile instancji tekstury potrzebujemy, aby pokry� ekran + jedna dodatkowa
			int numInstances = static_cast<int>(windowWidth / scaledWidth) + 2;
			// Przesuni�cie pocz�tkowe, aby pierwsza instancja zaczyna�a si� poza ekranem
			float startX = xPos;
			for (int i = 0; i < numInstances; ++i) {
				float drawX = startX + i * scaledWidth;
				DrawTextureEx(texture, { drawX, 0 }, 0.0f, universalScale, WHITE);
			}
			};

		auto drawForegroundLayer = [&](Texture2D texture, float xPos, float scaledWidth) {
			int numInstances = static_cast<int>(windowWidth / scaledWidth) + 2;
			float startX = xPos;
			for (int i = 0; i < numInstances; ++i) {
				float drawX = startX + i * scaledWidth;
				float textureHeight = texture.height * universalScale;
				float drawY = windowHeight - textureHeight;
				DrawTextureEx(texture, { drawX, drawY }, 0.0f, universalScale, WHITE);
			}
			};

		drawTileableLayer(bgTexture, bgX, bgScaledWidth);
		drawTileableLayer(mgTexture, mgX, mgScaledWidth);
		drawForegroundLayer(fgTexture, fgX, fgScaledWidth);
		drawTileableLayer(groundTexture, groundX, groundScaledWidth);
	}

	/**
	 * @brief Rysuje interfejs u�ytkownika (np. wynik, ikony �y�).
	 */
	void drawUI() {
		Texture2D lifeFull = resources.getHeartIcon();
		Texture2D lifeEmpty = resources.getLifeLostIcon();

		Texture2D icon = (!gameOver) ? lifeFull : lifeEmpty;
		float x = 10;
		float y = 10;
		DrawTextureEx(icon, { x, y }, 0.f, numbScale, WHITE);


		Texture2D numbers = resources.getNumbersTexture();
		int s = static_cast<int>(score);
		std::string scoreText = std::to_string(s);

		int digitWidth = numbers.width / 10;
		int digitHeight = numbers.height;

		float scaledWidth = digitWidth * numbScale;
		float totalWidth = scaledWidth * scoreText.length();
		windowWidth = GetScreenWidth();

		for (int i = 0; i < scoreText.length(); ++i) {
			int digit = scoreText[i] - '0';
			Rectangle src = { static_cast<float>(digit * digitWidth), 0, static_cast<float>(digitWidth), static_cast<float>(digitHeight) };
			Rectangle dest = {
				static_cast<float>(windowWidth - totalWidth - 10 + i * scaledWidth),
				10.0f,
				static_cast<float>(scaledWidth),
				static_cast<float>(digitHeight * numbScale)
			};
			DrawTexturePro(numbers, src, dest, { 0, 0 }, 0.0f, WHITE);
		}
	}
};
