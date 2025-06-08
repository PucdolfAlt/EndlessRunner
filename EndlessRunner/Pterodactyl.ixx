/**
 * @file Pterodactyl.ixx
 * @brief Modu� definiuj�cy klas� Pterodactyl, reprezentuj�c� przeszkod� typu pterodaktyl.
 *
 * Klasa Pterodactyl dziedziczy po Obstacle i specjalizuje zachowanie dla pterodaktyla.
 */

module;
#include "raylib.h"
export module PterodactylModule;

import ObstacleModule;
import ConfigModule;

/**
 * @class Pterodactyl
 * @brief Klasa reprezentuj�ca przeszkod� typu pterodaktyl.
 *
 * Pterodactyl jest animowan� przeszkod�, kt�ra porusza si� w lewo i pojawia si� na okre�lonej wysoko�ci.
 */
export class Pterodactyl : public Obstacle {
public:
    /**
    * @brief Inicjalizuje pterodaktyla.
    * @param tex Tekstura pterodaktyla.
    * @param startX Pocz�tkowa pozycja X.
    * @param startY Pocz�tkowa pozycja Y (przesuni�ta o 260 pikseli w g�r�).
    * @param scaleFactor Skala animacji.
    * @param frameCount Liczba klatek animacji.
    * @param animUpdateTime Czas mi�dzy zmianami klatek.
    */
    void init(const Texture2D& tex, float startX, float startY, float scaleFactor, int frameCount, float animUpdateTime) override {
        Obstacle::init(tex, startX, startY - 260.f, scaleFactor, frameCount, animUpdateTime);
    }

    /**
     * @brief Aktualizuje stan pterodaktyla.
     * @param deltaTime Czas od ostatniej klatki (w sekundach).
     */
    void update(float deltaTime) override {
        Obstacle::update(deltaTime); // Upewniamy si�, �e animacja jest aktualizowana
    }

    /**
     * @brief Rysuje pterodaktyla na ekranie.
     */
    void draw() const override {
        AnimatedSprite::draw();
    }

    /**
    * @brief Zwraca prostok�t kolizji pterodaktyla.
    * @return Prostok�t kolizji w przestrzeni ekranu.
    */
    Rectangle getCollisionRec() const override {
        return AnimatedSprite::getCollisionRec();
    }
};