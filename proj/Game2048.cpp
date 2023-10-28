#include "Game2048.hh"

#include <stdio.h>
#include <cmath>

Game2048::Game2048(): desiredValue(2048)
{
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            board[i][j]=-1;
}

void Game2048::Render()
{
    for(int i=0; i<4; i++)
    {
        printf("____\n");
        for(int j=0; j<4; j++)
        {
            printf("|");
            printf("%d", board[i][j]);
        }
        printf("|\n");
    }
    printf("____\n\n");
}

void Game2048::Play()
{
    Render();
}