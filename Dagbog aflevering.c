#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

#define MAX_FILENAME_LENGTH 256
#define MAX_TEXT_LENGTH 1024

/* Funktion: at slette en fil */
void deleteFile(char *filename) {
    if (remove(filename) == 0) {
        printf("%s filen er slettet:\n", filename);
    } else {
        perror("Kunne ikke slette filen");
    }
}

/* Funktion: at vise indholdet af en fil */
void displayFileContents(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Kunne ikke åsbne filen '%s'\n", filename);
        return;
    }

    char line[MAX_TEXT_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }

    fclose(file);
}

/* Funktion: at redigere indholdet af en fil */
void editFileContents(char *filename) {
    FILE *file = fopen(filename, "a+");  // �bn filen i "a+"-tilstand for at tilf�je
    if (file == NULL) {
        printf("Kunne ikke �bne filen '%s'\n", filename);
        return;
    }

    fseek(file, 0, SEEK_END);  // Placer filpegeren i filens ende

    char ch;
    printf("Rediger teksten (tryk Ctrl+S for at gemme og Ctrl+R for at vende tilbage til hovedmenuen):\n");

    while (1) {
        ch = getchar();
        if (ch == 19) {  // Ctrl+S for at gemme (Ctrl og bogstavet 'S')
            fclose(file);
            printf("�ndringerne er gemt.\n");
            break;
        } else if (ch == 18) {  // Ctrl+R for at vende tilbage til hovedmenuen (Ctrl og bogstavet 'R')
            fclose(file);
            printf("Afslutter redigering...\n");
            break;
        } else {
            ungetc(ch, stdin);
            char modifiedContents[MAX_TEXT_LENGTH] = "";
            if (fgets(modifiedContents, sizeof(modifiedContents), stdin) == NULL) {
                break;  // Afslut n�r der st�des p� EOF (f.eks. Ctrl+D)
            }
            fprintf(file, "%s", modifiedContents);  // Tilf�j den nye tekst
        }
    }
}

/* Funktion: at oprette en ny fil (jeg har ikke kunne få denne funktion til at virke men alle andre funktioner fungere) */
int createNewFile(char *filename) {
    if (access(filename, F_OK) != -1) {
        printf("Filen findes allerede\n");
        return 0;
    }

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Kunne ikke oprette filen '%s'\n", filename);
        return 1;
    }

    fclose(file);
    return 2;
}

/* Funktion: at vise �ndringstidspunktet for en fil */
void showFileModificationTime(char *filename) {
    struct stat attrib;
    if (stat(filename, &attrib) == 0) {
        time_t mod_time = attrib.st_mtime;
        struct tm *timeInfo = localtime(&mod_time);
        char mod_time_str[20];
        strftime(mod_time_str, sizeof(mod_time_str), "%Y-%m-%d %H:%M:%S", timeInfo);
        printf("Filens �ndringstidspunkt: %s\n", mod_time_str);
    } else {
        printf("Kunne ikke hente filens ændringstidspunkt.\n");
    }
}

int main() {
    char filename[MAX_FILENAME_LENGTH];
    printf("Indtast filnavn: ");
    scanf("%s", filename);

    int choice;

    while (1) {
        printf("\nHovedmenu:\n");
        printf("1. Vis dagbog\n");
        printf("2. Redigere dagbog\n");
        printf("3. Opret ny dagbog\n");
        printf("4. Sidst �ndret\n");
        printf("5. Slet dagbog\n");
        printf("6. Afslut\n");
        printf("Valg: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayFileContents(filename);
                break;
            case 2:
                editFileContents(filename);
                break;
            case 3:
                createNewFile(filename);
                break;
            case 4:
                showFileModificationTime(filename);
                break;
            case 5:
                deleteFile(filename);
                break;
            case 6:
                printf("Afslutter...\n");
                return 0;
            default:
                printf("Ugyldigt valg. Prøv igen.\n");
        }
    }
}
