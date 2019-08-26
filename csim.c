/*
 *
 * csim.c
 *
 * Written by : julien piet
 * ID : julien.piet
 *
 */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cachelab.h"


/* Globals set by command line args */
int verbosity = 0; /* print trace if set */
int s = 0;         /* set index bits */
int b = 0;         /* block offset bits */
int E = 0;         /* associativity */
char* trace_file = NULL;
unsigned long time = 0;

/*
 * printUsage - Print usage info
 */
void printUsage(char* argv[])
{
    printf("Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n", argv[0]);
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n");
    printf("\nExamples:\n");
    printf("  linux>  %s -s 4 -E 1 -b 4 -t traces/yi.trace\n", argv[0]);
    printf("  linux>  %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n", argv[0]);
    exit(0);
}

typedef unsigned char byte;

typedef struct {
  byte v;
  unsigned long tag;
  unsigned long last_access;
} line;

typedef line *set;

set *initCache(){
  //Initializes cache
  set *cache = malloc((1<<s)*sizeof(set));
  for (int i = 0; i < 1<<s; i++){
    cache[i] = calloc(E, sizeof(line));
  }
  return cache;
}

/*
 * main - Main routine 
 */
int main(int argc, char* argv[])
{
  char c;
  
  int m = 0, e = 0, h = 0;
  
  while( (c=getopt(argc,argv,"s:E:b:t:vh")) != -1){
    switch(c){
    case 's':
      s = atoi(optarg);
      break;
    case 'E':
      E = atoi(optarg);
      break;
    case 'b':
      b = atoi(optarg);
      break;
    case 't':
      trace_file = optarg;
      break;
    case 'v':
      verbosity = 1;
      break;
    case 'h':
      printUsage(argv);
      exit(0);
    default:
      printUsage(argv);
      exit(1);
    }
  }

  /* Make sure that all required command line args were specified */
  if (s == 0 || E == 0 || b == 0 || trace_file == NULL) {
    printf("%s: Missing required command line argument\n", argv[0]);
    printUsage(argv);
    exit(1);
  }
  
  //open file
  
  FILE *trace = fopen(trace_file, "r");
  if(!trace){
    printf("Error while loading the file. Exiting");
    exit(0);
  }
  
  char line[256];
  
  //init mock cache;
  
  set *cache = initCache();
  
  unsigned long setMask = ((1<<s) - 1)<<b;
  unsigned long tagMask = (-1 - ((1<<(b+s))-1));

  while (fgets(line, sizeof(line), trace)) {
    
      if(line[0] == 'I') continue;
      
      time++;
      
      char type;
      unsigned long addr;
      
      sscanf(line, " %c %lx", &type, &addr);
          
      unsigned int setNumber = (setMask & addr)>>b;
      unsigned long tag = tagMask & addr;
    
      if(verbosity){
	printf("\nNew line. Set : %d; Tag : %lu; Content: %s", setNumber, tag, line);
      }

      if(!(type == 'M' || type == 'S' || type == 'L')) continue;
    
      //if the type is M, there will be a compulsory hit
      if(type == 'M') {
	h++;
	if (verbosity == 1){
	  printf("hit");
	}
      }
                      
      //these values will be used to store the possible location in cache using LRU for loading in case of a m
      int loadIndex = -1;
      int evictIndex = -1;
      unsigned long oldest = -1;
      int success = 0;

      //we have to look for the specified data              
      for (int i = 0; i < E; i++){
          if(cache[setNumber][i].v == 1 && cache[setNumber][i].tag == tag){
              h++;
	      if (verbosity == 1){
		printf("hit");
	      }
              cache[setNumber][i].last_access = time;
              success = 1;
	  }
          //If the line is not appropriate, update the possible locations for loading
          if(cache[setNumber][i].v == 0) loadIndex = i;
          else{
              if(cache[setNumber][i].last_access <= oldest){
                  oldest = cache[setNumber][i].last_access;
                  evictIndex = i;
              }
          }
      }

      if(success) continue;
      //if get to here, that means that we med
      m++;
      if (verbosity == 1){
	printf("miss");
      }
      //will now try to load form memory. If loadIndex is -1, we will have to evict.
      if(loadIndex == -1){
          loadIndex = evictIndex;
          e++;
	  if(verbosity == 1){
	    printf("evict");
	  }
      }

      cache[setNumber][loadIndex].v = 1;
      cache[setNumber][loadIndex].tag = tag;
      cache[setNumber][loadIndex].last_access = time;
  }

  fclose(trace);

  printSummary(h, m, e);
  return 0;

}
