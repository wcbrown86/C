#include <unistd.h>
#include <stdio.h>

int main () {

int results;
char unitName[100];

results = gethostname(unitName, sizeof(unitName));

if(results == 0){
    printf("Host name = %s\n", unitName);
}

    

  return 0;
}
