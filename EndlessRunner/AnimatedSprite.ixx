/**
 * @file AnimatedSprite.ixx
 * @brief Modu� definiuj�cy klas� bazow� dla animowanych sprite'�w w grze.
 *
 * Zawiera klas� AnimatedSprite, kt�ra zarz�dza animacj�, pozycj� i renderowaniem sprite'�w.
 * Jest klas� bazow� dla gracza, przeszk�d i efekt�w wizualnych (np. py�u).
 */

module;
#include "raylib.h"
#include <iostream>
export module AnimatedSpriteModule;

import ConfigModule;

/**
 * @class AnimatedSprite
 * @brief Klasa bazowa dla animowanych sprite'�w w grze.
 *
 * Odpowiada za zarz�dzanie pozycj�, skal�, animacj� i renderowaniem sprite'�w.
 * Klasa jest abstrakcyjna i wymaga nadpisywania metod wirtualnych w klasach pochodnych.
 */
export class AnimatedSprite {
protected:
    /** @brief Pozycja sprite'a na ekranie (w pikselach). */
    Vector2 screenPos{ 0.f, 0.f };
    /** @brief Skala sprite'a (mno�nik rozmiaru). */
    float scale{ 1.f };

    /** @brief Tekstura sprite'a. */
    Texture2D texture{};
    /** @brief Szeroko�� pojedynczej klatki animacji (w pikselach). */
    float width{ 0.f };
    /** @brief Wysoko�� sprite'a (w pikselach). */
    float height{ 0.f };

    /** @brief Aktualna klatka animacji (indeks). */
    int frame{ 0 };
    /** @brief Maksymalna liczba klatek w animacji. */
    float maxFrames{ 1.f };
    /** @brief Czas mi�dzy zmianami klatek animacji (w sekundach). */
    float updateTime{ Config::ANIMATION_UPDATE_TIME };
    /** @brief Czas od ostatniej zmiany klatki (w sekundach). */
    float runningTime{ 0.f };

    /** @brief Flaga okre�laj�ca, czy sprite jest aktywny (widoczny i aktualizowany). */
    bool isActive{ true };

public:
    AnimatedSprite() = default;
    virtual ~AnimatedSprite() = default;

    /**
    * @brief Inicjalizuje sprite'a z podan� tekstur� i parametrami.
    * @param tex Tekstura sprite'a.
    * @param startX Pocz�tkowa pozycja X na ekranie.
    * @param startY Pocz�tkowa pozycja Y na ekranie.
    * @param scaleFactor Skala sprite'a.
    * @param frameCount Liczba klatek w animacji.
    * @param animUpdateTime Czas mi�dzy zmianami klatek (domy�lnie Config::ANIMATION_UPDATE_TIME).
    */
    virtual void init(const Texture2D& tex, float startX, float startY, float scaleFactor, int frameCount, float animUpdateTime = Config::ANIMATION_UPDATE_TIME) {
        texture = tex;
        maxFrames = static_cast<float>(frameCount);
        width = static_cast<float>(tex.width) / maxFrames;
        height = static_cast<float>(tex.height);
        screenPos = { startX, startY };
        scale = scaleFactor;
        frame = 0;
        runningTime = 0.f;
        isActive = true;
        updateTime = animUpdateTime;
    
    }

    /**
     * @brief Aktualizuje stan sprite'a (np. animacj�).
     * @param deltaTime Czas od ostatniej klatki (w sekundach).
     */
    virtual void update(float deltaTime) {
        if (!isActive) return;
        updateAnimation(deltaTime);
    }

    /**
    * @brief Rysuje sprite'a na ekranie.
    */
    virtual void draw() const {
        if (!isActive) return;
        Rectangle source{ frame * width, 0.f, width, height };
        Rectangle dest{ screenPos.x, screenPos.y, width * scale, height * scale };
        DrawTexturePro(texture, source, dest, { 0.f, 0.f }, 0.f, WHITE);
    }

    /**
     * @brief Zwraca prostok�t kolizji sprite'a.
     * @return Prostok�t kolizji w przestrzeni ekranu.
     */
    virtual Rectangle getCollisionRec() const {
        return Rectangle{
            screenPos.x,
            screenPos.y,
            width * scale,
            height * scale
        };
    }

    /**
    * @brief Zwraca aktualn� pozycj� sprite'a.
    * @return Wektor z wsp�rz�dnymi X i Y.
    */
    Vector2 getPosition() const { return screenPos; }

    /**
     * @brief Sprawdza, czy sprite jest aktywny.
     * @return True, je�li sprite jest aktywny.
     */
    bool getIsActive() const { return isActive; }

    /**
    * @brief Zwraca tekstur� sprite'a.
    * @return Referencja do tekstury.
    */
    const Texture2D& getTexture() const { return texture; }

    /**
    * @brief Zwraca aktualn� klatk� animacji.
    * @return Indeks aktualnej klatki.
    */
    int getFrame() const { return frame; }

protected:
    /**
     * @brief Aktualizuje animacj� sprite'a.
     * @param deltaTime Czas od ostatniej klatki (w sekundach).
     */
    void updateAnimation(float deltaTime) {
        runningTime += deltaTime;
        if (runningTime >= updateTime) {
            runningTime = 0.f;
            frame++;
            if (frame >= maxFrames) frame = 0;
        }
    }

    /**
    * @brief Ustawia aktywno�� sprite'a.
    * @param active True, aby aktywowa� sprite; false, aby dezaktywowa�.
    */
    void setActive(bool active) { isActive = active; }
};