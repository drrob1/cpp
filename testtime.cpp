#include "/home/rob/mymacros.h"

int main(argcargv) IS
  time_t rawtime;
  struct tm POINTER ptimeinfo;
  char buffer[80];

  time(ADROF rawtime);
  ptimeinfo = localtime (ADROF rawtime);

  printf(" current local time and date: %s", asctime(ptimeinfo));


  strftime (buffer,80,"Now it's %I:%M%p.",ptimeinfo);
  puts (buffer);

  return 0;
END;
