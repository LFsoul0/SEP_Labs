#pragma once

#include <string>

using std::string;

class Buffer {
private:
    // （Done)TODO: add a List here for storing the input lines
    struct node {
        string s;
        node* next;
        node(string s = "", node* next = nullptr):
            s(s), next(next) {}
    };

    node* lines;
    node* currentLine;
    int maxLine;

    node* locate(int idx) const;

public:
    Buffer();
    ~Buffer();
    int getMaxLine() { return maxLine; }
    void writeToFile(const string &filename) const;
    const string &moveToLine(int idx);
    void showLines(int from, int to);
    void deleteLines(int from, int to);
    void insertLine(const string &text);
    void appendLine(const string &text);
};
