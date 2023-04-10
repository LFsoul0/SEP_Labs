#include <iostream>
#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

#include "lexicon.h"

using namespace std;

class Boggle
{
private:
    vector<string> board;
    Lexicon dict;
    Lexicon ans;
    Lexicon found;

    void getAns();
    void getAns(int scale, int pos, vector<bool> &mark, string &path);

public:
    Boggle(const string &filename);
    ~Boggle();
    void run();
    void playerTurn();
    void computerTurn();
};


int main()
{
    Boggle bog("./EnglishWords.txt");
    bog.run();

    return 0;
}


Boggle::Boggle(const string &filename)
    : dict(filename)
{

}

Boggle::~Boggle()
{

}

void Boggle::run()
{
    int n;
    cin >> n;
    board.resize(n);
    for (int i = 0; i < n; ++i) {
        cin >> board[i];
    }

    getAns();

    playerTurn();

    computerTurn();

}

void Boggle::getAns()
{
    int n = board.size();

    vector<bool> mark;
    mark.resize(n * n);
    fill(mark.begin(), mark.end(), false);

    ans.clear();
    string s;
    for (int i = 0; i < n * n; i++) {
        getAns(n, i, mark, s);
    }

}

void Boggle::getAns(int scale, int pos, vector<bool> &mark, string &path)
{
    if (mark[pos]) return;
    if (!dict.containsPrefix(path)) return;

    mark[pos] = true;
    path += board[pos / scale][pos % scale];
    if (path.length() >= 4 && dict.contains(path)) {
        ans.add(path);
    }

    if (pos / scale - 1 >= 0) {
        getAns(scale, pos - scale, mark, path);
        if (pos % scale - 1 >= 0)
            getAns(scale, pos - scale - 1, mark, path);
        if (pos % scale + 1 < scale)
            getAns(scale, pos - scale + 1, mark, path);
    }
    if (pos / scale + 1 < scale) {
        getAns(scale, pos + scale, mark, path);
        if (pos % scale - 1 >= 0)
            getAns(scale, pos + scale - 1, mark, path);
        if (pos % scale + 1 < scale)
            getAns(scale, pos + scale + 1, mark, path);
    }
    if (pos % scale - 1 >= 0)
        getAns(scale, pos - 1, mark, path);
    if (pos % scale + 1 < scale)
        getAns(scale, pos + 1, mark, path);

    mark[pos] = false;
    path.pop_back();

}

void Boggle::playerTurn()
{
    vector<string> foundList;
    found.clear();
    unsigned long point = 0;
    string input;

    while (true) {
        for (string s : board) {
            cout << s << endl;
        }
        cout << "Your Score: " << point << endl << "Your Words:";
        for (string s : foundList) {
            cout << " " << s;
        }
        cout << endl;

        cin >> input;
        if (input == "???") break;

        transform(input.begin(), input.end(), input.begin(), (int (*)(int))tolower);
        if (input.length() < 4) {
            cout << input << " is too short." << endl;
        }
        else if (!dict.contains(input)) {
            cout << input << " is not a word." << endl;
        }
        else if (!ans.contains(input)) {
            cout << input << " is not on board." << endl;
        }
        else if (found.contains(input)) {
            cout << input << " is already found." << endl;
        }
        else {
            point += input.length() - 3;
            found.add(input);
            foundList.push_back(input);
        }

    }
}

void Boggle::computerTurn()
{
    set<string> pcAns = ans.toStlSet();
    unsigned long pcPoint = 0;

    found.mapAll([&](string s){ pcAns.erase(s); });

    for (string s : pcAns) {
        pcPoint += s.length() - 3;
    }

    cout << "Computer Score: " << pcPoint << endl << "Computer Words:";
    for (string s : pcAns) {
        transform(s.begin(), s.end(), s.begin(), (int (*)(int))toupper);
        cout << " " << s;
    }
}
