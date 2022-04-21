#include <stdio.h>
#include <cstdio>
#include <string.h>

typedef struct{
    int nrMat;
    int is;
    int grupa;
    int an;
    int absenteCurs[10];
    int absenteSeminar[10];
} Student;

void conversie(char s1[]){
    FILE* f, * g;
    Student s;
    int i, totalAbsenteCurs, totalAbsenteSeminar;
    char s2[20] = {0};

    f=fopen(s1, "rb");

    if (f==NULL) {
        printf("Fisierul nu a putut fi gasit");
        return;
    }

    printf("\nFisier text: ");
    scanf("%s", s2);
    g=fopen(s2,"w+");

    if(g==NULL) {
        printf("Eroare la crearea fisierului");
        return;
    }

    fprintf(g, "IS \t, Nr matricol \t, Grupa \t, Anul de studiu \t, Absente curs \t, Absente seminar \t");


    fread(&s, sizeof(s), 1, f);
    while (!feof(f)) {
        if (s.is == 1) {
            totalAbsenteCurs = 0;
            totalAbsenteSeminar = 0;
            for (i=0;i<10;i++) {
                totalAbsenteCurs+=s.absenteCurs[i];
                totalAbsenteSeminar+=s.absenteSeminar[i];
            }
            fprintf(g, "\n %d \t, %d \t\t, %d \t, %d \t\t, %d\t\t, %d", s.is, s.nrMat, s.an, s.grupa, totalAbsenteCurs, totalAbsenteSeminar);

        }
        fread(&s, sizeof(s), 1, f);
    }

    fclose(f);
    fclose(g);
}

int nrArticole(FILE *f)
{
    fseek(f, 0, 2);
	return ftell(f) / sizeof(Student);
}


int main(){
    FILE *f;
    char nume[20], textFinal[1000], textLucru[100];
    Student s;
    int i, nrMatricol, total, nrArt, varLucru, maxAbs=-1, totalAbs;

    printf("\nFisier: ");
    gets(nume);
    f=fopen(nume,"wb+");

    if(f==NULL) {
        printf("Eroare la deschiderea/crearea fisierului");
        return 0;
    }

    printf("Nr articole pt preformare: ");
    scanf("%d", &total);
    s.is = 0;
    for (i = 0; i <= total; i++) {
        fwrite(&s, sizeof(s), 1, f);
    }

    printf("\nNumar matricol: ");
    scanf("%d",&nrMatricol);
    while(nrMatricol < 1) {
        printf("Numarul matricol nu poate fi mai mic decat 1. Nr matricol: ");
        scanf("%d",&nrMatricol);
    }

    while(!feof(stdin)){
        if(nrMatricol>nrArticole(f)) {
            s.is=0;
            for(i=nrArticole(f);i<=nrMatricol;i++) {
                fwrite(&s,sizeof(s), 1, f);
            }
        }
        fseek(f, nrMatricol * sizeof(Student), 0);
        fread(&s,sizeof(s), 1, f);

        if(s.is==1) {
            printf("\nExista deja un student cu acest numar matricol");
        }
        else {
            s.is=1;
            s.nrMat = nrMatricol;


            printf("Grupa: ");
            scanf("%d",&varLucru);
            while (varLucru<1) {
                printf("Grupa trebuie sa fie mai mare decat 0. Introduceti grupa: ");
                scanf("%d", &varLucru);
            }
            s.grupa = varLucru;

            printf("An: ");
            scanf("%d",&varLucru);
            while (varLucru<1 || varLucru>5) {
                printf("Anul trebuie sa fie intre 1 si 5. Introduceti anul din nou: ");
                scanf("%d", &varLucru);
            }
            s.an = varLucru;

            for(i=0;i<10;i++){
                printf("Absente curs %d: ",i+1);
                scanf("%d", &varLucru);
                while (varLucru<0 || varLucru>10) {
                    printf("Studentul poate avea min 0 si max 10 absente/semestru. Absente curs %d: ",i+1);
                    scanf("%d", &varLucru);
                }
                s.absenteCurs[i] = varLucru;

                printf("Absente seminar %d: ",i+1);
                scanf("%d", &varLucru);
                while (varLucru<0 || varLucru>10) {
                    printf("Studentul poate avea min 0 si max 10 absente/semestru. Absente seminar %d: ",i+1);
                    scanf("%d", &varLucru);
                }
                s.absenteSeminar[i] = varLucru;
            }

            fseek(f, nrMatricol * sizeof(Student), 0);
            fwrite(&s, sizeof(s), 1, f);
        }

        printf("\nNr matricol nou: ");
        scanf("%d",&nrMatricol);
        while(nrMatricol < 1) {
            printf("Numarul matricol nu poate fi mai mic decat 1. Nr matricol: ");
            scanf("%d",&nrMatricol);
        }
    }

    fclose(f);

    conversie(nume);

    if(!(f=fopen(nume,"rb"))) {
        printf("\nFisierul %s nu poate fi deschis", nume);
        return 0;
    }

	fread(&s, sizeof(Student), 1, f);
	while(!feof(f)){
        if(s.is==1){
            totalAbs = 0;
            for(i=0; i<10; i++) {
                totalAbs+=s.absenteCurs[i];
                totalAbs+=s.absenteSeminar[i];
            }
            if(totalAbs>=maxAbs) {
                if(totalAbs>maxAbs) {
                    sprintf(textFinal, "Nr matricol(e) cu ele mai multe absente: %d", s.nrMat);
                } else {
                    textLucru[0]='\0';
                    sprintf(textLucru, ", %d", s.nrMat);
                    strcat(textFinal, textLucru);
                }

                maxAbs=totalAbs;
            }
        }
	    fread(&s, sizeof(Student), 1, f);
    }

    fclose(f);

    printf("%s", textFinal);

	return 0;
}


