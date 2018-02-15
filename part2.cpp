#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <string>
using namespace std;
#define MAX_LINE 80

int main(void)
{
	
	int should_run = 1;
	
	while(should_run)
	{
		printf("osh>");
		fflush(stdout);
		//easier for me to use string
		string a[5], hist[10];
		//read in the line of input
		getline(cin, a[0]);
		int found[3], history = 1, history1 = 0;
        if(a == "history")
        {
            /////////
        }
        else if(isdigit(a[0]))
        {//////////////
            int i = stoi(a[0]);
            if((i <= history) && (i >= history - 9))
            {
                a[0] = hist[history - i];
            }
            
        }
        if(history1 % 9 == 0)
        {/////////////////
            history1 = 0;
        }
        else
        {
            hist[history1] = a[0];
        }
		//parse string with space being the divider
		found[0] = a[0].find(" ");
		found[1] = a[0].find(" ", found[0]+1);
		//add /bin/ at the beginning of every command
		a[1] = "/bin/";
		//store the arguments into different strings
		a[1] += a[0].substr(0, found[0]); 
		a[2] = a[0].substr(found[0]+1, (found[1]-found[0])-1);
		//checks for '&' to later activate a wait for child
		if(found[0] != -1)
		{
			if (a[2] == "&")
			{
				//cout << "There is an &: |" << a[3];
				found[0] = -1;
				found[2] = 1;
			}
			
		}		
		if(found[1] != -1)
		{
			a[3] = a[0].substr(found[1]+1);
			if (a[3] == "&")
			{
				//cout << "There is an &: |" << a[3];
				found[1] = -1;
				found[2] = 1;
			}
			
		}
		//following creates a child process
		pid_t pid;
		pid = fork();
		if(pid < 0)
		{
			fprintf(stderr, "Fork Failed");
			return 1;
		}
		//if the child process exists then runs this
		else if (pid == 0)
		{
			//depending on how many arguments were passed one of the following will execute
			if(found[0] != -1 && found[1] == -1)
			{
				char *args[MAX_LINE/2 +1] = {(char*)a[1].c_str(), (char*)a[2].c_str()};			
				execvp(args[0], args);
			}
			else if(found[1] != -1 && found[0] != -1)
			{
				char *args[MAX_LINE/2 +1] = {(char*)a[1].c_str(), (char*)a[2].c_str(), (char*)a[3].c_str()};			
				execvp(args[0], args);
			}
			else
			{
				char *args[MAX_LINE/2 +1] = {(char*)a[1].c_str()};			
				execvp(args[0], args);
			}
		}
		//will wait for child process to end if there was an '&' at the end of input
		else
		{
			if(found[2] == 1)
			{
				found[2] = 0;
				wait(NULL);
			}
			printf("Child Complete\n");
		}
	}
	return 0;
}
