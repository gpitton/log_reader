#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// maximum length of lines in log file. Change this value if needed
#define MAX_STRING_LENGTH 256


// linked list to store cluster nodes' names
struct strarray
{
    char name[MAX_STRING_LENGTH];
    FILE *fptr;
    struct strarray *next;
};

// function to check if a given string is already present in the list
// of known nodes. If not, appends it. In both cases, returns the 
// id of the node (an integer)
struct strarray* check_and_update(struct strarray* nnames, char [MAX_STRING_LENGTH]);

int main(int argc, char** args)
{
  // maximum length of lines in log file. Change this value if needed
  const size_t          lsize = MAX_STRING_LENGTH;
  char                  date[lsize],
                        time[lsize],
                        node_name[lsize],
                        target_name[lsize],
                        message[lsize],
                        file_name[lsize],
                        output_file[lsize],
                        line[lsize];
  // struct to store the names of all the cluster's nodes
  struct strarray       *nnames;

  
  // read input parameters (3 expected)
  if (argc < 1)
  {
      printf("error: too few parameters.\n");
      printf("usage: lread --file /path/to/log/file --node cx1-node-name -- output parsed.txt\n");
      exit(EXIT_FAILURE);
  }

  for (int i=0; i<argc/2; i++)
  {
      if (strcmp(args[2*i+1], "--file") == 0)
      {
          strcpy(file_name, args[2*i+2]);
      }
      else if (strcmp(args[2*i+1], "--node") == 0)
      {
          strcpy(target_name, args[2*i+2]);
      }
      else if (strcmp(args[2*i+1], "--output") == 0)
      {
          strcpy(output_file, args[2*i+2]);
      }
      else if (strcmp(args[2*i+1], "--help") == 0)
      {
          printf("usage: lread --file /path/to/log/file --node cx1-node-name --output parsed.txt\n");
          printf("or, to get the output from all the nodes:\n");
          printf("usage: lread --file /path/to/log/file --node all\n");
          exit(EXIT_SUCCESS);
      }
  }

  printf("reading file: %s\n", file_name);
  FILE *fptr = fopen(file_name, "r");

  if (fptr == NULL)
  {
      printf("error: unable to open file %s\n", file_name);
      exit(EXIT_FAILURE);
  }

  int           line_count = 0,
                local_count = 0;
  
  // first case: get the output from all the nodes
  if (strcmp(target_name, "all") == 0)
  {
  // initialize nnames struct
  nnames = malloc(sizeof(struct strarray));

  // read log file line by line, until end of file is reached
  while (fgets(line, lsize, fptr) != NULL)
  {
      line_count++;
      sscanf(line, "%s %s %s\t%s", date, time, node_name, message);
      if ((date[0] == '2') && (date[1] == '0'))
      {
          // retrieve the handle (a number) associated to the node name.
          // if there is no handle, create one.
          struct strarray *cursor = check_and_update(nnames, node_name);
          if (cursor->fptr != NULL)
          {
              fprintf(cursor->fptr, "%s\n", message);
          }
      }
  }
  // now close all files
  /*
  int i = 0;
  do
  {
      fclose(nnames[i].fptr);
      i++;
  } while (nnames[i-1].next != NULL);
  */

  }
  // second case: get the output only from a specified node
  else
  {
  FILE *optr = fopen(output_file, "w");
  // read log file line by line, until end of file is reached
  while (fgets(line, lsize, fptr) != NULL)
  {
      line_count++;
      sscanf(line, "%s %s %s\t%s", date, time, node_name, message);
      if ((date[0] == '2') && (date[1] == '0'))
      {
          if (strcmp(target_name, node_name) == 0)
          {
              local_count++;
              fprintf(optr, "%s\n", message);
          }
      }
  }
  fclose(optr);
  }

  printf("read %d lines, of which %d containing messages from\n node named %s\n", line_count, local_count, target_name);

  fclose(fptr);

  return 0;
}


// function implementation
struct strarray* check_and_update(struct strarray* node_names, char current_node[MAX_STRING_LENGTH])
{
    int i = 0, match = 0;
    struct strarray *cursor = node_names;
    struct strarray *lcursor;

    do
    {
        //printf("%s %s\n", node_names[i].name, current_node);
        // if some old node matches, return the node id
        if (strcmp(cursor->name, current_node) == 0)
        {
            match = 1;
            break;
        }
        i++;
        lcursor = cursor;
        cursor = cursor->next;
    } while (cursor != NULL);

    // if no node matches, then allocate a new block in the linked list
    i--;
    if (match == 0)
    {
        lcursor->next = malloc(sizeof(struct strarray));
        strcpy(lcursor->name, current_node);
        lcursor->fptr = fopen(current_node, "w");
    }

    return lcursor;
}
