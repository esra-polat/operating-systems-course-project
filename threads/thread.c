#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <stdbool.h>
#include <semaphore.h>
#include <time.h>
#include <ctype.h>

#define MAX_CHAR	80


sem_t semaphore_queue;

sem_t rw_mutex;
sem_t reader_mutex;
sem_t rw_mutex_upper;
sem_t rw_mutex_replace;
sem_t rw_mutex_writer;

pthread_cond_t	upper_cond;
pthread_mutex_t upper_mutex;

pthread_cond_t	replace_cond;
pthread_mutex_t replace_mutex;

pthread_cond_t	writer_cond;
pthread_mutex_t writer_mutex;


int readTnumber;
int upperTnumber;
int replaceTnumber;
int writeTnumber;

int totalLines;
int num_readers = 0;
int current_readLine = 0;
int upper_Line = 0;
int replace_Line = 0;
int writer_Line = 0;

char **lines;
int *point;
int *cUpRep;

int count = 0;
int a = 0;
int b = 0;
int d = 0;

//This struct holds thread numbers and file name
typedef struct {
	char *fileToRead[MAX_CHAR];
	int readTnumber;
	int upperTnumber;
	int replaceTnumber;
	int writeTnumber;
} ThreadInfo;

//Struct for Read thread information
typedef struct {
	pthread_t tid;
	int order;
	char *fileToRead[MAX_CHAR];
} READ_THREAD_PARAM;

//Struct for Upper thread information
typedef struct {
	pthread_t tid;
	int order;
} UPPER_THREAD_PARAM;

//Struct for Replace thread information
typedef struct {
	pthread_t tid;
	int order;
} REPLACE_THREAD_PARAM;

//Struct for Write thread information
typedef struct {
	pthread_t tid;
	int order;
	char *fileToWrite[MAX_CHAR];
} WRITE_THREAD_PARAM;


//Function for Read thread
void *reader(void *param){

	int desired_line;
	sem_wait(&semaphore_queue);
	while(true && current_readLine < totalLines) {
		wait(reader_mutex);
		num_readers++;
		if(num_readers == 1)
			wait(rw_mutex);
		signal(reader_mutex);
		
		//start reading
		READ_THREAD_PARAM *temp = param;

		FILE *file = fopen(temp->fileToRead[0], "r");

		bool copy_characters = false;
		int line_number = 1;
 

		if(current_readLine < readTnumber){
			desired_line = temp->order;

		}
		else if (current_readLine >= readTnumber || current_readLine < totalLines){
			desired_line = current_readLine + 1;
		}
		
		point[current_readLine] = desired_line;


	  	char toCopy[50];
		for(int y = 0; y < 50; y++){
			toCopy[y] = '\0';
		}

	  	int i=0;

		for (;;) {
			int c = fgetc(file);
			if (desired_line == line_number) {
				copy_characters = true;
			}

			if (EOF == c) {
				break;
			} 
			else if ('\n' == c) {
				line_number++;
				if (desired_line == line_number) {
					copy_characters = true;
				} else if (copy_characters) {
					break;
				}
			} 
			else if (copy_characters) {
				toCopy[i] = c;
	      			i++;
			}
			
		}
		

		current_readLine++;
		lines[desired_line] = &toCopy[0];

		//signal for starting upper case
		pthread_cond_broadcast(&upper_cond);

		
		putchar('\n');
	  	printf("Read_%d		Read_%d	read the line %d which is '%s'\n", temp->order, temp->order, desired_line, lines[desired_line]);
		count++;
		fclose(file);
		sleep(1);
		//finish reading

		wait(reader_mutex);
		num_readers--;
		if(num_readers == 0)
			signal(rw_mutex);
		signal(reader_mutex);
	}
	sem_post(&semaphore_queue);	
	
	pthread_exit(NULL);

}

//Function for Upper thread
void *upper_case(void *param){
	
	sem_wait(&semaphore_queue);
	while(true && (upper_Line < totalLines)){
		
		wait(rw_mutex_upper);

		pthread_mutex_lock(&upper_mutex);
		
		if(current_readLine != totalLines){
			pthread_cond_wait(&upper_cond, &upper_mutex);
		}
		
		UPPER_THREAD_PARAM *temp = param;

		if((lines[point[a]] != NULL) && (count > upper_Line)){

			int j = 0;
			char ch;
			char *str = lines[point[a]];
			putchar('\n');
			printf("Upper_%d 	Upper_%d read index %d and converted '%s' to ", temp->order, temp->order, point[a], str);
			while(str[j] != '\0'){
				ch = str[j];
				lines[point[a]][j] = toupper(ch);
				j++;
			}

			cUpRep[point[a]]++;

			printf("'%s'\n", lines[point[a]]);
			upper_Line++;
			a++;

		}
		
		//signal for starting replace case
		pthread_cond_broadcast(&replace_cond);

		pthread_mutex_unlock(&upper_mutex);

		signal(rw_mutex_upper);
		
	}

	sem_post(&semaphore_queue);

	pthread_exit(NULL);

}

//Function for Replace thread
void *replace_case(void *param){
	
	sem_wait(&semaphore_queue);
	while(true && (replace_Line < totalLines)){
		
		wait(rw_mutex_replace);

		pthread_mutex_lock(&replace_mutex);
		
		if(upper_Line != totalLines){
			pthread_cond_wait(&replace_cond, &replace_mutex);
		}
		
		REPLACE_THREAD_PARAM *temp = param;


		if((lines[point[b]] != NULL) && (count > replace_Line)){

			int j = 0;
			char *str = lines[point[b]];
			putchar('\n');
			printf("Replace_%d 	Replace_%d read index %d and converted '%s' to ", temp->order, temp->order, point[b], str);

			while(str[j] != '\0'){
				if(str[j] == ' '){
					lines[point[b]][j] = '_';
				}
				j++;
			}
			
			cUpRep[point[b]]++;

			printf("'%s'\n", lines[point[b]]);
			replace_Line++;
			b++;
		}
		
		//signal for starting writer
		pthread_cond_broadcast(&writer_cond);

		pthread_mutex_unlock(&replace_mutex);

		signal(rw_mutex_replace);

		
	}

	sem_post(&semaphore_queue);

	pthread_exit(NULL);
	
}

//Function for Write thread
void *writer(void *param){

	sem_wait(&semaphore_queue);
	while(true && (writer_Line < totalLines)){
		
		wait(rw_mutex_writer);
		
		pthread_mutex_lock(&writer_mutex);
		
		if(replace_Line != totalLines){
			pthread_cond_wait(&writer_cond, &writer_mutex);
		}


		WRITE_THREAD_PARAM *temp = param;
		
		bool copy_characters1 = false;
		int line_number = 1;


		if(cUpRep[point[d]] == 2){
			//printf("%d\n", cUpRep[point[d]]);
			FILE *file = fopen(temp->fileToWrite[0], "r+");

			for (;;) {
				if (point[d] == line_number) {
					fprintf(file,"%s\n" ,lines[point[d]]);
					putchar('\n');
					printf("Writer_%d 	Writer_%d write line %d back which is '%s'\n", temp->order, temp->order, point[d], lines[point[d]]);
					d++;
					writer_Line++;
		      			break;
				}

				int f = fgetc(file);

				if (EOF == f) {
					break;
				} 
				else if ('\n' == f) {
					line_number++;
					if (point[d] == line_number) {
						copy_characters1 = true;
					} else if (copy_characters1) {
						break;
					}
				} 
				else if (copy_characters1) {
					fprintf(file,"%s\n" ,lines[point[d]]);
					putchar('\n');
					printf("Writer_%d 	Writer_%d write line %d back which is '%s'\n", temp->order, temp->order, point[d], lines[point[d]]);
					d++;
					writer_Line++;
		      			break;
				}
				
			}
				
		}

		pthread_mutex_unlock(&writer_mutex);

		signal(rw_mutex_writer);

		
	}

	sem_post(&semaphore_queue);

	pthread_exit(NULL);
}

//This function gets thread numbers and file name
void argSeparate(char *argv[], ThreadInfo *temp)
{
	temp->readTnumber = atoi(argv[4]); //readThread number
	temp->upperTnumber = atoi(argv[5]); //upperThread number
	temp->replaceTnumber = atoi(argv[6]); //replaceThread number
	temp->writeTnumber = atoi(argv[7]); //writeThread number
	temp->fileToRead[0] = argv[2]; //file to Read
}

//This function finds the total lines number in file
int findLines(ThreadInfo *temp)
{
	FILE *fp; 
    	int count = 0;  // Line counter (result)  
    	char c;  // To store a character read from file 
  
    	// Open the file 
    	fp = fopen(temp->fileToRead[0], "r"); 
  
    	// Check if file exists 
    	if (fp == NULL) 
    	{ 
        	printf("Could not open file %s", temp->fileToRead[0]); 
        	return 0; 
    	} 
  
    	// Extract characters from file and store in character c 
    	for (c = getc(fp); c != EOF; c = getc(fp)) 
        	if (c == '\n') // Increment count if this character is newline 
            	count = count + 1; 
  
    	// Close the file 
    	fclose(fp); 
    	return count;
}

//This functions creates all threads
void createThreads(ThreadInfo *temp)
{
	readTnumber = temp->readTnumber;
	upperTnumber = temp->upperTnumber;
	replaceTnumber = temp->replaceTnumber;
	writeTnumber = temp->writeTnumber;
	
	pthread_t readThreads[readTnumber];
	pthread_t upperThreads[upperTnumber];
	pthread_t replaceThreads[replaceTnumber];
	pthread_t writeThreads[writeTnumber];
	
	int rc;
	int i;
	void *status;
	
	//create read threads
	for(i=0; i<readTnumber; i++)
	{
		READ_THREAD_PARAM* param = (READ_THREAD_PARAM*)malloc(sizeof(READ_THREAD_PARAM));
		param->order = i+1;
		param->fileToRead[0] = temp->fileToRead[0];

		rc = pthread_create(&readThreads[i], NULL, reader, (void*)param);
		
		if(rc){
			fprintf(stderr, "ERROR; return code from phtread_create() is %d\n", rc);
			exit(-1);
		}
	}

	//create upper threads
	for(i=0; i<upperTnumber; i++)
	{
		UPPER_THREAD_PARAM* param = (UPPER_THREAD_PARAM*)malloc(sizeof(UPPER_THREAD_PARAM));
		param->order = i+1;
		rc = pthread_create(&upperThreads[i], NULL, upper_case, (void*)param);
		
		if(rc){
			fprintf(stderr, "ERROR; return code from phtread_create() is %d\n", rc);
			exit(-1);
		}
	}

	//create replace threads
	for(i=0; i<replaceTnumber; i++)
	{
		REPLACE_THREAD_PARAM* param = (REPLACE_THREAD_PARAM*)malloc(sizeof(UPPER_THREAD_PARAM));
		param->order = i+1;
		rc = pthread_create(&replaceThreads[i], NULL, replace_case, (void*)param);
		
		if(rc){
			fprintf(stderr, "ERROR; return code from phtread_create() is %d\n", rc);
			exit(-1);
		}
	}
		

	//create write threads
	for(i=0; i<writeTnumber; i++)
	{
		WRITE_THREAD_PARAM* param = (WRITE_THREAD_PARAM*)malloc(sizeof(WRITE_THREAD_PARAM));
		param->order = i+1;
		param->fileToWrite[0] = temp->fileToRead[0];

		rc = pthread_create(&writeThreads[i], NULL, writer, (void*)param);
		
		if(rc){
			fprintf(stderr, "ERROR; return code from phtread_create() is %d\n", rc);
			exit(-1);
		}
	}	

	//wait for read threads
	for(i=0; i<readTnumber; i++)
	{
		rc = pthread_join(readThreads[i], &status);
		
		if(rc){
			fprintf(stderr, "ERROR; return code from phtread_create() is %d\n", rc);
			exit(-1);
		}
	}

	//wait for upper threads
	for(i=0; i<upperTnumber; i++)
	{
		rc = pthread_join(upperThreads[i], &status);
		
		if(rc){
			fprintf(stderr, "ERROR; return code from phtread_create() is %d\n", rc);
			exit(-1);
		}
	}

	//wait for replace threads
	for(i=0; i<replaceTnumber; i++)
	{
		rc = pthread_join(replaceThreads[i], &status);
		
		if(rc){
			fprintf(stderr, "ERROR; return code from phtread_create() is %d\n", rc);
			exit(-1);
		}
	}

	//wait for writer threads
	for(i=0; i<writeTnumber; i++)
	{
		rc = pthread_join(writeThreads[i], &status);
		
		if(rc){
			fprintf(stderr, "ERROR; return code from phtread_create() is %d\n", rc);
			exit(-1);
		}
	}
}

   
int main(int argc, char *argv[]) 
{
	ThreadInfo temp;

	argSeparate(argv, &temp);

	/*printf("read = %d\n", temp.readTnumber);
	printf("upp = %d\n", temp.upperTnumber);
	printf("rep = %d\n", temp.replaceTnumber);
	printf("write = %d\n", temp.writeTnumber);*/

	int read = temp.readTnumber;
	int upp = temp.upperTnumber;
	int rep = temp.replaceTnumber;
	int write = temp.writeTnumber;
	
	//Total thread numbers
	int total = read + upp + rep + write;

	//printf("total = %d\n", total);

	//Initializing semaphore
	sem_init (&semaphore_queue,0,total);

	totalLines = findLines(&temp);
	
	//Lines keep all lines in file
	lines = (char **) malloc(sizeof(char *) * (totalLines + 1));
	for(int i = 0; i < totalLines; i++){
		lines[i] = (char *) malloc(sizeof(char) * (totalLines + 1));
	}
	
	//Point keeps the number of line
	point = (int *) malloc(sizeof(int) * totalLines);

	//cUpRep for control upper and replace 
	cUpRep = (int *) malloc(sizeof(int) * (totalLines + 1));

	for(int i=0; i<=totalLines; i++){
		cUpRep[i] = 0;
	}
	
	createThreads(&temp);

	//printf("totalline = %d\n", totalLines);

	pthread_mutex_destroy(&upper_mutex);
	pthread_mutex_destroy(&replace_mutex);	
	pthread_mutex_destroy(&writer_mutex);	
	sem_destroy(&semaphore_queue);
	sem_destroy(&rw_mutex);
	sem_destroy(&reader_mutex);
	sem_destroy(&rw_mutex_upper);
	sem_destroy(&rw_mutex_replace);
	sem_destroy(&rw_mutex_writer);

	free(lines);
	free(point);
	free(cUpRep);

    	exit(0);
}
