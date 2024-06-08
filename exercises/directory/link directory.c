#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    printf("Usage: %s <directory>\n", argv[0]);
    exit(1);
  }

  /**
   * @param directory puntatore alla directory passata da riga di comando
   */
  DIR *directory = opendir(argv[1]);

  /**
   * @param directoryPointer struttura dati per leggere il contenuto della directory
   */
  struct dirent *directoryPointer;

  /**
   * @param buff struttura dati per leggere le informazioni del file
   */
  struct stat *buff;

  while ((directoryPointer = readdir(directory)) != NULL)
  {
    /**
     * @param path variabile con il percorso completo di ogni file
     */
    char path[1024];

    sprintf(path, "%s/%s", argv[1], directoryPointer->d_name);

    /**
     * funzione per ottenere informazioni sul file specificato
     * @param path percorso completo del file
     * @param buff struttura dove vengono memorizzare le informazioni
     */
    lstat(path, buff);

    /**
     * macro S_ISLNK
     * utilizzata per verificare se il file è un link simbolico
     * @param st_mode permessi del file
     */
    if (S_ISLNK(buff->st_mode))
    {
      printf("name: %s\n", directoryPointer->d_name);
    }
  }

  closedir(directory);
  exit(0);
}