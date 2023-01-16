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
char uzytkownicy[30][15];
int users=0;
char blok[5];

//funkcja czytajaca z dyskryptora
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

//funkcja czytajaca z bufora do podanego stringa bez ostatniego znaku (wazne)
void zamina_na_uzytkownik(char* u){
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
//wysylanie wiadomosci sprawdza czy wszystko wyslane
void wysylanie(int i){
    int toSend = strlen(msg);
    int dataCount = 0, w;
    while (dataCount < toSend) {
        w = write(i, msg+dataCount, toSend-dataCount);
        dataCount += w;
    }

}
//czyta linie z pliku txt
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
//wczytuje uzytkownikow to tabeli uzytkownicy wazne przy logowaniu i rejsetracji
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
//sprawdza czy uzytkownik jest juz zasubskrobawny funkcja strstr nie dawala satysfakcjonujacych wynikow
int czy_jest_zasubo(char* linia, char* uzy){
    int i=0;
    int x=0;
    int czy=0;

    while(linia[i]!='\n' && i<strlen(linia) && linia[i]!='\0'){
        if (czy==strlen(uzy)) return 1;
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
void czyszczenie(){
    for (int i=0;i<1024;i++){
        msg[i]='\0';
    }
}
//czysci tablice ze smieci
void czyszczenie2(char* b,int z){
    for (int i=0;i<z;i++){
        b[i]='\0';
    }
}

int zaloguj(int i){
    char uzytkownik[20];
    odczyt(i,'\n');
    zamina_na_uzytkownik(uzytkownik);
    printf("%s",uzytkownik);
	printf("\n");
    FILE* plik=fopen("users.txt", "r");

	char logowanie[40];
 
	int sprawdzanie_czy_nie_EOF=fscanf(plik,"%s\n",logowanie);

	int czy_zalogowany=0;
    int which=0;
	while (sprawdzanie_czy_nie_EOF!= EOF){
	if (strcmp(uzytkownik,logowanie)==0 ){

		czy_zalogowany=1;
        break;
       }
       sprawdzanie_czy_nie_EOF=fscanf(plik,"%s",logowanie);
       which++;
    }
	fclose(plik);
	return czy_zalogowany;
}

//czy w tej lini jest uzytkownik zalogowany(tzn pierwszy)
int czy_uzytkownik(char *p,char *uzytkownik){
    int c=0;
    int wynik=0;

    while(p[c]!= ' ' && p[c]!='\0' && p[c]!='\n'){
        
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

int subskrybuj(int i){
    char uzytkownik[20];
    char *t;
    char *linia_do_dodania;
    char nowa_subskrybcja[20];
    

    czyszczenie();
    odczyt(i,'\n');
    zamina_na_uzytkownik(uzytkownik);
    printf("%s\n",uzytkownik);
    strcpy(msg,"podaj subskrybcje\n");
    wysylanie(i);
    czyszczenie();
    while(msg[0]=='\0' || msg[0]=='\n'){
    odczyt(i,'\n');
    }
    zamina_na_uzytkownik(nowa_subskrybcja);
    printf("%s\n",nowa_subskrybcja);
    FILE* plik_s=fopen("sub.txt","w");
    fclose(plik_s);
    int check_user=0;

    if(strcmp(uzytkownik,nowa_subskrybcja)==0){ 
        
        return -2;
        }
    for (int i=0;i<users;i++){
        if (strcmp(uzytkownicy[i],nowa_subskrybcja)==0){
            check_user=1;
        }
    }
    if (check_user==0){
        
        return 0;
    }
    //zainisjowanie lini jedna zapamietuje linie z pliku(t-zmienna pomocnicza) druga (linia_do_dodania) zapamietuje subskrybcje uzytkownika 
    linia_do_dodania=(char*)malloc(100*sizeof(char));
    t=(char*)malloc(100*sizeof(char));
    //blokowanie pisanie do tego samego pliku w tym samym momencie
    FILE* blok2=fopen("blok","r");
    czytaj_linie(blok2,blok);
    fclose(blok2);
    //patrzymy czy jest ktos juz zablokowany                
    while (blok[0]=='1') {
        sleep(1);
        blok2=fopen("blok","r");
        czytaj_linie(blok2,blok);
        fclose(blok2);
    }
    FILE* blok3=fopen("blok","w");
    fputs("1",blok3);
    fclose(blok3);
    FILE* plik2=fopen("subskrybcje.txt", "r");
    FILE* plik1=fopen("sub.txt","a");
    int czy=1;
    while(czy!=0){

        
        czy=czytaj_linie(plik2,t);
        if (czy_uzytkownik(t,uzytkownik)==1){
            if(czy_jest_zasubo(t,nowa_subskrybcja)){ 
                blok3=fopen("blok","w");
                fputs("0",blok3);
                fclose(blok3);
                return -1;
             }
            strcpy(linia_do_dodania,t);
            czyszczenie2(t,100);
        }
        else{
            fputs(t,plik1);
            fputs("\n",plik1);
            czyszczenie2(t,100);
        }
    }
    //dodanie lini z nowa subskrybcja
    strcat(linia_do_dodania," ");
    strcat(linia_do_dodania,nowa_subskrybcja);
    fputs(linia_do_dodania,plik1);
    

    fclose(plik1);
    fclose(plik2);
    //podmiana plik przy okazji usuwam puste linie
    plik1=fopen("sub.txt","r");
    plik2=fopen("subskrybcje.txt","w");
    removeEmptyLines(plik1,plik2);
    fclose(plik1);

    fclose(plik2);
    free(t);
    free(linia_do_dodania);
    blok3=fopen("blok","w");
    fputs("0",blok3);
    fclose(blok3);
    return 1;
}


int dodaj_wpis(int i){
    char uzytkownik[20];
    czyszczenie();
    odczyt(i,'\n');
    zamina_na_uzytkownik(uzytkownik);
    strcpy(msg,"mozesz dodac\n");
    wysylanie(i);


    char *katalog;
    char linia[100];
    int czy=1;
    FILE* plik1;
    czyszczenie();
    while(msg[0]=='\0' || msg[0]=='\n'){
        odczyt(i,'\n');
    }
    FILE* plik_suby=fopen("subskrybcje.txt","r");
    
    while(czy!=0){
        czyszczenie2(linia,100);
        czy=czytaj_linie(plik_suby,linia);
        printf("1-%s\n",linia);
        if (czy_uzytkownik(linia,uzytkownik)){
            NULL;
        }
        else if(czy_jest_zasubo(linia,uzytkownik)){
            printf("2-%s\n",linia);
            for(int c=0;c<users;c++){
                
                if(czy_uzytkownik(linia,uzytkownicy[c])){

                    katalog=malloc(50*sizeof(char));
                    strcpy(katalog,"wpisy/");
                    strcat(katalog,uzytkownicy[c]);
                    plik1=fopen(katalog,"a");
                    fputs(uzytkownik,plik1);
                    fputs(msg,plik1);
                    //fputs("\n",plik1);
                    fclose(plik1);
                    free(katalog);
                }
            }
        }
    }
    fclose(plik_suby);
    return 1;
}

int zarejestruj(int i){
    czyszczenie();
    strcpy(msg,"podaj login");
    wysylanie(i);
    char nowy_uzytkownik[20];
    czyszczenie();
    odczyt(i,'\n');
    zamina_na_uzytkownik(nowy_uzytkownik);

    for(int i=0;i<users;i++){
        if(strcmp(uzytkownicy[i],nowy_uzytkownik)==0){ return 0;}
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

void odczytaj_wpisy(int x){
    char uzytkownik[20];
    czyszczenie();
    odczyt(x,'\n');
    zamina_na_uzytkownik(uzytkownik);
    char *katalog=malloc(50*sizeof(char));
    strcpy(katalog,"wpisy/");
    strcat(katalog,uzytkownik);
    FILE *fd;
    char len[400];        
    int i;
    int ch;
    int count;
    int g=0;
    fd = fopen(katalog, "r");
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
            msg[g++]=len[i];
        }
        msg[g++]='\n';
    }
    wysylanie(x);
    fclose(fd);



}


void subs_to_user(int i){
    char uzytkownik[20];
    czyszczenie();
    odczyt(i,'\n');
    zamina_na_uzytkownik(uzytkownik);
    czyszczenie();
    FILE* plik2=fopen("subskrybcje.txt", "r");
    int czy=1;
    char* t=(char*)malloc(100*sizeof(char));
    while(czy!=0){
        czy=czytaj_linie(plik2,t);
        if (czy_uzytkownik(t,uzytkownik)==1){
            t=t+strlen(uzytkownik)+1;
            break;
        }
    }
    strcpy(msg,t);
    wysylanie(i);
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
            wczytaj_uzytkownikow();
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
	        while(czy_zalogowany){   
                
	            odczyt(cfd, '\n') == 0;
                //printf("%s\n",msg);
                
                if(strcmp("subskrybuj\n", msg) == 0){
                    //blokowanie pisanie do tego samego pliku w tym samym momencie
                    FILE* blok2=fopen("blok","r");
                    czytaj_linie(blok2,blok);
                    fclose(blok2);
                    
                    while (blok[0]=='1') {
                        sleep(1);
                        blok2=fopen("blok","r");
                        czytaj_linie(blok2,blok);
                        fclose(blok2);
                    }
                    int czy_dodano=subskrybuj(cfd);
                    
                    if (czy_dodano>0){
                        czyszczenie();
                        strcpy(msg,"1");
                    }
                    else{ 
                        czyszczenie();
                        strcpy(msg,"0");
                    }
                    wysylanie(cfd);
                }
                else if(strcmp(msg, "dodaj\n") == 0){
                    
                    int czy_dodano=dodaj_wpis(cfd);
                    if (czy_dodano){
                        czyszczenie();
                        strcpy(msg,"dodano");
                    }
                    else{
                        czyszczenie();
                        strcpy(msg,"nie dodano");
                    }
                    wysylanie(cfd);
                    
                } else if(strcmp(msg,"wpisy\n")==0){
                    odczytaj_wpisy(cfd);
                }
                else if(strcmp(msg,"subs\n")==0){
                    subs_to_user(cfd);
                }
                else if(strcmp(msg,"koniec\n")==0){ 
                    czyszczenie();
                    break;
                }
                wczytaj_uzytkownikow();
                czyszczenie();
            }

        }
        close(cfd);
    }
    close(sfd);
    return EXIT_SUCCESS;
}
