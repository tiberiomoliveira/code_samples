/**
 * Tic-Tac-Toe Judgment
 * Determines the game outcome after a player's turn in Tic-Tac-Toe.
 *
 * @param board The current state of the game board. A 3x3 array where:
 *        'X' → Player X's mark
 *        'O' → Player O's mark
 *        ' ' → Empty cell
 *
 * @return:
 *        "X"    → X won
 *        "O"    → O won
 *        "Tie"  → Game is a draw
 *        "None" → Game is still ongoing

Given a 3×3 board, determine the result:
- Check rows/columns/diagonals 
- If no winner, check for empty cells 
- Return X / O / Tie / None
 */
#include <string>

// Helper
char winner(char a, char b, char c)
{
    if (a != ' ' && a == b && b == c)
        return a;
    return ' ';
}

std::string judgeTicTacToe(char board[3][3])
{
    char w;
    // Check rows and columns together
    for (int i = 0; i < 3; ++i)
    {
        // Check row
        if ((w = winner(board[i][0],
                        board[i][1],
                        board[i][2])) != ' ')
            return std::string(1, w);
        // Check column
        if ((w = winner(board[0][i],
                        board[1][i],
                        board[2][i])) != ' ')
            return std::string(1, w);
    }
    // Check main diagonal
    if ((w = winner(board[0][0],
                    board[1][1],
                    board[2][2])) != ' ')
        return std::string(1, w);
    // Check anti-diagonal
    if ((w = winner(board[0][2],
                    board[1][1],
                    board[2][0])) != ' ')
        return std::string(1, w);
    // Check for empty cell
    for (int r = 0; r < 3; r++)
    {
        for (int c = 0; c < 3; c++)
        {
            if (board[r][c] == ' ')
            {
                return "None";
            }
        }
    }

    return "Tie";
}
