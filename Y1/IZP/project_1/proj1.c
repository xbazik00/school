/* ******************************************************************************************************
 * File:          proj1.c                                                                               *
 * Date:          2015/10/30                                                                            *
 * Author:        Bazik Martin, xbazik00@stud.fit.vutbr.cz                                              *
 * Project:       Work with a text; project no. 1 for subject IZP                                       *
 * Description:   Program reads words from a text and implements functions for detection of numbers,    *
 *                detection of calendar date, test for a prime number, and detection of palindrome,     *
 *                Text input is given by standard input (stdin). File input is being read word by word  *
 *                in order of definition given by converse specificator %s of function scanf().         *
 ****************************************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>
#define MAXIMAL_LENGTH_OF_WORD 101

void palindrome(char str[],int numberOfC,int type);      //checks for palindrome
int tp(char str[],int numberOfC);                        //checks type of data
void prime(char str[],int type);                         //checks for prime number
void printTp (int type);                                 //prints out type of data
void date(char str[],int type);                          //check for correctness of a date and prints out day in a week
int wordMax(int numberOfC);                              //checks for maximal number of characters in a word
int unexpectedInput(char str[],int order);               //checks for unexpected input

int main(int argc,char *argv[])
{
    if (argc>1)                                          //if the program was run with arguments, it is shut down
    {
        printf("Name: proj1.c \nAuthor: Martin Bazik \nDescription: Program for detection of numbers, dates, words, palindromes and primes.\nMaximal length of a word is 100 characters and maximal value of prime is INT_MAX.\n");
        return 0;
    }

    (void) argv;
    char str[102];                                       //string input
    int numberOfC;                                       //number of characters
    int type;                                            //type of input
    int unexpected=0;                                    //value of unexpected input
    int lastChar;                                        //stores last written character
    int number;                                          //stores value of a number
    char *ptr;                                           //string part of strol

    //while (lastChar!='\n')
    while (lastChar!=EOF)                            //while last characters is not end of file the program proceeds to run given algorithm
    {
        scanf("%101s", str);
        lastChar = getchar();

        for(int j=0;j<=MAXIMAL_LENGTH_OF_WORD ;j++)                          //goes one by one throughout all of the characters of the given string
        {
            if (str[j]=='\0')                            //counts number of characters
            {
                numberOfC = j;
                break;
            }
            else                                         //looks for unexpected input
            {
                unexpected = unexpectedInput(str,j);
            }
        }
        if (unexpected==0&&lastChar!=EOF)                //if everything is fine, program gives output
        {
            type = tp(str,numberOfC-1);
            printTp(type);
            date(str,type);
            if (wordMax(numberOfC)==1)                   //if the word given was longer than 100 characters it will print out error
            {
                printf("(Your word is too long and remaining part will be shown at the following line.)");
            }
            else
            {
                if (type==2)                             //if it is a number
                {
                    number=strtol(str,&ptr,10);
                    printf("%d ",number);
                }
                else
                {
                    printf("%s ", str);
                    palindrome(str,numberOfC,type);
                }
            }
            prime(str,type);
        }
        else if (unexpected==1&&lastChar!=EOF)           //if there was an unexpected input and last character was not end of file it prints out error
        {
            printf("Invalid input");
        }
        printf("\n");//new line
    }
    return 0;
}

/*
Function palindrome goes through all the characters of the given word and checks if the character is different from
the character on the same place from opposite side of the word. If no then prints out "(palindrome)". At the start we assume taht word is a palindrome.
*/
void palindrome(char str[],int numberOfC,int type)
{
    int isPalindrome = 1;
    if (type == 0)
    {
        for (int g=0;g<(numberOfC/2);g++)
        {
            if(str[g]!=str[numberOfC-1-g])
            {
                isPalindrome = 0;
            }
        }
        if (isPalindrome == 1)
        {
            printf("(palindrome)");
        }
    }
}

/*
Function tp checks ,for the first, if the string is a date, if not then if it is a number, else it is supposed to be a word
outputs: 0 is word; 1 is date; 2 is number
*/
int tp(char str[],int numberOfC)
{
    int type=0;
    int pos = 0;
    int d;                                       //day
    int m;                                       //month
    int y;                                       //year
    char year[5]="";                             //string of year
    char mon[3]="";                              //string of month
    char day[3]="";                              //string of day
    char *ptr;
    if (numberOfC == 9 && (str[4]=='-'&&str[7]=='-'))
    {
        for (int l=0;l<=3;l++)
        {
            if(str[l]>='0' && str[l]<='9')
            {
                type=1;
                year[l]= str[l];
            }
            else {return 0;}
        }
        y = strtol(year,&ptr,10);
        for (int l=5;l<=6;l++)
        {
            if(str[l]>='0' && str[l]<='9')
            {
                type=1;
                mon[pos++]= str[l];
            }
            else {return 0;}
        }
        m = strtol(mon,&ptr,10);
        for (int l=8;l<=9;l++)
        {
            if(str[l]>='0' && str[l]<='9')
            {
                type=1;
                day[pos++]= str[l];
            }
            else {return 0;}
        }
        d = strtol(day,&ptr,10);
    }
    if (type==1)
    {
        struct tm time_info;

        time_info.tm_year = y-1900;
        time_info.tm_mon = m-1;
        time_info.tm_mday = d;
        time_info.tm_hour = 0;
        time_info.tm_min =0;
        time_info.tm_sec = 0;
        time_info.tm_isdst = 0;
        if (mktime(&time_info)>-1)
        {
            type = 1;
        }
        else type = 0;
    return type;
    }
    else
    {
        for (int l=0;l<=numberOfC;l++)
        {
            if(str[l]>='0' && str[l]<='9')
            {
                type=2;
            }
            else{return 0;}
        }
        return type;
    }
}

/*
Function wordMax checks for a maximal length of a word
*/
int wordMax (int numberOfC)
{
    if (numberOfC==MAXIMAL_LENGTH_OF_WORD)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
Function printTp prints out input type
*/
void printTp (int type)
{
    switch (type)
    {
        case 0: printf("word: ");
                break;
        case 1: printf("date: ");
                break;
        case 2: printf("number: ");
                break;
    }
}

/*
Function prime checks for a prime number using simple algorithm by looking for a composited number by continuous division by numbers
greater than 2 and smaller than a given number. Output is a standard text output.
*/
void prime(char str[],int type)
{
    if (type==2)
    {
        long number;
        char *ptr;                                        //string to store non-numeral characters of a string;should not appear
        number = strtoll(str,&ptr,10);
        if(number<=INT_MAX)                               //checks only if number is smaller than upper limit for integer
        {
            int isPrime=0;
            int di = 2;                                   //divisor
            while (di<number)
            {
                if ((number%di)==0)
                {
                    isPrime=0;
                    break;
                }
                else {isPrime=1;di++;}
            }
            if (isPrime==1 || number==2)
            {
                printf("(prime)");
            }
        }
    }
}

/*
Function unexpectedInput checks for characters out of printable ASCII table range.
If it is not character out of range it returns 0 else 1.
*/
int unexpectedInput(char str[],int order)
{
    if (isprint(str[order]))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/*
Function date checks for correctness of a date and prints out day in a week.
It uses function mkTime from library time.h.
If date cannot be calculated by function mkTime it prints out error.
*/
void date(char str[],int type)
{
    if (type==1)
    {
        int pos = 0;
        int d;                                       //day
        int m;                                       //month
        int y;                                       //year
        char year[5]="";                             //string of year
        char mon[3]="";                              //string of month
        char day[3]="";                              //string of day
        char *ptr;                                   //string to store non-numeral characters of a string;should not appear

        for(int g=0;g<=3;g++)                        //assigns year
        {
            if (str[g]>='0' && str[g] <='9')
            {
                year[g]= str[g];
            }
        }
        y = strtol(year,&ptr,10);

        for(int g=5;g<=6;g++)                         //assigns month
        {
            if (str[g]>='0' && str[g] <='9')
            {
                mon[pos++]= str[g];
            }
        }
        m = strtol(mon,&ptr,10);

        pos=0;
        for(int g=8;g<=9;g++)                         //assigns day
        {
            if (str[g]>='0' && str[g] <='9')
            {
                day[pos++]= str[g];
            }
        }
        d = strtol(day,&ptr,10);

        struct tm time_info;

        time_info.tm_year = y-1900;
        time_info.tm_mon = m-1;
        time_info.tm_mday = d;
        time_info.tm_hour = 0;
        time_info.tm_min =0;
        time_info.tm_sec = 0;
        time_info.tm_isdst = 0;
        if (mktime(&time_info)>-1)
        {
            switch ( time_info.tm_wday )
            {
                case 0: printf("Sun ");
                        break;
                case 1: printf("Mon ");
                        break;
                case 2: printf("Tue ");
                        break;
                case 3: printf("Wed ");
                        break;
                case 4: printf("Thu ");
                        break;
                case 5: printf("Fri ");
                        break;
                case 6: printf("Sat ");
                        break;
                default: break;
            }
        }
        else printf("(Incorrect date) ");
    }
}

