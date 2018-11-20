#include <iostream>
#include <fstream>
#include <pthread.h>
#include <ctime>

using namespace std;
int* a;
ofstream writefile;

struct params {
    int low, high;
};
void mergePartitions(int low, int mid, int high) {
    int i =low, j = mid + 1, k = 0;
    int temp[high-low+1];
    //sort elements resulted from both threads in temp array
    while (i <= mid && j <= high) {
        if (a[i] > a[j]){
            temp[k++] = a[j++];
        } else {
            temp[k++] = a[i++];
        }
    }
    //loop on remaining -if any- elements of sub array 1 add them to temp
    while (i <= mid) {
        temp[k++] = a[i++];
    }
    //loop on remaining -if any- elements of sub array 2 add them to temp
     while (j <= high) {
        temp[k++] = a[j++];
    }
    //return he sorted elements back in original array
    for (i = 0; i < k; i++) {
        a[low++] = temp[i];
    }
}

void* mergeSort(void* param) {
    //extract parameers from the thread
    struct params* args = (struct params*) param;
    int low = args ->low;
    int high = args ->high;
    int mid = (high + low)/2;

    //if only one element in thread exit thread
    if (low == high) {
        pthread_exit(0);
    }
    //create 2 threads for each half of array and initialize their parameters
    pthread_t thread1, thread2;
    struct params p1, p2;
    p1.low = low;
    p1.high = mid;

    p2.low = mid+1;
    p2.high = high;

    //create thread and join
    int status1 = pthread_create(&thread1, NULL, mergeSort, (void*) &p1);
    if (status1 > 0) {
        cout << "Failed to Create First thread" << endl;
    }
    int status2 = pthread_create(&thread2, NULL, mergeSort, (void*) &p2);
    if (status2 > 0) {
        cout << "Failed to Create Second thread" << endl;
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    //merge the 2 sorted parts of each thread.
    mergePartitions(low, mid, high);
}
int* readInput(int &arraySize) {
    ifstream readfile;
    readfile.open("input.txt");
    //check that file has opened successfully
    if (!readfile.is_open()) {
        cout << "File Is Not Opened" << endl;
    }
    //Read array
    readfile >> arraySize;
    a = new int[arraySize];
    for (int i = 0; i < arraySize; i++) {
        readfile >> a[i];
    }
    //close file
    readfile.close();
    return a;
}
void start() {
        //Read array
    int arraySize;
    a = readInput(arraySize);

    //create new parameters of type struct
    struct params param;
    //set values to whole array size
    param.high = arraySize - 1;
    param.low = 0;

    //create firt thread that calls merge sort on whole array
    pthread_t firstThread;
    clock_t exeTime =clock();
    //create thread with passing method and its parameters
    int status = pthread_create(&firstThread, NULL, mergeSort, &param);

    if (status > 0) {
        cout << "Failed to Create Main thread" << endl;
    }
    //Join thread so that main thread waits for its execution and compeletion
    pthread_join(firstThread, NULL);
    //print array and time of execution
    exeTime = clock() - exeTime;
    for(int i = 0; i < arraySize; i++) {
        cout << a[i] << " ";
    }
    cout<< endl << "Time elapsed : " << (float)exeTime/CLOCKS_PER_SEC << "  Seconds"<<endl;
}
int main()
{
    start();

    return 0;
}
