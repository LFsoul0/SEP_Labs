#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <iomanip>
#include "Tree.h"

using namespace std;

void run_testcase(string testcasename)
{
    clock_t start, end;
    ifstream testcase;
    testcase.open(testcasename);

    start = clock();
    BinaryDimonTree *tree = new BinaryDimonTree();
    testcase >> *tree;
    //cout << "Tree constructed." << endl;

    int testNum;
    testcase >> testNum;
    bool res = true;
    for (int i = 0; i < testNum; i++)
    {
        int x, y, z, w;
        testcase >> x;
        testcase >> y;
        testcase >> z;
        testcase >> w;
        TreeNode *node = tree->find_nearest_node(x, y);
        if (z == node->getX() && w == node->getY())
        {

        }
        else
        {
            cout << "case:" << x << " " << y << ",";
            cout << "expect:" << z << " " << w << ",";
            cout << "actual:" << node->getX() << " " << node->getY() << endl;
            res = false;
            break;
        }
    }
    if(res){
        end = clock();
        cout << "pass " << fixed << setprecision(3) << double(end - start)/CLOCKS_PER_SEC << "s" << endl;
    }
    delete tree;
    testcase.close();
}

int main()
{
    cout << "The first group of test cases' results." << endl;
    run_testcase("1.txt");

    cout << "The second group of test cases' results." << endl;
    run_testcase("2.txt");

    cout << "The final group of test cases' results." << endl;
    run_testcase("c10.txt");
    return 0;
}
