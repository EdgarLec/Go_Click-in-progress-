#ifndef PLAYER_H
    #define PLAYER_H
    #include "Window.hpp"
    #include <utility>
    #include "Include.hpp"
    #include <SFML/Graphics.hpp>
    #include <SFML/System.hpp>

class Window;

typedef std::pair<int, int> coo_t;
void place_map(int x, int y, Window &window, int nb, int lim);

class Player
{
private:
    int mid[4];
    coo_t pos;
    sf::Sprite player_sprite;
    void next(int nb, int &tmp, coo_t &tmp_pos, char c, sf::IntRect &rect, Window &win);
    int turn;
    int move;
    int val_pos;
    int pm;

public:

    Player();
    void draw_player(Window &win);
    void goto_player(Window &win, Player &player);
    int go_next(Window &win);

    coo_t get_pos() {return pos;}
    void set_pos(coo_t coo) {pos = coo;}

    int get_turn() {return turn;}
    void set_turn(int nb) {turn = nb;}

    int get_move() {return move;}
    void set_move(int nb) {move = nb;}

    int get_val_pos() {return val_pos;}
    void set_val_pos(int nb) {val_pos = nb;}

    int get_pm() {return pm;}
    void set_pm(int nb) {pm = nb;}

    void set_color(sf::Color const& color) {player_sprite.setColor(color);}

};

#endif
