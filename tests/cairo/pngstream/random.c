#include <stdio.h>
#include <time.h>

int random(int range){
    int num;
    num = rand() % range;
    return num;
}

int foo()
{
   printf("foo\n");
}

int bar()
{
   printf("bar\n");
}


int main(){
    int number = 0;
    int i;
    int middle = 500;

    // Seed random number generator
    srand(time(NULL));

    //for ( ;; ){
    for ( number=1;number<=1000;number++ ){
        i = random(1000);
        printf("%d\n",i);
	sleep(1);
	printf("We are on loop: %d\n", number);

	if(i >= middle)
	{
	   foo();
	}
	else
	{
	   bar();
	}
    }
    return 0;
}

