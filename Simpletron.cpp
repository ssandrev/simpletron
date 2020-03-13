#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Variaveis globais
double acumulador;
double memoria [101];
int contProg;

//-----------------------------
//	DECLARACAO DAS FUNCOES
void printMem ();
void readProg ();
int cpu ();
void decExec (int inst);
void halt ();
void reset ();
void load (int endMem);
void store (int endMem);
void add (int endMem);
void subtract(int endMem);
void divide(int endMem);
void multiply(int endMem);
void branch(int endMem);
void branchNeg(int endMem);
void branchZero(int endMem);
//-----------------------------


// ~~~~ Entrada e Saida ~~~~

    //~ Imprime a memoria em um arquivo txt
void printMem () {
     FILE *saida;
    saida = fopen("saida.txt", "w");
    fprintf (saida,"REGISTERS: \nacumulador \t\t%+05.lf\n", acumulador);
    fprintf(saida,"contador de programa \t%5.02d\n\n",contProg);
    fprintf (saida,"MEMORY:\n\n");
    fprintf(saida,"\t  0\t  1\t  2\t  3\t  4\t  5\t  6\t  7\t  8\t  9\n");
    fprintf(saida,"   --------------------------------------------------------------------------------\n");
    for (int i = 0; i < 100; i +=10){
        fprintf(saida,"%2d|   %+05.lf   %+05.lf   %+05.lf   %+05.lf   %+05.lf", i, memoria[i], memoria[i+1], memoria[i+2], memoria[i+3], memoria[i+4]);
        fprintf(saida,"   %+05.lf   %+05.lf   %+05.lf   %+05.lf   %+05.lf|\n",memoria[i+5], memoria[i+6], memoria[i+7], memoria[i+8], memoria[i+9]);
    }
    fprintf(saida,"   --------------------------------------------------------------------------------\n");
    fclose(saida);
}

    //~ Le o programa e coloca-o na memoria
void readProg () {
    FILE *entrada;
    char instrucao[2];
    entrada = fopen ("instrucao.txt", "r");
    if (entrada == NULL){
        printf ("Nao foi possivel abrir o arquivo de instrucao\n");
        printf ("Programa finalizado\n");
        exit (0);
    }
     while ( !feof(entrada)){
        fscanf (entrada, "%s", &instrucao);
        int x = atoi (instrucao);
        fscanf (entrada, "%s", &instrucao);
        int y = atoi (instrucao);
        memoria[x] = y;
    }
    fclose (entrada);
}

// ~~~~ Unidade de controle ~~~~

int cpu () {
    int inst = memoria[contProg];   // BUSCA
	while (inst !=  4300) {
		contProg++;                     // PC = PC+1
		decExec (inst);					// DECODIFICA E EXECUTA
		inst = memoria[contProg];   	// BUSCA
	}
	halt();
    return 0;
}

void decExec (int inst) {
	int Ms = inst/100;
	int ms = inst%100;
	switch (Ms) {
		case 20:
			load (ms);
			break;
		case 21:
			store (ms);
			break;
		case 30:
			add (ms);
			break;
		case 31:
			subtract (ms);
			break;
		case 32:
			divide (ms);
			break;
		case 33:
			multiply (ms);
			break;
		case 40:
			branch (ms);
			break;
		case 41:
			branchNeg (ms);
			break;
		case 42:
			branchZero (ms);
			break;
		case 43:
			halt ();
			break;
	}
}


// ~~~~ Instrucoes ~~~~

    //~ Finaliza o programa
void halt () {
    printMem();
    reset();
    for (int i = 0; i < 101; i ++)
        memoria[i] = 0.0;
}

    //~ Reseta o programa
void reset () {
    contProg = 0;
}

    //~ Recebe o endereco de um espaco da memoria,
    //~ e carrega o conteudo deste espaco no acumulador.
void load (int endMem) {
    acumulador = memoria[endMem];
}

    //~ Recebe o endereco de um espaco da memoria,
    //~ e armazena o conteudo do acumulador neste espaco.
void store (int endMem) {
    memoria[endMem] = acumulador;
}

    //~ recebe um endereco de um espaco da memoria,
    //~ e soma com o acumulador.
    //~ Deixa o resultado no acumulador.
void add (int endMem){
    acumulador += memoria[endMem];
}

    //~ Recebe um endereco de um espaco da memoria,
    //~ e subtrai com o acumulador.
    //~ Deixa o resultado no acumulador.
void subtract(int endMem){
    acumulador -= memoria[endMem];
}

    //~ Recebe um endereco de um espaco da memoria,
    //~ e divide com o acumulador.
    //~ Deixa o resultado no acumulador.
void divide(int endMem){
    acumulador /= memoria[endMem];
}

    //~ Recebe um endereco de um espaco da memoria,
    //~ e multiplica com o acumulador.
    //~ Deixa o resultado no acumulador.
void multiply(int endMem){
    acumulador *= memoria[endMem];
}

	//~ Desvia para um local especifico (JUMP).
void branch(int endMem){
	contProg = endMem;
}

	//~ Desvia para um local especifico,
	//~ se o acumulador for negativo.
void branchNeg(int endMem){
	if(acumulador < 0)
		contProg = endMem;
}

	//~ Desvia para um local especifico,
	//~ se o acumulador for zero.
void branchZero(int endMem){
	if(acumulador == 0)
		contProg = endMem;
}


// ~~~~ MAIN ~~~~
int main () {
    system("color A");
    printf ("Programa inicializado.\n");
	readProg();
	cpu();
    printf ("Programa finalizado.\n");
return 0;
}
