
void fun1(int x)
{
  int y;
  int z;
  printf("%d \n", x+y+z); 
}

int fun2(float z)
{
   float x;

   return (int)(z+x);
}

int main(int argc, char** argv)
{
  int w;

  fun1(w);
  fun2();

  return 0;
}


/* ******************** */

/* prog2.c */


void f1(int *i)
{
 i = malloc(sizeof(int));
}

int f2()
{
  
  float x[5];
}

int main(int argc, char **argv)
{
  int i;
  int j;

  i = f2();
  f1(j);
}

/* ******************** */

/* prog3.c */

int x = 10;
int y;
 
int f(int p, int q)
{
    int j = 5;
    x = 5;
    return p * q + j;
}
 
int main()
{
   int i = x;
 
   y = f(i, i);
   return 0;
}

/* ******************** */

/* prog4.c */

int x = 10;
int y;
 
void f(int *p, int q)
{
    *p = 5;
}
 
int main()
{
    int i = x;

    f(&i, i);
    return 0;
}

/* ******************** */

/* string_tools.c */

char *concat_wrong(char *s1, char *s2) 
{
  char result[70];

  strcpy (result, s1);
  strcat (result, s2);

  return result;
}


char *concat(char *s1, char *s2)
{
  char *result;

  result = malloc(strlen(s1) + strlen(s2) + 1);
  if (result == NULL) {
      printf ("Error: malloc failed\n");
      exit(1);
  }

  strcpy (result, s1);
  strcat (result, s2);

  return result;
}

/* ******************** */

/* sumArray.c */

int sum(int *a, int size) {
   int i, s = 0;

   for(i = 0; i < size; i++) 
       s += a[i];

   return s;
}


int main()
{
  int N = 5;
  int i[N] = {10, 9, 8, 7, 6};

  printf("sum is %d\n", sum(i,N));

  return 0;
}

/* ******************** */
