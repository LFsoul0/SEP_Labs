#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <limits.h>
#include <vector>
#include <queue>
#include <algorithm>

#include "Tree.h"

using namespace std;

/****************************************************************
 *                    Write your code below
 ****************************************************************/

ostream &operator<<(ostream &out, const TreeNode &b)
{
    out << b.data[0] << " " << b.data[1];
    return out;
}

long long int TreeNode::distance(int x, int y)
{
    return ((long long int)data[X] - x) * ((long long int)data[X] - x) + ((long long int)data[Y] - y) * ((long long int)data[Y] - y);
}

long long int TreeNode::distance(int x, int y, Dimon dim)
{
    if (dim == X)
        return ((long long int)data[X] - x) * ((long long int)data[X] - x);
    else
        return ((long long int)data[Y] - y) * ((long long int)data[Y] - y);
}

long long int TreeNode::range_distance(int x, int y)
{
    if (x < min[X]) {
        if (y < min[Y])
            return ((long long int)min[X] - x) * ((long long int)min[X] - x) + ((long long int)min[Y] - y) * ((long long int)min[Y] - y);
        else if (y > max[Y])
            return ((long long int)min[X] - x) * ((long long int)min[X] - x) + ((long long int)max[Y] - y) * ((long long int)max[Y] - y);
        else
            return ((long long int)min[X] - x) * ((long long int)min[X] - x);
    }
    else if (x > max[X]) {
        if (y < min[Y])
            return ((long long int)max[X] - x) * ((long long int)max[X] - x) + ((long long int)min[Y] - y) * ((long long int)min[Y] - y);
        else if (y > max[Y])
            return ((long long int)max[X] - x) * ((long long int)max[X] - x) + ((long long int)max[Y] - y) * ((long long int)max[Y] - y);
        else
            return ((long long int)max[X] - x) * ((long long int)max[X] - x);
    }
    else {
        if (y < min[Y])
            return ((long long int)min[Y] - y) * ((long long int)min[Y] - y);
        else if (y > max[Y])
            return ((long long int)max[Y] - y) * ((long long int)max[Y] - y);
        else
            return 0;
    }
}

bool TreeNode::compare(long long int cur_dis, long long int min_dis, TreeNode* guess)
{
    if (cur_dis < min_dis)
        return true;
    else if (cur_dis == min_dis) {
        if (data[X] < guess->data[X])
            return true;
        else if (data[X] == guess->data[X] && data[Y] < guess->data[Y])
            return true;
    }

    return false;
}

TreeNode::TreeNode(int x, int y, TreeNode* l, TreeNode* r)
{
    data[0] = x;
    data[1] = y;
    left = l;
    right = r;
}

int TreeNode::getX()
{
    return data[0];
}

int TreeNode::getY()
{
    return data[1];
}

TreeNode::~TreeNode()
{
    //deconstruct function
}


istream &operator>>(istream &in, BinaryDimonTree &tree)
{
    int M, samp[100], samp_dist = 1, samp_size = 100;
    long long int mid = 0;
    TreeNode* buf = nullptr;

    in >> M;
    if (M > 100) {
        samp_dist = M / 100;
        for (int i = 0; i < 100; i++) {
            samp[i] = samp_dist * i + rand() % samp_dist;
        }
    }
    else {
        samp_size = M;
        for (int i = 0; i < 100; i++) {
            samp[i] = i;
        }
    }

    int x, y, curr_samp = 0;
    for (int i = 0; i < M; i++) {
        in >> x >> y;
        if (curr_samp < 100 && i == samp[curr_samp]) {
            mid += x;
            curr_samp++;
        }
        buf = new TreeNode(x, y, buf);
    }
    mid /= samp_size;

    tree.split(mid, buf, M, tree.root, X);

    return in;
}


void BinaryDimonTree::split(long long int mid, TreeNode* buf, int scale, TreeNode*& root, Dimon dim)
{
    root = nullptr;

    if (scale == 0)
        return;

    TreeNode* lbuf = nullptr, * rbuf = nullptr, *tmp;
    int lscale = 0, rscale = 0;
    long long int lmid = 0, rmid = 0;
    int Xmax, Xmin, Ymax, Ymin;
    int samp[100], samp_dist = 1, curr_samp = 0, lsamp = 0, rsamp = 0;
    Dimon next_dim;

    if (dim == X) {
        next_dim = Y;
    }
    else {
        next_dim = X;
    }

    if (scale > 100) {
        samp_dist = scale / 100;
        for (int i = 0; i < 100; i++) {
            samp[i] = samp_dist * i + rand() % samp_dist;
        }
    }
    else {
        for (int i = 0; i < 100; i++) {
            samp[i] = i;
        }
    }

    Xmax = Xmin = buf->data[X];
    Ymax = Ymin = buf->data[Y];

    for (int i = 0; i < scale; i++) {
        if (buf->data[X] > Xmax) {
            Xmax = buf->data[X];
        }
        else if (buf->data[X] < Xmin) {
            Xmin = buf->data[X];
        }
        if (buf->data[Y] > Ymax) {
            Ymax = buf->data[Y];
        }
        else if (buf->data[Y] < Ymin) {
            Ymin = buf->data[Y];
        }

        if (buf->data[dim] < mid) {
            tmp = buf;
            buf = tmp->left;
            tmp->left = lbuf;
            lbuf = tmp;
            lscale++;

            if (curr_samp < 100 && i == samp[curr_samp]) {
                lmid += lbuf->data[next_dim];
                curr_samp++;
                lsamp++;
            }
        }
        else if (root == nullptr) {
            tmp = buf;
            buf = tmp->left;
            root = tmp;
            if (curr_samp < 100 && i == samp[curr_samp]) {
                samp[curr_samp]++;
            }
        }
        else {
            if (buf->data[dim] < root->data[dim]) {
                tmp = buf;
                buf = root;
                root->left = tmp->left;
                root = tmp;
            }
            tmp = buf;
            buf = tmp->left;
            tmp->left = rbuf;
            rbuf = tmp;
            rscale++;

            if (curr_samp < 100 && i == samp[curr_samp]) {
                rmid += rbuf->data[next_dim];
                curr_samp++;
                rsamp++;
            }
        }
    }

    root->max[X] = Xmax;
    root->max[Y] = Ymax;
    root->min[X] = Xmin;
    root->min[Y] = Ymin;

    if (lsamp != 0) {
        lmid /= lsamp;
    }
    else if (lscale != 0) {
        lmid = lbuf->data[next_dim];
    }

    if (rsamp != 0) {
        rmid /= rsamp;
    }
    else if (rscale != 0) {
        rmid = rbuf->data[next_dim];
    }


    split(lmid, lbuf, lscale, root->left, next_dim);
    split(rmid, rbuf, rscale, root->right, next_dim);

    return;
}

void BinaryDimonTree::recur_search(TreeNode* cur, int x, int y, long long int& min_dis, TreeNode*& guess)
{
    if (cur == nullptr)
        return;

    long long int cur_dis = cur->distance(x, y);
    if (cur->compare(cur_dis, min_dis, guess)) {
        min_dis = cur_dis;
        guess = cur;
    }

    if (cur->left == nullptr) {
        if (cur->right == nullptr) {

        }
        else {
            recur_search(cur->right, x, y, min_dis, guess);
        }
    }
    else if (cur->right == nullptr) {
        recur_search(cur->left, x, y, min_dis, guess);
    }
    else {
        long long int ldis = cur->left->range_distance(x, y), rdis = cur->right->range_distance(x, y);
        if (ldis <= rdis) {
            if (ldis <= min_dis) {
                recur_search(cur->left, x, y, min_dis, guess);
                if (rdis <= min_dis) {
                    recur_search(cur->right, x, y, min_dis, guess);
                }
            }
        }
        else {
            if (rdis <= min_dis) {
                recur_search(cur->right, x, y, min_dis, guess);
                if (ldis <= min_dis) {
                    recur_search(cur->left, x, y, min_dis, guess);
                }
            }
        }
    }

    return;
}

void BinaryDimonTree::clear(TreeNode* root)
{
    if (root == nullptr)
        return;

    clear(root->left);
    clear(root->right);
    delete root;

    return;
}

BinaryDimonTree::BinaryDimonTree()
{
    root = nullptr;
}

TreeNode* BinaryDimonTree::find_nearest_node(int x, int y)
{
    long long int min_dis = root->distance(x, y);
    TreeNode* guess = root;

    recur_search(root, x, y, min_dis, guess);

    return guess;
}

BinaryDimonTree::~BinaryDimonTree()
{
    clear(root);
}
