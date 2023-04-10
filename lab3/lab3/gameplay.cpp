#include "gameplay.h"
#include "termio.h"

Gameplay::~Gameplay()
{
    movement* tmp;

    if (ops != nullptr) {
        tmp = ops;
        ops = ops->prev;
        delete tmp;
    }
}

void Gameplay::initial(int n)
{
    movement* tmp;

    if (ops != nullptr) {
        tmp = ops;
        ops = ops->prev;
        delete tmp;
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            map[i][j] = 0;
        }
        top[i] = -1;
    }

    for (int k = 0; k < n; k++) {
        map[0][k] = n - k;
    }
    top[0] = n - 1;

    Termio::ResetBuffer();

    for (int i = 0; i < Termio::CANVAS_WIDTH; i++) {
        Termio::buffer[Termio::CANVAS_HEIGHT - 1][i] = '-';
    }
    for (int j = 0; j < Termio::CANVAS_HEIGHT; j++) {
        Termio::buffer[j][5] = '|';
        Termio::buffer[j][20] = '|';
        Termio::buffer[j][35] = '|';
    }

    for (int k = 0; k < n; k++) {
        for (int j = 5 - n + k; j <= 5 + n - k; j++) {
            Termio::buffer[Termio::CANVAS_HEIGHT - 2 - 2 * k][j] = '*';
        }
    }
}

void Gameplay::move(int from, int to, Gameplay::Mode mode)
{
    if (top[from] == -1 || (top[to] != -1 && map[from][top[from]] >= map[to][top[to]])) {
        throw string("Invalid Movement.");
    }

    int disk_size;

    disk_size = map[to][++top[to]] = map[from][top[from]];

    for (int j = -disk_size; j <= disk_size; j++) {
        Termio::buffer[Termio::CANVAS_HEIGHT - 2 - 2 * top[to]][5 + 15 * to + j] = '*';
        Termio::buffer[Termio::CANVAS_HEIGHT - 2 - 2 * top[from]][5 + 15 * from + j] = ' ';
    }
    Termio::buffer[Termio::CANVAS_HEIGHT - 2 - 2 * top[from]][5 + 15 * from] = '|';

    map[from][top[from]--] = 0;

    if (mode == Manual) {
        ops = new movement(from, to, ops);
    }
}

bool Gameplay::check()
{
    return top[0] == -1 && top[2] == -1;
}

void Gameplay::autoplay()
{
    movement* tmp;

    while (ops != nullptr) {
        std::cout << "Auto moving:" << ops->to + 1 << "->" << ops->from + 1 << std::endl;
        move(ops->to, ops->from, Auto);
        Termio::Draw();

        tmp = ops;
        ops = ops->prev;
        delete tmp;
    }

    solve(top[0] + 1, 0, 1, 2);
}

void Gameplay::solve(int n, int from, int to, int buf)
{
    if (n == 0)
        return;

    solve(n - 1, from, buf, to);

    std::cout << "Auto moving:" << from + 1 << "->" << to + 1 << std::endl;
    move(from, to, Auto);
    Termio::Draw();

    solve(n - 1, buf, to, from);
}

void Gameplay::run()
{
    string buf;

    while (true) {
        while (true) {
            std::cout << "How many disks do you want? (1 ~ 5)" << std::endl;
            getline(std::cin, buf);
            if (buf.length() == 1) {
                if (buf[0] == 'Q')
                    return;

                if (buf[0] >= '1' && buf[0] <= '5') {
                    initial(buf[0] - '0');
                    Termio::Clear();
                    Termio::Draw();
                    break;
                }
            }
            Termio::Clear();
        }

        while (true) {
            while (true) {
                std::cout << "Move a disk. Format: x y" << std::endl;
                getline(std::cin, buf);
                if (buf.length() == 3 && buf[1] == ' ') {
                    if (buf[0] >= '1' && buf[0] <= '3' && buf[2] >= '1' && buf[2] <= '3') {
                        try {
                            move(buf[0] - '1', buf[2] - '1');
                        }
                        catch (string s) {
                            Termio::Clear();
                            Termio::Draw();
                            continue;
                        }
                        Termio::Clear();
                        Termio::Draw();
                        break;
                    }
                    else if (buf[0] == '0' && buf[2] == '0') {
                        Termio::Clear();
                        autoplay();
                        break;
                    }
                }
                Termio::Clear();
                Termio::Draw();
            }

            if (check()) {
                std::cout << "Congratulations! You win!" << std::endl;
                break;
            }
        }
    }
}
