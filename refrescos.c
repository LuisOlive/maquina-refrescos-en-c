/*

	Desarrollador:
		Luis M. P. Olivares
	derechos reservados.

	Proyecto:

		Desarrollar un programa que simule la compra de refrescos en una máquina.

	Requisitos:

		1.	El usuario puede comprar todas las bebidas que desee hasta que se 		acabe su dinero.
		2.	El programa lleva cuenta cuantos refrescos le quedan.
		3.  El programa recauda el dinero hasta que el administrador lo cobre
		4.	El programa no permitira comprar refrescos agotados.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 30
#define C 6

typedef struct{
	int codigo;
	char nombreRef[N];
	int restantes;
	float precio;
}refrescos;

//Funciones de apoyo
int decidirSi(char*,int);
void hr();

//Funciones de control
void resetear();
int menuRef(refrescos*, int);
void ingresarDinero(float*);
void actualizar(refrescos*);
void comprar(refrescos*,int*, float*);
void administrar();
int contrasegna();
void actContra(char*);
void recargar();
void vaciarCaja();

int main(){
	int i;
	refrescos refresco[C];
	float saldo=0;

	FILE *rst = fopen("dBase.bin","r");
		if(rst==NULL) resetear();
		else fclose(rst);

	do{
		puts("\n\t\t\t.:\tMaquina de refrescos\t:.");
		hr();

		printf("Saldo: $%.2f\n",saldo);

		do	ingresarDinero(&saldo);	while(saldo==0);

		int select = menuRef(refresco,1);
		hr();

		if(select>=0&&select<C){
			comprar(refresco,&select,&saldo);
			actualizar(refresco);
		}else if(select==C){
			if(	contrasegna()	)
				administrar();
		}else if(select==C+1){
			if(decidirSi("seguro que deseas salir",0)) break;
		}else
			puts("Elige una opcion valida");

		hr();
	}while(	decidirSi("comprar otra vez",1)	);

	puts("\n\n\t\t\tGracias por su compra\n");
	printf("\t\t\t\t\tCambio:	$%.2f\n\n",saldo);
	system("pause");

	return 0;
}

int decidirSi(char texto[N],int limpia){
	char dec;
	printf("%s?? [s/n]	",texto);

	fflush(stdin);
	scanf("%c",&dec);

	if(limpia)	system("cls");

	if(dec=='s'||dec=='S')	return 1;
	else	return 0;
}
void hr(){
	puts("-------------------------------------------------------------------------------\n");
	return;
}

void resetear(){

	vaciarCaja();
	puts("Caja de ganancias vaciada");

	FILE*a = fopen("ganacias.bin","w");
		fputs("\t\tIngresos obtenidos:  \n\n",a);
	fclose(a);
	puts("Ticket de ganacias vaciado");

	recargar();

	return;
}
int menuRef(refrescos*r, int a){
	int s,i;

	FILE *ar = fopen("dBase.bin","r");
		fread(r,sizeof(/**r*/refrescos),C,ar);
	fclose(ar);

	if(a) puts("\nQue refresco deseas comprar??\n");

	for(i=0;i<C;i++)
		printf("\t%d.\t%s\t\t\t%d\t$%.2f\n",i+1,r[i].nombreRef,r[i].restantes,r[i].precio);

	if(a){
	printf("\n\t%d.\t--Modo administrador--\n",C+1);
	printf("\t%d.\t--Salir--\n\n",C+2);

	printf("Opcion: ");
	scanf("%d",&s);
	}
	return s-1;
}
void ingresarDinero(float*dinero){
	float x;

	printf("Ingrese una cantidad de dinero	$");
	scanf("%f",&x);
	*dinero+=x;

	return;
}
void comprar(refrescos*r, int*elec, float*saldo){
	int i,j;
	float k=0;

	FILE *a = fopen("money.bin","r+");
		fscanf(a,"%f",&k);
	fclose(a);

	printf("Cuantos refrescos??  ");
	scanf("%d",&j);

	for(i=0;i<j;i++){
		if(*saldo >= r[*elec].precio && r[*elec].restantes>0){
			r[*elec].restantes--;
			*saldo -= r[*elec].precio;
			k += r[*elec].precio;
		}
		else if(*saldo < r[*elec].precio){
			if(decidirSi("\nNo tienes suficiciente dinero, ingresar mas",0)){
				ingresarDinero(&*saldo);
				i--;
			}else break;
		}
		else if(r[*elec].restantes==0){
			puts("Este refresco se ha agotado");
			break;
		}
	}

	FILE *b = fopen("money.bin","w");
		fprintf(b,"%.2f",k);
	fclose(b);

	hr();
	if(i>0){
		printf("%d refresco",i);
		if(i!=1) puts("s comprados");
		else puts(" comprado");
	}else puts("no se han comprado refrescos");

	return;
}
void actualizar(refrescos*r){
	FILE *a = fopen("dBase.bin","wb");
		fwrite(r,sizeof(refrescos),C,a);
	fclose(a);
	return;
}
void administrar(){
	int s;
	float k;
	char c;
	char cadena[20], cadena2[20];
	refrescos ref[C];

	time_t t;
  struct tm *tm;
  t=time(NULL);
  tm=localtime(&t);

	do{
			{
				system("cls");
				puts("\n\t\t\t.:\tPanel de control\t:.");

				hr();
				puts("\t1. Mostrar ganancias");
				puts("\t2. Cobrar dinero en maquina");
				puts("\t3. Recargar refrescos");
				puts("\t4. Cambiar contrasegna");
				puts("\t5. Reestablecer sistema");
				puts("\t6. Salir");
			}
		printf("Opcion: ");
		scanf("%d",&s);

		FILE *a = fopen("money.bin","r+");
			fscanf(a,"%f",&k);
		fclose(a);

		hr();
		FILE *b = fopen("ganacias.bin","a+");
			switch(s){
				case 1:{
					rewind(b);
          while((c=fgetc(b))!=EOF)
            if(c=='\n') puts("");
            else putchar(c);
					hr();
					system("pause");
					break;
				}
				case 2:{
					printf("\t\tDinero en maquina:  $%.2f\n",k);
					fprintf(b,"\t%02d/%02d/%02d\t%0000.2f\n",1900+tm->tm_year, tm->tm_mon, tm->tm_mday,k);

					fclose(b);
					vaciarCaja();

					hr();
					system("pause");
					break;
				}
				case 3:{
					recargar();
					puts("Refresco recargado. ");
					if( decidirSi("ver",0) ){
						puts("");
						menuRef(ref,0);
					}
					system("pause");
					break;
				}
				case 4:{
					puts("Se requiere de la actual contrasegna");
				 	if(	contrasegna()	) while(1){
						printf("\nIngrese la nueva contrasegna:	");
						fflush(stdin);
						gets(cadena);

						printf("Confirme la nueva contrasegna:	");
						fflush(stdin);
						gets(cadena2);

						if(	strcmp(cadena,cadena2)==0	){
							actContra(cadena);
							puts("\nActualizada correctemente\n");
							system("pause");
							break;
						}
					}
					break;
				}
				case 5:{
					if(decidirSi("El sistema borrara datos de refrescos y dinero, continuar",0)){
						resetear();
						puts("Refrescos en menu reiniciados");

						actContra("root");
						puts("Contrasegna reestablecida como 'root'");

						system("pause");

					}
					break;
				}
				case 6: break; //para no mostrar "elige..."
				default:
					puts("Elige una opcion valida");
			}

		if(s!=2) fclose(b);

	}while(s!=6);

	hr();
	return;
}
int contrasegna(){
	int i,j=0;
	char password[20];
	char intento[20];

	FILE *a = fopen("password.bin","r");
		if(a!=NULL){
			fread(password,sizeof(char),20,a);
			fclose(a);
			//puts(password);
		}else{
			actContra("root");
			j++;
		}

	for(i=4;i>-1;i--){
		if(i!=4) puts("");
		printf("Introduzca la contrasegna:  ");
		fflush(stdin);
		gets(intento);

		if(strcmp(password,intento)==0||j)	return 1;
		else if(i>1)	printf("incorrecto, quedan %d intentos\n",i);
		else if(i==1)	puts("incorrecto, queda 1 intento");
	}

	puts("\nAcceso denegado");
	return 0;
}
void actContra(char *password){
	//puts(password);

	FILE *a = fopen("password.bin","wb");
		fwrite(password,sizeof(char),20,a);
	fclose(a);

	return;
}
void recargar(){
	int stock=20;

	refrescos refresco[C]={
		{2077,"Coca-Cola"	,stock,9	},
		{4210,"Sprite   "	,stock,7.5},
		{1189,"Fanta    "	,stock,8	},
		{1998,"Pepsi    "	,stock,8.5},
		{7825,"Seven Up "	,stock,8.5},
		{1390,"Mirinda  "	,stock,7	}
	};
	actualizar(refresco);
	return;
}
void vaciarCaja(){

	FILE *a = fopen("money.bin","wb");
		fprintf(a,"%f",0.00);
	fclose(a);

	return;
}
