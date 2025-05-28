#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>   
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

#define check_error(cond,msg)\
  do {\
    if (!(cond)) {\
      perror(msg);\
      fprintf(stderr, "File: %s\nFunction: %s\nLine: %d\n", __FILE__, __func__, __LINE__);\
      exit(EXIT_FAILURE);\
    }\
  } while (0)
	

int main(int argc, char* argv[])
{
  //Proveravamo da li su ispravno zadati argumenti komandne linije
  check_error((argc == 3), "Nije ispravan broj argumenata");

  const char *target = argv[1];   //Naziv datoteke za koji zelimo da kreiramo simbolicki link
  const char *linkpath = argv[2];   //Naziv simbolickog linka koji kreiramo

  //Kreiranje simbolickog linka
  if(symlink(target, linkpath) == -1)
  {
      perror("Error symlink");
      return -1;
  }

  printf("Kreirano: %s -> %s\n", linkpath, target);

  //Provera da li dadtoteka za koju kreiramo link postoji
  if (access(target, F_OK) == -1)
  {
      //Ako fajl ne postoji, kreira se novi prazan
      int file = creat(target, 0644);        
      check_error((file != -1), "Target nije uspesno kreiran!");

      close(file);

      printf("Target fajl: %s nije postojao i kreiran je prazan.\n", target);
  }
  else
  {
      printf("Target fajl: %s je vec postojao\n", target);
  }
    
  //Citanje putanje na koju pokazuje simbolicki link
  char buffer[BUFFER_SIZE];
  ssize_t size = readlink(linkpath, buffer, sizeof(buffer) - 1);
  check_error((size != -1), "Error");

  buffer[size] = '\0';

  printf("Simblocki link: %s ,pokazuje na: %s\n", linkpath, buffer);

  return 0;
}