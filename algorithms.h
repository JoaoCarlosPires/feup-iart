#pragma once

#include "Node.h"
#include <algorithm>
#include <iostream>

Node* newNode(int value, int depth);

int minimax(Node* root, int depth, bool player);

int minimaxAB(Node* root, int depth, int alpha, int beta, bool player);



