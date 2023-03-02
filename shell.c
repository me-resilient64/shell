#include  <stdio.h>
#include  <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

/* Splits the string by space and returns the array of tokens
*
*/
char **tokenize(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
      token[tokenIndex] = '\0';
      if (tokenIndex != 0){
	tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
	strcpy(tokens[tokenNo++], token);
	tokenIndex = 0; 
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }
 
  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}


int main(int argc, char* argv[]) {
	char  line[MAX_INPUT_SIZE];            
	char  **tokens;              
	int i;


	while(1) {			
		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		printf("$ ");
		scanf("%[^\n]", line);
		getchar();
		printf("Command entered: %s (remove this debug output later)\n", line);
		/* END: TAKING INPUT */
//return check
    if(strlen(line)==0) continue;
		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);
   
       //do whatever you want with the commands, here we just print them
    int len_tok=0;
    while(tokens[len_tok]!=NULL){
	    printf("found token %s (remove this debug output later)\n", tokens[len_tok++]);
    }
		/*for(i=0;tokens[i]!=NULL;i++){
			printf("found token %s (remove this debug output later)\n", tokens[i]);
		}*/
//background check
//TODO: periodically check for reaping child process and add 'exit' keyword
    printf("%c\n",tokens[len_tok-1][0]);
    if(tokens[len_tok-1][0]=='&'){
    tokens[len_tok-1]=NULL;
    int f=fork();
    if(f<0){
      fprintf(stderr, "Process failed (fork fail)\n");
      exit(0);
    }
    else if(f==0){
      execvp(tokens[0], tokens);
      exit(1);
    }
    else if(f>0){
// Freeing the allocated memory	
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);
    }

     continue; 
    }
//chdir check
    if(tokens[0][0]=='c' && tokens[0][1]=='d' && tokens[0][2]=='\0'){
      chdir(tokens[1]);
      if(errno < 0){
          printf("Shell: Incorrect command\n");
      }
      else{
      printf("Changed directory to %s\n",tokens[1]);
      }
      continue;
    }
    int f=fork();
    if(f<0){
      fprintf(stderr, "Process failed (fork fail)\n");
      exit(0);
    }
    else if(f==0){
      execvp(tokens[0], tokens);
      exit(1);
    }
    else if(f>0){
      int rc_wair = waitpid(f, NULL, 0);
      printf("fork output : %d\n", f);
      printf("This is the parent process\n");
    }
		// Freeing the allocated memory	
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);

	}
	return 0;
}
