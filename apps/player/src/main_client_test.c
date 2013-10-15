#include "client.h"
#include <stdio.h>

int main() {
   char sendline[1000];
   init_client();

   while (fgets(sendline, 10000, stdin) != NULL) {
      talk_client(sendline);
   }
   return 0;
}

