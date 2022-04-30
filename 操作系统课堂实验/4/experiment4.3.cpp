#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 6

int partition(int arr[], int low, int high){
    int key;
    key = arr[low];
    while(low<high){
        while(low <high && arr[high]>= key )
            high--;
        if(low<high)
            arr[low++] = arr[high];
        while( low<high && arr[low]<=key )
            low++;
        if(low<high)
            arr[high--] = arr[low];
    }
    arr[low] = key;

    return low;
}

void quick_sort(int arr[], int start, int end){
    int pos;
    if (start<end){
        pos = partition(arr, start, end);
        quick_sort(arr,start,pos-1);
        quick_sort(arr,pos+1,end);
    }

    return;
}

int main(void){
    int i;
    int arr[N]={32,12,7, 78, 23,45};
    int times = 1000000000;
    clock_t start, finish;
    printf("Before sorting \n");
    for(i=0;i<N;i++)
        printf("%d\t",arr[i]);
    start = clock();
    for (int i = 0 ; i < times ; i++)
        quick_sort(arr,0,N-1);
    finish = clock();
    printf("\n After sorting \n");
    for(i=0; i<N; i++)
        printf("%d\t", arr[i]);
    printf ("Time Cost:%f\n", (double)(finish - start) / CLOCKS_PER_SEC);

    return 0;
}