#ifndef WINDOW_h
    #define WINDOW_h
    #include "player.hpp"
    #include <SFML/Graphics.hpp>
    #include <SFML/System.hpp>
    #include <string>
    #include <cstring>
    #include <vector>
    #define ZOOM 14
    #define PM 3
    #define SIZE 50

class Player;

class Window : public sf::RenderWindow
{
private:
    bool m_is_ia_thinking;
    char **m_grid;
    int turn;
    int victory;
    float decal;
    float spacing;
    int mode;
    sf::Text m_turn, m_player_1, ai_thinking, y_lose, y_win;
    sf::Font font;
    sf::Text m_new_round;
    sf::RectangleShape dark;
    sf::RectangleShape light;
    sf::RectangleShape player;
    sf::RectangleShape black_rec;
    sf::Sprite player_sprite;
    sf::RectangleShape rectangle_h;
    sf::RectangleShape rectangle__;
    sf::RectangleShape rectangle_v;

public:

    int grid;
    Window(sf::VideoMode mode, std::string name, sf::Uint8 style);
    ~Window();


    char **get_grid(void) {return (char **)m_grid;}
    bool get_is_ia_thinking(void) {return m_is_ia_thinking;}

    void draw_grid();
    void Update_view(Player &player);
    void draw_pebble(float decal, float spacing);

    char *operator[](int const& i) {return m_grid[i];}
    void set_is_ia_thinking(bool new_val) {m_is_ia_thinking = new_val;}

    void draw_texts();
    void draw_ai_thinking();
    void change_victory(int nb);
    int get_victory();
    void set_turn(int nb) {turn = nb;}
    int get_turn(void) {return turn;}
    void set_mode(int nb) {mode = nb;}
    int get_mode(void) {return mode;}

    int get_decal(void) {return decal;}
    int get_spacing(void) {return spacing;}

    void clear_map(void);
};

void draw_grid(Window &window);
#endif
