#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include <conio.h>


/* this function will count how many words in a text file*/
int list_Words_number(char * filename)
{
    int i;
    char str[30];
    FILE * fp = fopen(filename, "r");
    for (i = 0; fgets(str, 30, fp) != NULL ; i++) {}
    fclose(fp);
    return i;
}

/* this function will return the number if was 1 or 2 and will return 0 if the number was not 1 or 2*/
int choice_number(int x)
{
    if (x != 2 && x != 1)
        return 0;
    return x;
}

/*this function will make sure if the input was a number and will return 0 if it was not a number*/
int scan_a_number(int x)
{
    fflush(stdin); // to clear stdin before getting new input
    if (scanf("%d", & x) == 0)
        return 0;
    return x;
}

/*this function will measure the minutes elapsed*/
int time_min(time_t s, time_t e)
{
    int T =(e - s);
    int min = T / 60;
    return min;
}

/*this function will measure the seconds elapsed*/
int time_sec(time_t s, time_t e)
{
    int T = e - s;
    int sec = T % 60;
    return sec;
}


int main()
{
    //first we initialize the replay parameter Which controls the game replay
    int Re_play = 1;
    while (Re_play == 1)
    {
        int Level_choice = 0; // to store user choice//

        /* we will keep asking for a value until the user enter 1 or 2 which are the level choices*/
        while (Level_choice == 0)
        {
            printf("Welcome to Hangman game\n please choose your level:-\n 1- Easy \n 2- Hard\n");
            printf("\nPlease enter a valid choice number: ");
            Level_choice = scan_a_number(Level_choice);
            // if the function returned zero that means the entered value was zero or character
            Level_choice = choice_number(Level_choice);
            // if the function returned zero that means the entered value was not 1 or 2 so we keep doing the loop
            system("cls");
            // to clear the screen
        }

        fflush(stdin); // to clear stdin before getting the guessed letters
        int NUM;   // this is the maximum value of wrong guessed letter
        char * filename; // to store the name of the file
        if (Level_choice == 1)
        {
            filename = "Easy_words_list.txt";
            NUM = 15;  // set the maximum value of wrong guessed letters to 15 in case of easy level
        }
        else
        {
            filename = "Hard_words_list.txt";
            NUM = 10;  // set the maximum value of wrong guessed letters to 15 in case of easy level
        }
        FILE * fp = fopen(filename, "r"); // opening the file
        if (fp == NULL)
        {
            printf("Failed To Open the File\n"); // in case the file did not open
            return 1;
        }

        int List_col = list_Words_number(filename);  // to store how many words in the file
        int arr[List_col];
        //this array will store the number of letters in each word

        char str[64];
        // to store each word form the file and 64 is the maximum number of letters in each word in the file
        int I = 0;
        for (I = 0; fgets(str, 64, fp) != NULL ; I++)
        {
            arr[I] = strlen(str);
        }
        fclose(fp); // closing the file

        char ** words_list = (char ** ) malloc(List_col * sizeof(char * ));
        words_list[0] = (char * ) malloc(List_col * arr[0] * sizeof(char));

        for (I = 1; I < List_col; ++I)
            words_list[I] = words_list[I - 1] + arr[I - 1];
        //we will use dynamic memory handling for the 2D array

        fp = fopen(filename, "r"); // opening the file to read words
        if (fp == NULL)
        {
            printf("Failed To Open the File\n"); // if file failed to open
            return 1;
        }

        for (I = 0; fgets(str, 64, fp) != NULL ; I++) //str is the string which store letter in each word
        {
            sscanf(str, "%s", words_list[I]);
            //scanning the letters form the string
        }

        fclose(fp); //closing the file

        time_t T_s = time(NULL);  // we start counting the time
        srand(time(NULL));
        // to change the seed every time we open the program
        int random_number = rand() % List_col;
        // to choose a number between [ 0 and List_col -1 ]

        int wrong_letters = 0;    // to store number of wrong letters
        int Correct_letters = 0;  // to store number of correct letters
        int pre_correct_letters = 0; // to store number of previous correct letters
        int letter_entereded_already = 0;  // to detect if the letter entered already
        int exit = 0;
        // to know if the user want to give up the program will set it to 1
        size_t Word_length = strlen(words_list[random_number]);
        int word_letters[Word_length];

        for (I = 0; I < Word_length; I++)
            word_letters[I] = 0;
        // we will set all letters to 0 because later we will set correct guessed letters to 1

        char letters_entered[50];
        char gussed_letter; // to store the guessed lettter
        while (Correct_letters < Word_length)
            // if the user did't get all the letters correct then continue looping
        {
            printf("\nThe word is: ");
            for (I = 0; I < Word_length; I++)
            {
                if (word_letters[I] == 1)
                    printf("%c", words_list[random_number][I]);
                else
                    printf("-");
            }
            // to print the correct guessed letter and "-" if it is not guessed
            printf("\n Guess a letter or give up: ");
            fgets(letters_entered, 50, stdin);

            if (strncmp(letters_entered, "exit", 4) == 0 || strncmp(letters_entered, "give up", 7) == 0 || strncmp(letters_entered, "quit", 4) == 0)
            {
                exit = 1;
                break;
                // to get out from the while loop
            }
            // if the user enter exit, give up, or quit then we will get out of the while loop
            gussed_letter = letters_entered[0]; // to store the first letter in the string to guessed letter
            letter_entereded_already = 0;
            pre_correct_letters = Correct_letters;

            for (I = 0; I < Word_length; I++)
            {

                if (word_letters[I] == 1)
                {
                    if (words_list[random_number][I] == gussed_letter)
                        // if the user guessed a letter which was already guessed
                    {
                        letter_entereded_already = 1;
                        break; // exit from the for loop
                    }
                }

                if (gussed_letter == words_list[random_number][I])
                    // if the gussed letter was in the word
                {
                    word_letters[I] = 1;
                    Correct_letters++;
                }

            }

            if (pre_correct_letters == Correct_letters && letter_entereded_already == 0)
                // if the the correct letter did not increase and the user did not enter a letter guessed alredy
                // the the user did not guess it right so it counts a miss
            {
                wrong_letters++;
                printf("\nyou guessed a wrong letter! There are %d wrong attempts remaining \n", NUM - wrong_letters);
                if (wrong_letters == NUM)
                    //if the wrong letters equal the maximum wrong letters
                    break; // exit form while loop
            }
            else if (letter_entereded_already == 1)
                printf("\nThe letter has already been guessed! Try another one\n");
            else
                printf("\nyou guessed a correct letter!\n");
        }
        if (exit == 1)
        {
            printf("\nYou gave up! the word was: %s\n", words_list[random_number]);
        }
        else if (wrong_letters == NUM)
        {
            printf("\nGame over, the word was: %s\n", words_list[random_number]);
        }
        else
            printf("\nCONGRATOLATIONS YOU WON!!\n");

        time_t T_e = time(NULL);
        // store the end time

        int min = time_min(T_s, T_e);
        int sec = time_sec(T_s, T_e);
        printf("\nYout time is : %d min and %d sec \n", min, sec);

        free(words_list[0]);
        free(words_list);
        // To free the memory

        int K = 0;
        while (K == 0)
        {
            printf("\nDo you want to play again?\n 1- yes \n 2- No\n");
            printf("\nPlease enter a valid choice number: ");
            K = scan_a_number(K);
            K = choice_number(K);
            system("cls");
        }
        if (K == 1) Re_play = 1;
        //if the user chose 1 then he\she want to play again so keep loping
        else
        {
            Re_play = 0;
            // if Re_play set to 0 then it will get out from the replying while loop
            printf("\nThank you for playing Hangman\n");
        }
    }

    return 0;
}
