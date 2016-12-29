//  main.c
//  threadCount
//  Created by Benjamin Gutierrez on 12/1/16.
//  Copyright © 2016 Benjamin Gutierrez. All rights reserved.

#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define NUMCARDS 52
#define DEALTCARDS 3
#define PLAYERCARDS 3

int card_Stack[NUMCARDS];
int player_Cards[PLAYERCARDS];
int dealt_Cards[DEALTCARDS];
bool gameOver = false;
bool newGame = true;
int turn, count = 0;
int rounds = 1;
int seed;
FILE *fp;

bool getCardSet(int * card_Stack, int size);
void swap(int *a, int *b);
void randomize ( int arr[], int n );
bool cardIsOut(int checkThisCard);
int nextTurn(int i);
int pickAcard();
void printExitPlayer(int turn);

pthread_t actors[4];
pthread_mutex_t newTurn;
pthread_cond_t turnCond;
pthread_mutex_t newTurn2;
pthread_cond_t turnCond2;
pthread_mutex_t newTurn1;
pthread_cond_t turnCond1;
pthread_mutex_t newTurn3;
pthread_cond_t turnCond3;
pthread_mutex_t dumb;
pthread_mutex_t dumb2;
pthread_cond_t stupid;
pthread_cond_t stupid2;


void go(pthread_cond_t *cond1, pthread_cond_t *cond2,
              pthread_mutex_t *lock1, pthread_mutex_t *lock2, int player)
{
    pthread_mutex_lock(lock1);
    int loc = pickAcard();
    int myCard = card_Stack[loc];

    fp = fopen("results.txt", "a");
    fprintf(fp, "PLAYER %d", player);
    fprintf(fp, ": hand %d\n", player_Cards[player-1]);
    fprintf(fp, "PLAYER %d", player);
    fprintf(fp, ": draws %d\n", myCard);
    fclose(fp);

    printf("PLAYER %d\n", player);
    printf("HAND card %d", player_Cards[player-1]);
    printf(" card %d\n", myCard);
    
    if(myCard == player_Cards[player-1])
    {
	printf("WIN yes\n");
        printExitPlayer(player);
	newGame = true;
        rounds++;
        if(rounds == 4)
        {
            gameOver = true;
    	    fp = fopen("results.txt", "a");
            fprintf(fp,"Game Over!!! \n");
            printf("Game Over!!! \n");
            fclose(fp);
            int i = 0;
            for(;i<4;i++)
            {
                pthread_detach(actors[i]);
            }
        }
    }
    else
    {
	printf("WIN no\n");
        int j = rand() % 2;
        fp = fopen("results.txt", "a");
        if(j == 1)
        {
            fprintf(fp, "PLAYER %d", player);
            fprintf(fp, ": discards %d\n", player_Cards[player-1]);
            dealt_Cards[player-1] = loc;
            player_Cards[player-1] = myCard;
        }
	else
	{
            fprintf(fp, "PLAYER %d", player);
            fprintf(fp, ": discards %d\n", myCard);
	
	}
        printf("DECK:  ");
        fprintf(fp, "DECK:  ");
        int k = 0;
	bool isThere = false;
        for(;k<NUMCARDS;k++)
	{
	    int s = 0;
	    for(;s<3;s++)
	    {
		if(k == dealt_Cards[s])
		{
		    isThere = true;
		}
	    }
            (isThere) ? isThere : 
             fprintf(fp,"%d ",card_Stack[k]), printf("%d ",card_Stack[k]);
             isThere = false;
	}
	fprintf(fp, "\n");
	printf("\n");
	fclose(fp);
        (turn==3) ? (turn = 1) : (turn = nextTurn(turn));
    }
    pthread_mutex_lock(lock2);
    pthread_cond_signal(cond1);
    pthread_mutex_unlock(lock2);
    pthread_cond_wait(cond2, lock1);
    pthread_mutex_unlock(lock1);
}
void dontGo(pthread_cond_t *cond1, pthread_cond_t *cond2,
              pthread_mutex_t *lock1, pthread_mutex_t *lock2)
{
    pthread_mutex_lock(lock1);
    pthread_mutex_lock(lock2);
    pthread_cond_signal(cond1);
    pthread_mutex_unlock(lock2);
    pthread_cond_wait(cond2, lock1);
    pthread_mutex_unlock(lock1);
}

static void * dealerRun(void * _)
{
    srand(seed);
    while(gameOver == false)
    {
        if(newGame)
        {
            randomize(card_Stack, NUMCARDS);
            printf("DEALER: Shuffle\n");
	    fp = fopen("results.txt", "a");
            fprintf(fp,"DEALER: Shuffle\n");
            fclose(fp);
            int i = 0;
            for(; i < 3; i++)
            {
                int j = rand() % 53;
                player_Cards[i] = card_Stack[j];
                dealt_Cards[i] = j;
            }
            newGame = false;
            switch(rounds)
            {
                case 1:
                    turn = 1;
                    break;
                case 2:
                    turn = 2;
                    break;
                case 3:
                    turn = 3;
                    break;
                default:
                    break;
            }
        }
        switch (turn)
        {
            case 1:
                dontGo(&turnCond,&turnCond2,&newTurn,&newTurn2);
                break;
            case 2:
                dontGo(&turnCond1,&turnCond3,&newTurn1,&newTurn3);
                break;
            case 3:
                dontGo(&stupid2,&stupid,&dumb,&dumb2);
                break;
            default:
                break;
        }
    }
    pthread_exit(0);
}

static void * player_RUN(void * arg)
{
    int id = (intptr_t) arg; 
    srand(seed);
    while(gameOver == false)
    {
        if(id == 1)
        {
            (turn!=1) ?
            (dontGo(&turnCond2, &turnCond, &newTurn,  &newTurn2)) :
            (go(&turnCond2, &turnCond, &newTurn,  &newTurn2, id));
        }
        else if(id == 2)
        {
            (turn!=2) ?
            (dontGo(&turnCond3, &turnCond1, &newTurn1,  &newTurn3)) :
            (go(&turnCond3, &turnCond1, &newTurn1,  &newTurn3, id));
        }
        else
        {
            (turn !=3) ?
            (dontGo(&stupid, &stupid2, &dumb,  &dumb2)) :
            (go(&stupid, &stupid2, &dumb,  &dumb2,id));
        }
    }
    pthread_exit(0);
}


int main(int argc, const char * argv[])
{
    int i;
    if(argc == 1)
    {
	printf("Please provide the seed arguement\n");
	return 0;
    }
    seed = atoi(argv[1]);
    srand(seed);

    getCardSet(card_Stack, NUMCARDS);
    pthread_mutex_init(&newTurn, NULL);
    pthread_mutex_init(&newTurn2, NULL);
    pthread_mutex_init(&newTurn3, NULL);
    pthread_mutex_init(&newTurn1, NULL);
    pthread_mutex_init(&dumb2, NULL);
    pthread_mutex_init(&dumb, NULL);
    pthread_cond_init(&turnCond, NULL);
    pthread_cond_init(&turnCond2, NULL);
    pthread_cond_init(&turnCond3, NULL);
    pthread_cond_init(&turnCond1, NULL);
    pthread_cond_init(&stupid, NULL);
    pthread_cond_init(&stupid2, NULL);
    pthread_create(&actors[0], NULL, dealerRun, NULL);
    i = 0;
    for(;i<3;i++)
    {
        pthread_create(&actors[i+1], NULL, player_RUN,
                       (void *) (intptr_t) i+1);
    }
    i = 0;
    for(;i<4;i++)
    {
        pthread_join(actors[i],NULL);

    }
    pthread_mutex_destroy(&newTurn);
    pthread_mutex_destroy(&newTurn2);
    pthread_mutex_destroy(&newTurn3);
    pthread_mutex_destroy(&newTurn1);
    pthread_mutex_destroy(&dumb);
    pthread_mutex_destroy(&dumb2);
    pthread_cond_destroy(&turnCond);
    pthread_cond_destroy(&turnCond2);
    pthread_cond_destroy(&turnCond3);
    pthread_cond_destroy(&turnCond1);
    pthread_cond_destroy(&stupid);
    pthread_cond_destroy(&stupid2);
    return 0;
}
bool getCardSet(int * card_Stack, int size)
{
    int currentCard = 0;
    int i =0;
    int j = 0;
    for(; i < 4; i++)
    {
        j = 0;
        for(; j < 13; j++)
        {
            card_Stack[currentCard] = j;
            currentCard++;
        }
    }
    return false;
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void randomize ( int arr[], int n )
{
    int i = NUMCARDS-1;
    for (; i > 0; i--)
    {
        int j = rand() % (i+1);
        swap(&arr[i], &arr[j]);
    }
}
bool cardIsOut(int checkThisCard)
{
    int i = 0;
    for (; i < DEALTCARDS; i++)
    {
        if (dealt_Cards[i] == checkThisCard)
        {
            return true;
        }
    }
    return false;
}

int nextTurn(int i)
{
    return (i+1) % 4;
}

int pickAcard()
{
    int choosenCard = 0;
    bool tryAgain = true;
    while(tryAgain)
    {
        int j = rand() % 53;
        if(!cardIsOut(j))
        {
            choosenCard = j;
            tryAgain = false;
        }
    }
    return choosenCard;
}

void printExitPlayer(int turn)
{
    int one, two;
    fp = fopen("results.txt", "a");
    fprintf(fp, "PLAYER %d", turn);
    fprintf(fp, ": wins \n");
    fprintf(fp, "PLAYER %d", turn);
    fprintf(fp, ": exits round  \n");
    switch(turn)
    {
	case 1:
	   one = turn+1;
	   two = turn+2;
	   break;
	case 2:
	   one = turn -1;
	   two = turn + 1;
	   break;
	default:
	   one = turn -1;
	   two = turn -2;
	   break;
    }
    fprintf(fp, "PLAYER %d", one);
    fprintf(fp, ": exits round  \n");
    fprintf(fp, "PLAYER %d", two);
    fprintf(fp, ": exits round  \n");
    fclose(fp);	
}

