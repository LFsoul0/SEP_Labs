#ifndef GAMEPLAY_H
#define GAMEPLAY_H


class Gameplay
{
private:
    struct movement
    {
        int from, to;
        movement* prev;
        movement(int from, int to, movement* prev = nullptr): from(from), to(to), prev(prev) { }
        ~movement() { }
    };

    enum Mode { Manual, Auto };

    int map[3][5];
    int top[3];
    movement* ops;

    void initial(int n);
    void move(int from, int to, Mode mode = Manual);
    bool check();
    void autoplay();
    void solve(int n, int from, int to, int buf);

public:
    Gameplay(): map{{ 0 }}, top{ -1, -1, -1 }, ops(nullptr) { }
    ~Gameplay();
    void run();
};

#endif // GAMEPLAY_H
