#include <iostream>
#include <sstream>
#include "Editor.h"

using namespace std;

Editor::Editor()
{
    buffer = new Buffer();
}
Editor::~Editor()
{
    // (Done)TODO: Implement destructor
    delete buffer;
}

void Editor::run()
{
    string cmd;
    while (true)
    {
        cout << "cmd> ";
        cout.flush();
        getline(cin, cmd);
        if (cmd == "Q")
            break;
        try {
            dispatchCmd(cmd);
        } catch (const char *e) {
            cout << "? " << e << endl;
        } catch (const out_of_range &oor) {
            cout << "? " << oor.what() << endl;
        } catch (const range_error &re) {
            cout << "? " << re.what() << endl;
        }
    }
}
void Editor::cmdAppend()
{
    cout << "It's input mode now. Quit with a line with a single dot(.)" << endl;
    // (Done)TODO: finish cmdAppend.
    while (true) {
        string text;
        getline(cin, text);
        if (text == ".")
            break;
        buffer->appendLine(text);
    }
}

void Editor::cmdInsert()
{
    cout << "It's input mode now. Quit with a line with a single dot(.)" << endl;
    bool firstLine = true;
    while (true)
    {
        string text;
        getline(cin, text);
        if (text == ".")
            break;
        if (firstLine) {
            buffer->insertLine(text);
            firstLine = false;
        }  else {
            buffer->appendLine(text);
        }
    }
}

void Editor::cmdDelete(int start, int end)
{
    buffer->deleteLines(start, end);
}

void Editor::cmdNull(int line)
{
    cout << buffer->moveToLine(line) << endl;
}

void Editor::cmdNumber(int start, int end)
{
    buffer->showLines(start, end);
}

void Editor::cmdWrite(const string &filename)
{
    buffer->writeToFile(filename);
}

void Editor::dispatchCmd(const string &cmd)
{
    if (cmd == "a") {
        cmdAppend();
        return;
    }
    if (cmd == "i") {
        cmdInsert();
        return;
    }
    if (cmd[0] == 'w' && (cmd[1] == '\0' || cmd[1] == ' ' || cmd[1] == '\t')) {
        // (Done)TODO: call cmdWrite with proper arguments
        int i = 1;
        while (true) {
            if (cmd[i] == '\0')
                throw "Filename not specified";
            else if (cmd[i] == ' ' || cmd[i] == '\t')
                i++;
            else
                break;
        }
        cmdWrite(cmd.substr(i));
        return;
    }
    // (Done)TODO: handle special case "1,$n".
    if (cmd == "1,$n") {
        cmdNumber(1, buffer->getMaxLine());
        return;
    }
    if (cmd == "")
        throw "Bad/Unknown command";

    //前面程序已判断处理了Append，Insert，Write，"1,$n"以及指令为空的情况
    int start, end;
    char comma, type, res;
    stringstream ss(cmd);  //将cmd转化为stringstream类型，便于通过流输入析取指令的不同部分
    ss >> start; //试图析取指令开头的数字至start
    if (ss.eof()) {  //若成功读取数字start并且指令已经结束（如果指令为空，在前面代码中已加入判断并抛出异常；如果指令不为空但未读取成功，则badbit置为true但未达到eof）
        cmdNull(start);  //判断为Null Command，调用对应函数
        return;  //结束对本条指令的解析
    }
    ss >> comma >> end >> type;  //针对"?,?x"型指令，试图从指令中析取','至comma、析取第二个数字至end、析取操作类型至type
    if (ss.good() && comma == ',' && !(ss >> res)) {  //若上述两次析取均成功（goodbit为true），且指令满足"?,?x"形式(数字以逗号分隔，无多余字符）
        if (type == 'n') {  //若指令以n结尾
            cmdNumber(start, end);  //判断为Number Command，调用对应函数
            return;  //结束对本条指令的解析
        } else if (type == 'd') {  //若指令以d结尾
            cmdDelete(start, end);  //判断为Delete Command，调用对应函数
            return;  //结束对本条指令的解析
        }
    }
    throw "Bad/Unknown command";  //不是规定的某个指令，抛出异常
}
