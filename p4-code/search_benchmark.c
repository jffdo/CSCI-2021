// Complete this main to benchmark the search algorithms outlined in
// the project specification

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "search.h"

int main(int argc, char *argv[]){
if(argc < 4){
  printf("usage: ./search_benchmark <minpow> <maxpow> <repeats> [alg1 alg2 ...]\n");
  printf("with alg1 alg2 as a combination of:\n");
  printf("la : Linear Array Search\n");
  printf("ll : Linked List Search\n");
  printf("ba : Binary Array Search\n");
  printf("bt : Binary Tree Search\n");
  printf("(default all)\n");
  return 1;
}

  int minpow = atoi(argv[1]);
  int maxpow = atoi(argv[2]);
  int repeats = atoi(argv[3]);

  int search_size = 1;

  for(int i = 0; i < minpow; i++){ // 2^<minpow>
    search_size *= 2;
  }

  double time_la = 0;
  double time_ll = 0;
  double time_ba = 0;
  double time_bt = 0;

  clock_t begin, end;
  double cpu_time;

  int do_la = 0;
  int do_ll = 0;
  int do_ba = 0;
  int do_bt = 0;

  if(argc > 4){ // Arguments greater than 4 allow selection of searches
    for(int i = 4; i < argc; i++){
      if(strcmp(argv[i], "la") == 0){
        do_la = 1;
      }
      if(strcmp(argv[i], "ll") == 0){
        do_ll = 1;
      }
      if(strcmp(argv[i], "ba") == 0){
        do_ba = 1;
      }
      if(strcmp(argv[i], "bt") == 0){
        do_bt = 1;
      }
    }
  } else { // Defaults to all
    do_la = 1;
    do_ll = 1;
    do_ba = 1;
    do_bt = 1;
  }
  
  printf("%s %s ", "LENGTH", "SEARCHES"); // Table formatting

  if(do_la){
    printf("%10s ", "array");
  }
  if(do_ll){
    printf("%10s ", "list");
  }
  if(do_ba){
    printf("%10s ", "binary");
  }
  if(do_bt){
    printf("%10s ", "tree");
  }
  printf("\n");

  for(int j = minpow; j <= maxpow; j++){ // 2^<minpow> to 2^<maxpow>
    printf("%6d %8d ",  search_size, 2*search_size);
    if(do_la){ // Linear array
      int* arr = make_evens_array(search_size);
      for(int i = 0; i < repeats; i++){ // repeat <reapeats> times
        begin = clock();
        for(int k = 0; k < (2 * search_size) - 1; k++){
          linear_array_search(arr, search_size, k);
        }
        end = clock();
        cpu_time = ((double) (end - begin)) / CLOCKS_PER_SEC;
        time_la += cpu_time;
      }
      free(arr);
      printf("%10.4e ", time_la);
      time_la = 0;
    }
    
    if(do_ll){ // Linked List
      list_t* list= make_evens_list(search_size);
      for(int i = 0; i < repeats; i++){ // repeat <reapeats> times
        begin = clock();
        for(int k = 0; k < (2 * search_size) - 1; k++){
          linkedlist_search(list, search_size, k);
        }
        end = clock();
        cpu_time = ((double) (end - begin)) / CLOCKS_PER_SEC;
        time_ll += cpu_time;
      }
      list_free(list);
      printf("%10.4e ", time_ll);
      time_ll = 0;
    }

    if(do_ba){ // Binary Array
      int* arr = make_evens_array(search_size);
      for(int i = 0; i < repeats; i++){ // repeat <reapeats> times
        begin = clock();
        for(int k = 0; k < (2 * search_size) - 1; k++){
          binary_array_search(arr, search_size, k);
        }
        end = clock();
        cpu_time = ((double) (end - begin)) / CLOCKS_PER_SEC;
        time_ba += cpu_time;
      }
      free(arr);
      printf("%10.4e ", time_ba);
      time_ba = 0;
    }

    if(do_bt){ // Bianary Tree
      bst_t* bst = make_evens_tree(search_size);
      for(int i = 0; i < repeats; i++){ // repeat <reapeats> times
        begin = clock();
        for(int k = 0; k < (2 * search_size) - 1; k++){
          binary_tree_search(bst, search_size, k);
        }
        end = clock();
        cpu_time = ((double) (end - begin)) / CLOCKS_PER_SEC;
        time_bt += cpu_time;
      }
      bst_free(bst);
      printf("%10.4e ", time_bt);
      time_bt = 0;
    }
    search_size *= 2;
    printf("\n"); 
  }  
  return 0;
}

