#include <iostream>
#include"sha1.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/timeb.h>
#include <omp.h>

int main()
{
	omp_set_num_threads(100);
	int found = 0;
	double timee;
	struct timeb start,  end;
	srand(time(NULL));


	char word[100], hash[100], result[100];

	strcpy_s(word, "cata");
	strcpy_s(hash, "a31ae9fe898b3f1d73e28d0d501014e3385ac1d4");

	ftime(&start);
	while (found == 0)
	{
		//printf("WHILe");
		
		#pragma omp parallel
		{
			SHA1_CTX sha;
			uint8_t results[100];
			char buf[100];
			char result_hash[42];
			int n;
			//printf("Hello from thread %d\n", omp_get_thread_num());
			strcpy_s(buf, word);
			while (found == 0 && strlen(buf) < 20)
			{
				n = strlen(buf);
				buf[n] = char(rand() % 127);
				buf[++n] = '\0';

				//printf("Hashing: %s \n",buf);
				/*for (int j = 0; j < 100; j++)
				{
					printf("%c", (buf[j]));
				}
				printf("\n");*/
				SHA1Init(&sha);
				SHA1Update(&sha, (uint8_t*)buf, n);
				SHA1Final(results, &sha);
				for (int j = 0; j < 20; j++)
				{
					sprintf(&result_hash[j * 2], "%02x", results[j]);
				}
				//printf("RESULT HASH: %s", result_hash);

				if (strcmp(result_hash, hash) == 0)
				{
					found = 1;
					//printf("Found: %s", (buf));
					strcpy(result, buf);
				}
			}
		}
	}
	ftime(&end);

	timee = end.time - start.time + ((double)end.millitm - (double)start.millitm) / 1000.0;
	printf("Found: %s \n", result);
	printf("Duration = % .2lf\n", timee);
}
