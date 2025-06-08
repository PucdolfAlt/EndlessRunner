/**
 * @file CollisionHandling.ixx
 * @brief Modu� obs�uguj�cy wykrywanie kolizji w grze.
 *
 * Zawiera generyczn� funkcj� do sprawdzania kolizji mi�dzy prostok�tami z opcjonalnym paddingiem oraz koncept dla obiekt�w kolizyjnych.
 */

module;
#include "raylib.h"
#include <concepts>
export module CollisionHandlingModule;

/**
 * @concept Collidable
 * @brief Koncept dla obiekt�w, kt�re dostarczaj� prostok�t kolizji.
 *
 * Obiekty spe�niaj�ce ten koncept musz� posiada� metod� `getCollisionRec`, kt�ra zwraca prostok�t (`Rectangle`) reprezentuj�cy obszar kolizji.
 * @tparam T Typ obiektu do sprawdzenia.
 */
export template<typename T>
concept Collidable = requires(T t) {
    { t.getCollisionRec() } -> std::convertible_to<Rectangle>;
};

/**
 * @brief Sprawdza kolizj� mi�dzy dwoma obiektami kolizyjnymi z opcjonalnym paddingiem.
 * @tparam T Pierwszy typ obiektu (musi spe�nia� koncept Collidable).
 * @tparam U Drugi typ obiektu (musi spe�nia� koncept Collidable).
 * @param a Pierwszy obiekt kolizyjny (np. przeszkoda).
 * @param b Drugi obiekt kolizyjny (np. gracz).
 * @param padding Odst�p (padding) zmniejszaj�cy obszar kolizji prostok�ta pierwszego obiektu (domy�lnie 55.0f pikseli).
 * @return True, je�li prostok�ty kolizji (z uwzgl�dnieniem paddingu) si� pokrywaj�; false w przeciwnym razie.
 */
export template<Collidable T, Collidable U>
bool obstacleCollision(const T& a, const U& b, float padding = 55.0f) {
    Rectangle rectA = a.getCollisionRec();
    Rectangle rectB = b.getCollisionRec();
    rectA.x += padding;
    rectA.y += padding;
    rectA.width -= 2 * padding;
    rectA.height -= 2 * padding;
    return CheckCollisionRecs(rectA, rectB);
}