/**
 * @file Dust.ixx
 * @brief Modu� definiuj�cy klas� Dust, reprezentuj�c� efekt py�u.
 *
 * Klasa Dust dziedziczy po AnimatedSprite i obs�uguje animacj� py�u pojawiaj�cego si� pod graczem.
 */

module;
#include "raylib.h"
export module DustModule;

import AnimatedSpriteModule;
import ConfigModule;


/**
 * @class Dust
 * @brief Klasa reprezentuj�ca efekt py�u w grze.
 *
 * Py� pojawia si�, gdy gracz biegnie po ziemi, i zanika po zako�czeniu animacji.
 */
export class Dust : public AnimatedSprite {
public:
    /**
    * @brief Inicjalizuje efekt py�u.
    * @param dustTex Tekstura py�u.
    * @param startX Pocz�tkowa pozycja X.
    * @param startY Pocz�tkowa pozycja Y.
    * @param scaleFactor Skala py�u.
    * @param frameCount Liczba klatek animacji.
    * @param animUpdateTime Czas mi�dzy zmianami klatek.
    */
    void init(const Texture2D& dustTex, float startX, float startY, float scaleFactor, int frameCount, float animUpdateTime) override {
        AnimatedSprite::init(dustTex, startX, startY, scaleFactor, frameCount, animUpdateTime);
    }

    /**
     * @brief Aktualizuje stan py�u (animacj� i aktywno��).
     * @param deltaTime Czas od ostatniej klatki (w sekundach).
     */
    void update(float deltaTime) override {
        if (!isActive) return;
        runningTime += deltaTime;
        if (runningTime >= updateTime) {
            runningTime = 0.f;
            frame++;
            if (frame >= maxFrames) {
                isActive = false;
                frame = static_cast<int>(maxFrames) - 1;
            }
        }
    }

    /**
    * @brief Rysuje py� z efektem zanikania (alpha).
    */
    void draw() const override {
        if (!isActive) return;
        Rectangle source{ frame * width, 0.f, width, height };
        Rectangle dest{ screenPos.x, screenPos.y, width * scale, height * scale };
        float alpha = (frame < maxFrames - 1) ? 1.f : 1.f - (runningTime / updateTime);
        Color color{ 255, 255, 255, static_cast<unsigned char>(255 * alpha) };
        DrawTexturePro(texture, source, dest, { 0.f, 0.f }, 0.f, color);
    }
};