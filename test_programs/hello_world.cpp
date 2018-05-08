#include <stdio.h>

int main() {
  

  int sum = 10000;
  for(int i = 0; i < 10; i++){
    sum /= 5;
    sum *= 5;
    sum -= 100;
    sum += sum;
  }


}
