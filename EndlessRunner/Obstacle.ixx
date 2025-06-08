/**
 * @file Obstacle.ixx
 * @brief Modu� definiuj�cy klas� bazow� dla przeszk�d w grze.
 *
 * Klasa Obstacle dziedziczy po AnimatedSprite i definiuje wsp�lne zachowanie dla przeszk�d.
 */


module;
#include "raylib.h"
export module ObstacleModule;

import AnimatedSpriteModule;
import ConfigModule;


/**
 * @class Obstacle
 * @brief Klasa bazowa dla przeszk�d w grze.
 *
 * Odpowiada za ruch w lewo i jest klas� abstrakcyjn� dla konkretnych przeszk�d (np. Bat, Pterodactyl).
 */
export class Obstacle : public AnimatedSprite {
protected:
    /** @brief Pr�dko�� przesuwania przeszkody w lewo (piksele/s). */
    float velocity{ -200.f }; // Pr�dko�� przesuwania w lewo

public:
    Obstacle() = default;
    virtual ~Obstacle() = default;

    /**
     * @brief Inicjalizuje przeszkod�.
     * @param tex Tekstura przeszkody.
     * @param startX Pocz�tkowa pozycja X.
     * @param startY Pocz�tkowa pozycja Y.
     * @param scaleFactor Skala przeszkody.
     * @param frameCount Liczba klatek animacji.
     * @param animUpdateTime Czas mi�dzy zmianami klatek.
     */
    virtual void init(const Texture2D& tex, float startX, float startY, float scaleFactor, int frameCount, float animUpdateTime) override {
        AnimatedSprite::init(tex, startX, startY, scaleFactor, frameCount, animUpdateTime);
    }

    /**
    * @brief Aktualizuje stan przeszkody (ruch i animacj�).
    * @param deltaTime Czas od ostatniej klatki (w sekundach).
    */
    virtual void update(float deltaTime) override {
        AnimatedSprite::update(deltaTime);
        screenPos.x += velocity * deltaTime;
    }

    /**
    * @brief Rysuje przeszkod� na ekranie.
    */
    virtual void draw() const override = 0;

    /**
     * @brief Zwraca prostok�t kolizji przeszkody.
     * @return Prostok�t kolizji w przestrzeni ekranu.
     */
    virtual Rectangle getCollisionRec() const override = 0;

    /**
     * @brief Zwraca pozycj� X przeszkody.
     * @return Wsp�rz�dna X przeszkody.
     */
    virtual float getPositionX() const { return screenPos.x; }
};