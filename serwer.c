#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


char msg[1024];
char users_tab[30][15];
int users=0;
char blok[5];

//funkcja czytajaca z dyskryptora
int reader(int i,char sep){
    int count = 0;
    while (1) {
        read(i, msg+count, 1);
        if (msg[count] == sep) {
            return count;
        }
        else 
            count++;
    }
    return count;
}

//funkcja czytajaca z bufora do podanego stringa bez ostatniego znaku (wazne)
void cut_the_last(char* u){
    int i=0;
    while (msg[i]!='\n'){
        u[i]=msg[i];
        i++;
    }
    while(i<20){
        u[i]='\0';
        i++;
    }
}
//send_to wiadomosci sprawdza czy wszystko wyslane
void send_to(int i){
    int toSend = strlen(msg);
    int dataCount = 0, w;
    while (dataCount < toSend) {
        w = write(i, msg+dataCount, toSend-dataCount);
        dataCount += w;
    }

}
//czyta linie z pliku txt
int read_the_line(FILE* plik,char *linia){

    char ch = getc(plik);
    int count = 0;
    int end=0;

    while ((ch != '\n') && (ch != EOF)) {
        
        linia[count] = ch;
        count++;
        ch = getc(plik);

        end++;
    }
    
    return end;
}

//sprawdza czy uzytkownik jest juz zasubskrobawny funkcja strstr nie dawala satysfakcjonujacych wynikow
int chech_subs(char* linia, char* uzy){
    int i=0;
    int x=0;
    int czy=0;

    while(linia[i]!='\n' && i<strlen(linia) && linia[i]!='\0'){
        if (czy==strlen(uzy) && (linia[i]==' ' || linia[i]=='\n')) return 1;
        else if (czy==strlen(uzy)){
            czy=0;
            i++;
        }
        if(linia[i]==' '){
            i++;
            x=0;
            if (czy==strlen(uzy)) return 1;
            czy=0;
        }
        else if(x>strlen(uzy)) {
            i++;
            czy=0;
        }
        else if(linia[i]==uzy[x]){
            czy++;
            i++;
            x++;
        }
        else{
            i++;
            x=0;
            czy=0;
        }
    }
    if (czy==strlen(uzy)) return 1;
    return 0;
    
}
//czysci bufor ze smieci
void clear_msg(){
    for (int i=0;i<1024;i++){
        msg[i]='\0';
    }
}
//czysci tablice ze smieci
void clear_table(char* b,int z){
    for (int i=0;i<z;i++){
        b[i]='\0';
    }
}

//wczytuje uzytkownikow to tabeli users_tab wazne przy logowaniu i rejsetracji
void take_users(){
	FILE* plik=fopen("users.txt","r");
	char *buf;
    buf=malloc(20*sizeof(char));
	int chck=read_the_line(plik,buf);
	int i=0;
	while (chck!=0){
	   for(int x=0;x<strlen(buf);x++){
	   	users_tab[i][x]=buf[x];

	   }

	   i++;
	   users++;
       clear_table(buf,20);
       chck=read_the_line(plik,buf);
	}
    free(buf);
    //printf("%d",users);
	fclose(plik);
}


int log_in(int i){
    char usr[20];
    reader(i,'\n');
    cut_the_last(usr);
    printf("%s",usr);
	printf("\n");
    FILE* plik=fopen("users.txt", "r");

	char line_log[40];
 
	int if_EOF=fscanf(plik,"%s\n",line_log);

	int if_log_in=0;
    int which=0;
	while (if_EOF!= EOF){
	if (strcmp(usr,line_log)==0 ){

		if_log_in=1;
        break;
       }
       if_EOF=fscanf(plik,"%s",line_log);
       which++;
    }
	fclose(plik);
	return if_log_in;
}

//czy w tej lini jest uzytkownik zalogowany(tzn pierwszy)
int chech_whom(char *p,char *user){
    int c=0;


    while(p[c]!= ' ' && p[c]!='\0' && p[c]!='\n'){
        
        if(p[c]==user[c]){
            c++;
        }
        else{
            return 0;
        }
    }
    if(c==strlen(user)){ return 1;}
    else{return 0;}
}
//sprawdza czy linia w pliku jest pusta
int isEmpty(const char *str)
{
    char ch;

    do
    {
        ch = *(str++);

        // Check non whitespace character
        if(ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r' && ch != '\0')
            return 0;

    } while (ch != '\0');

    return 1;
}

/**
 * Function to remove empty lines from a file.
 */
void removeEmptyLines(FILE *srcFile, FILE *tempFile)
{
    char buffer[1000];

    while ((fgets(buffer, 1000, srcFile)) != NULL)
    {
        /* If current line is not empty then write to temporary file */
        if(!isEmpty(buffer))
            fputs(buffer, tempFile);
    }
}

int subscribe(int i){
    char usr[20];
    char *t;
    char *line_to_add;
    char new_sub[20];
    
    take_users();
    clear_msg();
    reader(i,'\n');
    cut_the_last(usr);
    printf("%s\n",usr);
    
    clear_msg();
    while(msg[0]=='\0' || msg[0]=='\n'){
    reader(i,'\n');
    }
    cut_the_last(new_sub);
    printf("%s\n",new_sub);
    FILE* plik_s=fopen("sub.txt","w");
    fclose(plik_s);
    int check_user=0;

    if(strcmp(usr,new_sub)==0){ 
        
        return -2;
        }
    for (int i=0;i<users;i++){
        if (strcmp(users_tab[i],new_sub)==0){
            check_user=1;
        }
    }
    if (check_user==0){
        
        return 0;
    }
    //zainisjowanie lini jedna zapamietuje linie z pliku(t-zmienna pomocnicza) druga (line_to_add) zapamietuje subskrybcje uzytkownika 
    line_to_add=(char*)malloc(100*sizeof(char));
    t=(char*)malloc(100*sizeof(char));
    //blokowanie pisanie do tego samego pliku w tym samym momencie
    FILE* blok2=fopen("blok","r");
    read_the_line(blok2,blok);
    fclose(blok2);
    //patrzymy czy jest ktos juz zablokowany                
    while (blok[0]=='1') {
        sleep(1);
        blok2=fopen("blok","r");
        read_the_line(blok2,blok);
        fclose(blok2);
    }
    FILE* blok3=fopen("blok","w");
    fputs("1",blok3);
    fclose(blok3);
    FILE* plik2=fopen("subskrybcje.txt", "r");
    FILE* plik1=fopen("sub.txt","a");
    int czy=1;
    while(czy!=0){

        clear_table(t,100);
        czy=read_the_line(plik2,t);
        if (chech_whom(t,usr)==1){
            if(chech_subs(t,new_sub)){ 
                blok3=fopen("blok","w");
                fputs("0",blok3);
                fclose(blok3);
                printf("%s\n",t);
                return -1;
             }
            strcpy(line_to_add,t);
            clear_table(t,100);
        }
        else{
            fputs(t,plik1);
            fputs("\n",plik1);
            clear_table(t,100);
        }
    }
    //dodanie lini z nowa subskrybcja
    strcat(line_to_add," ");
    strcat(line_to_add,new_sub);
    fputs(line_to_add,plik1);
    

    fclose(plik1);
    fclose(plik2);
    //podmiana plik przy okazji usuwam puste linie
    plik1=fopen("sub.txt","r");
    plik2=fopen("subskrybcje.txt","w");
    removeEmptyLines(plik1,plik2);
    fclose(plik1);

    fclose(plik2);
    free(t);
    free(line_to_add);
    blok3=fopen("blok","w");
    fputs("0",blok3);
    fclose(blok3);
    return 1;
}


int add_post(int i){
    char usr[20];
    clear_msg();
    reader(i,'\n');
    cut_the_last(usr);
    
    take_users();

    char katalog[50];
    char linia[200];
    int czy=1;
    FILE* plik1;
    clear_msg();
    while(msg[0]=='\0' || msg[0]=='\n'){
        reader(i,'\n');
    }
    FILE* blok2=fopen("blok","r");
    read_the_line(blok2,blok);
    fclose(blok2);
    //patrzymy czy jest ktos juz zablokowany                
    while (blok[0]=='1') {
        sleep(1);
        blok2=fopen("blok","r");
        read_the_line(blok2,blok);
        fclose(blok2);
    }
    FILE* blok3=fopen("blok","w");
    fputs("1",blok3);
    fclose(blok3);
    FILE* plik_suby=fopen("subskrybcje.txt","r");
    
    while(czy!=0){
        clear_table(linia,200);
        czy=read_the_line(plik_suby,linia);
        //printf("1-%s\n",linia);
        if (chech_whom(linia,usr)){
            NULL;
        }
        else if(chech_subs(linia,usr)){
            //printf("2-%s\n",linia);
            for(int c=0;c<users;c++){
                //printf("%s\n",users_tab[c]);
                if(chech_whom(linia,users_tab[c])){

                    clear_table(katalog,50);
                    strcpy(katalog,"wpisy/");
                    strcat(katalog,users_tab[c]);
                    plik1=fopen(katalog,"a");
                    fputs(usr,plik1);
                    fputs(msg,plik1);
                    //fputs("\n",plik1);
                    fclose(plik1);
                    
                }
            }
        }
    }
    blok3=fopen("blok","w");
    fputs("0",blok3);
    fclose(plik_suby);
    return 1;
}

int r_in(int i){
    // clear_msg();
    // strcpy(msg,"podaj login");
    // send_to(i);
    char nowy_uzytkownik[20];
    clear_msg();
    reader(i,'\n');
    cut_the_last(nowy_uzytkownik);

    for(int i=0;i<users;i++){
        if(strcmp(users_tab[i],nowy_uzytkownik)==0){ return 0;}
    }

    FILE* plik1=fopen("subskrybcje.txt","a");
    fputs("\n",plik1);
    fputs(nowy_uzytkownik,plik1);
    fclose(plik1);
    plik1=fopen("users.txt","a");
    fputs("\n",plik1);
    fputs(nowy_uzytkownik,plik1);
    fclose(plik1);
    char katalog[30];
    strcpy(katalog,"wpisy/");
    strcat(katalog,nowy_uzytkownik);
    plik1=fopen(katalog,"w");
    fclose(plik1);
    FILE* pliktmp=fopen("sub.txt","w");
    plik1=fopen("users.txt","r");
    removeEmptyLines(plik1,pliktmp);
    fclose(plik1);
    fclose(pliktmp);
    remove("users.txt");
    rename("sub.txt","users.txt");
    pliktmp=fopen("sub.txt","w");
    plik1=fopen("subskrybcje.txt","r");
    removeEmptyLines(plik1,pliktmp);
    fclose(plik1);
    fclose(pliktmp);
    remove("subskrybcje.txt");
    rename("sub.txt","subskrybcje.txt");
    pliktmp=fopen("sub.txt","w");
    fclose(pliktmp);

    return 1;


}

void take_posts(int x){
    char usr[20];
    clear_msg();
    reader(x,'\n');
    cut_the_last(usr);
    char *katalog=malloc(50*sizeof(char));
    strcpy(katalog,"wpisy/");
    strcat(katalog,usr);
    FILE *fd;
    char len[400];        
    int i;
    int ch;
    int count;
    int g=0;
    fd = fopen(katalog, "r");
    char* post=(char*)malloc(1024*sizeof(char));
    clear_table(post,1024);
    fseek(fd, 0, SEEK_END);
    while (ftell(fd) > 1 ){
        fseek(fd, -2, SEEK_CUR);
        if(ftell(fd) <= 2)
             break;
        ch =fgetc(fd);
        count = 0;
        while(ch != '\n'){
            len[count++] = ch;
            if(ftell(fd) < 2)
                break;
            fseek(fd, -2, SEEK_CUR);
            ch =fgetc(fd);
        }
        for (i =count -1 ; i >= 0 && count > 0  ; i--){
            post[g++]=len[i];
        }
        post[g++]='\n';
    }
    
    sprintf(msg,"%ld",strlen(post)-1);
    printf("%s\n",msg);
    strcat(msg,"\n");
    send_to(x);
    fclose(fd);
    clear_msg();
    strcpy(msg,post);
    send_to(x);



}


void subs_to_user(int i){
    char usr[20];
    clear_msg();
    reader(i,'\n');
    cut_the_last(usr);
    printf("%s\n",usr);
    
    FILE* plik2=fopen("subskrybcje.txt", "r");
    int czy=1;
    char* t=(char*)malloc(100*sizeof(char));
    while(czy!=0){
        clear_table(t,100);
        czy=read_the_line(plik2,t);
        if (chech_whom(t,usr)==1){
            t=t+strlen(usr)+1;
            break;
        }
    }
    
    printf("%s\n",t);
    strcpy(msg,t);
    strcat(msg,"\n");

    send_to(i);
}



int main(int argc, char **argv){
    socklen_t slt;
    int sfd, cfd, on = 1;
    struct sockaddr_in saddr, caddr;
    

    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(1234);
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
    bind(sfd, (struct sockaddr*) &saddr, sizeof(saddr));
    listen(sfd, 10);
    take_users();

    while(1){

        slt = sizeof(caddr);
        
        cfd = accept(sfd, (struct sockaddr*)&caddr, &slt);
        printf("new connection: %s\n",
             inet_ntoa((struct in_addr)caddr.sin_addr));
        if (fork()==0){
            strcpy(msg,"zaloguj lub zarejstruj jesli nie masz konta\n");
            send_to(cfd);
            int if_log_in;
            clear_msg();
            reader(cfd,'\n');
            
            take_users();
            if(msg[0]=='1'){
	            if_log_in=log_in(cfd);
                if (if_log_in){
                    strcpy(msg,"zalogowano\n");
            	    send_to(cfd);
                }
                else{
            	    strcpy(msg,"nie zalogowano\n");
                    send_to(cfd);
                }
            }
            else if(msg[0]=='2'){
                if_log_in=r_in(cfd);
                if (if_log_in){
                    strcpy(msg,"zalogowano\n");
            	    send_to(cfd);
                }
                else{
            	    strcpy(msg,"nie zalogowano\n");
                    send_to(cfd);
                }
            }  
            
            

            clear_msg();
	        while(if_log_in){   
                
	            reader(cfd, '\n');
                printf("%s\n",msg);
                
                if(strcmp("subskrybuj\n", msg) == 0){
                    //blokowanie pisanie do tego samego pliku w tym samym momencie
                    FILE* blok2=fopen("blok","r");
                    read_the_line(blok2,blok);
                    fclose(blok2);
                    
                    while (blok[0]=='1') {
                        sleep(1);
                        blok2=fopen("blok","r");
                        read_the_line(blok2,blok);
                        fclose(blok2);
                    }
                    int czy_dodano=subscribe(cfd);
                    
                    if (czy_dodano>0){
                        clear_msg();
                        strcpy(msg,"1\n");
                    }
                    else{ 
                        clear_msg();
                        strcpy(msg,"0\n");
                    }
                    send_to(cfd);
                }
                else if(strcmp(msg, "dodaj\n") == 0){
                    
                    int czy_dodano=add_post(cfd);
                    if (czy_dodano){
                        clear_msg();
                        strcpy(msg,"dodano\n");
                    }
                    else{
                        clear_msg();
                        strcpy(msg,"nie dodano\n");
                    }
                    send_to(cfd);
                    
                } else if(strcmp(msg,"wpisy\n")==0){
                    take_posts(cfd);
                }
                else if(strcmp(msg,"subs\n")==0){
                    subs_to_user(cfd);
                }
                else if(strcmp(msg,"koniec\n")==0){ 
                    clear_msg();
                    break;
                }
                take_users();
                clear_msg();
            }

        }
        close(cfd);
    }
    close(sfd);
    return EXIT_SUCCESS;
}
