#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(int argc, char** args)
{
  // maximum length of lines in log file. Change this value if needed
  const size_t          lsize = 256;
  char                  date[lsize],
                        time[lsize],
                        node_name[lsize],
                        target_name[lsize],
                        message[lsize],
                        file_name[lsize],
                        output_file[lsize],
                        line[lsize];

  //for (int i=0; i<argc;i++)
  //    printf("%s\n", args[i]);
  
  // read input parameters (2 expected)
  if (argc < 1)
  {
      printf("error: too few parameters.\n");
      printf("usage: lread --file /path/to/log/file --node cx1-node-name -- output parsed.txt\n");
      exit(EXIT_FAILURE);
  }

  for (int i=0; i<3; i++)
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
          exit(EXIT_SUCCESS);
      }
  }

  printf("reading file: %s\n", file_name);
  FILE *fptr = fopen(file_name, "r");
  FILE *optr = fopen(output_file, "w");

  if (fptr == NULL)
  {
      printf("error: unable to open file %s\n", file_name);
      exit(EXIT_FAILURE);
  }

  // read log file line by line, until end of file is reached
  int           line_count = 0,
                local_count = 0;
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
          //printf("%s %s %s\n", date, node_name, message);
      }
  }

  printf("read %d lines, of which %d containing messages from\n node named %s\n", line_count, local_count, target_name);

  fclose(optr);
  fclose(fptr);

  return 0;
}
