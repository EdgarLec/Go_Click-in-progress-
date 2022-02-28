#include "Nme.hpp"
#define max(a,b) (a < b) ? b : a
#define min(a,b) (a > b) ? b : a

void Nme::create_nme(Window &win)
{
    int nb = rand() % 4 + 1;
    float decal = win.get_decal();
    float spacing = win.get_spacing();
    sf::Vector2u size = win.getSize();
    sf::Vector2f tmp = win.getView().getCenter();
    tmp.x -= size.x / 2;
    tmp.y -= size.y / 2;
    int xf = max(int((tmp.x - decal) / spacing) + 1, 0);
    int yf = max(int((tmp.y) / spacing) + 1, 0);
    int xs = min(int((tmp.x + size.x - decal) / spacing) - 2, SIZE - 1);
    int ys = min(int((tmp.y + size.y) / spacing) - 2, SIZE - 1);
    coo_t pos;
    char **m_grid = win.get_grid();

    for (int i = 0; i < nb; i++) {
        pos = coo_t(xf + rand() % (xs - xf), yf + rand() % (ys - yf));
        for (int j = 0; m_grid[pos.first][pos.second] != 0; j++)
            pos = coo_t(xf + rand() % (xs - xf), yf + rand() % (ys - yf));
        _all_nme.push_back(Player());
        _all_nme.back().set_color(sf::Color(255, 80, 50, 255));
        _all_nme.back().set_pos(pos);
        win[_all_nme.back().get_pos().first][_all_nme.back().get_pos().second] = -2;
    }
}

void Nme::draw_nme(Window &win)
{
    for (Player i : _all_nme)
        i.draw_player(win);
}


void Nme::destroy_nme(Window &win)
{
    for (Player i : _all_nme)
        win[i.get_pos().first][i.get_pos().second] = 0;
    _all_nme.clear();
}

void Nme::move_to(Window &win, Player &player)
{
    coo_t player_pos = player.get_pos();
    if (_all_nme.size() > (unsigned long int)(turn - 1) && _all_nme[turn - 1].get_pm() > 0) {
        if (_all_nme.size() > (unsigned long int)(turn)) {
            if (_all_nme[turn - 1].go_next(win) == 1) {
                turn++;
                win.clear_map();
                win[_all_nme[turn - 1].get_pos().first][_all_nme[turn - 1].get_pos().second] = 0;
                place_map(player_pos.first, player_pos.second, win, 50, 0);
                change_base(win, _all_nme[turn - 1]);
                win[player_pos.first][player_pos.second] = 0;
            }
        } else
            if (_all_nme[turn - 1].go_next(win) == 1) {
                for (Player &i : _all_nme)
                    i.set_pm(3);
                turn = 0;
                win.clear_map();
                player.set_turn(1);
            }
        return;
    }
    if (_all_nme.size() > (unsigned long int)(turn - 1)) {
        turn++;
        win.clear_map();
        if (_all_nme.size() > (unsigned long int)(turn - 1)) {
            win[_all_nme[turn - 1].get_pos().first][_all_nme[turn - 1].get_pos().second] = 0;
            place_map(player_pos.first, player_pos.second, win, 50, 0);
            change_base(win, _all_nme[turn - 1]);
            win[player_pos.first][player_pos.second] = 0;
        }
        return;
    }
    for (Player &i : _all_nme)
        i.set_pm(3);
    turn = 0;
    win.clear_map();
    player.set_turn(1);
}

Nme::Nme()
{
    turn = 0;
}

void Nme::clear_nme(Window &win)
{
    for (Player &i : _all_nme)
        win[i.get_pos().first][i.get_pos().second] = 0;
}

void Nme::val_pos_nme(Window &win)
{
    for (Player &i : _all_nme) {
        i.set_val_pos(win[i.get_pos().first][i.get_pos().second]);
        win[i.get_pos().first][i.get_pos().second] = -1;
    }
}
