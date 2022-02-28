#include "Include.hpp"
#include "player.hpp"
#include "Window.hpp"
#include "Nme.hpp"
#include <vector>
#define ZOOM 14
#define SIZE 50
#define WINDOW_SIZE sf::Vector2f(1920, 1080)
#define SIZE 50

void place_map(int x, int y, Window &window, int nb, int lim)
{
    window[x][y] = nb;
    if (nb == lim)
        return;
    if (x > 0 && window[x - 1][y] >= 0 && window[x - 1][y] < nb -1)
        place_map(x - 1, y, window, nb - 1, lim);
    if (y > 0 && window[x][y - 1] >= 0 && window[x][y - 1] < nb -1)
        place_map(x, y - 1, window, nb - 1, lim);
    if (x < SIZE - 1 && window[x + 1][y] >= 0 && window[x + 1][y] < nb -1)
        place_map(x + 1, y, window, nb - 1, lim);
    if (y < SIZE - 1 && window[x][y + 1] >= 0 && window[x][y + 1] < nb -1)
        place_map(x, y + 1, window, nb - 1, lim);
}

void change_base(Window &win, Player &player)
{
    coo_t player_pos = player.get_pos();
    player.set_val_pos(win[player_pos.first][player_pos.second]);
    win[player_pos.first][player_pos.second] = -1;
}

void manage_mouse_release(sf::Event &ev, Window &win, Player &player)
{
    sf::Vector2u size = win.getSize();
    sf::Vector2f tmp = win.getView().getCenter();
    coo_t player_pos = player.get_pos();
    tmp.x -= size.x / 2;
    tmp.y -= size.y / 2;
    float decal = (size.x - size.y) / 2;
    float spacing = size.y / ZOOM;
    int x = int((tmp.x + ev.mouseButton.x - decal) / spacing);
    int y = int((tmp.y + ev.mouseButton.y) / spacing);
    if (x < 0 || y < 0 || x > SIZE - 1 || y > SIZE - 1 || win[x][y] < 0)
        return;
    if (win.get_mode() == 0) {
        player.set_val_pos(0);
        win[player_pos.first][player_pos.second] = 0;
        win.clear_map();
        place_map(x, y, win, 50, 0);
        change_base(win, player);
    } else if (player.get_turn() == 1) {
        win.clear_map();
        win[player_pos.first][player_pos.second] = 0;
        place_map(x, y, win, player.get_pm(), 0);
        change_base(win, player);
    }
    player.set_move(1);
}

void poll_window_events(Window &win, Nme &nme, Player &player)
{
    sf::Event ev;

    while (win.pollEvent(ev)) {
        if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape)
            win.close();
        if (ev.type == sf::Event::MouseButtonReleased || ev.type == sf::Event::MouseButtonPressed)
            manage_mouse_release(ev, win, player);
        if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::G)
            win.grid = !win.grid;
        if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Enter && player.get_turn() == 1) {
            coo_t player_pos = player.get_pos();
            player.set_turn(0);
            nme.set_turn(1);
            win.clear_map();
            win[player_pos.first][player_pos.second] = 0;
            nme.clear_nme(win);
            place_map(player_pos.first, player_pos.second, win, 50, 0);
            nme.val_pos_nme(win);
            change_base(win, player);
            player.set_pm(3);
        }
        if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::M) {
            win.clear_map();
            player.set_pm(3);
            win.set_mode(!win.get_mode());
            player.set_turn(1);
            if (win.get_mode() == 1) {
                nme.create_nme(win);
                place_map(player.get_pos().first, player.get_pos().second, win, 100, 100 - player.get_pm());
                nme.val_pos_nme(win);
                player.set_val_pos(101);
            } else
                nme.destroy_nme(win);
        }
    }
}

void draw_window(Window &win, Player player, Nme nme)
{
    win.clear(sf::Color(20,20,20, 255));

    // win.draw_texts();

    win.draw_grid();
    player.draw_player(win);
    win.Update_view(player);
    if (win.get_mode() == 1)
        nme.draw_nme(win);
    win.display();
}

void game_loop(Window &win, Player &player, Nme &nme)
{
    if (win.get_mode() == 0 || player.get_turn() == 1)
        if (player.go_next(win) == 1 && win.get_mode() == 1 && player
        .get_turn() == 1 && player.get_pm() != 0) {
            place_map(player.get_pos().first, player.get_pos().second, win, 100, 100 - player.get_pm());
            player.set_val_pos(101);
        }
    if (nme.get_turn() != 0)
        nme.move_to(win, player);
}


int main(void)
{
    Window window(sf::VideoMode::getDesktopMode(), "Gomoku", sf::Style::Fullscreen);
    window.setFramerateLimit(60);
    Player player;
    Nme nme;
    sf::Clock clock;
    sf::Clock clock2;
    for (int i = 0; window.isOpen(); i++) {
        if (clock2.getElapsedTime() >= sf::seconds(0.05)) {
            game_loop(window, player, nme);
            clock2.restart();
        }
        if (clock.getElapsedTime() >= sf::seconds(0.01)) {
            draw_window(window, player, nme);
            poll_window_events(window, nme, player);
            clock.restart();
        }
    }
}
