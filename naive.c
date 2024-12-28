//These libraries used in the naive
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUM_UNIQUE_WORDS 260000
#define MAX_WORDS_LENGTH 100
//global variables
int childSize;
int TotalnumWords;
struct Data
{
    char word[MAX_WORDS_LENGTH];
    int frequency;
    int noElementsFilled;
};
//////////////merge sort for numbers///////////////////
void mergeN(struct Data *data, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // Create temp arrays
    struct Data *L = (struct Data *)calloc(n1, sizeof(struct Data));
    struct Data *R = (struct Data *)calloc(n2, sizeof(struct Data));

    // Copy data to temp arrays L[] and R[]
    for (i = 0; i < n1; i++)
    {
        strcpy(L[i].word, data[l + i].word);
        L[i].frequency = data[l + i].frequency; // FIXED INDEX
    }
    for (j = 0; j < n2; j++)
    {
        strcpy(R[j].word, data[m + 1 + j].word);    // FIXED OFFSET
        R[j].frequency = data[m + 1 + j].frequency; // FIXED INDEX
    }

    // Merge the temp arrays back into data[l..r]
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i].frequency <= R[j].frequency)
        {
            strcpy(data[k].word, L[i].word);
            data[k].frequency = L[i].frequency;
            i++;
        }
        else
        {
            strcpy(data[k].word, R[j].word);
            data[k].frequency = R[j].frequency;
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if any
    while (i < n1)
    {
        strcpy(data[k].word, L[i].word);
        data[k].frequency = L[i].frequency;
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if any
    while (j < n2)
    {
        strcpy(data[k].word, R[j].word);
        data[k].frequency = R[j].frequency;
        j++;
        k++;
    }

    // Free the temporary arrays
    free(L);
    free(R);
}

// Merge sort function
void mergeSortN(struct Data *data, int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;

        // Sort first and second halves
        mergeSortN(data, l, m);
        mergeSortN(data, m + 1, r);

        // Merge the sorted halves
        mergeN(data, l, m, r);
    }
}
//////////////////////////////////////////////////////////
//This merge sort to sort the numbers according to the word//
void merge(struct Data *data, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // Create temp arrays
    struct Data *L = (struct Data *)calloc(n1, sizeof(struct Data));
    struct Data *R = (struct Data *)calloc(n2, sizeof(struct Data));

    // Copy data to temp arrays L[] and R[]
    for (i = 0; i < n1; i++)
    {
        strcpy(L[i].word, data[l + i].word);
        L[i].frequency = data[l + i].frequency; // FIXED INDEX
    }
    for (j = 0; j < n2; j++)
    {
        strcpy(R[j].word, data[m + 1 + j].word);    // FIXED OFFSET
        R[j].frequency = data[m + 1 + j].frequency; // FIXED INDEX
    }

    // Merge the temp arrays back into data[l..r]
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (strcasecmp(L[i].word, R[j].word) <= 0)
        {
            strcpy(data[k].word, L[i].word);
            data[k].frequency = L[i].frequency;
            i++;
        }
        else
        {
            strcpy(data[k].word, R[j].word);
            data[k].frequency = R[j].frequency;
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if any
    while (i < n1)
    {
        strcpy(data[k].word, L[i].word);
        data[k].frequency = L[i].frequency;
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if any
    while (j < n2)
    {
        strcpy(data[k].word, R[j].word);
        data[k].frequency = R[j].frequency;
        j++;
        k++;
    }

    // Free the temporary arrays
    free(L);
    free(R);
}
//////////////////////////////////////////////////////////////////
//This function used only if mergeSort for the words in the array of struct used
int binarySearch(struct Data *data, int low, int high, char *word)
{
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        int cmp = strcasecmp(data[mid].word, word);
        // Check if x is present at mid
        if (cmp == 0)
            return mid;

        // If x greater, ignore left half
        if (cmp < 0)
            low = mid + 1;

        // If x is smaller, ignore right half
        else
            high = mid - 1;
    }
    // If we reach here, then element was not present
    return -1;
}
// Merge sort function
void mergeSort(struct Data *data, int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2;

        // Sort first and second halves
        mergeSort(data, l, m);
        mergeSort(data, m + 1, r);

        // Merge the sorted halves
        merge(data, l, m, r);
    }
}
/////////////////////////////////////////////////
//to find the number of non unique words
//will return a counter to that number
int numWords()
{
    int count = 0;
    char word[MAX_WORDS_LENGTH];
    FILE *fp = fopen("text8.txt", "r");
    if (fp == NULL)
    {
        printf("Error in opening the file!\n");
        exit(0);
    }
    while (fscanf(fp, "%s ", word) != EOF)
    {
        count++;
    }
    fclose(fp);
    return count;
}
///This function will fill all non unique words in the data array of struct
void fillAllWords(struct Data *data)
{
    int count = 0;
    FILE *fp = fopen("text8.txt", "r");
    if (fp == NULL)
    {
        printf("Error in opening the file!\n");
        exit(0);
    }
    int indexToAddWord = 0;
    char word[MAX_WORDS_LENGTH];
    //each word in the file will be stored in the array will frequency of 1
    while (fscanf(fp, "%s", word) != EOF)
    {
        strcpy(data[indexToAddWord].word, word);
        data[0].noElementsFilled++;
        indexToAddWord++;
    }
    fclose(fp);
}
//This function used linear search to search for a certain word 
//used when filter the frequent words
int findWordIndex(struct Data *data, char *word, int index)
{
    for (int i = 0; i < index; ++i)
    {
        //when find the words--> return the index of it
        if (strcasecmp(data[i].word, word) == 0)
        {
            return i;
        }
    }
    //if the word not found return -1
    return -1;
}
//This function will move all words to one array, 
//if the word is already exist then increment the frequency
//if the word is not exist add this word and initialize its frequency
void addWordsToArray(struct Data *data, struct Data *allWordsD, int start, int end)
{
    for (int i = start; i < end; i++)
    {
        //The process to find the word by the linear search 
        //int indexFound = binarySearch(data, 0, data[0].noElementsFilled, allWordsD[i].word);
        int indexFound = findWordIndex(data, allWordsD[i].word, data[0].noElementsFilled);
        //if the word exist then increment the frequency
        if (indexFound != -1)
        {
            data[indexFound].frequency++;
        }
        else
        {
            //if the word is not exist then initialize the word with initial frequency 
            int c = data[0].noElementsFilled;
            strcpy(data[c].word, allWordsD[i].word);
            data[c].frequency = 1;
            data[0].noElementsFilled++;
        }
    }
}
void addNew(struct Data *data, char *word, int frequency)
{
    int indexFound = findWordIndex(data, word, data[0].noElementsFilled);
    if (indexFound != -1)
    {
        data[indexFound].frequency++;
    }
    else
    {
        int c = data[0].noElementsFilled;
        strcpy(data[c].word, word);
        data[c].frequency = 1;
        data[0].noElementsFilled++;
    }
}
int main()
{
    //Here to find the number of all non-unique words
    TotalnumWords = numWords();
    struct Data *allWords = (struct Data *)calloc(TotalnumWords, sizeof(struct Data));
    struct Data *finalData = (struct Data *)calloc(NUM_UNIQUE_WORDS, sizeof(struct Data));
    //to put all non-unique words into allWords array of struct
    fillAllWords(allWords);
    //-----------------------------------------------------------------------------
    /* We can use mergeSort before the Organization of the data to speedup the execution
    mergeSort(allWords, 0, TotalnumWords - 1);
    // addWordsToArray(finalData, allWords, 0, TotalnumWords);
    */
   //-------------------------------------------------------------------------------
    //This loop and the function call inside in considered as a filter of the data
    //addNew Function take the word to be added to the (finalData) and test it 
    //if the word exist then it will increment the frequency --> if not 
    //it will initialize the word
    for (int i = 0; i < TotalnumWords; i++)
    {
        addNew(finalData, allWords[i].word, allWords[i].frequency);
    }
    //------------------------------------------------------------------------------
    //Here the data is ready with the frequent words, and to sort this data to 
    //get top 10 frequenct we used mergeSort for numbers
    mergeSortN(finalData, 0, finalData[0].noElementsFilled - 1);
    //print all sorted words with the top 10 most frequent
    for (int i = finalData[0].noElementsFilled - 1, c = 0; c < 10; c++, i--)
    {
        printf("%s - %d\n", finalData[i].word, finalData[i].frequency);
    }

    return 0;
}