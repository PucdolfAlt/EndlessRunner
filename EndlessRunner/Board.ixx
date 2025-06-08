/**
 * @file Board.ixx
 * @brief Modu� definiuj�cy klas� Board, zarz�dzaj�c� rozgrywk�.
 *
 * Klasa Board integruje gracza, przeszkody, py� i losowe generowanie przeszk�d.
 */

module;
#include "raylib.h"
#include <concepts> 
export module BoardModule;

import <array>;
import <vector>;
import <memory>;
import <random>;
import PlayerModule;
import DustModule;
import BatModule;
import PterodactylModule;
import ResourcesModule;
import CollisionHandlingModule;
import ObstacleModule;
import ObstacleFactoryModule;
import ConfigModule;
import ObstacleFactoryModule;


/**
 * @class Board
 * @brief Klasa zarz�dzaj�ca g��wn� logik� rozgrywki.
 *
 * Odpowiada za aktualizacj� i renderowanie gracza, przeszk�d, py�u oraz sprawdzanie kolizji.
 */
export class Board {
private:
    /** @brief Obiekt gracza. */
    Player player{};
    /** @brief Wektor przechowuj�cy aktywne przeszkody. */
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    /** @brief Pozycja X ostatniej przeszkody. */
    float lastObstacleX{ 0.f };
    /** @brief Minimalna odleg�o�� mi�dzy przeszkodami. */
    float minObstacleDistance{ Config::MIN_OBSTACLE_DISTANCE };
    /** @brief Maksymalna odleg�o�� mi�dzy przeszkodami. */
    float maxObstacleDistance{ Config::MAX_OBSTACLE_DISTANCE };

    /** @brief Tekstura wybranego dinozaura. */
    Texture2D selectedDinoTex{};
    /** @brief Liczba klatek animacji dinozaura. */
    int selectedDinoFrameCount{ 6 };

    /** @brief Obiekt py�u (efekt wizualny). */
    Dust dust{};
    /** @brief Licznik czasu do spawnu py�u. */
    float dustSpawnTimer{ 0.0f };
    /** @brief Interwa� spawnu py�u. */
    float dustSpawnInterval{ Config::DUST_SPAWN_INTERVAL };

    /** @brief Generator liczb losowych. */
    std::random_device rd;
    /** @brief Silnik losuj�cy. */
    std::mt19937 gen{ rd() };

    /** @brief Szeroko�� okna gry. */
    int windowWidth{};
    /** @brief Wysoko�� okna gry. */
    int windowHeight{};

    /** @brief Referencja do zasob�w gry (tekstury). */
    Resources& resources;
    /** @brief Fabryka przeszk�d. */
    ObstacleFactory obstacleFactory;
    /** @brief Aktualny typ t�a gry. */
    BackgroundType currentBgType;

    /**
    * @brief Generyczna metoda aktualizuj�ca przeszkody okre�lonego typu.
    * @tparam T Typ przeszkody (musi dziedziczy� po Obstacle).
    * @param deltaTime Czas od ostatniej klatki (w sekundach).
    */
    template<ObstacleType T>
    void updateObstacles(float deltaTime) {
        for (auto& obstacle : obstacles) {
            if constexpr (std::is_base_of_v<Obstacle, T>) {
                obstacle->update(deltaTime);
            }
        }
    }

    /**
    * @brief Generyczna metoda rysuj�ca przeszkody okre�lonego typu.
    * @tparam T Typ przeszkody (musi dziedziczy� po Obstacle).
    */
    template<ObstacleType T>
    void drawObstacles() const {
        for (const auto& obstacle : obstacles) {
            if constexpr (std::is_base_of_v<Obstacle, T>) {
                obstacle->draw();
            }
        }
    }

public:

    /**
     * @brief Konstruktor klasy Board.
     * @param res Referencja do zasob�w gry.
     */
    Board(Resources& res) : resources(res), obstacleFactory(res) {}


    /**
    * @brief Ustawia tekstur� dinozaura dla gracza.
    * @param dinoTex Tekstura dinozaura.
    */
    void setDinoTex(const Texture2D& dinoTex) {
        selectedDinoTex = dinoTex;
    }

    /**
     * @brief Ustawia liczb� klatek animacji dinozaura.
     * @param frameCount Liczba klatek animacji.
     */
    void setDinoFrameCount(int frameCount) {
        selectedDinoFrameCount = frameCount;
    }

    /**
     * @brief Ustawia typ t�a gry.
     * @param bgType Typ t�a (np. DESERT_DAY, FOREST_NIGHT).
     */
    void setBackgroundType(BackgroundType bgType) {
        currentBgType = bgType;
    }

    /**
    * @brief Inicjalizuje plansz� gry.
    * @param dinoTex Tekstura dinozaura.
    * @param windowWidth Szeroko�� okna gry (w pikselach).
    * @param windowHeight Wysoko�� okna gry (w pikselach).
    */
    void init(const Texture2D& dinoTex, int windowWidth, int windowHeight) {
        if (selectedDinoTex.id == 0) {
            selectedDinoTex = dinoTex;
            selectedDinoFrameCount = 6;
        }

        this->windowWidth = windowWidth;
        this->windowHeight = windowHeight;

        float playerWidth = (static_cast<float>(selectedDinoTex.width) / selectedDinoFrameCount) * Config::PLAYER_SCALE;
        float startX = (windowWidth - playerWidth) / 2.f;
        player.init(selectedDinoTex, startX, static_cast<float>(windowHeight), Config::PLAYER_SCALE, selectedDinoFrameCount, Config::ANIMATION_UPDATE_TIME);

        dust.init(resources.getDustRun(), 0, 0, Config::OBSTACLE_SCALE, Config::DUST_FRAME_COUNT, Config::ANIMATION_UPDATE_TIME);

        lastObstacleX = static_cast<float>(windowWidth);
        obstacles.clear();
        spawnObstacle(windowWidth, windowHeight);
    }

    /**
     * @brief Aktualizuje stan gry.
     * @param deltaTime Czas od ostatniej klatki (w sekundach).
     * @param windowHeight Aktualna wysoko�� okna gry (w pikselach).
     */
    void update(float deltaTime, int windowHeight) {
        this->windowHeight = windowHeight;
        player.update(deltaTime, windowHeight);
        updateObstacles<Obstacle>(deltaTime); 

        obstacles.erase(
            std::remove_if(obstacles.begin(), obstacles.end(),
                [](const auto& obs) { return obs->getPositionX() < -200.f; }),
            obstacles.end()
        );

        if (obstacles.empty() || obstacles.back()->getPositionX() < lastObstacleX - getRandomDistance()) {
            spawnObstacle(windowWidth, windowHeight);
        }

        dust.update(deltaTime);

        if (player.isOnGround(windowHeight)) {
            dustSpawnTimer += deltaTime;
            if (dustSpawnTimer >= dustSpawnInterval) {
                spawnDust();
                dustSpawnTimer = 0.f;
            }
        }
    }

    /**
     * @brief Rysuje wszystkie elementy gry.
     */
    void draw() const {
        drawObstacles<Obstacle>(); // Jawne podanie typu
        dust.draw();
        player.draw();
    }

    /**
     * @brief Sprawdza, czy gracz przegra� (kolizja z przeszkod�).
     * @return True, je�li wykryto kolizj� z przeszkod�.
     */
    bool checkLoss() const {
        for (const auto& obstacle : obstacles) {
            if (obstacleCollision(*obstacle, player)) {
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Zwraca referencj� do obiektu gracza.
     * @return Sta�a referencja do gracza.
     */
    const Player& getPlayer() const { return player; }

private:

    /**
     * @brief Spawnuje efekt py�u, gdy gracz jest na ziemi.
     */
    void spawnDust() {
        if (!dust.getIsActive()) {
            Vector2 playerPos = player.getPosition();
            float dustX = playerPos.x - 20;
            float dustY = (windowHeight - 65.f) - (dust.getTexture().height * Config::OBSTACLE_SCALE);
            dust.init(dust.getTexture(), dustX, dustY, Config::OBSTACLE_SCALE, Config::DUST_FRAME_COUNT, Config::ANIMATION_UPDATE_TIME);
        }
    }

    /**
     * @brief Spawnuje now� przeszkod�.
     * @param windowWidth Szeroko�� okna.
     * @param windowHeight Wysoko�� okna.
     */
    void spawnObstacle(int windowWidth, int windowHeight) {
        float startX = lastObstacleX + getRandomDistance();
        float startY = static_cast<float>(windowHeight - 65);
        obstacles.push_back(obstacleFactory.createObstacle(startX, startY, currentBgType));
        lastObstacleX = startX;
    }

    /**
    * @brief Generuje losow� odleg�o�� mi�dzy przeszkodami.
    * @return Losowa odleg�o�� w pikselach.
    */
    float getRandomDistance() {
        std::uniform_real_distribution<float> dis(minObstacleDistance, maxObstacleDistance);
        return dis(gen);
    }
};