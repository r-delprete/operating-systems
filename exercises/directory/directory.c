#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#define pathMax 1024

/**
 * function printRegularFilesFromDirectory
 * @param directoryPath percorso della directory
 * @returns nome e dimensione di tutti i file regolari (normali file) all'interno di quella directory
 * con permessi di lettura e scrittura per il proprietario e per gli altri utenti.
 */
void printRegularFilesFromDirectory(char *directoryPath)
{
  /**
   * @param directory directory passata come directory path
   */
  DIR *directory = opendir(directoryPath);
  /**
   * @param entries informazioni di ogni voce della directory
   */
  struct dirent *entries = readdir(directory);
  /**
   * @param fileInfos informazioni del file
   */
  struct stat fileInfos;

  if (directory == NULL)
  {
    printf("Errore nell'apertura della directory!\n");
    return;
  }

  while (entries != NULL)
  {
    char filePath[pathMax];

    if (stat(filePath, &fileInfos) == -1)
    {
      printf("Errore nell'ottenimento delle info del file!\n");
      return;
    }

    if (S_ISREG(fileInfos.st_mode) && (fileInfos.st_mode & S_IRUSR) && (fileInfos.st_mode & S_IWUSR) && (fileInfos.st_mode & S_IROTH) && (fileInfos.st_mode & S_IWOTH))
    {
      printf("File: %s, Dimensione: %ld bytes", entries->d_name, fileInfos.st_size);
    }
  }

  closedir(directory);
}

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    printf("usage: %s <directory_name>", argv[0]);
    return 1;
  }

  char *directoryPath = argv[1];

  if (access(directoryPath, F_OK) == -1)
  {
    printf("Error in access!\n");
    return 1;
  }

  printRegularFilesFromDirectory(directoryPath);

  return 0;
}