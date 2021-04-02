#include "algorithms.h"

using namespace std;

Node* newNode(int value, int depth) {

	Node* tmp = new Node;
	tmp->value = value;
	tmp->depth = depth;
	
	return tmp;

}

int minimax(Node* root, int depth, bool player) { //1 if player1 0 if player 2

	if (depth == 0) {
		return root->value;
	}

	if (player) {
		int maxVal = -1000;
		for (unsigned int i = 0; i < root->children.size(); i++) {
			int val = minimax(root->children.at(i), (depth - 1), false);
			maxVal = max(maxVal, val);
		}
		return maxVal;
	}
	else {
		int minVal = 1000;
		for (unsigned int i = 0; i < root->children.size(); i++) {
			int val = minimax(root->children.at(i), (depth - 1), true);
			minVal = min(minVal, val);
		}
		return minVal;
	}
	return 0;
}

int minimaxAB(Node* root, int depth, int alpha, int beta, bool player) {

	if (depth == 0) {
		return root->value;
	}
	if (player) {
		int maxVal = -1000;
		for (unsigned int i = 0; i < root->children.size(); i++) {
			int val = minimaxAB(root->children.at(i), (depth - 1), alpha, beta, false);
			maxVal = max(maxVal, val);
			alpha = max(alpha, val);
			if (beta <= alpha) {
				cout << "pruning occurred ALPHA(" << alpha << ")" << " BETA(" << beta << ")" << endl;
				break;
			}
		}
		return maxVal;
	}

	else {
		int minVal = 1000;
		for (unsigned int i = 0; i < root->children.size(); i++) {
			int val = minimaxAB(root->children.at(i), (depth - 1), alpha, beta, true);
			minVal = min(minVal, val);
			beta = min(beta, val);
			if (beta <= alpha) {
				cout << "pruning occurred ALPHA(" << alpha << ")" << " BETA(" << beta << ")" << endl;
				break;
			}
		}
		return minVal;
	}
	return 0;
}




