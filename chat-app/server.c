#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
struct client_info {
	int sockno;
	char ip[INET_ADDRSTRLEN];
	char client_groups[10][15];
};
struct client_info c[100];

int clients[100];
int n = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void sendtoall(char *msg, struct client_info cl, char *tmp)
{
	int i;
	int j;
	pthread_mutex_lock(&mutex);
	printf("\n##%s--%s---tmp:%s\n",c[0].client_groups[0],cl.ip, tmp);
	printf("\n##%s--%s---tmp:%s\n",c[1].client_groups[0],cl.ip, tmp);
	for(i = 0; i < n; i++) {
		for(j = 0; j < 10; j++){
			if(strcmp(c[i].client_groups[j], tmp) == 0 && (c[i].sockno != cl.sockno)) {
				if(send(clients[i],msg,strlen(msg),0) < 0) {
					perror("sending failure");
					continue;
				}
			}
		}
	}
	pthread_mutex_unlock(&mutex);
}
//gcc server.c -pthread
void *recvmg(void *sock)
{
	struct client_info cl = *((struct client_info *)sock);
	char msg[500];
	char msg1[500];
	char tmp[15];
	char tmp2[485];
	int len;
	int i;
	int j;
	int g=0;
	char command1[5];
	char command2[6];
	//printf("\nin func : %s---%d\n",cl.ip, cl.sockno);
	while((len = recv(cl.sockno,msg,500,0)) > 0) {
		msg[len] = '\0';
		//printf("\nLen :%d\n",len);
		int k = 0;
		i = 0;
		j=0;
		int msg1_len = 0;
		printf("%s",msg);
		if(msg[0] != '\0'){
			for ( i = 0; i < len; i++)
			{
		 		//printf("%c",msg[i]);
				if (msg[i] == ':')
				{
					k = i;
					msg1_len = i;
					i=0;
					break;
				}
			}
			//gcc server.c -pthread
			//printf("%s\n : msg1",msg1);
			k++;
			//printf("\n%d***\n",k);
			for(i=0;i<4;i++){
				command1[i] = msg[k];
				command2[i] = msg[k];
				k++;
			}	
			if(msg[4] != ' '){
				command2[4] = msg[k];
				k++;
			}
			//k++;
			/*j=0;
			printf("\n%d***\n",k);
			i = k;
			while (msg[i] != ' ')
			{
				tmp[j] = msg[i];
		 		printf("%c",tmp[j]);
				j++;
				i++;
			}
			tmp[j] = '\0';
			printf("\nTemp : %s\n",tmp);
			printf("\n%d---%d jjj\n",j,i);
			j=0;
			i++;
			printf("###%d###\n", i);
			while (msg[i] != '\0')
			{
				msg1[j] = msg[i];
		 		printf("%c",msg1[j]);
				j++;
				i++;
			}
			printf("msg2 : %s\n",msg1);
			//printf("\n%d ggg\n",j);*/
			
			command1[4] = '\0';
			command2[5] = '\0';
			/*printf("comm1 : %s\n",command1);
			printf("comm2 : %s\n",command2);*/

			if(strcmp(command1, "join") == 0){
				j=0;
				for(i=k;i<len;i++,j++)
					tmp[j] = msg[i];
				for (i = 0; i < 100; i++)
				{
					if(cl.sockno == clients[i])
						break;
				}
				tmp[j-1] = '\0';
				//printf("@@!!@@%d\n",i);
				strcpy(c[i].client_groups[g], tmp);
				//printf("%s---%d", c[i].client_groups[g], g);
				g++;
				//printf("%d\n",g);
			}

			if(strcmp(command1, "send") == 0){

				j=0;
				int head;
				//printf("\n%d***\n",k);
				i = k;
				while (msg[i] != ' ')
				{
					tmp[j] = msg[i];
		 			//printf("%c",tmp[j]);
					j++;
					i++;
				}
				tmp[j] = '\0';
				printf("\nTemp : %s\n",tmp);
				//printf("\n%d---%d jjj\n",j,i);
				/*j=0;
				i++;
				printf("###%d###\n", i);
				while (msg[i] != '\0')
				{
					msg1[j] = msg[i];
		 			printf("%c",msg1[j]);
					j++;
					i++;
				}*/
				/*while(k < len) {
					msg[k] = msg[k+1];
					k++;
				}*/
				strncpy(msg1, msg, msg1_len+1);
				strncpy(tmp2, msg+k+j, len-k);
				strcat(msg1, tmp2);
				printf("msg1 : %s\n",msg1);
				sendtoall(msg1, cl, tmp);
				//printf("\n%d ggg\n",j);
			}

			if(strcmp(command1, "quit") == 0){
				printf("%s quited\n", cl.ip);
				pthread_mutex_lock(&mutex);
				printf("%s disconnected\n",cl.ip);
				for(i = 0; i < n; i++) {
					if(clients[i] == cl.sockno) {
						j = i;
						while(j < n-1) {
							clients[j] = clients[j+1];
							j++;
						}
					}
				}
				n--;
				pthread_mutex_unlock(&mutex);
			}
			
			if(strcmp(command2, "leave") == 0){
				j=0;
				for(i=k+1;i<len;i++,j++)
					tmp[j] = msg[i];

				for (i = 0; i < 100; i++)
				{
					if(cl.sockno == clients[i])
						break;
				}
				tmp[j-1] = '\0';
				//printf("^^%s\n",tmp);
				//printf("\n%diii\n",i);
				for (j = 0; j < 10; j++)
				{	
					//printf("%d : %s---%s\n",j, c[i].client_groups[j], tmp);
					if(strcmp(c[i].client_groups[j], tmp) == 0)
						break;
				}
				int h;
				//printf("\n%djjj\n",j);
				for (h = j; h < g-j; h++)
				{
					strcpy(c[i].client_groups[h],c[i].client_groups[h+1]);
				}
				strcpy(c[i].client_groups[h],"");
				printf("%s left %s :",cl.ip, tmp);
				printf("existing groups of client %s :",cl.ip);
				for (h = 0; h < g-j; h++)
					printf("%s---", c[i].client_groups[h]);
				
			}
				
		}
		//gcc server.c -pthread
		memset(msg,'\0',sizeof(msg));
		memset(msg1,'\0',sizeof(msg));
		memset(command1,'\0',sizeof(command1));
		memset(command2,'\0',sizeof(command2));
		memset(tmp,'\0',sizeof(tmp));
		memset(tmp2,'\0',sizeof(tmp2));
	}
	pthread_mutex_lock(&mutex);
	printf("%s disconnected\n",cl.ip);
	for(i = 0; i < n; i++) {
		if(clients[i] == cl.sockno) {
			j = i;
			while(j < n-1) {
				clients[j] = clients[j+1];
				j++;
			}
		}
	}
	n--;
	pthread_mutex_unlock(&mutex);
}
int main(int argc,char *argv[])
{
	struct sockaddr_in my_addr,their_addr;
	int my_sock;
	int their_sock;
	socklen_t their_addr_size;
	int portno;
	pthread_t sendt,recvt;
	char msg[500];
	int len;
	struct client_info cl;
	char ip[INET_ADDRSTRLEN];;
	;
	if(argc > 2) {
		printf("too many arguments");
		exit(1);
	}
	portno = atoi(argv[1]);
	my_sock = socket(AF_INET,SOCK_STREAM,0);
	memset(my_addr.sin_zero,'\0',sizeof(my_addr.sin_zero));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(portno);
	my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	their_addr_size = sizeof(their_addr);

	if(bind(my_sock,(struct sockaddr *)&my_addr,sizeof(my_addr)) != 0) {
		perror("binding unsuccessful");
		exit(1);
	}

	if(listen(my_sock,5) != 0) {
		perror("listening unsuccessful");
		exit(1);
	}

	while(1) {
		if((their_sock = accept(my_sock,(struct sockaddr *)&their_addr,&their_addr_size)) < 0) {
			perror("accept unsuccessful");
			exit(1);
		}
		pthread_mutex_lock(&mutex);
		inet_ntop(AF_INET, (struct sockaddr *)&their_addr, ip, INET_ADDRSTRLEN);
		printf("%s connected\n",ip);
		cl.sockno = their_sock;
		c[n].sockno = their_sock;
		strcpy(cl.ip,ip);
		strcpy(c[n].ip,ip);
		clients[n] = their_sock;
		printf("%s---%d\n",c[n].ip, c[n].sockno);
		n++;
		pthread_create(&recvt,NULL,recvmg,&cl);
		pthread_mutex_unlock(&mutex);
	}
	return 0;
}

//gcc server.c -pthread
//gcc client.c -pthread