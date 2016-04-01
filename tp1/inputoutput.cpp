#include<iostream>
#include<fstream>
#include<vector>
#include<stdio.h>
#include<string.h>
#include<iterator>
#include<algorithm>
#include<iomanip>  
#include<sys/time.h>
#include "hornear.h"
using namespace metnum;

#define PI 3.1415926535897932384626433832795 

using namespace std;



/*Observaciones:
 * Si trabajamos con int en lugar de con double, cuando hacemos spy(A) en matlab, no aparecen graficadas las constantes distintas a 1
 * Si subimos la precicion a double, estas constantes si aparecen graficadas en matlab, pero cuando resolvemos usando matlab le ecuación Ax=b,
 * no obtenemos los resultados que la catedra espera que obtengamos para los tests. No se si es porque esta mal la matriz A o por un tema de precisión
 * */

timeval start, end;

double ri, re;
	int m, n, iso, ninst;					//n=#angulos, m=#radios
									//ri=radio interno, re=radio externo
		
void init_time(){
     gettimeofday(&start,NULL);
}

double get_time(){
	     gettimeofday(&end,NULL);
	     return (1000000*(end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec))/1000000.0;}											//problema a resolver
										
vector <vector <double> > A, C;				
											
										
vector <double> b;							


int F(int i, int j, int n){
	return (i-1)*n+j;
}

int main(int argc, char* argv[]){ //argc, cantidad de argumentos pasados desde linea de comandos, en el array de argv
	
	//Ficheros que voy a usar para testear 						
	ifstream fichero_entrada(argv[1]);
	ofstream fichero_salida(argv[2]);

	fichero_salida << setprecision(10) << fixed;

	fichero_entrada>>ri>>re>>m>>n>>iso>>ninst;
	
	double dtita=(2*PI)/n;
	double dradio=(re-ri)/(m-1);
	
	vector <vector <double> > C(m*n, std::vector<double>(m*n+1));
	vector<double> b(m*n);
	
	/*Armamos la matriz C*/
	
	int k;
	for(int i=1; i<=m; i++){
			for(int j=1; j<=n; j++){
					C[F(i,j,n)-1][F(i,j,n)-1] = (-2/(dradio*dradio))+(1/((ri+(i-1)*dradio)*dradio))-(2/((ri+(i-1)*dradio)*(ri+(i-1)*dradio)*dtita*dtita));
					
					if(F(i,j,n)>n && F(i,j,n)<=n*m-n){
							C[F(i,j,n)-1][F(i,j,n)+n-1] = 1/(dradio*dradio);	
							C[F(i,j,n)-1][F(i,j,n)-n-1] = (1/(dradio*dradio))-(1/((ri+(i-1)*dradio)*dradio));
					}
					
					if(j==1){k=F(i,n,n);}else{k=F(i,j,n)-1;}	
							C[F(i,j,n)-1][k-1] = 1/((ri+(i-1)*dradio)*(ri+(i-1)*dradio)*dtita*dtita);
					
					if(j==n){k=F(i,1,n);}else{k=F(i,j,n)+1;}
							C[F(i,j,n)-1][k-1] = 1/((ri+(i-1)*dradio)*(ri+(i-1)*dradio)*dtita*dtita);
			}
	}

	/*Dejamos el sector de arriba solo con la Id*/
	for(int i=0; i<n; i++){
		for(int j=0; j<m*n; j++){
					if(i==j){
							C[i][j]=1;
					}else{
							C[i][j]=0;
					}
		}
	}
	
	/*Dejamos el sector de abajo solo con la Id*/
	for(int i=m*n-n; i<m*n; i++){
		for(int j=0; j<m*n; j++){
					if(i==j){
							C[i][j]=1;
					}else{
							C[i][j]=0;
					}
		}
	}
	double tiempoTomado;
	ofstream archivo_isoterma("/home/esteban/Escritorio/METODOS REPOSITORIO/metodos/isoth.out");
	vector<double> isoterma;
	

	if(*argv[3] == '0'){ 

	init_time(); ///// ACA COMIENZO A CONTAR EL TIEMPO HACERLO SIEMPRE DENTRO LOS IFS QUE ES CUANDO NOS IMPORTA.

	//Aca voy a leer la matriz C y el ninst y aplicar la funcion "hornear" para cada instancia
	double temp;
	vector<double> solution;
	
	for(int i=0; i<ninst; i++){
		for(int k=0; k<n; k++){														
					fichero_entrada>>temp;
					b[k]=temp;				//guardo las temperaturas internas	
									
			}
	
			for(int i=n; i<m*n-n; i++){				//completo con ceros entre las temp internas y externas
					b[i]=0;
						
			}
			for(int k=m*n-n; k<m*n; k++){
					fichero_entrada>>temp;
					b[k]=temp;				//guardo las temperaturas externas
						
			}
			
			cout << endl << endl;
			
			for(int i=0; i<n*m; i++){		//armo la matriz C, que es A|b(ninst)
					C[i][n*m] = b[i];
			}
			
			
			solution = hornear(C);
	
			for(int i = 0; i < C.size(); i++){
				fichero_salida<<solution[i]<<" "<<endl;
			}fichero_salida<<endl;
			
			isoterma = calcularIsoterma(solution, iso, ri, re, m, n);
		for(int i = 0; i<n; i++){
			archivo_isoterma<<isoterma[i]<<endl;		
		}	archivo_isoterma<<endl;
			
	}
	tiempoTomado = get_time(); ///////HASTA ACA TOMÁS EL TIEMPO
	cout<<"Tiempo transcurrido durante el algoritmo cronometrado fue:"<<tiempoTomado<<endl; ////////IMPRIMO POR PANTALLA PARA ANOTAR
	archivo_isoterma.close();	
	fichero_salida.close();

	
	return 0;
	}
	
	
	if(*argv[3] == '1'){
		for(int i=0; i<n*m; i++){ // CAPAZ NO HAGA FALTA, POR LAS DUDAS para la LU
			C[i][n*m] = 0;	
		}
	
		init_time();
		
		vector< vector<double> > L = C;  // obtengo L y U.
		
		vector< vector<double> > U = C;
	
		
		factorizacionL(L);
		factorizacionU(U);
		
		

		double temp;                               // obtener en cada instancia el b de temperaturas.
		vector<double> x;
		vector<double> y;
		for(int i=0; i<ninst; i++){
			for(int k=0; k<n; k++){														
					fichero_entrada>>temp;
					b[k]=temp;				//guardo las temperaturas internas	
								
			}
	
			for(int i=n; i<m*n-n; i++){				//completo con ceros entre las temp internas y externas
					b[i]=0;
						
			}
			for(int k=m*n-n; k<m*n; k++){
					fichero_entrada>>temp;
					b[k]=temp;				//guardo las temperaturas externas
						
			}
			
			cout << endl << endl;
			
			for(int i=0; i<n*m; i++){	 //armo L metiendole el b al final de L(Ly =b)
					L[i][n*m] = b[i];
			}
				y = despejarL(L);  //obtengo y
				
				for(int j = 0; j<n*m; j++){      //meto y en U para despejar x (Ux = y)
					U[j][n*m] = y[j];				
				}
				
				x = despejarU(U);     //despejo x
				
				for(int i = 0; i < C.size(); i++){
				fichero_salida<<x[i]<<" "<<endl;
			}fichero_salida<<endl;
				
				
				isoterma = calcularIsoterma(x,iso, ri, re, m, n);
				for(int i = 0; i< n; i++){				
					archivo_isoterma<<isoterma[i]<<endl;			
				}archivo_isoterma<<endl;
			}
		
	
	tiempoTomado = get_time(); ///////HASTA ACA TOMÁS EL TIEMPO
	cout<<"Tiempo transcurrido durante el algoritmo cronometrado fue:"<<tiempoTomado<<endl; ////////IMPRIMO POR PANTALLA PARA ANOTAR

	archivo_isoterma.close();
	fichero_salida.close();
	
	return 0;
	
	}
	
	cout<<"\n";
	cout<<"Ingrese como cuarto parametro 1 o 0."<<endl;
	cout<<"\n";
	return 0; 

}

