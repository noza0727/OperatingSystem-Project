#include <stdio.h>
#include "utils/bootstrap/bootstrap.c"

int main() {
  setbuf(stdout, NULL);

  return startServer();
}
