/**
 * @file Shop.ixx
 * @brief Modu� definiuj�cy klas� Shop, obs�uguj�c� wyb�r dinozaur�w.
 *
 * Klasa Shop pozwala graczowi wybra� dinozaura do gry.
 */

module;
#include "raylib.h"
#include <array>
#include <string>
export module ShopModule;

import ResourcesModule;
import ConfigModule;


/**
 * @class Shop
 * @brief Klasa obs�uguj�ca interfejs sklepu w grze.
 *
 * Wy�wietla dost�pne dinozaury i pozwala na ich wyb�r.
 */
export class Shop
{
private:
    /** @brief Liczba dinozaur�w w sklepie. */
    static constexpr int DinoCount{ Config::SHOP_DINO_COUNT }; 
    /** @brief Tekstury dinozaur�w. */
    Texture2D dinos[DinoCount];
    /** @brief Prostok�ty klikalnych obszar�w dinozaur�w. */
    Rectangle dinoRecs[DinoCount];
    /** @brief Tekstura t�a sklepu. */
    Texture2D background;
    /** @brief Obiekt zasob�w gry. */
    Resources resources;
    /** @brief Bazowe skale dla ka�dego dinozaura. */
    std::array<float, DinoCount> baseScales{ 6.0f, 7.5f, 8.5f, 7.0f };
    /** @brief Szeroko�ci klatek animacji dinozaur�w. */
    std::array<float, DinoCount> frameWidths;
    /** @brief Wysoko�ci klatek animacji dinozaur�w. */
    std::array<float, DinoCount> frameHeights;
    /** @brief Liczby klatek animacji dla ka�dego dinozaura. */
    std::array<int, DinoCount> frameCounts{ 6, 4, 4, 6 };

    /** @brief Aktualna klatka animacji. */
    int frame{ 0 };
    /** @brief Czas od ostatniej zmiany klatki. */
    float runningTime{ 0 };
    /** @brief Czas mi�dzy zmianami klatek. */
    float updateTime{ Config::SHOP_ANIMATION_UPDATE_TIME }; 
    /** @brief Indeks wybranego dinozaura. */
    int selectedDinoIndex{ 0 };

    /** @brief Pocz�tkowa szeroko�� ekranu (dla skalowania). */
    int initialScreenWidth{ 0 };
    /** @brief Pocz�tkowa wysoko�� ekranu (dla skalowania). */
    int initialScreenHeight{ 0 };

public:
    /**
    * @brief Inicjalizuje sklep.
    * @param res Referencja do zasob�w gry.
    * @param screenWidth Szeroko�� ekranu.
    * @param screenHeight Wysoko�� ekranu.
    */
    void init(Resources& res, int screenWidth, int screenHeight)
    {
        resources = res;
        dinos[0] = resources.getGreenIdle();
        dinos[1] = resources.getBlueIdle();
        dinos[2] = resources.getYellowIdle();
        dinos[3] = resources.getRedIdle();

        initialScreenWidth = screenWidth;
        initialScreenHeight = screenHeight;

        for (int i{ 0 }; i < DinoCount; i++) {
            frameWidths[i] = static_cast<float>(dinos[i].width / 4);
            frameHeights[i] = static_cast<float>(dinos[i].height);
        }
    }

    /**
     * @brief Wy�wietla interfejs sklepu.
     * @param screenWidth Szeroko�� ekranu.
     * @param screenHeight Wysoko�� ekranu.
     */
    void show(int screenWidth, int screenHeight) {
        runningTime = 0;
        frame = 0;

        while (!WindowShouldClose()) {
            float deltaTime = GetFrameTime();
            runningTime += deltaTime;
            if (runningTime >= updateTime) {
                runningTime = 0;
                frame++;
                if (frame > 3) frame = 0;
            }

            if (IsKeyPressed(KEY_ESCAPE)) return;
          
            screenWidth = GetScreenWidth();
            screenHeight = GetScreenHeight();

            float widthScale = static_cast<float>(screenWidth) / initialScreenWidth;
            float heightScale = static_cast<float>(screenHeight) / initialScreenHeight;

           
            for (int i{ 0 }; i < DinoCount; i++) {
                dinoRecs[i] = Rectangle{
                    static_cast<float>(i * screenWidth / DinoCount),
                    static_cast<float>(screenHeight * 0.65f - 110.f * heightScale),
                    static_cast<float>(screenWidth / DinoCount),
                    220.f * heightScale
                };
            }

            Vector2 mouse = GetMousePosition();
            for (int i{ 0 }; i < DinoCount; i++) {
                if (CheckCollisionPointRec(mouse, dinoRecs[i])) {
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        selectedDinoIndex = i;
                    }
                }
            }

            BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawTexturePro(
                resources.getShopBackground(),
                Rectangle{ 0, 0, static_cast<float>(resources.getShopBackground().width), static_cast<float>(resources.getShopBackground().height) },
                Rectangle{ 0, 0, static_cast<float>(screenWidth), static_cast<float>(screenHeight) },
                Vector2{ 0, 0 },
                0.0f,
                WHITE
            );

            for (int i = 0; i < DinoCount; i++) {
                float currentScale = baseScales[i] * (widthScale + heightScale) / 2.0f;

                Rectangle dest = {
                    dinoRecs[i].x + (dinoRecs[i].width - frameWidths[i] * currentScale) / 2,
                    screenHeight * 0.85f - frameHeights[i] * currentScale,
                    frameWidths[i] * currentScale,
                    frameHeights[i] * currentScale
                };

                Rectangle source = { frame * frameWidths[i], 0.0f, frameWidths[i], frameHeights[i] };

                DrawTexturePro(dinos[i], source, dest, Vector2{ 0, 0 }, 0.0f, WHITE);

                if (i == selectedDinoIndex) {
                    Rectangle selectionFrame = dinoRecs[i];
                    selectionFrame.y += 50.f * heightScale; // Przesuni�cie w d�, skalowane
                    DrawRectangleLinesEx(selectionFrame, 4.f * widthScale, GREEN);
                }
            }

            DrawText("Press ESC to return", screenWidth / 2 - MeasureText("Press ESC to return", 20) / 2, screenHeight - 30, 20, BLACK);
            EndDrawing();
        }
    }

    /**
     * @brief Zwraca indeks wybranego dinozaura.
     * @return Indeks wybranego dinozaura.
     */
    int getSelectedDino() const {
        return selectedDinoIndex;
    }


    /**
     * @brief Zwraca liczb� klatek animacji wybranego dinozaura.
     * @return Liczba klatek animacji.
     */
    int getSelectedDinoFrameCount() const {
        return frameCounts[selectedDinoIndex];
    }
};