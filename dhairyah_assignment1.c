/**
 * @dhairyah_assignment1
 * @author  Dhairya Hardikbhai Desai <dhairyah@buffalo.edu>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * This contains the main function. Add further description here....
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <strings.h>

#include "../include/global.h"
#include "../include/logger.h"

/**
 * main function
 *
 * @param  argc Number of arguments
 * @param  argv The argument list
 * @return 0 EXIT_SUCCESS
 */


struct xxmyaddr
       {
              char ip[100];
              char hostname[100];
       };

struct xxmyaddr xxgetmyaddr();

int xxvalidateip(char*);

int xxvalidateport(char*);

int xxvalidatearg(char*, char*);

int main(int argc, char **argv)
{
	/*Init. Logger*/
	cse4589_init_log(argv[2]);

	/*Clear LOGFILE*/
	fclose(fopen(LOGFILE, "w"));
	
	
/*
References :

http://beej.us/guide/bgnet/output/html/multipage/index.html

*/

		
if(strcmp(argv[1],"c")==0)
{
	
char ip_list[10][100];
char host_list[10][100];
char port_list[10][100];
int active_clients = 0;
char myhostname[100], myip[100];
char block_ip[10][100];

struct xxmyaddr myaddr;
myaddr = xxgetmyaddr();

strcpy(myip,myaddr.ip);
strcpy(myhostname,myaddr.hostname);

int client_sock, con_int, file_socket, file_con_int,file_accept_socket,file_recv_socket;
struct sockaddr_in server_end, file_addr,file_accept_addr;

int l_ack=0,rec_bytes, send_bytes, block_cnt=0;

int l_rec;

char recmsg[280];
char *rec_head;

int fd_max, i;
fd_set pri_list, sec_list;

FD_ZERO(&pri_list);
FD_ZERO(&sec_list);


file_accept_socket = socket(AF_INET, SOCK_STREAM, 0);


file_accept_addr.sin_family = AF_INET;
file_accept_addr.sin_port= htons(atoi(argv[2]));
file_accept_addr.sin_addr.s_addr= INADDR_ANY;
memset(file_accept_addr.sin_zero,'\0',sizeof(file_accept_addr.sin_zero));
bind(file_accept_socket,(struct sockaddr*)&file_accept_addr,sizeof(file_accept_addr));
listen(file_accept_socket, 5);

FD_SET(0,&pri_list);
FD_SET(file_accept_socket,&pri_list);

fd_max = file_accept_socket;

while(1==1)
{       

		
		sec_list = pri_list;
		select(fd_max+1, &sec_list, NULL, NULL, NULL);
		
		for (i=0;i <= fd_max;i++)
		{
			
		if (FD_ISSET(i, &sec_list))
		{
			
		if (i == 0)
			
		{
			
			char *cmd,cmd1[100],cmd2[100],buff[500];
			int cmd_len=0, login_status = 0, rec_bytes, send_bytes;

			char *command = (char*) malloc(sizeof(char)*500);
			memset(command,'\0',500);
			fgets(command, 500, stdin);
			
			cmd_len = strlen(command);
			
			if (cmd_len > 0 && command[cmd_len-1] == '\n')
			{
				command[--cmd_len] = '\0';
			}
				
			cmd = strtok(command," ");

			if(strcmp(cmd,"LOGIN")==0)
			{
							if(xxvalidatearg("LOGIN",command)==1)
							{
					cmd = strtok(NULL," ");
					strcpy(cmd1,cmd);

									if(xxvalidateip(cmd1)==1)
									{

											cmd = strtok(NULL," ");
													strcpy(cmd2,cmd);
													if(xxvalidateport(cmd1)==1)
													{
															
															client_sock =  socket(AF_INET, SOCK_STREAM, 0);

															if(client_sock == -1)
															{
																
																  cse4589_print_and_log("[%s:ERROR]\n", command);
																  cse4589_print_and_log("[%s:END]\n", command);
																  continue;
															}
															
															server_end.sin_family = AF_INET;
															server_end.sin_port = htons(atoi(cmd2));
															server_end.sin_addr.s_addr = inet_addr(cmd1);

															memset(server_end.sin_zero,'\0',sizeof(server_end));

															con_int = connect(client_sock,(struct sockaddr *)&server_end,sizeof(server_end));
															
															FD_SET(client_sock,&pri_list);
															fd_max = ((fd_max>client_sock)?fd_max:client_sock);
															

															if(con_int < 0)
															{
																	close(client_sock);
																
																  cse4589_print_and_log("[%s:ERROR]\n", command);
																  cse4589_print_and_log("[%s:END]\n", command);
																	continue;
															}
															
															
															FD_SET(client_sock, &pri_list);
															
															
															char ldetails[200]="L||";
															
															strcat(ldetails,myip);
															strcat(ldetails,"||");
															
															strcat(ldetails,myhostname);
															strcat(ldetails,"||");
															
															strcat(ldetails,argv[2]);
															strcat(ldetails,"||");
															
															strcat(ldetails,"logged-in");
																														
															send_bytes = send(client_sock,ldetails,sizeof(ldetails),0);
															
															rec_bytes = recv(client_sock, buff,sizeof(buff), 0);

															if(rec_bytes == -1 || send_bytes == -1)
															{
																	close(client_sock);
																	
																  cse4589_print_and_log("[%s:ERROR]\n", command);
																  cse4589_print_and_log("[%s:END]\n", command);
																	continue;
															}
															else
															{
																	char *inst;
																	
																	inst = strtok(buff,"||");
																	
																	if(strcmp(inst,"T")==0)
																	{
																		
																	inst = strtok(NULL,"||");
																	active_clients = atoi(inst);
																	
																	for(int i=0;i<active_clients;i++)
																	{
																		
																	inst = strtok(NULL,"||");	
																	strcpy(ip_list[i],inst);
																	
																	inst = strtok(NULL,"||");	
																	strcpy(host_list[i],inst);
																	
																	inst = strtok(NULL,"||");	
																	strcpy(port_list[i],inst);
																	
																	}
																	
																	}
																	login_status++;
																  cse4589_print_and_log("[%s:SUCCESS]\n", command);
																  cse4589_print_and_log("[%s:END]\n", command);
															}
													}
													else
													{
															printf("invalid port");
															continue;
													}
									}
									else
									{
											printf("IP is invalid");
											continue;
									}
							}
							else
							{
									printf("Invalid arguments for command login");
							}
			}
			else if(strcmp(cmd,"LIST")==0)
			{
				if(xxvalidatearg("LIST",command)==1)
				{	
					int r=0;
					char temp[100];

					for(r=0; r< active_clients;r++)
					{
					for(int s=r+1;s<active_clients;s++)
					{
					if(atoi(port_list[r])>atoi(port_list[s]))
					{	 					 
						strcpy(temp,port_list[r]);
						strcpy(port_list[r],port_list[s]);
						strcpy(port_list[s],temp);
						
						strcpy(temp,ip_list[r]);
						strcpy(ip_list[r],ip_list[s]);
						strcpy(ip_list[s],temp);
						
						strcpy(temp,host_list[r]);
						strcpy(host_list[r],host_list[s]);
						strcpy(host_list[s],temp);
																		 
					}
					}
					}
					cse4589_print_and_log("[%s:SUCCESS]\n", command);
					for(r=0; r< active_clients;r++)
					{
						int iport=atoi(port_list[r]);					
						cse4589_print_and_log("%-5d%-35s%-20s%-8d\n", (r+1), host_list[r], ip_list[r], iport);
					}
					 cse4589_print_and_log("[%s:END]\n", command);

				}
				else
				{
					printf("Invalid arguments for command list");
				}
			}
			else if(strcmp(cmd,"LOGOUT")==0)
			{
				if(xxvalidatearg("LOGOUT",command)==1)
				{	
				char logoutmsg[100] = "Y||";
				strcat(logoutmsg,myip);
				
			
				send_bytes = send(client_sock,logoutmsg,sizeof(logoutmsg),0);	
				
				if(send_bytes == -1)
				{

				cse4589_print_and_log("[%s:ERROR]\n", command);
				cse4589_print_and_log("[%s:END]\n", command);
				
				}
				else
				{
				login_status--;
				cse4589_print_and_log("[%s:SUCCESS]\n", command);
				cse4589_print_and_log("[%s:END]\n", command);
				
				}
					
				}
				else
				{
					printf("Invalid arguments for command logout");
				}
			}
			else if(strcmp(cmd,"EXIT")==0)
			{
				if(xxvalidatearg("EXIT",command)==1)
				{
					
					char logoutmsg[100] = "E||";
					strcat(logoutmsg,myip);
					
				
					send_bytes = send(client_sock,logoutmsg,sizeof(logoutmsg),0);	
				
					if(send_bytes == -1)
					{

					cse4589_print_and_log("[%s:ERROR]\n", command);
					cse4589_print_and_log("[%s:END]\n", command);
				
					}
					else
					{
					close(client_sock);
					close(file_accept_socket);
					cse4589_print_and_log("[%s:SUCCESS]\n", command);
					cse4589_print_and_log("[%s:END]\n", command);
					
					exit(0);
					
					}
				}
				else
				{
					printf("Invalid arguments for command exit");
				}
			}
			else if(strcmp(cmd,"REFRESH")==0)
			{
				if(xxvalidatearg("REFRESH",command)==1)
				{
					char logoutmsg[100] = "R||";
					strcat(logoutmsg,myip);
					
					send_bytes = send(client_sock,logoutmsg,sizeof(logoutmsg),0);

					rec_bytes = recv(client_sock, buff,sizeof(buff), 0);

				
					if(send_bytes == -1 || rec_bytes == -1)
					{

					cse4589_print_and_log("[%s:ERROR]\n", command);
					cse4589_print_and_log("[%s:END]\n", command);
				
					}
					else
					{
						
					char *inst;
																	
					inst = strtok(buff,"||");
					
					if(strcmp(inst,"T")==0)
					{
						
					inst = strtok(NULL,"||");
					active_clients = atoi(inst);
					
					for(int i=0;i<active_clients;i++)
					{
						
					inst = strtok(NULL,"||");	
					strcpy(ip_list[i],inst);
					
					inst = strtok(NULL,"||");	
					strcpy(host_list[i],inst);
					
					inst = strtok(NULL,"||");	
					strcpy(port_list[i],inst);
					
					}
					
					}
						
					cse4589_print_and_log("[%s:SUCCESS]\n", command);
					cse4589_print_and_log("[%s:END]\n", command);
					
					}
					
				}
				else
				{
					printf("Invalid arguments for command refresh");
				}
			}
			else if(strcmp(cmd,"BLOCK")==0)
			{
				if(xxvalidatearg("BLOCK",command)==1)
				{
					char blockmsg[100] = "U||";
					int block_flag =0, valid_client=0;
					
					cmd = strtok(NULL," ");
					
					for(i=0;i<block_cnt;i++)
					{
						if(strcmp(cmd,block_ip[i])==0)
						{
						block_flag = 1;
						continue;
						}
					}
					
				for(int pid=0; pid< active_clients;pid++)
				{
					if(strcmp(ip_list[pid],cmd)==0)
					{
						valid_client=1;
						break;
					}
				}
					
					if(block_flag == 0 && xxvalidateip(cmd)==1  && valid_client ==1)
					{
						strcpy(block_ip[block_cnt],cmd);
						strcat(blockmsg,cmd);
						
						strcat(blockmsg,"||");
						strcat(blockmsg,myip);
						
						block_cnt++;
						
						send_bytes = send(client_sock,blockmsg,sizeof(blockmsg),0);
						if(send_bytes == -1)
						{
						
						cse4589_print_and_log("[%s:ERROR]\n", command);
						cse4589_print_and_log("[%s:END]\n", command);
						
						}
						else
						{
						cse4589_print_and_log("[%s:SUCCESS]\n", command);
						cse4589_print_and_log("[%s:END]\n", command);
							
						}
					}
					else
					{
						cse4589_print_and_log("[%s:ERROR]\n", command);
						cse4589_print_and_log("[%s:END]\n", command);
						
					}
					
					
					
				}
				
				else
				{
					printf("Invalid arguments for command block");
				}
			}
			else if(strcmp(cmd,"UNBLOCK")==0)
			{
				if(xxvalidatearg("UNBLOCK",command)==1)
				{
					char blockmsg[100] = "W||";
					int block_flag=0,block_id=0;
					
					cmd = strtok(NULL," ");
					
					for(i=0;i<block_cnt;i++)
					{
						if(strcmp(cmd,block_ip[i])==0)
						{
						block_flag = 1;
						block_id = i;
						}
					}
					
					if(block_flag == 0)
					{
					cse4589_print_and_log("[%s:ERROR]\n", command);
					cse4589_print_and_log("[%s:END]\n", command);
					}
					else
					{
						
					strcat(blockmsg,cmd);
					
					strcat(blockmsg,"||");
					strcat(blockmsg,myip);
					
					
					for(i=block_id;i<block_cnt;i++)
					{					 
					strcpy(block_ip[i],ip_list[i+1]);
					}
					block_cnt--;

					send_bytes = send(client_sock,blockmsg,sizeof(blockmsg),0);
					if(send_bytes == -1)
					{
					
					cse4589_print_and_log("[%s:ERROR]\n", command);
					cse4589_print_and_log("[%s:END]\n", command);
					
					}
					else
					{
					cse4589_print_and_log("[%s:SUCCESS]\n", command);
					cse4589_print_and_log("[%s:END]\n", command);
						
					}
					}
					
					
					
				}
				else
				{
					printf("Invalid arguments for command block");
				}
			}
			else if(strcmp(cmd,"AUTHOR")==0)
			{
				char your_ubit_name[] = "dhairyah";
				cse4589_print_and_log("[%s:SUCCESS]\n", command);
				cse4589_print_and_log("I, %s, have read and understood the course academic integrity policy.\n", your_ubit_name);
				cse4589_print_and_log("[%s:END]\n", command);
			}
			else if(strcmp(cmd,"IP")==0)
			{
				cse4589_print_and_log("[%s:SUCCESS]\n", command);
				cse4589_print_and_log("IP:%s\n", myip);
				cse4589_print_and_log("[%s:END]\n", command);
			}
			else if(strcmp(cmd,"PORT")==0)
			{
				cse4589_print_and_log("[%s:SUCCESS]\n", command);
				cse4589_print_and_log("PORT:%s\n", argv[2]);
				cse4589_print_and_log("[%s:END]\n", command);
			}
			else if(strcmp(cmd,"SENDFILE")==0)
			{
				char recv_ip[100],file_name[100], readbuf[1500];
				int recv_port=0;
				ssize_t read_bytes;
				
				cmd = strtok(NULL," ");
				strcpy(recv_ip,cmd);
				
				cmd = strtok(NULL," ");
				strcpy(file_name,cmd);
				
				for(int pid=0; pid< active_clients;pid++)
				{
					if(strcmp(ip_list[pid],recv_ip)==0)
					{
						recv_port = atoi(port_list[pid]);
						break;
					}
				}
				

				
				
				file_socket =  socket(AF_INET, SOCK_STREAM, 0);
				
				if(file_socket == -1)
				{
					printf("\nSocket not created");
					fflush(stdout);
				}
				
				file_addr.sin_family = AF_INET;
				file_addr.sin_port = htons(recv_port);
				file_addr.sin_addr.s_addr = inet_addr(recv_ip);


				memset(file_addr.sin_zero,'\0',sizeof(file_addr));
				
				file_con_int = connect(file_socket,(struct sockaddr *)&file_addr,sizeof(file_addr));						
				
				if(file_con_int < 0)
				{
					close(file_socket);
					cse4589_print_and_log("[%s:ERROR]\n", command);
					cse4589_print_and_log("[%s:END]\n", command);
					continue;
				}
								
				
				send(file_socket, file_name, sizeof(file_name), 0);
				
				recv(file_socket, readbuf, sizeof(readbuf), 0);
								
				
				if(strcmp(readbuf,"A")==0)
				{
				
					FILE *fp = fopen(file_name, "r");
					
					while(1==1)
					{
					   read_bytes = fread(readbuf, 1, sizeof(readbuf)-1, fp);
					   
					   if(read_bytes <= 0)
					   {
						   break;
					   }
					   
					   send(file_socket, readbuf, read_bytes, 0);
					}

					close(file_socket);
					fclose(fp);			
					
					cse4589_print_and_log("[%s:SUCCESS]\n", command);
					cse4589_print_and_log("[%s:END]\n", command);
				}
				else
				{
					close(file_socket);
					cse4589_print_and_log("[%s:ERROR]\n", command);
					cse4589_print_and_log("[%s:END]\n", command);
				}
			}			
			else if(strcmp(cmd,"SEND")==0)
			{	

				int tot_len = 0;
				char msg[500]="S||",msg_str[500],msg_len_str[2],to_client_ip[100];
				int msg_len=0,valid_client=0;
				
				cmd = strtok(NULL," ");
				strcat(msg,cmd);
				strcpy(to_client_ip,cmd);
								
				for(int pid=0; pid< active_clients;pid++)
				{
					if(strcmp(ip_list[pid],to_client_ip)==0)
					{
						valid_client=1;
						break;
					}
				}
				
				if((xxvalidateip(to_client_ip)==1 )&& (valid_client == 1))
				{
				
				
					strcat(msg,"||");
					strcat(msg,myip);
					strcat(msg,"||");
					
					

					strcpy(msg_str,cmd+14);
					msg_len = strlen(msg_str);
					
					sprintf(msg_len_str, "%d", msg_len);
					
					strcat(msg,msg_len_str);
					
					strcat(msg,"||");
					
					strcat(msg,cmd+14);
					
					tot_len = strlen(msg);
						
					while(1==1)
					{
						
						if(l_ack==0)
						{

							int tot_send=0;
							int tot_left = tot_len;
							while(tot_send < tot_len)
							{	
								send_bytes = send(client_sock,msg+tot_send,tot_left,0);
								if(send_bytes == -1)
								{
									break;
								}
								else
								{
									tot_send = tot_send + send_bytes;
									tot_left = tot_left - send_bytes;
								}
							}
							
							if(tot_send == tot_len)
							{	
						
								l_ack++;
							}
							
						}
						else
						{
							
							rec_bytes = recv(client_sock, buff,sizeof(buff), 0);
							
							if(rec_bytes != -1)
							{
								if(strcmp(buff,"A")==0)
								l_ack--;
							
								cse4589_print_and_log("[%s:SUCCESS]\n", command);
								cse4589_print_and_log("[%s:END]\n", command);
								break;
							}
							
						}
					}
				}
				else
				{
					cse4589_print_and_log("[%s:ERROR]\n", command);
					cse4589_print_and_log("[%s:END]\n", command);
				}
			}
			else if(strcmp(cmd,"BROADCAST")==0)
			{	

				int tot_len = 0;
				char msg[500]="P||",msg_str[500],msg_len_str[2],to_client_ip[100];
				int msg_len=0;
						
				strcat(msg,myip);
				strcat(msg,"||");
				
				
				cmd = strtok(NULL," ");
				strcpy(msg_str,cmd);
				msg_len = strlen(msg_str);
				
				sprintf(msg_len_str, "%d", msg_len);
				
				strcat(msg,msg_len_str);
				
				strcat(msg,"||");
				
				
				strcat(msg,msg_str);
				
				tot_len = strlen(msg);
					
					while(1==1)
					{
						
						if(l_ack==0)
						{

							int tot_send=0;
							int tot_left = tot_len;
							while(tot_send < tot_len)
							{	
								send_bytes = send(client_sock,msg+tot_send,tot_left,0);
								if(send_bytes == -1)
								{
									break;
								}
								else
								{
									tot_send = tot_send + send_bytes;
									tot_left = tot_left - send_bytes;
								}
							}
							
							if(tot_send == tot_len)
							{	
						
								l_ack++;
							}
							
							
						}
						else
						{
							rec_bytes = recv(client_sock, buff,sizeof(buff), 0);
							
							if(rec_bytes != -1)
							{
								if(strcmp(buff,"A")==0)
								l_ack--;
							
								cse4589_print_and_log("[%s:SUCCESS]\n", command);
								cse4589_print_and_log("[%s:END]\n", command);
								break;
							}
							
						}
					}
				
			}
			
			
		}
		else if(i == client_sock)
		{
			
			
			
			char from_client_ip[100],client_msg[500],msg_len_str[100];
			int msg_len=0;
			
			rec_bytes = recv(client_sock, recmsg,sizeof(recmsg), 0);
			
			if(rec_bytes != -1)
			{
			
			rec_head = strtok(recmsg,"||");
			
			if(rec_head!=NULL)
			{
			
			if(strcmp(rec_head,"M")==0)
			{		
			
					rec_head = strtok(NULL,"||");
					strcpy(from_client_ip,rec_head);
					
					rec_head = strtok(NULL,"||");
					strcpy(msg_len_str,rec_head);
									

					msg_len = (atoi(msg_len_str));
					
					
					rec_head = strtok(NULL,"||");
					strcpy(client_msg,rec_head);
					
					msg_len = msg_len - strlen(client_msg);
										
					
					while(msg_len >0)
					{
					 rec_bytes = recv(client_sock, recmsg,sizeof(recmsg), 0);										
					 
					 if(rec_bytes == -1)
					 {
						 
						 break;
					 }
					 else
					 {	

						msg_len = msg_len - strlen(recmsg);  

						strcat(client_msg,recmsg);
					 }
					}
					
					
					

					
					send(client_sock,"A",sizeof("A"),0);
					cse4589_print_and_log("[RECEIVED:SUCCESS]\n");
					cse4589_print_and_log("msg from:%s\n[msg]:%s\n", from_client_ip, client_msg);
					cse4589_print_and_log("[RECEIVED:END]\n");
			
			
			}
			}			
			}
			
			
			
			
			
			
			
		}
		else if(i == file_accept_socket)
		{
			char recv_buff[1500],file_name[100];
			int recv_bytes=0;
			
			file_recv_socket=accept(file_accept_socket,NULL,NULL);
			
			
			recv(file_recv_socket, recv_buff,sizeof(recv_buff), 0);
			
			strcpy(file_name,recv_buff);
			
			send(file_recv_socket, "A", sizeof("A"), 0);
			
			FILE * fprecv = fopen(file_name, "w");
			
			while(1==1)
               {
				  recv_bytes = recv(file_recv_socket, recv_buff, sizeof(recv_buff), 0);
                  
                  if (recv_bytes == 0)
				  {
					  break;
				  }
				  else
				  {
                  fwrite(recv_buff, 1, recv_bytes, fprecv);
				  }
               }
			   
			fclose(fprecv);
			close(file_recv_socket);
			
		}
}
}
}
}
else if(strcmp(argv[1],"s")==0)
{

	
int active_client=0,l,logged_in_clients=0;

int server_con_sock,server_data_sock,i,nbytes,j,bind_int,listen_int,select_int,rec_bytes,send_bytes;
char buff[200];
server_con_sock = socket(AF_INET, SOCK_STREAM, 0);

char ip_list[10][100];
char host_list[10][100];
char port_list[10][100];
char status_list[10][100];
int sd_list[10];
int status_flag_list[10];
char from_block[10][100];
char to_block[10][100];
int msg_sent[10] = {0,0,0,0,0,0,0,0,0,0};
int msg_received[10] = {0,0,0,0,0,0,0,0,0,0};
int block_tot=0;
char myip[100];

struct xxmyaddr myaddr;
myaddr = xxgetmyaddr();

strcpy(myip,myaddr.ip);


if(server_con_sock == -1)
{
        printf("\nSocket not created");
}
else
{

        struct sockaddr_in server_side;
        server_side.sin_family = AF_INET;
        server_side.sin_port= htons(atoi(argv[2]));
        server_side.sin_addr.s_addr= INADDR_ANY;

        memset(server_side.sin_zero,'\0',sizeof(server_side.sin_zero));

        bind_int = bind(server_con_sock,(struct sockaddr*)&server_side,sizeof(server_side));

        if(bind_int == -1)
        {
                printf("\nBind failure");
        }
        else
        {
                int fd_max;
                fd_set pri_list, sec_list;

                FD_ZERO(&pri_list);
                FD_ZERO(&sec_list);


                listen_int = listen(server_con_sock,5);

                if(listen_int == - 1)
                {
                        printf("\nListen failure");
                }
                else
                {

                        FD_SET(server_con_sock, &pri_list);
                        FD_SET(0,&pri_list);
                        fd_max = server_con_sock;

                        while(1==1)
                        {
							sec_list = pri_list;
							select_int = select(fd_max+1, &sec_list, NULL, NULL, NULL);
							

							if(select_int == -1)
							{
									printf("\nSelect failure");
									continue;
							}

							for (i=0;i <= fd_max;i++)
							{
								if (FD_ISSET(i, &sec_list))
								{
									if(i == server_con_sock)
									{
									server_data_sock=accept(server_con_sock,NULL,NULL);
									if(server_data_sock == -1)
									{
											printf("\nAccept failure");
									}
									FD_SET(server_data_sock,&pri_list);
									fd_max = ((fd_max>server_data_sock)?fd_max:server_data_sock);
									printf("\nNew connection received\n");
									}
									else if (i == 0)
									{
									
									char *server_ip = (char*) malloc(sizeof(char)*256);
									memset(server_ip,'\0',256);
									fgets(server_ip, 255, stdin);
									
									
									int cmd_len = strlen(server_ip);
									char *cmd;
									
									if (cmd_len > 0 && server_ip[cmd_len-1] == '\n')
									{
										server_ip[--cmd_len] = '\0';
									}
									
									cmd = strtok(server_ip," ");
									
									
									if(strcmp(cmd,"LIST")==0)
									{
										int r=0;
										char temp[100],temp_no;

										for(r=0; r< logged_in_clients;r++)
										{
										for(int s=r+1;s<logged_in_clients;s++)
										{
										if(atoi(port_list[r])>atoi(port_list[s]))
										{	 					 
											strcpy(temp,port_list[r]);
											strcpy(port_list[r],port_list[s]);
											strcpy(port_list[s],temp);
											
											strcpy(temp,ip_list[r]);
											strcpy(ip_list[r],ip_list[s]);
											strcpy(ip_list[s],temp);
											
											strcpy(temp,host_list[r]);
											strcpy(host_list[r],host_list[s]);
											strcpy(host_list[s],temp);
											
											strcpy(temp,status_list[r]);
											strcpy(status_list[r],status_list[s]);
											strcpy(status_list[s],temp);
											
											temp_no = sd_list[r];
											sd_list[r] = sd_list[s];
											sd_list[s] = temp_no;
											
											temp_no = status_flag_list[r];
											status_flag_list[r] = status_flag_list[s];
											status_flag_list[s] = temp_no;
											
											temp_no = msg_sent[r];
											msg_sent[r] = msg_sent[s];
											msg_sent[s] = temp_no;
											
											temp_no = msg_received[r];
											msg_received[r] = msg_received[s];
											msg_received[s] = temp_no;
																							 
										}
										}
										}
										cse4589_print_and_log("[%s:SUCCESS]\n", server_ip);
										for(r=0; r< logged_in_clients;r++)
										{
											int iport=atoi(port_list[r]);					
											cse4589_print_and_log("%-5d%-35s%-20s%-8d\n", (r+1), host_list[r], ip_list[r], iport);
										}
										 cse4589_print_and_log("[%s:END]\n", server_ip);
										
										
									}
									else if(strcmp(cmd,"BLOCKED")==0)
									{
										char from_ip[100], to_ip_buff[10][100],to_host_buff[10][100],to_port_buff[10][100],temp[100];
										int buff_cnt=0,valid_client=0;
										
										cmd = strtok(NULL," ");
										strcpy(from_ip, cmd);
										
										for(int rc=0; rc< active_client;rc++)
										{
											if(strcmp(from_ip,ip_list[rc]))
											{
												valid_client=1;
											}
										}
																			
										if(valid_client == 1 && xxvalidateip(from_ip) == 1)
										{
										
											
											for(l=0;l<block_tot;l++)
											{
												if(strcmp(from_ip,from_block[l])==0)
												{
													strcpy(to_ip_buff[buff_cnt],to_block[l]);
													for(int r=0; r< logged_in_clients;r++)
													{
														if(strcmp(to_ip_buff[buff_cnt],ip_list[r])==0)
														{
															strcpy(to_host_buff[buff_cnt],host_list[r]);
															strcpy(to_port_buff[buff_cnt],port_list[r]);
														}
													}
													
													buff_cnt++;
													
												}
											}
											
											
											for(int r=0; r< buff_cnt;r++)
											{
											for(int s=r+1;s<buff_cnt;s++)
											{
											if(atoi(to_port_buff[r])>atoi(to_port_buff[s]))
											{	 					 
												strcpy(temp,to_port_buff[r]);
												strcpy(to_port_buff[r],to_port_buff[s]);
												strcpy(to_port_buff[s],temp);
												
												strcpy(temp,to_ip_buff[r]);
												strcpy(to_ip_buff[r],to_ip_buff[s]);
												strcpy(to_ip_buff[s],temp);
												
												strcpy(temp,to_host_buff[r]);
												strcpy(to_host_buff[r],to_host_buff[s]);
												strcpy(to_host_buff[s],temp);
																								 
											}
											}
											}
											
											cse4589_print_and_log("[%s:SUCCESS]\n", server_ip);
											for(int r=0; r< buff_cnt;r++)
											{
												int iport=atoi(to_port_buff[r]);					
												cse4589_print_and_log("%-5d%-35s%-20s%-8d\n", (r+1), to_host_buff[r], to_ip_buff[r], iport);
											}
											 cse4589_print_and_log("[%s:END]\n", server_ip);
										}
										else
										{
											cse4589_print_and_log("[%s:ERROR]\n", server_ip);
											cse4589_print_and_log("[%s:END]\n", server_ip);
											
											
										}
										
									} 
									
									else if(strcmp(cmd,"STATISTICS")==0)
									{
										
										int r=0;
										char temp[100],temp_no;

										for(r=0; r< active_client;r++)
										{
										for(int s=r+1;s<active_client;s++)
										{
										if(atoi(port_list[r])>atoi(port_list[s]))
										{	 					 
											strcpy(temp,port_list[r]);
											strcpy(port_list[r],port_list[s]);
											strcpy(port_list[s],temp);
											
											strcpy(temp,ip_list[r]);
											strcpy(ip_list[r],ip_list[s]);
											strcpy(ip_list[s],temp);
											
											strcpy(temp,host_list[r]);
											strcpy(host_list[r],host_list[s]);
											strcpy(host_list[s],temp);
											
											strcpy(temp,status_list[r]);
											strcpy(status_list[r],status_list[s]);
											strcpy(status_list[s],temp);
											
											temp_no = sd_list[r];
											sd_list[r] = sd_list[s];
											sd_list[s] = temp_no;
											
											temp_no = status_flag_list[r];
											status_flag_list[r] = status_flag_list[s];
											status_flag_list[s] = temp_no;
											
											temp_no = msg_sent[r];
											msg_sent[r] = msg_sent[s];
											msg_sent[s] = temp_no;
											
											temp_no = msg_received[r];
											msg_received[r] = msg_received[s];
											msg_received[s] = temp_no;
																							 
										}
										}
										}
										cse4589_print_and_log("[%s:SUCCESS]\n", server_ip);
										for(r=0; r< active_client;r++)
										{
											int iport=atoi(port_list[r]);					
											cse4589_print_and_log("%-5d%-35s%-8d%-8d%-8s\n", (r+1), host_list[r], msg_sent[r], msg_received[r], status_list[r]);
										}
										 cse4589_print_and_log("[%s:END]\n", server_ip);
										
										
										
									} 
									
									else if(strcmp(cmd,"AUTHOR")==0)
									{
										char your_ubit_name[] = "dhairyah";
										cse4589_print_and_log("[%s:SUCCESS]\n", server_ip);
										cse4589_print_and_log("I, %s, have read and understood the course academic integrity policy.\n", your_ubit_name);
										cse4589_print_and_log("[%s:END]\n", server_ip);
									} 
									else if(strcmp(cmd,"IP")==0)
									{
										cse4589_print_and_log("[%s:SUCCESS]\n", server_ip);
										cse4589_print_and_log("IP:%s\n", myip);
										cse4589_print_and_log("[%s:END]\n", server_ip);
									}
									else if(strcmp(cmd,"PORT")==0)
									{
										cse4589_print_and_log("[%s:SUCCESS]\n", server_ip);
										cse4589_print_and_log("PORT:%s\n", argv[2]);
										cse4589_print_and_log("[%s:END]\n", server_ip);
									}	
									
									
									free(server_ip);
									}
									else
									{

									char *client_data = (char*) malloc(sizeof(char)*500);
									memset(client_data,'\0',500);
									rec_bytes = recv(i, client_data, 500, 0);
									
									if(rec_bytes == 0)
									{
									printf("\nConnection closed");
									close(i);
									FD_CLR(i,&pri_list);
									}
									else if(rec_bytes < 0)
									{
									printf("Error in rec");
									close(i);
									FD_CLR(i,&pri_list);
									}
									else
									{
									char *inst, client_list[200] ="T||", active_client_str[2];
									inst = strtok(client_data,"||");
									

									if(strcmp(inst,"L")==0)
									{	
										inst = strtok(NULL, "||");
										strcpy(ip_list[active_client],inst);
										
										inst = strtok(NULL, "||");
										strcpy(host_list[active_client],inst);
										
										inst = strtok(NULL, "||");
										strcpy(port_list[active_client],inst);
										
										inst = strtok(NULL, "||");
										strcpy(status_list[active_client],inst);
										
										status_flag_list[active_client] = 1;
										
										sd_list[active_client] = i;

										active_client_str[0] = logged_in_clients+1+'0';
										active_client_str[1] = '\0';
										strcat(client_list,active_client_str);
										strcat(client_list,"||");
																				
										
										for(l=0;l<=active_client;l++)
										{
											
										if(status_flag_list[l] == 1)
										{
										
										strcat(client_list,ip_list[l]);
										strcat(client_list,"||");
										
										strcat(client_list,host_list[l]);
										strcat(client_list,"||");
										
										strcat(client_list,port_list[l]);
										if(l <= (active_client-1))
										{
										strcat(client_list,"||");
										}
										}
										}

										send_bytes = send(i,client_list,sizeof(client_list),0);
										printf("\nSend : %s", client_list);
										fflush(stdout);
										if (send_bytes == - 1)
										{
										printf("\nSending failure");
										}
										
										active_client++;
										logged_in_clients++;
										
									}
									else if(strcmp(inst,"Y")==0)
									{
										inst = strtok(NULL, "||");
										
										
										for(l=0;l<active_client;l++)
										{
										 if(strcmp(inst,ip_list[l])==0)
										 {
										  status_flag_list[l] = 0;
										  strcpy(status_list[l],"logged-out");
										  logged_in_clients--;
										  break;
										 }
										}
										
									}
									
									else if(strcmp(inst,"E")==0)
									{
										int client_id;
										inst = strtok(NULL, "||");										
										
										for(l=0;l<active_client;l++)
										{
										 if(strcmp(inst,ip_list[l])==0)
										 {
										  client_id = l;
										  break;
										 }
										}
										
										if(status_flag_list[client_id] == 1)
										{
											logged_in_clients--;
										}
										
										for(l=client_id;l<active_client-1;l++)
										{
										 
											strcpy(ip_list[l],ip_list[l+1]);
											strcpy(host_list[l],host_list[l+1]);
											strcpy(port_list[l],port_list[l+1]);
											strcpy(status_list[l],status_list[l+1]);
											status_flag_list[l] = status_flag_list[l+1];
											sd_list[l] = sd_list[l+1];
											msg_received[l]=msg_received[l+1];
											msg_sent[l]=msg_sent[l+1];
										 
										}
										
										active_client--;
										
										
										
									}
									else if(strcmp(inst,"R")==0)
									{
										
										active_client_str[0] = logged_in_clients+'0';
										active_client_str[1] = '\0';
										strcat(client_list,active_client_str);
										strcat(client_list,"||");
										
										for(l=0;l<active_client;l++)
										{
											
										if(status_flag_list[l] == 1)
										{
										
										strcat(client_list,ip_list[l]);
										strcat(client_list,"||");
										
										strcat(client_list,host_list[l]);
										strcat(client_list,"||");
										
										strcat(client_list,port_list[l]);
										if(l < (active_client-1))
										{
										strcat(client_list,"||");
										}
										}
										}
										send_bytes = send(i,client_list,sizeof(client_list),0);
										
										
									}	
									else if(strcmp(inst,"U")==0)
									{
										inst = strtok(NULL, "||");
										strcpy(to_block[block_tot],inst);
										
										inst = strtok(NULL, "||");
										strcpy(from_block[block_tot],inst);
										
										block_tot++;
										
										
									}
									else if(strcmp(inst,"W")==0)
									{
										char from_ip[100],to_ip[100];
										int block_id = 0;
										
										inst = strtok(NULL, "||");
										strcpy(to_ip,inst);
										
										inst = strtok(NULL, "||");
										strcpy(from_ip,inst);
										
										for(l=0;l<block_tot;l++)
										{
											if(strcmp(from_ip,from_block[l])==0 && strcmp(to_ip,to_block[l])==0)
											{
												block_id = l;
											}
										}
										
										for(l=block_id;l<block_tot-1;l++)
										{
											strcpy(from_block[l],from_block[l+1]);
											strcpy(to_block[l],to_block[l+1]);
										}
										
										
										block_tot--;
										
										
									}
									else if(strcmp(inst,"S")==0)
									{
																		
										char rec_msg[500], send_msg[500]="M||",from_ip[100],to_ip[100],msg[500],msg_len_str[100];
										int to_client_id,msg_len=0,block_flag=0,from_client_no,to_client_no;
										
										
										inst = strtok(NULL, "||");
										//strcat(send_msg,inst);
										strcpy(to_ip,inst);										
										
										for(l=0;l<active_client;l++)
										{
										 if(strcmp(inst,ip_list[l])==0)
										 {
										  to_client_id = sd_list[l];
										  to_client_no = l;
										  break;
										 }
										}
										
										
										//strcat(send_msg,"||");
										
										inst = strtok(NULL, "||");
										strcat(send_msg,inst);
										strcpy(from_ip,inst);
										
										for(l=0;l<active_client;l++)
										{
										 if(strcmp(from_ip,ip_list[l])==0)
										 {
										  from_client_no = l;
										  break;
										 }
										}
										
										for(l=0;l<block_tot;l++)
										{
											if(strcmp(from_ip,to_block[l])==0 && strcmp(to_ip,from_block[l])==0)
											{
												block_flag=1;
											}
										}
										if(block_flag == 1)
										{
											send_bytes = send(i,"A",sizeof("A"),0);
										}
										else
										{
											strcat(send_msg,"||");
											
											inst = strtok(NULL, "||");
											
											strcpy(msg_len_str,inst);
											msg_len = atoi(msg_len_str);
											strcat(send_msg,inst);
											strcat(send_msg,"||");
											
											inst = strtok(NULL, "||");
											
											strcat(send_msg,inst);
											strcpy(msg,inst);
											
											msg_len = msg_len - strlen(msg); 
											
											while(msg_len !=0)
											{
												
											 rec_bytes = recv(i, rec_msg,sizeof(rec_msg), 0);										
											 
											 if(rec_bytes == -1)
											 {
												 
												 break;
											 }
											 else
											 {
												msg_len = msg_len - strlen(rec_msg);  
												strcat(send_msg,rec_msg);
												strcat(msg,rec_msg);
											 }
											}
										
											
																					
											int tot_len = strlen(send_msg);
											int tot_send=0;
											int tot_left = tot_len;
											while(tot_send < tot_len)
											{
												send_bytes = send(to_client_id,send_msg,sizeof(send_msg),0);
												if(send_bytes == -1)
												{
													break;
												}
												else
												{	
													tot_send = tot_send + send_bytes;
													tot_left = tot_left - send_bytes;
												}
											}
											
											//while(1==1)
											//{	
										
												rec_bytes = recv(to_client_id, rec_msg, sizeof(rec_msg), 0);
												
												if(rec_bytes == -1)
												{
													
													break;
												}
												else
												{
													
													if(strcmp(rec_msg,"A")==0)
														
													{
														send_bytes = send(i,"A",sizeof("A"),0);
														msg_received[to_client_no] = msg_received[to_client_no]+1;
														msg_sent[from_client_no]= msg_sent[from_client_no]+1;
														cse4589_print_and_log("[RELAYED:SUCCESS]\n");
														cse4589_print_and_log("msg from:%s, to:%s\n[msg]:%s\n", from_ip, to_ip, msg);
														cse4589_print_and_log("[RELAYED:END]\n");
														
														break;
													}
												}
											//}
										}
														
																	
									}
									else if(strcmp(inst,"P")==0)
									{
									
										char rec_msg[500], send_msg[500]="M||",from_ip[100],to_ip[100],msg[500],msg_len_str[100];
										int to_client_id,msg_len=0,block_flag=0;
										
										inst = strtok(NULL, "||");
										strcpy(from_ip,inst);
										
										strcat(send_msg,inst);
										strcat(send_msg,"||");
										
										
										inst = strtok(NULL, "||");
											
										strcpy(msg_len_str,inst);
										msg_len = atoi(msg_len_str);
										strcat(send_msg,inst);
										strcat(send_msg,"||");
										
										inst = strtok(NULL, "||");
										
										strcat(send_msg,inst);
										strcpy(msg,inst);
										
										printf("\nMessage : %s",msg);
										fflush(stdout);
										
										msg_len = msg_len - strlen(msg); 
										
										while(msg_len !=0)
										{
											
										 rec_bytes = recv(i, rec_msg,sizeof(rec_msg), 0);										
										 
										 if(rec_bytes == -1)
										 {
											 
											 break;
										 }
										 else
										 {
											msg_len = msg_len - strlen(rec_msg);  
											strcat(send_msg,rec_msg);
											strcat(msg,rec_msg);
										 }
										}
										
										
										printf(" Active client : %d",active_client);
										
										for(l=0;l<active_client;l++)
										{
											
										if(strcmp(from_ip,ip_list[l])!=0)
										{
										
										if(status_flag_list[l] == 1)
										{
										
										to_client_id = sd_list[l];
										
										
										for(int ll=0;ll<block_tot;ll++)
										{
											if(strcmp(from_ip,to_block[ll])==0 && strcmp(to_ip,from_block[ll])==0)
											{
												block_flag=1;
											}
										}
										if(block_flag == 1)
										{
											continue;
										}
										else
										{																						
											int tot_len = strlen(send_msg);
											int tot_send=0;
											int tot_left = tot_len;
											while(tot_send < tot_len)
											{
												send_bytes = send(to_client_id,send_msg,sizeof(send_msg),0);
												if(send_bytes == -1)
												{
													break;
												}
												else
												{	
													tot_send = tot_send + send_bytes;
													tot_left = tot_left - send_bytes;
												}
											}
												
												rec_bytes = recv(to_client_id, rec_msg, sizeof(rec_msg), 0);
												
												if(rec_bytes == -1)
												{
													break;
												}
												else
												{
													
													if(strcmp(rec_msg,"A")==0)	
													{
														
													 continue;
													}
												}
										}

										}	
										}
										}
									send_bytes = send(i,"A",sizeof("A"),0);	
									cse4589_print_and_log("[RELAYED:SUCCESS]\n");
									cse4589_print_and_log("msg from:%s, to:255.255.255.255\n[msg]:%s\n", from_ip,msg);
									cse4589_print_and_log("[RELAYED:END]\n");	
										
										
										
										
										
										
									}
									
									free(client_data);
									}
									}
								}
							}

                        }

                        close(server_con_sock);
                }
        }
}


	
}	

	return 0;
}



struct xxmyaddr xxgetmyaddr()
{
        struct xxmyaddr myaddr;

        int mysock, sock_len;
        struct sockaddr_in server_end, myend;

        mysock =  socket(AF_INET,SOCK_DGRAM, 0);

        server_end.sin_family = AF_INET;
        server_end.sin_port = htons(53);
        server_end.sin_addr.s_addr = inet_addr("8.8.8.8");
        memset(server_end.sin_zero,'\0',sizeof(server_end));

        connect(mysock,(struct sockaddr *)&server_end,sizeof(server_end));

        sock_len = sizeof(myend);
        getsockname(mysock,(struct sockaddr*)&myend, &sock_len);
        strcpy(myaddr.ip,inet_ntoa(myend.sin_addr));

        struct hostent *he;
        struct in_addr ipaddr;
        ipaddr = myend.sin_addr;
        he = gethostbyaddr(&ipaddr, sizeof(ipaddr), AF_INET);

        strcpy(myaddr.hostname,he->h_name);
        return myaddr;
}


int xxvalidateip(char *ipv4)
{
    struct sockaddr_in ipc;
    return inet_pton(AF_INET, ipv4, &(ipc.sin_addr));
}

int xxvalidateport(char *port)
{
        return 1;
}


int xxvalidatearg(char *comm, char *arg)
{
        return 1;
}
