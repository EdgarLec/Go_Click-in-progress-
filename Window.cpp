#include "Include.hpp"
#include "Window.hpp"
#include "player.hpp"
#include <vector>
#define WINDOW_SIZE sf::Vector2f(1920, 1080)

typedef std::pair<int, int> move_t;
typedef std::vector<move_t> move_list;

std::string toString(int integer)
{
    char numstr[10];
    sprintf(numstr, "%i", integer);
    return numstr;
}

void init_text(sf::Text &text, sf::Font &font, Window *w, std::string t, sf::Vector2f pos = sf::Vector2f(0, 0))
{
    text.setFont(font);
    text.setFillColor(sf::Color::Black);
    text.setString(t);
    text.setPosition(pos);
    text.setCharacterSize(w->getSize().y / 15);
    text.setFillColor(sf::Color::Black);
}

void rec_creat(int x, int y, char **m_grid)
{
    m_grid[x][y] = -2;
    int tmp = rand() % 5;

    if (x <= 0 || y <= 0 || x >= SIZE - 1 || y >= SIZE - 1)
        return;
    switch(tmp) {
        case 1:rec_creat(x - 1, y, m_grid);
            break;
        case 2:rec_creat(x + 1, y, m_grid);
            break;
        case 3:rec_creat(y - 1, y, m_grid);
            break;
        case 4:rec_creat(y + 1, y, m_grid);
            break;
    }
}

Window::Window(sf::VideoMode mode, std::string name, sf::Uint8 style):
    sf::RenderWindow(mode, name, style),
    m_is_ia_thinking(false),
    turn(1),
    mode(0)
{
    srand(time(NULL));
    victory = 0;
    m_grid = (char **)malloc(sizeof(char *) * SIZE);
    for (int i = 0; i < SIZE; i++) {
        m_grid[i] = (char *)malloc(sizeof(char) * SIZE);
        memset(m_grid[i], 0, sizeof(char) * SIZE);
    }
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (rand() % 40 == 1)
                rec_creat(i, j, m_grid);
        }
    }

    sf::Vector2u size = getSize();
    decal = (size.x - size.y) / 2;
    spacing = size.y / ZOOM;
    black_rec.setSize(sf::Vector2f(decal, size.y));
    black_rec.setFillColor(sf::Color::Black);
    grid = 0;

    m_grid[0][0] = 0;

    font.loadFromFile("/home/Edo/B-CLICK_TO_GO/font.ttf");

    init_text(m_new_round, font, this, "ENTER to retry", sf::Vector2f(size.x * 0.01, size.y * 0.85));
    init_text(m_turn, font, this, "", sf::Vector2f(size.x * 0.01, size.y * 0.55));
    init_text(ai_thinking, font, this, "AI is thinking...", sf::Vector2f(size.x / 2, size.y / 2.5));
    init_text(m_player_1, font, this, "Player 1: ", sf::Vector2f(size.x * 0.01, size.y * 0.15));
    init_text(y_lose, font, this, "You Lose !", sf::Vector2f(size.x * 0.01, size.y * 0.75));
    init_text(y_win, font, this, "You Won !", sf::Vector2f(size.x * 0.01, size.y * 0.75));

    y_win.setFillColor(sf::Color::Red);
    y_lose.setFillColor(sf::Color::Red);

    rectangle_h.setSize(sf::Vector2f(spacing * SIZE, 1));
    rectangle__.setSize(sf::Vector2f(spacing * SIZE, spacing * SIZE));
    rectangle_v.setSize(sf::Vector2f(1, spacing * SIZE));
    rectangle_h.setFillColor(sf::Color(0, 0, 0, 50));
    rectangle__.setFillColor(sf::Color(100, 100, 100, 255));
    rectangle_v.setFillColor(sf::Color(0, 0, 0, 50));

    dark.setSize(sf::Vector2f(1080 / ZOOM, 1080 / ZOOM));
    dark.setFillColor(sf::Color(0, 0, 0, 150));
    light.setSize(sf::Vector2f(1080 / ZOOM, 1080 / ZOOM));
    light.setFillColor(sf::Color(0, 255, 0, 5.1));
}

void Window::change_victory(int nb)
{
    victory = nb;
}

int Window::get_victory()
{
    return victory;
}

void Window::draw_texts()
{
    m_turn.setString("Turn number " + toString(this->turn));

    draw(m_turn);
    draw(m_player_1);


    if (victory == 1)
        draw(y_win);
    if (victory == 2)
        draw(y_lose);
    if (victory)
        draw(m_new_round);
}

Window::~Window()
{
}

void Window::draw_pebble(float decal, float spacing)
{
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (m_grid[i][j] > 50) {
                light.setPosition(sf::Vector2f(decal + spacing * (i), spacing * (j)));
                light.setFillColor(sf::Color(0, 255, 0, 5.1 * (m_grid[i][j] - 15)));
                // for (int h = 15; h < m_grid[i][j]; h++)
                    draw(light);
            } else if (m_grid[i][j] == -2) {
                dark.setPosition(sf::Vector2f(decal + spacing * (i), spacing * (j)));
                draw(dark);
            }
        }
    }
}

void Window::draw_grid()
{
    sf::Vector2f view = getView().getCenter();
    sf::Vector2u size = getSize();
    rectangle_h.setPosition(sf::Vector2f(decal, 0));
    rectangle__.setPosition(sf::Vector2f(decal, 0));
    rectangle_v.setPosition(sf::Vector2f(decal, 0));
    draw(rectangle__);
    draw(rectangle_h);
    draw(rectangle_v);
    rectangle_v.setPosition(sf::Vector2f(decal + spacing * SIZE, 0));
    rectangle_h.setPosition(sf::Vector2f(decal, spacing * SIZE));
    draw(rectangle_h);
    draw(rectangle_v);
    for (int i = 0; grid == 1 && i < SIZE + 1; i++) {
        rectangle_h.setPosition(sf::Vector2f(decal, spacing * (i)));
        rectangle_v.setPosition(sf::Vector2f(decal + spacing * (i), 0));
        draw(rectangle_h);
        draw(rectangle_v);
    }
    draw_pebble(decal, spacing);
}


void Window::Update_view(Player &player)
{
    std::pair<int, int> pos = player.get_pos();
    sf::Vector2f view = getView().getCenter();
    sf::Vector2f tmp = sf::Vector2f(pos.first * spacing + decal, pos.second * spacing);

    if (tmp.x < view.x)
        view.x -= 5;
    if (tmp.y < view.y)
        view.y -= 5;
    if (tmp.x > view.x)
        view.x += 5;
    if (tmp.y > view.y)
        view.y += 5;
    if (mode == 0)
        setView(sf::View(view, WINDOW_SIZE));
}

void Window::clear_map(void)
{
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (m_grid[i][j] > 0)
                m_grid[i][j] = 0;
}