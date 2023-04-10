//
// Created by Smile on 2019/4/8.
//

#ifndef C_BINARYDIMEN_TREE_H
#define C_BINARYDIMEN_TREE_H

#include <stdio.h>
#include <iostream>
#include <vector>
using namespace std;

/****************************************************************
 *                    Write your code below
 ****************************************************************/

enum Dimon { X, Y };

class TreeNode
{
  friend ostream &operator<<(ostream &out, const TreeNode &b);
  friend class BinaryTree;
  friend class BinaryDimonTree;

private:
  /* data */
  int data[2];
  int max[2];
  int min[2];
  TreeNode* left, * right;

  long long int distance(int x, int y);
  long long int distance(int x, int y, Dimon dim);
  long long int range_distance(int x, int y);
  bool compare(long long int cur_dis, long long int min_dis, TreeNode* guess);

public:
  /* methods */
  TreeNode(int x, int y, TreeNode* l = nullptr, TreeNode* r = nullptr);
  int getX();  /* DO NOT CHANGE */
  int getY();  /* DO NOT CHANGE */
  ~TreeNode(); /* DO NOT CHANGE */
};


class BinaryDimonTree
{
friend istream &operator>>(istream &in, BinaryDimonTree &tree); /* DO NOT CHANGE */

private:
  /* data */
  TreeNode* root;

  static void split(long long int mid, TreeNode* buf, int scale, TreeNode*& root, Dimon dim);
  static void recur_search(TreeNode* cur, int x, int y, long long int& min_dis, TreeNode*& guess);
  static void clear(TreeNode* root);

public:
  /* methods */
  BinaryDimonTree();          /* DO NOT CHANGE */
  TreeNode* find_nearest_node(int x, int y);  /* DO NOT CHANGE */
  ~BinaryDimonTree();
};

#endif //C_BINARYDIMEN_TREE_H
