// main function for a linked list manager, student version

#include <stdio.h>
#include <string.h>
#include "hashmap.h"

int main(int argc, char *argv[]){
  int echo = 0;                                // controls echoing, 0: echo off, 1: echo on
  if(argc > 1 && strcmp("-echo",argv[1])==0) { // turn echoing on via -echo command line option
    echo=1;
  }

  printf("Hashmap Main\n");
  printf("Commands:\n");
  printf("  hashcode <key>   : prints out the numeric hash code for the given key (does not change the hash map)\n");
  printf("  put <key> <val>  : inserts the given key/val into the hash map, overwrites existing values if present\n");
  printf("  get <key>        : prints the value associated with the given key or NOT FOUND\n");
  printf("  print            : shows contents of the hashmap ordered by how they appear in the table\n");
  printf("  structure        : prints detailed structure of the hash map\n");
  printf("  clear            : reinitializes hash map to be empty with default size\n");
  printf("  save <file>      : writes the contents of the hash map the given file\n");
  printf("  load <file>      : clears the current hash map and loads the one in the given file\n");
  printf("  next_prime <int> : if <int> is prime, prints it, otherwise finds the next prime and prints it\n");
  printf("  expand           : expands memory size of hashmap to reduce its load factor\n");
  printf("  quit             : exit the program\n");
  
  char cmd[128], cmd2[128];          // Second buffer used for the put command
  hashmap_t hash;
  int success;
  hashmap_init(&hash, HASHMAP_DEFAULT_TABLE_SIZE);

  while(1){
    printf("HM> ");                   // print prompt
    success = fscanf(stdin,"%s",cmd); // read a command
    if(success==EOF){                 // check for end of input
      printf("\n");                   // found end of input
      break;                          // break from loop
    }

    if( strcmp("hashcode", cmd)==0 ){ // hashcode command  
      fscanf(stdin, "%s", cmd);
      if(echo){
        printf("hashcode %s\n", cmd);
      }
      
      long code = hashcode(cmd);
      printf("%ld\n", code);
    }

    else if( strcmp("put", cmd)==0 ){ // put command    
      fscanf(stdin, "%s %s", cmd, cmd2);
      if(echo){
        printf("put %s %s\n", cmd, cmd2);
      }

      success = hashmap_put(&hash, cmd, cmd2);
      if(!success){                   // check for success
        printf("Overwriting previous key/val\n");
      }
    }

    else if( strcmp("get", cmd)==0 ){  // get command   
      fscanf(stdin, "%s", cmd);
      if(echo){
        printf("get %s\n", cmd);
      }

      char *x = hashmap_get(&hash, cmd);
      if(x == NULL){                   // check for success
        printf("NOT FOUND\n");
      } else {
        printf("FOUND: %s\n", x);
      }
    }

    else if( strcmp("print", cmd)==0 ){ // print command
      if(echo){
        printf("print\n");
      }
      hashmap_write_items(&hash, stdout);
    }

    else if( strcmp("structure", cmd)==0 ){ // structure command    
      if(echo){
        printf("structure\n");
      }
      hashmap_show_structure(&hash);
    }

    else if( strcmp("clear", cmd)==0 ){  // clear command   
      if(echo){
        printf("clear\n");
      }
      hashmap_free_table(&hash);
      hashmap_init(&hash, HASHMAP_DEFAULT_TABLE_SIZE);
    }

    else if( strcmp("save", cmd)==0 ){  // save command 
      fscanf(stdin, "%s", cmd);
      if(echo){
        printf("save %s\n", cmd);
      }
      hashmap_save(&hash, cmd);
    }

    else if( strcmp("load", cmd)==0 ){  // load command  
      fscanf(stdin, "%s", cmd);
      if(echo){
        printf("load %s\n", cmd);
      }
      success = hashmap_load(&hash, cmd);
      if(!success){                      // check for success
        printf("load failed\n");
      }
    }

    else if( strcmp("next_prime", cmd)==0 ){ // next_prime command  
      int x;
      fscanf(stdin, "%d", &x);
      if(echo){
        printf("next_prime %d\n", x);
      }
      printf("%d\n", next_prime(x));
    }

    else if( strcmp("expand", cmd)==0 ){  // expand command 
      if(echo){
        printf("expand\n");
      }
      hashmap_expand(&hash);
    }

    else if( strcmp("quit", cmd)==0 ){ // quit command
      if(echo){
        printf("quit\n");
      }
      break;                          // break from loop
    }
  }  
  hashmap_free_table(&hash);          // clean up hash
  return 0;
}
