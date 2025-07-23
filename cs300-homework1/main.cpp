#include <iostream>
#include "stack.h"
#include <fstream>
#include <vector>
#include "stack.cpp"


using namespace std;

bool isInSafePos(int row, int col, const vector<int>& queens) {
    for (int i = 0; i < row; i++) {
        // Check if another queen is in the same column
        if (queens[i] == col) {
            return false;
        }
        // Check if another queen is on the same diagonal
        if (abs(queens[i] - col) == abs(i - row)) {
            return false;
        }
    }
    return true;
}


void backTrack(int N, vector<vector<int>>& solutions) {
    Stack<int> columnStack;
    Stack<int> rowStack;

    vector<int> queens(N, -1);
    int row = 0;
    int col = 0;

    // Continue until backtracking from the first row is exhausted
    while (row >= 0) {
        // If we have placed all queens, record the solution
        if (row == N) {
            solutions.push_back(queens);  // Found a solution, store it
            row = rowStack.topAndPop();   // Backtrack to the previous row
            col = columnStack.topAndPop() + 1;  // Try the next column in the previous row
        }
            // Try placing a queen in the next safe column
        else if (col < N) {
            if (isInSafePos(row, col, queens)) {
                // Safe to place queen here
                queens[row] = col;
                rowStack.push(row);       // Push current row to stack
                columnStack.push(col);    // Push current column to stack

                row++;  // Move to the next row
                col = 0;  // Start from the first column
            } else {
                col++;  // Try the next column
            }
        }
            // Backtrack if no column is safe in the current row
        else {
            if (rowStack.isEmpty() && row == 0) {
                // No more solutions are possible if we are backtracking from the first row
                return;  // All possibilities exhausted
            }
            row = rowStack.topAndPop();  // Backtrack to the previous row
            col = columnStack.topAndPop() + 1;  // Move to the next column
        }
    }
}


int main() {
    int N;
    cout << "Enter the value of N for the N-Queens problem: ";
    cin >> N;

    // check if input is valid or not
    if (N <= 0 || N >= 14) {
        cout << "Invalid input. Please enter a positive integer" << endl;
        return 1;
    }

    vector<vector<int>> solutions;
    backTrack(N, solutions);

    cout << "Content of " << to_string(N) << "queens_solutions.txt:" << endl;

    // Output to file
    ofstream outFile(to_string(N) + "queens_solutions.txt");
    if (outFile.is_open()) {
        outFile << "Total solutions for N=" << N << ": " << solutions.size() << "\n\n";
        for (int i = 0; i < solutions.size(); i++) {
            outFile << "Solution " << (i+1) << ": [";
            for (int j = 0; j < solutions[i].size(); j++) {
                outFile << solutions[i][j];
                if (j < solutions[i].size() - 1) {
                    outFile << ", ";
                }
            }
            outFile << "]\n";
        }
        outFile.close();
        cout << "Solutions have been saved to '" << N << "queens_solutions.txt'" << endl;
    } else {
        cout << "Unable to open file for writing." << endl;
        return 1;
    }

    // Print solutions to console as well
    cout << "Total solutions for N=" << N << ": " << solutions.size() << "\n";
    for (int i = 0; i < solutions.size(); i++) {
        cout << "Solution " << (i+1) << ": [";
        for (int j = 0; j < solutions[i].size(); j++) {
            cout << solutions[i][j];
            if (j < solutions[i].size() - 1) {
                cout << ", ";
            }
        }
        cout << "]" << endl;
    }

    return 0;
}