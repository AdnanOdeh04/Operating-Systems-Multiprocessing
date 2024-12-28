#include <stdio.h>     // For printf, fprintf, fscanf, fopen, fclose
#include <stdlib.h>    // For calloc, free, exit
#include <string.h>    // For strcpy, strcmp, strcasecmp
#include <ctype.h>     // For character operations (not explicitly used but common in such programs)
#include <sys/types.h> // For pid_t
#include <unistd.h>    // For fork, exec, and process operations
#include <sys/wait.h>  // For wait
#include <sys/mman.h>  // For memory mapping with mmap
#include <semaphore.h> // For semaphores

#define sizeData 260000  //This size for the unique words
#define MAX_WORD_LENGTH 100 //This size for the word

struct Data
{
  char word[MAX_WORD_LENGTH];
  int frequency;
  int noElementsFilled; 
};
//This merge sort to sort the array according to words
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
//to find the number of non frequent words
int numWords()
{
  int count = 0;
  char word[MAX_WORD_LENGTH];
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
//to fill all the words in the non frequent words
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
  char word[MAX_WORD_LENGTH];
  while (fscanf(fp, "%s", word) != EOF)
  {
    strcpy(data[indexToAddWord].word, word);
    data[0].noElementsFilled++;
    indexToAddWord++;
  }
  fclose(fp);
}

void addWordsToArray(struct Data *data, struct Data *allWordsD, int start, int end)
{
  for (int i = start; i < end; i++)
  {
    //To search for a unique word linearly
    int indexFound = findWordIndex(data, allWordsD[i].word, data[0].noElementsFilled - 1);
    // int indexFound = binarySearch(data, 0, data[0].noElementsFilled, allWordsD[i].word);
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
//This is the inialization of the semaphore for synchronization
sem_t sem;
void addNew(struct Data *data, char *word, int frequency)
{
  //to synchronize the words in the waiting
  sem_wait(&sem);
  // int indexFound = binarySearch(data, 0, data[0].noElementsFilled, word);
  int indexFound = findWordIndex(data, word, data[0].noElementsFilled);
  if (indexFound != -1)
  {
    data[indexFound].frequency += frequency;
  }
  else
  {
    int c = data[0].noElementsFilled;
    data[c].frequency = frequency;
    strcpy(data[c].word, word);
    data[0].noElementsFilled++;
  }
  //if the process finished, then leave the lock to another process
  sem_post(&sem);
}
int main(int numPorcess)
{
  //to find the number of total non unique words
  int TotalnumWords = numWords();
  //initialize the semaphore
  sem_init(&sem, 0, 1);
  //initialize the array of struct
  struct Data *allWords = (struct Data *)calloc(TotalnumWords, sizeof(struct Data));
  // struct Data *finalData = (struct Data *)calloc(sizeData, sizeof(struct Data));
  struct Data *finalData = mmap(NULL, sizeData * sizeof(struct Data), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  fillAllWords(allWords);
  //if this merge sort used the binary search must be used in addWords, and that will optimize the time
  //
  // mergeSort(allWords, 0, TotalnumWords - 1);
  //
  //to divide the array of the TotalnumWords according to the number of processes
  int childSize = TotalnumWords / numProcess;
  char nameFile[numProcess][MAX_WORD_LENGTH];
  //This loop for each process made
  for (int i = 0; i < numProcess; i++)
  {
    //to create new process
    pid_t pid = fork();
    //child process
    if (pid == -1)
    {
      printf("Error in opening new process\n");
      exit(0);
    }
    else if (pid == 0)
    {
      //to find the start index, and the end index
      int start = i * childSize;
      int end = 0;
      if (i == (numProcess - 1))
      {
        end = TotalnumWords;
      }
      else
      {
        end = (i + 1) * childSize;
      }
      while (i == (numProcess - 1) && end < TotalnumWords)
      {
        end++;
      }
      struct Data *data = (struct Data *)calloc(childSize, sizeof(struct Data));
      //to move each part of the total array to different data
      addWordsToArray(data, allWords, start, end);
      //to move through all elements in the total array for each part of it
      for (int i = 0; i < data[0].noElementsFilled; i++)
      {
        addNew(finalData, data[i].word, data[i].frequency);
      }
      exit(0);
    }
  }
  for (int i = 0; i < numProcess; i++)
  {
    //to wait for the child process to finish
    wait(NULL);
  }
  //This will sort the final unique word to print the top 10 freuquent word
  mergeSortN(finalData, 0, finalData[0].noElementsFilled - 1);
  for (int i = finalData[0].noElementsFilled - 1, c = 0; c < 10; c++, i--)
  {
    printf("%s - %d\n", finalData[i].word, finalData[i].frequency);
  }
  return 0;
}