#ifndef NME_H
    #define NME_H
    #include "player.hpp"
    #include <SFML/Graphics.hpp>
    #include <SFML/System.hpp>
    #include <string>
    #include <cstring>
    #include <vector>
    #include <utility>
    #include <iostream>


class Player;

void change_base(Window &win, Player &player);

class Nme
{
private:
    std::vector<Player> _all_nme;
    int turn;

public:
    Nme();
    void draw_nme(Window &win);
    void create_nme(Window &win);
    void destroy_nme(Window &win);
    void clear_nme(Window &win);
    void val_pos_nme(Window &win);

    int get_turn() {return turn;}
    void set_turn(int nb) {turn = nb;}
    void move_to(Window &win, Player &player);
};

#endif
