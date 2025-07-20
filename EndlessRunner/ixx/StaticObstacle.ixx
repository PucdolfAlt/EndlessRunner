/**
 * @file StaticObstacle.ixx
 * @brief Modu� definiuj�cy klas� StaticObstacle, reprezentuj�c� statyczne przeszkody.
 *
 * Klasa StaticObstacle dziedziczy po Obstacle i obs�uguje przeszkody bez animacji.
 */
module;
#include "raylib.h"
export module StaticObstacleModule;

import ObstacleModule;
import ConfigModule;

/**
 * @class StaticObstacle
 * @brief Klasa reprezentuj�ca statyczn� przeszkod� w grze.
 *
 * Przeszkody statyczne (np. kaktusy, drzewa) poruszaj� si� w lewo bez animacji.
 */
export class StaticObstacle : public Obstacle {
public:

    /**
     * @brief Inicjalizuje statyczn� przeszkod�.
     * @param tex Tekstura przeszkody.
     * @param startX Pocz�tkowa pozycja X.
     * @param startY Pocz�tkowa pozycja Y (przesuni�ta w g�r� o wysoko�� tekstury).
     * @param scaleFactor Skala przeszkody.
     * @param frameCount Liczba klatek (zawsze 1 dla statycznych).
     * @param animUpdateTime Czas mi�dzy zmianami klatek (nieu�ywany).
     */
    void init(const Texture2D& tex, float startX, float startY, float scaleFactor, int frameCount, float animUpdateTime = Config::ANIMATION_UPDATE_TIME) override;/* {
        Obstacle::init(tex, startX, startY - tex.height * scaleFactor, scaleFactor, 1, animUpdateTime); 
    }*/

    /**
     * @brief Aktualizuje pozycj� przeszkody.
     * @param deltaTime Czas od ostatniej klatki (w sekundach).
     */
    void update(float deltaTime) override; /*{
        screenPos.x += velocity * deltaTime; 
    }*/


    /**
    * @brief Rysuje przeszkod� na ekranie.
    */
    void draw() const override;/* {
        Rectangle source{ 0.f, 0.f, width, height };
        Rectangle dest{ screenPos.x, screenPos.y, width * scale, height * scale };
        DrawTexturePro(texture, source, dest, { 0.f, 0.f }, 0.f, WHITE);
    }*/


    /**
    * @brief Zwraca prostok�t kolizji przeszkody.
    * @return Prostok�t kolizji w przestrzeni ekranu.
    */
    Rectangle getCollisionRec() const override;/* {
        return Rectangle{
            screenPos.x,
            screenPos.y,
            width * scale,
            height * scale
        };
    }*/
};