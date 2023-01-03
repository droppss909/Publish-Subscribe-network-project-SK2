#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


char msg[1024];
char uzytkownicy[30][15];
int users=0;

int odczyt(int i,char sep){
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
void wysylanie(int i){
    int toSend = strlen(msg);
    int dataCount = 0, w;
    while (dataCount < toSend) {
        w = write(i, msg+dataCount, toSend-dataCount);
        dataCount += w;
    }

}
int czytaj_linie(FILE* plik,char *linia){

    char ch = getc(plik);
    int count = 0;
    int koniec=0;

    while ((ch != '\n') && (ch != EOF)) {
        linia[count] = ch;
        count++;
        ch = getc(plik);
        koniec++;
    }
    
    return koniec;
}

void wczytaj_uzytkownikow(){
	FILE* plik=fopen("users.txt","r");
	char *buf;
    buf=malloc(20*sizeof(char));
	int sprawdzenie=czytaj_linie(plik,buf);
	int i=0;
	while (sprawdzenie!=0){
	   for(int x=0;x<strlen(buf);x++){
	   	uzytkownicy[i][x]=buf[x];

	   }

	   i++;
	   users++;
       free(buf);
       buf=malloc(20*sizeof(char));
       sprawdzenie=czytaj_linie(plik,buf);
	}
    //printf("%d",users);
	fclose(plik);
}

void czyszczenie(){
    for (int i=0;i<1024;i++){
        msg[i]='\0';
    }
}

int zaloguj(int i){
    char uzytkownik[20];
    read(i,uzytkownik,20);
    printf("%s",uzytkownik);
	printf("\n");
    FILE* plik=fopen("users.txt", "r");
	char logowanie[40];
	int sprawdzanie_czy_nie_EOF=fscanf(plik,"%s\n",logowanie);
	int czy_zalogowany=0;
	while (sprawdzanie_czy_nie_EOF!= EOF){
	if (strcmp(uzytkownik,logowanie)==0){
		
		czy_zalogowany=1;
        break;
       }
       sprawdzanie_czy_nie_EOF=fscanf(plik,"%s",logowanie);
    }
	fclose(plik);
	return czy_zalogowany;
}

//czy w tej lini jest uzytkownik zalogowany(tzn pierwszy w lini)
int czy_uzytkownik(char *p,char *uzytkownik){
    int c=0;
    int wynik=0;

    while(p[c]!= ' ' && p[c]!='\0'){
        
        if(p[c]==uzytkownik[c]){
            c++;
        }
        else{
            return 0;
        }
    }
    if(c==strlen(uzytkownik)){ return 1;}
    else{return 0;}
}
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

int subskrybuj(int i){
    char uzytkownik[20];
    char *t;
    char *linia_do_dodania;
    char nowa_subskrybcja[20];
    read(i,uzytkownik,20);
    while(read(i, nowa_subskrybcja, 20) == 0);
    FILE* plik_s=fopen("sub.txt","w");
    fclose(plik_s);
    int check_user=0;

    if(strcmp(uzytkownik,nowa_subskrybcja)==0){ return -2;}
    for (int i=0;i<users;i++){
        if (strcmp(uzytkownicy[i],nowa_subskrybcja)==0){
            check_user=1;
        }
    }
    if (check_user==0){
        return 0;
    }


    FILE* plik2=fopen("subskrybcje.txt", "r");
    FILE* plik1=fopen("sub.txt","a");
    linia_do_dodania=malloc(124*sizeof(char));
    int czy=1;
    while(czy!=0){
        t=malloc(100*sizeof(char));
        
        czy=czytaj_linie(plik2,t);
        printf("%s", t);
        if (czy_uzytkownik(t,uzytkownik)==1){
            if(strstr(t,nowa_subskrybcja)){ 
                return -1;
             }
            strcpy(linia_do_dodania,t);
            free(t);
        }
        else{
            fputs(t,plik1);
            fputs("\n",plik1);
            free(t);
        }
    }
    //dodanie lini z nowa subskrybcja
    strcat(linia_do_dodania," ");
    strcat(linia_do_dodania,nowa_subskrybcja);
    fputs(linia_do_dodania,plik1);
    free(linia_do_dodania);
    fclose(plik1);
    fclose(plik2);
    //podmiana plik przy okazji usuwam puste linie bo nie wiem skad sie biora
    plik1=fopen("sub.txt","r");
    plik2=fopen("subskrybcje.txt","w");
    removeEmptyLines(plik1,plik2);
    fclose(plik1);
    fclose(plik2);
    return 1;
}


int dodaj_wpis(int i){
    char uzytkownik[20];
    read(i,uzytkownik,20);
    strcpy(msg,"mozesz dodac\n");
    wysylanie(i);
    //printf("%s\n",uzytkownik);
    char *katalog;
    char linia[100];
    int czy=1;
    FILE* plik1;
    czyszczenie();
    odczyt(i,'\n');

    FILE* plik_suby=fopen("subskrybcje.txt","r");
    while(czy!=0){
        czy=czytaj_linie(plik_suby,linia);
        if (czy_uzytkownik(linia,uzytkownik)){
            break;
        }
    }
    fclose(plik_suby);
    //printf("%s",linia);
    char *tmp=linia+strlen(uzytkownik)+1;
    //printf("%s",linia);
    //free(linia);

    for (int i=0; i<users;i++){
        if (strstr(tmp,uzytkownicy[i])){
            //printf("%s\n", uzytkownicy[i]);
            katalog=malloc(50*sizeof(char));
            strcpy(katalog,"wpisy/");
            strcat(katalog,uzytkownicy[i]);
            plik1=fopen(katalog,"a");
            fputs(uzytkownik,plik1);
            fputs(msg,plik1);
            //fputs("\n",plik1);
            fclose(plik1);
            free(katalog);
        }
    }
    
}

int zarejestruj(int i){
    czyszczenie();
    strcpy(msg,"podaj login");
    wysylanie(i);
    char nowy_uzytkownik[20];
    read(i,nowy_uzytkownik,20);

    for(int i=0;i<users;i++){
        if(strcmp(uzytkownicy[i],nowy_uzytkownik)==0){ return 0;}
    }

    FILE* plik1=fopen("subskrybcje.txt","a");
    fputs(nowy_uzytkownik,plik1);
    fclose(plik1);
    plik1=fopen("users.txt","a");
    fputs(nowy_uzytkownik,plik1);
    fclose(plik1);
    char katalog[30];
    strcpy(katalog,"wpisy/");
    strcat(katalog,nowy_uzytkownik);
    plik1=fopen(katalog,"w");
    fclose(plik1);
    return 1;


}

int main(int argc, char **argv){
    socklen_t slt;
    int sfd, cfd, on = 1;
    struct sockaddr_in saddr, caddr;
    int czy_zalogowany=0;

    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(1234);
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
    bind(sfd, (struct sockaddr*) &saddr, sizeof(saddr));
    listen(sfd, 10);
    wczytaj_uzytkownikow();
   
    while(1){

        slt = sizeof(caddr);
        int n;
        cfd = accept(sfd, (struct sockaddr*)&caddr, &slt);
        printf("new connection: %s\n",
             inet_ntoa((struct in_addr)caddr.sin_addr));
        if (fork()==0){
            strcpy(msg,"1.zaloguj lub 2.zarejestruj\n");
            wysylanie(cfd);
            int czy_zalogowany;
            czyszczenie();
            odczyt(cfd,'\n');
            printf("%s",msg);
            if(msg[0]=='1'){
	            czy_zalogowany=zaloguj(cfd);
                if (czy_zalogowany){
                    strcpy(msg,"zalogowano\n");
            	    wysylanie(cfd);
                }
                else{
            	    strcpy(msg,"nie zalogowano\n");
                    wysylanie(cfd);
                }
            }
            else if(msg[0]=='2'){
                czy_zalogowany=zarejestruj(cfd);
                if (czy_zalogowany){
                    strcpy(msg,"zalogowano\n");
            	    wysylanie(cfd);
                }
                else{
            	    strcpy(msg,"nie zalogowano\n");
                    wysylanie(cfd);
                }
            }  
             
           
            czyszczenie();
	        if(czy_zalogowany){   
            
	        while(odczyt(cfd, '\n') == 0);
                printf("%s\n",msg);
                if(strcmp("subskrybuj\n", msg) == 0){
                    int czy_dodano=subskrybuj(cfd);
                    if (czy_dodano>0){
                        write(cfd,"1", 1);
                    }
                    else{ write(cfd,"0",1);}
                }
                else if(strcmp(msg, "dodaj\n") == 0){
                    dodaj_wpis(cfd);
                    
                } else{
                    write(cfd, "ERROR\n", 6);
                }
            }
        }
        close(cfd);
    }
    close(sfd);
    return EXIT_SUCCESS;
}
