/**
 * @file Bat.ixx
 * @brief Modu� definiuj�cy klas� Bat, reprezentuj�c� animowan� przeszkod� typu nietoperz.
 *
 * Klasa Bat dziedziczy po Obstacle i specjalizuje zachowanie dla nietoperza.
 */

module;
#include "raylib.h"
export module BatModule;

import ObstacleModule;
import ConfigModule;

/**
 * @class Bat
 * @brief Klasa reprezentuj�ca przeszkod� typu nietoperz.
 *
 * Nietoperz jest animowan� przeszkod�, kt�ra porusza si� w lewo i pojawia si� na okre�lonej wysoko�ci.
 */
export class Bat : public Obstacle {
public:

    /**
     * @brief Inicjalizuje nietoperza z podan� tekstur� i parametrami.
     * @param tex Tekstura nietoperza.
     * @param startX Pocz�tkowa pozycja X.
     * @param startY Pocz�tkowa pozycja Y (przesuni�ta o 150 pikseli w g�r�).
     * @param scaleFactor Skala nietoperza.
     * @param frameCount Liczba klatek animacji.
     * @param animUpdateTime Czas mi�dzy zmianami klatek.
     */
    void init(const Texture2D& tex, float startX, float startY, float scaleFactor, int frameCount, float animUpdateTime = Config::ANIMATION_UPDATE_TIME) override; /*{
        Obstacle::init(tex, startX, startY - 150.f, scaleFactor, frameCount, animUpdateTime);
    }*/


    /**
    * @brief Aktualizuje stan nietoperza.
    * @param deltaTime Czas od ostatniej klatki (w sekundach).
    */
    void update(float deltaTime) override; /*{
        Obstacle::update(deltaTime); 
    }*/


    /**
    * @brief Rysuje nietoperza na ekranie.
    */
    void draw() const override; /*{
        AnimatedSprite::draw();
    }*/

    /**
    * @brief Zwraca prostok�t kolizji nietoperza.
    * @return Prostok�t kolizji w przestrzeni ekranu.
    */
    Rectangle getCollisionRec() const override; /*{
        return AnimatedSprite::getCollisionRec();
    }*/
};