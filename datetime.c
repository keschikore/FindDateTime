#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//------------------------------------------------------------------------------------
// Returns 1 if the given string contains a valid date and time value.
// Otherwise returns 0.
// 
// Date and Time Format:
//     YYYY-MM-DDThh:mm:ssTZD
//
// Where:
//
//     YYYY = four-digit year
//     MM   = two-digit month (01 through 12)
//     DD   = two-digit day of month (01 through 31)
//     hh   = two digits of hour (00 through 23)
//     mm   = two digits of minute (00 through 59)
//     ss   = two digits of second (00 through 59)
//     TZD  = time zone designator (Z or +hh:mm or -hh:mm)
//------------------------------------------------------------------------------------
int isDateTime(const char* s, int len)
{
  // Check for YYYY-MM-DDThh:mm:ssZ
  if (len != 20 &&         // YYYY-MM-DDThh:mm:ssZ
      len != 25)           // YYYY-MM-DDThh:mm:ss+hh:mm
    {
      return 0;
    }

  // DDDD (0000 through 9999)
  if (!isdigit(s[0]) || !isdigit(s[1]) || !isdigit(s[2]) || !isdigit(s[3]))
    {
      return 0;
    }

  // -
  if (s[4] != '-') return 0;

  // MM (01 through 23)
  if (!((s[5] == '0' && '1' <= s[6] && s[6] <= '9') ||   // 01-09
        (s[5] == '1' && '0' <= s[6] && s[6] <= '9') ||   // 10-19
        (s[5] == '2' && '0' <= s[6] && s[6] <= '3')))    // 20-23
    {
      return 0;
    }

  // -
  if (s[7] != '-') return 0;

  // DD (01 through 31)
  if (!((s[8] == '0' && '1' <= s[9] && s[9] <= '9') ||   // 01-09
        (s[8] == '1' && '0' <= s[9] && s[9] <= '9') ||   // 10-19
        (s[8] == '2' && '0' <= s[9] && s[9] <= '9') ||   // 20-29
        (s[8] == '3' && '0' <= s[9] && s[9] <= '1')))    // 30-01
    {
      return 0;
    }

  // T
  if (s[10] != 'T') return 0;

  // hh (01 through 59)
  if (!('0' <= s[11] && s[11] <= '5' &&                  // 0X-5X
        '0' <= s[12] && s[12] <= '9'))                   // X0-X9
    {
      return 0;
    }

  // :
  if (s[13] != ':') return 0;

  // mm (01 through 59)
  if (!('0' <= s[14] && s[14] <= '5' &&                  // 0X-5X
        '0' <= s[15] && s[15] <= '9'))                   // X0-X9
    {
      return 0;
    }

  // :
  if (s[16] != ':') return 0;

  // ss (01 through 59)
  if (!('0' <= s[17] && s[17] <= '5' &&                  // 0X-5X
        '0' <= s[18] && s[18] <= '9'))                   // X0-X9
    {
      return 0;
    }

  // Validated YYYY-MM-DDThh:mm:ss.  
  // Check time zone designator (Z or +hh:mm or -hh:mm).

  // Validate YYYY-MM-DDThh:mm:ssZ
  if (len == 20)
    {
      // Z
      if (s[19] == 'Z')
        {
          return 1;
        }
      else
        {
          return 0;
        }
    }

  // Validate YYYY-MM-DDThh:mm:ss+hh:mm
  else if (len == 25)
    {
      // +hh:mm or -hh:mm
      if ((s[19] == '+' || s[19] == '-') &&                  // + or -
          '0' <= s[20] && s[20] <= '5' &&                    // 0X-5X
          '0' <= s[21] && s[21] <= '9' &&                    // X0-X9
          s[22] == ':' &&                                    // :
          '0' <= s[23] && s[23] <= '5' &&                    // 0X-5X
          '0' <= s[24] && s[24] <= '9')                      // X0-X9
        {
          return 1;
        }
      else
        {
          return 0;
        }
    }

  return 0;
}

struct ListNode
{
  char s[25];
  struct ListNode *next;
};

int main(int argc, char *argv[])
{
  FILE *fp = NULL;
  char s[100];
  struct ListNode *head = 0;
  struct ListNode *node = 0;

  if (argc != 2) 
    {
      printf("USAGE: %s <filename>\n", argv[0]);
      return 1;
    }

  fp = fopen(argv[1], "r");
  if (!fp)
    {
      printf("ERROR: failed to open %s.\n", argv[1]);
      return 1;
    }  

  // Read each date-time value from the input file.
  while (fscanf(fp, "%s", s) == 1)
    {
      if (isDateTime(s, strlen(s))) 
        {
          // Walk the list of date-time values to see if
          // this value is a duplicate value.
          node = head;
          while (node)
            {
              if (strcmp(node->s, s) == 0)
                {
                  break;  // found the value in the list
                }
              node = node->next;
            }

          // If this date-time value was not found in the
          // list, add the value to the list.
          if (!node)
            {
              node = malloc(sizeof(struct ListNode));
              if (!node)
                {
                  printf("ERROR: out of memory\n");
                  return 1;
                }

              strcpy(node->s, s);

              // Add the node to the list
              if (head)
                {
                  node->next = head;
                }
              head = node;
            }
        }
    }

  fclose(fp);

  // Print the list of distinct date-time values.
  node = head;
  while (node)
    {
      printf("%s\n", node->s);
      node = node->next;
    }

  return 0;
}
