#include <stdio.h>

//Compiler version gcc  6.3.0
int numToArray(int input, int maxDigits, int array[]);
int arrayToNum(int array[], int maxDigits);

int binaryToQuaternary (int input, int maxDigits) {
  maxDigits = (maxDigits%2 == 0? maxDigits: maxDigits+1);
  int array[maxDigits];
  int result[maxDigits/2];
  int res=0;
  int searchTill=0;

  searchTill = numToArray(input, maxDigits, array) + 1; //+1 for buffer
  for (int i = 1, j = 0; i<searchTill; i+=2, j++) {
    result[j]=array[i-1]+array[i]*2;
  }
  res = arrayToNum(result, searchTill/2);
  return(res);
}
int numToArray(int input, int maxDigits, int array[]) {
  int searchTill = 0;

  for(int i = 0; i<maxDigits && input > 0; i++) {
    array[i] = input % 10;
    input /= 10;
    if ((input == 0) && (input%10 == 0)) searchTill = i + 1;
  }
  return(searchTill);
}
int arrayToNum(int array[], int maxDigits) {
  int result = 0;
  for(int i = maxDigits-1; i>=0; i--) {
    result += array[i];
    if (i != 0) result *= 10;
  }
  return(result);
}
int main() {
  int m = 1011000;
  int x;
  x = binaryToQuaternary(m, 3171);
  printf("Number in Quaternary is: %4d\n",x);
  
  return 0;
}