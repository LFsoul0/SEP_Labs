#include <fstream>
#include <iostream>
#include "Buffer.h"

using namespace std;

//(Doing)TODO: your code here
//implement the functions in ListBuffer

Buffer::Buffer()
{
    lines = new node;
    currentLine = lines;
    maxLine = 0;
}

Buffer::~Buffer()
{
    node* p;

    while (lines) {
        p = lines->next;
        delete lines;
        lines = p;
    }
}

Buffer::node* Buffer::locate(int idx) const
{
    node* tag = lines;

    while (idx > 0) {
        tag = tag->next;
        idx--;
    }

    return tag;
}

void Buffer::writeToFile(const string &filename) const
{
    fstream fs;
    fs.open(filename, ios::out);

    node* p = lines;
    while (p->next) {
        p = p->next;
        fs << p->s << endl;
    }

    fs.close();

    fs.open(filename, ios::in|ios::binary);
    long fileLen;

    fs.seekg(0, ios::end);
    fileLen = fs.tellg();
    cout << fileLen << " byte(s) written" << endl;

    fs.close();
}

void Buffer::showLines(int from, int to)
{
    if (from > to)
        throw range_error("Number range error");
    if (from <= 0 ||to > maxLine)
        throw out_of_range("Line number out of range");

    currentLine = locate(from - 1);
    while (from <= to) {
        currentLine = currentLine->next;
        cout << from << '\t' << currentLine->s << endl;
        from++;
    }
}

void Buffer::deleteLines(int from, int to)
{
    if (from > to)
        throw range_error("Delete range error");
    if (from <= 0 || to > maxLine)
        throw out_of_range("Line number out of range");

    node* p;
    currentLine = locate(from - 1);
    while (from <= to) {
        p = currentLine->next;
        currentLine->next = p->next;
        delete p;

        from++;
        maxLine--;
    }

    if (currentLine->next) {
        currentLine = currentLine->next;
    }
}

void Buffer::insertLine(const string &text)
{
    if (currentLine == lines) {
        appendLine(text);
    }
    else {
        node* p = lines;
        while (p->next != currentLine) {
            p = p->next;
        }
        currentLine = p;

        appendLine(text);
    }
}

void Buffer::appendLine(const string &text)
{
    currentLine->next = new node(text, currentLine->next);
    currentLine = currentLine->next;
    maxLine++;
}

const string &Buffer::moveToLine(int idx)
{
    if (idx <= 0 || idx > maxLine)
        throw out_of_range("Line number out of range");

    currentLine = locate(idx);
    return currentLine->s;
}
