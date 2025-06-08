/**
 * @file Resources.ixx
 * @brief Modu� definiuj�cy klas� Resources, zarz�dzaj�c� teksturami gry.
 *
 * Klasa Resources odpowiada za �adowanie, przechowywanie i udost�pnianie tekstur u�ywanych w grze.
 */

module;
#include "raylib.h"
#include <iostream>

export module ResourcesModule;
import <array>;
import <unordered_map>;
import <string>;
import <filesystem>;
import <ranges>;
import <concepts>;


/**
 * @concept TextureType
 * @brief Koncept dla typ�w tekstur zgodnych z Texture2D.
 *
 * Typy spe�niaj�ce ten koncept musz� by� typu Texture2D oraz posiada� pola `id`, `width` i `height` konwertowalne odpowiednio na `unsigned int` i `int`.
 * @tparam T Typ obiektu do sprawdzenia.
 */
export template<typename T>
concept TextureType = std::same_as<T, Texture2D>&& requires(T t) {
    { t.id } -> std::convertible_to<unsigned int>;
    { t.width } -> std::convertible_to<int>;
    { t.height } -> std::convertible_to<int>;
};

/**
 * @class Resources
 * @brief Klasa zarz�dzaj�ca teksturami gry.
 *
 * Odpowiada za �adowanie, przechowywanie i udost�pnianie tekstur dla element�w gry, takich jak t�a, sprite'y i interfejs u�ytkownika.
 */
export class Resources {
private:
    /** @brief Mapa przechowuj�ca tekstury z kluczami w formacie std::string. */
    std::unordered_map<std::string, Texture2D> textures;

    /**
     * @struct TextureKeys
     * @brief Struktura grupuj�ca klucze tekstur wed�ug kategorii.
     */
    struct TextureKeys {
        /**
          * @struct UI
          * @brief Klucze dla tekstur interfejsu u�ytkownika.
          */
        struct UI {
            inline static const std::string MENU_BG = "menu_background"; /**< T�o menu g��wnego. */
            inline static const std::string LOADING_SCREEN_BG = "loading_screen_background"; /**< T�o ekranu �adowania. */
            inline static const std::string BAR_EMPTY = "loading_bar_empty"; /**< Pusty pasek �adowania. */
            inline static const std::string BAR_FULL = "loading_bar_full"; /**< Wype�niony pasek �adowania. */
            inline static const std::string REGISTRATION_BG = "registration"; /**< T�o ekranu rejestracji. */
            inline static const std::string SHOP_BG = "shop_background"; /**< T�o sklepu. */
            inline static const std::string LEADERBOARD = "leaderboard"; /**< T�o tablicy wynik�w. */
            inline static const std::string HEART_ICON = "heart_icon"; /**< Ikona �ycia. */
            inline static const std::string LIFE_LOST_ICON = "life_lost_icon"; /**< Ikona utraconego �ycia. */
            inline static const std::string NUMBERS = "numbers"; /**< Tekstura cyfr. */
            inline static const std::string BUTTON = "button"; /**< Tekstura przycisku. */
            inline static const std::string BANNER = "banner"; /**< Tekstura baneru. */
        };

        /**
          * @struct Characters
          * @brief Klucze dla tekstur postaci i animacji.
          */
        struct Characters {
            inline static const std::string DINO_SHADOW = "dino_shadow"; /**< Cie� dinozaura. */
            inline static const std::string DUST_RUN = "dust_run"; /**< Efekt py�u podczas biegu. */
            inline static const std::string GREEN_DINO_IDLE = "green_dino_idle"; /**< Zielony dinozaur w stanie bezczynno�ci. */
            inline static const std::string BLUE_DINO_IDLE = "blue_dino_idle"; /**< Niebieski dinozaur w stanie bezczynno�ci. */
            inline static const std::string YELLOW_DINO_IDLE = "yellow_dino_idle"; /**< ��ty dinozaur w stanie bezczynno�ci. */
            inline static const std::string RED_DINO_IDLE = "red_dino_idle"; /**< Czerwony dinozaur w stanie bezczynno�ci. */
            inline static const std::string GREEN_DINO_RUN = "green_dino_run"; /**< Zielony dinozaur podczas biegu. */
            inline static const std::string BLUE_DINO_RUN = "blue_dino_run"; /**< Niebieski dinozaur podczas biegu. */
            inline static const std::string YELLOW_DINO_RUN = "yellow_dino_run"; /**< ��ty dinozaur podczas biegu. */
            inline static const std::string RED_DINO_RUN = "red_dino_run"; /**< Czerwony dinozaur podczas biegu. */
            inline static const std::string NEBULA = "nebula"; /**< Tekstura mg�awicy. */
            inline static const std::string PTERO = "pterosaur"; /**< Tekstura pterodaktyla. */
            inline static const std::string BAT = "bat"; /**< Tekstura nietoperza. */
        };

        /**
          * @struct Backgrounds
          * @brief Klucze dla tekstur t�a gry.
          */
        struct Backgrounds {
            inline static const std::string GAME_BG = "game_background"; /**< Og�lne t�o gry. */
            inline static const std::string GAME_MG = "game_midground"; /**< Og�lny plan �rodkowy gry. */
            inline static const std::string GAME_FG = "game_foreground"; /**< Og�lny plan przedni gry. */
            inline static const std::string DESERT_BG = "desert_background"; /**< T�o pustyni za dnia. */
            inline static const std::string DESERT_MG = "desert_midground"; /**< Plan �rodkowy pustyni za dnia. */
            inline static const std::string DESERT_FG = "desert_foreground"; /**< Plan przedni pustyni za dnia. */
            inline static const std::string DESERT_GR = "desert_ground"; /**< Pod�o�e pustyni za dnia. */
            inline static const std::string DESERT_NIGHT_BG = "desert_night_background"; /**< T�o pustyni w nocy. */
            inline static const std::string DESERT_NIGHT_MG = "desert_night_midground"; /**< Plan �rodkowy pustyni w nocy. */
            inline static const std::string DESERT_NIGHT_FG = "desert_night_foreground"; /**< Plan przedni pustyni w nocy. */
            inline static const std::string DESERT_NIGHT_GR = "desert_night_ground"; /**< Pod�o�e pustyni w nocy. */
            inline static const std::string FOREST_BG = "forest_background"; /**< T�o lasu za dnia. */
            inline static const std::string FOREST_MG = "forest_midground"; /**< Plan �rodkowy lasu za dnia. */
            inline static const std::string FOREST_FG = "forest_foreground"; /**< Plan przedni lasu za dnia. */
            inline static const std::string FOREST_GR = "forest_ground"; /**< Pod�o�e lasu za dnia. */
            inline static const std::string FOREST_NIGHT_BG = "forest_night_background"; /**< T�o lasu w nocy. */
            inline static const std::string FOREST_NIGHT_MG = "forest_night_midground"; /**< Plan �rodkowy lasu w nocy. */
            inline static const std::string FOREST_NIGHT_FG = "forest_night_foreground"; /**< Plan przedni lasu w nocy. */
            inline static const std::string FOREST_NIGHT_GR = "forest_night_ground"; /**< Pod�o�e lasu w nocy. */
        };

        /**
          * @struct Obstacles
          * @brief Klucze dla tekstur przeszk�d statycznych.
          */
        struct Obstacles {
            inline static const std::string SMALL_CACTUS = "small_cactus"; /**< Ma�y kaktus. */
            inline static const std::string BIG_CACTUS = "big_cactus"; /**< Du�y kaktus. */
            inline static const std::string SKULLS = "skulls"; /**< Czaszki (pierwszy wariant). */
            inline static const std::string SKULLS2 = "skulls2"; /**< Czaszki (drugi wariant). */
            inline static const std::string ROCK_D = "rock_d"; /**< Kamie� pustynny za dnia. */
            inline static const std::string BIG_ROCK_D = "big_rock_d"; /**< Du�y kamie� pustynny za dnia. */
            inline static const std::string ROCK_D_N = "rock_d_n"; /**< Kamie� pustynny w nocy. */
            inline static const std::string BIG_ROCK_D_N = "big_rock_d_n"; /**< Du�y kamie� pustynny w nocy. */
            inline static const std::string ROCK_F = "rock"; /**< Kamie� le�ny. */
            inline static const std::string BIG_ROCK_F = "big_rock"; /**< Du�y kamie� le�ny. */
            inline static const std::string SMALL_TREE = "small_tree"; /**< Ma�e drzewo le�ne. */
            inline static const std::string BIG_TREE = "big_tree"; /**< Du�e drzewo le�ne. */
            inline static const std::string LOG = "log"; /**< K�oda le�na. */
            inline static const std::string SPIKES = "spikes"; /**< Kolce le�ne. */
            inline static const std::string SMALL_TREE_N = "small_tree_n"; /**< Ma�e drzewo le�ne w nocy. */
            inline static const std::string BIG_TREE_N = "big_tree_n"; /**< Du�e drzewo le�ne w nocy. */
            inline static const std::string LOG_N = "log_n"; /**< K�oda le�na w nocy. */
            inline static const std::string SPIKES_N = "spikes_n"; /**< Kolce le�ne w nocy. */
        };
    };

public:
    /**
     * @brief Konstruktor domy�lny klasy Resources.
     */
    Resources() = default;

    /**
    * @brief �aduje tekstur� i zapisuje j� pod okre�lonym kluczem.
    * @tparam T Typ tekstury (musi spe�nia� koncept TextureType).
    * @param key Klucz, pod kt�rym tekstura b�dzie przechowywana.
    * @param path �cie�ka do pliku tekstury.
    */
    template<TextureType T>
    void loadTexture(const std::string& key, const std::string& path) {
        if (std::filesystem::exists(path)) {
            textures[key] = LoadTexture(path.c_str());
            std::cout << "Loaded texture: " << key << " from " << path
                << ", ID: " << textures[key].id
                << ", Width: " << textures[key].width
                << ", Height: " << textures[key].height << "\n";
            if (textures[key].id == 0 || textures[key].width == 0 || textures[key].height == 0) {
                std::cout << "Warning: Texture " << key << " has invalid dimensions, loading fallback!\n";
                textures[key] = LoadTexture("textures/missing_texture.png");
            }
        }
        else {
            std::cout << "Texture not found: " << path << ", using missing_texture.png\n";
            textures[key] = LoadTexture("textures/missing_texture.png");
            if (textures[key].id == 0 || textures[key].width == 0 || textures[key].height == 0) {
                std::cout << "Error: Fallback missing_texture.png failed!\n";
            }
        }
    }

    /**
     * @brief �aduje wszystkie tekstury gry z predefiniowanych �cie�ek.
     */
    void loadTextures() {
        loadTexture<Texture2D>(TextureKeys::UI::MENU_BG, "textures/main_menu_background.png");
        loadTexture<Texture2D>(TextureKeys::UI::LOADING_SCREEN_BG, "textures/loading_screen.png");
        loadTexture<Texture2D>(TextureKeys::UI::BAR_EMPTY, "textures/barempty.png");
        loadTexture<Texture2D>(TextureKeys::UI::BAR_FULL, "textures/barfill.png");
        loadTexture<Texture2D>(TextureKeys::UI::REGISTRATION_BG, "textures/registration_with_table.png");
        loadTexture<Texture2D>(TextureKeys::UI::SHOP_BG, "textures/shop.png");
        loadTexture<Texture2D>(TextureKeys::UI::LEADERBOARD, "textures/leaderboard.png");
        loadTexture<Texture2D>(TextureKeys::UI::HEART_ICON, "textures/life_icon.png");
        loadTexture<Texture2D>(TextureKeys::UI::LIFE_LOST_ICON, "textures/life_lost_icon.png");
        loadTexture<Texture2D>(TextureKeys::UI::NUMBERS, "textures/numbers.png");
        loadTexture<Texture2D>(TextureKeys::UI::BUTTON, "textures/button.png");
        loadTexture<Texture2D>(TextureKeys::UI::BANNER, "textures/banner4.png");
        loadTexture<Texture2D>(TextureKeys::Characters::DINO_SHADOW, "textures/Characters/shadow_2.png");
        loadTexture<Texture2D>(TextureKeys::Characters::DUST_RUN, "textures/Characters/dust.png");
        loadTexture<Texture2D>(TextureKeys::Characters::GREEN_DINO_IDLE, "textures/Characters/Player 1/p1_idle.png");
        loadTexture<Texture2D>(TextureKeys::Characters::BLUE_DINO_IDLE, "textures/Characters/Player 2/p2_idle.png");
        loadTexture<Texture2D>(TextureKeys::Characters::YELLOW_DINO_IDLE, "textures/Characters/Player 3/p3_idle.png");
        loadTexture<Texture2D>(TextureKeys::Characters::RED_DINO_IDLE, "textures/Characters/Player 4/p4_idle.png");
        loadTexture<Texture2D>(TextureKeys::Characters::GREEN_DINO_RUN, "textures/Characters/Player 1/p1_run.png");
        loadTexture<Texture2D>(TextureKeys::Characters::BLUE_DINO_RUN, "textures/Characters/Player 2/p2_run.png");
        loadTexture<Texture2D>(TextureKeys::Characters::YELLOW_DINO_RUN, "textures/Characters/Player 3/p3_run.png");
        loadTexture<Texture2D>(TextureKeys::Characters::RED_DINO_RUN, "textures/Characters/Player 4/p4_run.png");
        loadTexture<Texture2D>(TextureKeys::Characters::NEBULA, "textures/12_nebula_spritesheet.png");
        loadTexture<Texture2D>(TextureKeys::Characters::PTERO, "textures/pterosaur_spritesheet.png");
        loadTexture<Texture2D>(TextureKeys::Characters::BAT, "textures/BatIdleMoving.png");
        loadTexture<Texture2D>(TextureKeys::Backgrounds::DESERT_BG, "textures/Desert/bg_1.png");
        loadTexture<Texture2D>(TextureKeys::Backgrounds::DESERT_MG, "textures/Desert/bg_2.png");
        loadTexture<Texture2D>(TextureKeys::Backgrounds::DESERT_FG, "textures/Desert/bg_3.png");
        loadTexture<Texture2D>(TextureKeys::Backgrounds::DESERT_GR, "textures/Desert/ground.png");
        loadTexture<Texture2D>(TextureKeys::Backgrounds::DESERT_NIGHT_BG, "textures/Desert/night_bg_1.png");
        loadTexture<Texture2D>(TextureKeys::Backgrounds::DESERT_NIGHT_MG, "textures/Desert/night_bg_2.png");
        loadTexture<Texture2D>(TextureKeys::Backgrounds::DESERT_NIGHT_FG, "textures/Desert/night_bg_3.png");
        loadTexture<Texture2D>(TextureKeys::Backgrounds::DESERT_NIGHT_GR, "textures/Desert/night_ground.png");
        loadTexture<Texture2D>(TextureKeys::Backgrounds::FOREST_BG, "textures/Forest/bg_1.png");
        loadTexture<Texture2D>(TextureKeys::Backgrounds::FOREST_MG, "textures/Forest/bg_2.png");
        loadTexture<Texture2D>(TextureKeys::Backgrounds::FOREST_FG, "textures/Forest/bg_3.png");
        loadTexture<Texture2D>(TextureKeys::Backgrounds::FOREST_GR, "textures/Forest/ground.png");
        loadTexture<Texture2D>(TextureKeys::Backgrounds::FOREST_NIGHT_BG, "textures/Forest/night_bg_1.png");
        loadTexture<Texture2D>(TextureKeys::Backgrounds::FOREST_NIGHT_MG, "textures/Forest/night_bg_2.png");
        loadTexture<Texture2D>(TextureKeys::Backgrounds::FOREST_NIGHT_FG, "textures/Forest/night_bg_3.png");
        loadTexture<Texture2D>(TextureKeys::Backgrounds::FOREST_NIGHT_GR, "textures/Forest/night_ground.png");
        loadTexture<Texture2D>(TextureKeys::Obstacles::SMALL_CACTUS, "textures/Desert/small_cactus.png");
        loadTexture<Texture2D>(TextureKeys::Obstacles::BIG_CACTUS, "textures/Desert/big_cactus.png");
        loadTexture<Texture2D>(TextureKeys::Obstacles::SKULLS, "textures/Desert/skulls.png");
        loadTexture<Texture2D>(TextureKeys::Obstacles::SKULLS2, "textures/Desert/skulls2.png");
        loadTexture<Texture2D>(TextureKeys::Obstacles::ROCK_D, "textures/Desert/rock.png");
        loadTexture<Texture2D>(TextureKeys::Obstacles::BIG_ROCK_D, "textures/Desert/big_rock.png");
        loadTexture<Texture2D>(TextureKeys::Obstacles::ROCK_D_N, "textures/Desert/rock_n.png");
        loadTexture<Texture2D>(TextureKeys::Obstacles::BIG_ROCK_D_N, "textures/Desert/big_rock_n.png");
        loadTexture<Texture2D>(TextureKeys::Obstacles::ROCK_F, "textures/Forest/rock_f.png");
        loadTexture<Texture2D>(TextureKeys::Obstacles::BIG_ROCK_F, "textures/Forest/big_rock_f.png");
        loadTexture<Texture2D>(TextureKeys::Obstacles::SMALL_TREE, "textures/Forest/small_tree.png");
        loadTexture<Texture2D>(TextureKeys::Obstacles::BIG_TREE, "textures/Forest/big_tree.png");
        loadTexture<Texture2D>(TextureKeys::Obstacles::LOG, "textures/Forest/log.png");
        loadTexture<Texture2D>(TextureKeys::Obstacles::SPIKES, "textures/Forest/spikes.png");
        loadTexture<Texture2D>(TextureKeys::Obstacles::SMALL_TREE_N, "textures/Forest/small_tree_n.png");
        loadTexture<Texture2D>(TextureKeys::Obstacles::BIG_TREE_N, "textures/Forest/big_tree_n.png");
        loadTexture<Texture2D>(TextureKeys::Obstacles::LOG_N, "textures/Forest/log_n.png");
        loadTexture<Texture2D>(TextureKeys::Obstacles::SPIKES_N, "textures/Forest/spikes_n.png");
    }

    /**
     * @brief Zwalnia wszystkie za�adowane tekstury z pami�ci.
     */
    void unloadTextures() {
        for (auto& texture : textures | std::ranges::views::values) {
            UnloadTexture(texture);
        }
    }


    /**
     * @brief Pobiera tekstur� na podstawie klucza.
     * @tparam T Typ tekstury (musi spe�nia� koncept TextureType).
     * @param key Klucz tekstury.
     * @return Referencja do tekstury; je�li klucz nie istnieje, �aduje domy�ln� tekstur� zast�pcz�.
     */
    template<TextureType T>
    T& getTexture(const std::string& key) {
        auto it = textures.find(key);
        if (it != textures.end()) {
            return it->second;
        }
        textures[key] = LoadTexture("textures/missing_texture.png");
        return textures[key];
    }

    /** @brief Pobiera tekstur� t�a menu. @return Referencja do tekstury. */
    Texture2D& getMenuBackground() { return getTexture<Texture2D>(TextureKeys::UI::MENU_BG); }
    /** @brief Pobiera tekstur� t�a sklepu. @return Referencja do tekstury. */
    Texture2D& getShopBackground() { return getTexture<Texture2D>(TextureKeys::UI::SHOP_BG); }
    /** @brief Pobiera tekstur� ekranu �adowania. @return Referencja do tekstury. */
    Texture2D& getLoadingScreenBackground() { return getTexture<Texture2D>(TextureKeys::UI::LOADING_SCREEN_BG); }
    /** @brief Pobiera tekstur� pustego paska �adowania. @return Referencja do tekstury. */
    Texture2D& getBarEmpty() { return getTexture<Texture2D>(TextureKeys::UI::BAR_EMPTY); }
    /** @brief Pobiera tekstur� pe�nego paska �adowania. @return Referencja do tekstury. */
    Texture2D& getBarFill() { return getTexture<Texture2D>(TextureKeys::UI::BAR_FULL); }
    /** @brief Pobiera tekstur� t�a rejestracji. @return Referencja do tekstury. */
    Texture2D& getRegistrationBackground() { return getTexture<Texture2D>(TextureKeys::UI::REGISTRATION_BG); }
    /** @brief Pobiera tekstur� cienia dinozaura. @return Referencja do tekstury. */
    Texture2D& getShadowTexture() { return getTexture<Texture2D>(TextureKeys::Characters::DINO_SHADOW); }
    /** @brief Pobiera tekstur� zielonego dinozaura w stanie bezczynno�ci. @return Referencja do tekstury. */
    Texture2D& getGreenIdle() { return getTexture<Texture2D>(TextureKeys::Characters::GREEN_DINO_IDLE); }
    /** @brief Pobiera tekstur� niebieskiego dinozaura w stanie bezczynno�ci. @return Referencja do tekstury. */
    Texture2D& getBlueIdle() { return getTexture<Texture2D>(TextureKeys::Characters::BLUE_DINO_IDLE); }
    /** @brief Pobiera tekstur� ��tego dinozaura w stanie bezczynno�ci. @return Referencja do tekstury. */
    Texture2D& getYellowIdle() { return getTexture<Texture2D>(TextureKeys::Characters::YELLOW_DINO_IDLE); }
    /** @brief Pobiera tekstur� czerwonego dinozaura w stanie bezczynno�ci. @return Referencja do tekstury. */
    Texture2D& getRedIdle() { return getTexture<Texture2D>(TextureKeys::Characters::RED_DINO_IDLE); }
    /** @brief Pobiera tekstur� og�lnego t�a gry. @return Referencja do tekstury. */
    Texture2D& getGameBackground() { return getTexture<Texture2D>(TextureKeys::Backgrounds::GAME_BG); }
    /** @brief Pobiera tekstur� og�lnego planu �rodkowego gry. @return Referencja do tekstury. */
    Texture2D& getGameMidground() { return getTexture<Texture2D>(TextureKeys::Backgrounds::GAME_MG); }
    /** @brief Pobiera tekstur� og�lnego przedniego planu gry. @return Referencja do tekstury. */
    Texture2D& getGameForeground() { return getTexture<Texture2D>(TextureKeys::Backgrounds::GAME_FG); }
    /** @brief Pobiera tekstur� animacji py�u podczas biegu. @return Referencja do tekstury. */
    Texture2D& getDustRun() { return getTexture<Texture2D>(TextureKeys::Characters::DUST_RUN); }
    /** @brief Pobiera tekstur� zielonego dinozaura w biegu. @return Referencja do tekstury. */
    Texture2D& getGreenDinoRun() { return getTexture<Texture2D>(TextureKeys::Characters::GREEN_DINO_RUN); }
    /** @brief Pobiera tekstur� niebieskiego dinozaura w biegu. @return Referencja do tekstury. */
    Texture2D& getBlueDinoRun() { return getTexture<Texture2D>(TextureKeys::Characters::BLUE_DINO_RUN); }
    /** @brief Pobiera tekstur� ��tego dinozaura w biegu. @return Referencja do tekstury. */
    Texture2D& getYellowDinoRun() { return getTexture<Texture2D>(TextureKeys::Characters::YELLOW_DINO_RUN); }
    /** @brief Pobiera tekstur� czerwonego dinozaura w biegu. @return Referencja do tekstury. */
    Texture2D& getRedDinoRun() { return getTexture<Texture2D>(TextureKeys::Characters::RED_DINO_RUN); }
    /** @brief Pobiera tekstur� mg�awicy. @return Referencja do tekstury. */
    Texture2D& getNebula() { return getTexture<Texture2D>(TextureKeys::Characters::NEBULA); }
    /** @brief Pobiera tekstur� pterozaura. @return Referencja do tekstury. */
    Texture2D& getPtero() { return getTexture<Texture2D>(TextureKeys::Characters::PTERO); }
    /** @brief Pobiera tekstur� nietoperza. @return Referencja do tekstury. */
    Texture2D& getBat() { return getTexture<Texture2D>(TextureKeys::Characters::BAT); }
    /** @brief Pobiera tekstur� ikony �ycia. @return Referencja do tekstury. */
    Texture2D& getHeartIcon() { return getTexture<Texture2D>(TextureKeys::UI::HEART_ICON); }
    /** @brief Pobiera tekstur� ikony utraconego �ycia. @return Referencja do tekstury. */
    Texture2D& getLifeLostIcon() { return getTexture<Texture2D>(TextureKeys::UI::LIFE_LOST_ICON); }
    /** @brief Pobiera tekstur� liczb. @return Referencja do tekstury. */
    Texture2D& getNumbersTexture() { return getTexture<Texture2D>(TextureKeys::UI::NUMBERS); }
    /** @brief Pobiera tekstur� przycisku. @return Referencja do tekstury. */
    Texture2D& getButtonTexture() { return getTexture<Texture2D>(TextureKeys::UI::BUTTON); }
    /** @brief Pobiera tekstur� tablicy wynik�w. @return Referencja do tekstury. */
    Texture2D& getLeaderboard() { return getTexture<Texture2D>(TextureKeys::UI::LEADERBOARD); }
    /** @brief Pobiera tekstur� baneru. @return Referencja do tekstury. */
    Texture2D& getBanner() { return getTexture<Texture2D>(TextureKeys::UI::BANNER); }
    /** @brief Pobiera tekstur� t�a pustyni za dnia. @return Referencja do tekstury. */
    Texture2D& getDesertBackground() { return getTexture<Texture2D>(TextureKeys::Backgrounds::DESERT_BG); }
    /** @brief Pobiera tekstur� �rodkowego planu pustyni za dnia. @return Referencja do tekstury. */
    Texture2D& getDesertMidground() { return getTexture<Texture2D>(TextureKeys::Backgrounds::DESERT_MG); }
    /** @brief Pobiera tekstur� przedniego planu pustyni za dnia. @return Referencja do tekstury. */
    Texture2D& getDesertForeground() { return getTexture<Texture2D>(TextureKeys::Backgrounds::DESERT_FG); }
    /** @brief Pobiera tekstur� ziemi pustyni za dnia. @return Referencja do tekstury. */
    Texture2D& getDesertGround() { return getTexture<Texture2D>(TextureKeys::Backgrounds::DESERT_GR); }
    /** @brief Pobiera tekstur� t�a lasu za dnia. @return Referencja do tekstury. */
    Texture2D& getForestBackground() { return getTexture<Texture2D>(TextureKeys::Backgrounds::FOREST_BG); }
    /** @brief Pobiera tekstur� �rodkowego planu lasu za dnia. @return Referencja do tekstury. */
    Texture2D& getForestMidground() { return getTexture<Texture2D>(TextureKeys::Backgrounds::FOREST_MG); }
    /** @brief Pobiera tekstur� przedniego planu lasu za dnia. @return Referencja do tekstury. */
    Texture2D& getForestForeground() { return getTexture<Texture2D>(TextureKeys::Backgrounds::FOREST_FG); }
    /** @brief Pobiera tekstur� ziemi lasu za dnia. @return Referencja do tekstury. */
    Texture2D& getForestGround() { return getTexture<Texture2D>(TextureKeys::Backgrounds::FOREST_GR); }
    /** @brief Pobiera tekstur� t�a pustyni w nocy. @return Referencja do tekstury. */
    Texture2D& getDesertNightBackground() { return getTexture<Texture2D>(TextureKeys::Backgrounds::DESERT_NIGHT_BG); }
    /** @brief Pobiera tekstur� �rodkowego planu pustyni w nocy. @return Referencja do tekstury. */
    Texture2D& getDesertNightMidground() { return getTexture<Texture2D>(TextureKeys::Backgrounds::DESERT_NIGHT_MG); }
    /** @brief Pobiera tekstur� przedniego planu pustyni w nocy. @return Referencja do tekstury. */
    Texture2D& getDesertNightForeground() { return getTexture<Texture2D>(TextureKeys::Backgrounds::DESERT_NIGHT_FG); }
    /** @brief Pobiera tekstur� ziemi pustyni w nocy. @return Referencja do tekstury. */
    Texture2D& getDesertNightGround() { return getTexture<Texture2D>(TextureKeys::Backgrounds::DESERT_NIGHT_GR); }
    /** @brief Pobiera tekstur� t�a lasu w nocy. @return Referencja do tekstury. */
    Texture2D& getForestNightBackground() { return getTexture<Texture2D>(TextureKeys::Backgrounds::FOREST_NIGHT_BG); }
    /** @brief Pobiera tekstur� �rodkowego planu lasu w nocy. @return Referencja do tekstury. */
    Texture2D& getForestNightMidground() { return getTexture<Texture2D>(TextureKeys::Backgrounds::FOREST_NIGHT_MG); }
    /** @brief Pobiera tekstur� przedniego planu lasu w nocy. @return Referencja do tekstury. */
    Texture2D& getForestNightForeground() { return getTexture<Texture2D>(TextureKeys::Backgrounds::FOREST_NIGHT_FG); }
    /** @brief Pobiera tekstur� ziemi lasu w nocy. @return Referencja do tekstury. */
    Texture2D& getForestNightGround() { return getTexture<Texture2D>(TextureKeys::Backgrounds::FOREST_NIGHT_GR); }
    /** @brief Pobiera tekstur� ma�ego kaktusa. @return Referencja do tekstury. */
    Texture2D& getSmallCactus() { return getTexture<Texture2D>(TextureKeys::Obstacles::SMALL_CACTUS); }
    /** @brief Pobiera tekstur� du�ego kaktusa. @return Referencja do tekstury. */
    Texture2D& getBigCactus() { return getTexture<Texture2D>(TextureKeys::Obstacles::BIG_CACTUS); }
    /** @brief Pobiera tekstur� czaszek (wariant 1). @return Referencja do tekstury. */
    Texture2D& getSkulls() { return getTexture<Texture2D>(TextureKeys::Obstacles::SKULLS); }
    /** @brief Pobiera tekstur� czaszek (wariant 2). @return Referencja do tekstury. */
    Texture2D& getSkulls2() { return getTexture<Texture2D>(TextureKeys::Obstacles::SKULLS2); }
    /** @brief Pobiera tekstur� kamienia (pustynia za dnia). @return Referencja do tekstury. */
    Texture2D& getRockDust() { return getTexture<Texture2D>(TextureKeys::Obstacles::ROCK_D); }
    /** @brief Pobiera tekstur� du�ego kamienia (pustynia za dnia). @return Referencja do tekstury. */
    Texture2D& getBigRockDust() { return getTexture<Texture2D>(TextureKeys::Obstacles::BIG_ROCK_D); }
    /** @brief Pobiera tekstur� kamienia (pustynia w nocy). @return Referencja do tekstury. */
    Texture2D& getRockDustNight() { return getTexture<Texture2D>(TextureKeys::Obstacles::ROCK_D_N); }
    /** @brief Pobiera tekstur� du�ego kamienia (pustynia w nocy). @return Referencja do tekstury. */
    Texture2D& getBigRockDustNight() { return getTexture<Texture2D>(TextureKeys::Obstacles::BIG_ROCK_D_N); }
    /** @brief Pobiera tekstur� kamienia (las). @return Referencja do tekstury. */
    Texture2D& getRockForest() { return getTexture<Texture2D>(TextureKeys::Obstacles::ROCK_F); }
    /** @brief Pobiera tekstur� du�ego kamienia (las). @return Referencja do tekstury. */
    Texture2D& getBigRockForest() { return getTexture<Texture2D>(TextureKeys::Obstacles::BIG_ROCK_F); }
    /** @brief Pobiera tekstur� ma�ego drzewa (las za dnia). @return Referencja do tekstury. */
    Texture2D& getSmallTree() { return getTexture<Texture2D>(TextureKeys::Obstacles::SMALL_TREE); }
    /** @brief Pobiera tekstur� du�ego drzewa (las za dnia). @return Referencja do tekstury. */
    Texture2D& getBigTree() { return getTexture<Texture2D>(TextureKeys::Obstacles::BIG_TREE); }
    /** @brief Pobiera tekstur� k�ody (las za dnia). @return Referencja do tekstury. */
    Texture2D& getLog() { return getTexture<Texture2D>(TextureKeys::Obstacles::LOG); }
    /** @brief Pobiera tekstur� kolc�w (las za dnia). @return Referencja do tekstury. */
    Texture2D& getSpikes() { return getTexture<Texture2D>(TextureKeys::Obstacles::SPIKES); }
    /** @brief Pobiera tekstur� ma�ego drzewa (las w nocy). @return Referencja do tekstury. */
    Texture2D& getSmallTreeNight() { return getTexture<Texture2D>(TextureKeys::Obstacles::SMALL_TREE_N); }
    /** @brief Pobiera tekstur� du�ego drzewa (las w nocy). @return Referencja do tekstury. */
    Texture2D& getBigTreeNight() { return getTexture<Texture2D>(TextureKeys::Obstacles::BIG_TREE_N); }
    /** @brief Pobiera tekstur� k�ody (las w nocy). @return Referencja do tekstury. */
    Texture2D& getLogNight() { return getTexture<Texture2D>(TextureKeys::Obstacles::LOG_N); }
    /** @brief Pobiera tekstur� kolc�w (las w nocy). @return Referencja do tekstury. */
    Texture2D& getSpikesNight() { return getTexture<Texture2D>(TextureKeys::Obstacles::SPIKES_N); }
};