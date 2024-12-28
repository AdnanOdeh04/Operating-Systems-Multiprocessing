#include <stdio.h>     // For printf, fprintf, fscanf, fopen, fclose
#include <stdlib.h>    // For calloc, free, exit
#include <string.h>    // For strcpy, strcmp, strcasecmp
#include <ctype.h>     // For character operations (not explicitly used but common in such programs)
#include <pthread.h>   //pthread_create, pthread_join, pthread_exit
#include <semaphore.h> // For semaphores
#define NUM_UNIQUE_WORDS 260000
#define MAX_WORDS_LENGTH 100
int childSize;
int TotalnumWords;
int numThreads;
sem_t sem;
struct Data
{
    char word[MAX_WORDS_LENGTH];
    int frequency;
    int noElementsFilled;
};
struct Data *finalData;
struct Data *allWords;
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
    while (fscanf(fp, "%s", word) != EOF)
    {
        count++;
    }
    fclose(fp);
    return count;
}
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
    while (fscanf(fp, "%s", word) != EOF)
    {
        strcpy(data[indexToAddWord].word, word);
        data[0].noElementsFilled++;
        indexToAddWord++;
    }
    fclose(fp);
}
int findWordIndex(struct Data *data, char *word, int index)
{
    for (int i = 0; i < index; ++i)
    {
        if (strcasecmp(data[i].word, word) == 0)
        {
            return i;
        }
    }
    return -1;
}
void addWordsToArray(struct Data *data, struct Data *allWordsD, int start, int end)
{
    for (int i = start; i < end; i++)
    {
        // int indexFound = findWordIndex(data, allWords[i].word, data[0].noElementsFilled);
        int indexFound = binarySearch(data, 0, data[0].noElementsFilled - 1, allWordsD[i].word);
        if (indexFound != -1)
        {
            data[indexFound].frequency++;
        }
        else
        {
            int c = data[0].noElementsFilled;
            strcpy(data[c].word, allWordsD[i].word);
            data[c].frequency = 1;
            data[0].noElementsFilled++;
        }
    }
}
void addNew(struct Data *data, char *word, int frequency)
{
    sem_wait(&sem);
    int indexFound = findWordIndex(data, word, data[0].noElementsFilled);
    if (indexFound != -1)
    {
        data[indexFound].frequency += frequency;
    }
    else
    {
        int c = data[0].noElementsFilled;
        strcpy(data[c].word, word);
        data[c].frequency = frequency;
        data[0].noElementsFilled++;
    }
    sem_post(&sem);
}
void *FunctionToDo(void *arg)
{
    int *num = (int *)arg;
    int start = (*num) * childSize;
    int end = 0;
    if ((*num) == (numThreads - 1))
    {
        end = TotalnumWords;
    }
    else
    {
        end = ((*num) + 1) * childSize;
    }
    while ((*num) == (numThreads - 1) && end < TotalnumWords)
    {
        end++;
    }
    struct Data *data = (struct Data *)calloc(childSize, sizeof(struct Data));
    addWordsToArray(data, allWords, start, end);
    for (int i = 0; i < data[0].noElementsFilled; i++)
    {
        addNew(finalData, data[i].word, data[i].frequency);
    }

    pthread_exit(0);
}
int main()
{
    TotalnumWords = numWords();
    allWords = (struct Data *)calloc(TotalnumWords, sizeof(struct Data));
    finalData = (struct Data *)calloc(NUM_UNIQUE_WORDS, sizeof(struct Data));
    sem_init(&sem, 0, 1);
    fillAllWords(allWords);
    //mergeSort(allWords, 0, TotalnumWords - 1);
    printf("Enter the number of threads to run: \n");
    scanf("%d", &numThreads);
    childSize = TotalnumWords / numThreads;
    pthread_t thread[numThreads];
    int numThread[numThreads];
    for (int i = 0; i < numThreads; i++)
    {
        //This function will create new thread, and pass function through it
        numThread[i] = i;
        pthread_create(&thread[i], NULL, FunctionToDo, (void *)&numThread[i]);
    }
    for (int i = 0; i < numThreads; i++)
    {
        //This will wait for the threads to finish 
        pthread_join(thread[i], NULL);
    }
    mergeSortN(finalData, 0, finalData[0].noElementsFilled - 1);
    for (int i = finalData[0].noElementsFilled - 1, c = 0; c < 10; c++, i--)
    {
        printf("%s - %d\n", finalData[i].word, finalData[i].frequency);
    }
}