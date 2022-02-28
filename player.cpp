#include "Include.hpp"
#include "player.hpp"
#include <iostream>

Player::Player():
    pos(0, 0),
    turn(0),
    move(0),
    val_pos(0)
{
    memset(mid, 0, sizeof(int) * 4);
    pos = {0, 0};
    sf::Texture *img = new sf::Texture;
    img->loadFromFile("/home/Edo/B-CLICK_TO_GO/img/perso.png");
    player_sprite.setTexture(*img);
    player_sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
    player_sprite.setScale(sf::Vector2f(float(1080 / ZOOM) / 32, float(1080 / ZOOM) / 32));
    pm = 3;

}


void Player::next(int nb, int &tmp, coo_t &tmp_pos, char c, sf::IntRect &rect, Window &win)
{
    mid[nb] = 0;
    tmp = c;
    tmp_pos = pos;
    rect.left = 0;
    if (win.get_mode() == 1)
        pm--;
}

void clear_mid(int mid[4], int is_do[4], int nb)
{
    for (int i = 0; i < nb; i++)
        mid[i] = 0;
    is_do[nb]++;
}

int Player::go_next(Window &win)
{
    char **m_grid = win.get_grid();
    m_grid[pos.first][pos.second] = -1;
    int tmp = val_pos;// m_grid[pos.first][pos.second];
    sf::IntRect rect = player_sprite.getTextureRect();
    coo_t tmp_pos = pos;
    int is_do[4] = {0, 0, 0, 0};
    if (pos.first < SIZE - 1 && (m_grid[pos.first + 1][pos.second] > tmp)) {
        clear_mid(mid, is_do, 0);
        if (mid[0] == 2) {
            next(0, tmp, tmp_pos, m_grid[pos.first + 1][pos.second], rect, win);
            tmp_pos.first++;
        } else {
            mid[0]++;
            rect.left = 32 * mid[0];
        }
        rect.top = 96;
    }
    if (pos.first > 0 && (m_grid[pos.first - 1][pos.second] > tmp)) {
        clear_mid(mid, is_do, 1);
        if (mid[1] == 2) {
            next(1, tmp, tmp_pos, m_grid[pos.first - 1][pos.second], rect, win);
            tmp_pos.first--;
        } else {
            mid[1]++;
            rect.left = 32 * mid[1];
        }
        rect.top = 32;
    }
    if (pos.second < SIZE - 1 && (m_grid[pos.first][pos.second + 1] > tmp)) {
        clear_mid(mid, is_do, 2);
        if (mid[2] == 2) {
            next(2, tmp, tmp_pos, m_grid[pos.first][pos.second + 1], rect, win);
            tmp_pos.second++;
        } else {
            mid[2]++;
            rect.left = 32 * mid[2];
        }
        rect.top = 0;
    }
    if (pos.second > 0 && (m_grid[pos.first][pos.second - 1] > tmp)) {
        clear_mid(mid, is_do, 3);
        if (mid[3] == 2) {
            next(3, tmp, tmp_pos, m_grid[pos.first][pos.second - 1], rect, win);
            tmp_pos.second--;
        } else {
            mid[3]++;
            rect.left = 32 * mid[3];
        }
        rect.top = 64;
    }
    int test = 0;
    for (int i = 0; i < 4; i++) {
        if (is_do[i] == 0)
            mid[i] = 0;
        else
            test++;
    }
    if (test == 0) {
        move = 0;
        return 1;
    }
    player_sprite.setTextureRect(rect);
    if (pos != tmp_pos) {
        m_grid[pos.first][pos.second] = 0;
        pos = tmp_pos;
        val_pos = m_grid[pos.first][pos.second];
        m_grid[pos.first][pos.second] = -1;
    }
    return 0;
}

void Player::draw_player(Window &win)
{
    float decal = win.get_decal();
    float spacing = win.get_spacing();
    player_sprite.setPosition(sf::Vector2f(decal + spacing * (pos.first), spacing * (pos.second)));
    if (mid[3])
        player_sprite.setPosition(sf::Vector2f(decal + spacing * (pos.first), spacing * (pos.second - mid[3] / float(3))));
    if (mid[2])
        player_sprite.setPosition(sf::Vector2f(decal + spacing * (pos.first), spacing * (pos.second + mid[2] / float(3))));
    if (mid[1])
        player_sprite.setPosition(sf::Vector2f(decal + spacing * (pos.first - mid[1] / float(3)), spacing * (pos.second)));
    if (mid[0])
        player_sprite.setPosition(sf::Vector2f(decal + spacing * (pos.first + mid[0] / float(3)), spacing * (pos.second)));
    win.draw(player_sprite);
}

