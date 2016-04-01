
#include    <iostream>
#include    <fstream>
#include    <vector>
#include    <stdio.h>
#include    <string.h>
#include    <algorithm>
#include    <iomanip>  
#include    <math.h>
#include    <string.h>
#include    "auxiliares.h"
#include    <boost/numeric/ublas/matrix.hpp>

  
using namespace std;


int main(int argc, char** argv) {
    
    double arq_x = 125.0, arq_y;            // posicion inicial del arquero
    double arco_inf, arco_sup;              // limites del arco (sobre el eje y)
    double mu;                              // max desplazamiento del arquero por instante de tiempo
    int T = 0;                              // tiempo que dura una jugada.
    
    vector <punto>   recorrido_pelota;
    vector <punto>   jugadores;
    
    ifstream path_tiro     (argv[1]);
    ifstream path_jugadores(argv[2]);       // puede estar vacío o no vacío
    ofstream path_arq      (argv[3]);           
    
    int metodo = 0;                         // método titular = 0 ; metodo suplente = 1
    
    
    /* Leo path_jugadores */

    punto temp_jug;
    while(!path_jugadores.eof()) {
            path_jugadores >> temp_jug.x >> temp_jug.y;
            jugadores.push_back(temp_jug);
    }
    
    
    /* Leo path_tiro */
    /* On-the fly, voy estimando con lo que tengo la futura posicion de la pelota. Muevo en consecuencia al arquero */
    /* Si la pelota le cae a un jugador, limpio toda la info del recorrido */
    
    int grado_polinomio = 3;                                            // harcodeo para estimar función recorrido de la pelota
    punto temp_punto; 
    vector <double> temp_polinomio;     
  
    vector <vector <double> > A;
    vector <double> x, y;
  
    path_tiro >> arq_y >> arco_inf >> arco_sup >> mu;
  
    double pos_final_pelota, pos_ant_pelota = arq_y;
    double pos_actual_arquero = arq_y;
    double r, rant, medida, distancia, tam;
    string str;
    vector <double> ultimas_mediciones;
  
    while(!path_tiro.eof()) {
        
        path_tiro >> temp_punto.x >> temp_punto.y;
        T++;
        
        if(temp_punto.x > 125.0) {    
            recorrido_pelota.push_back(temp_punto);
        }
        
        
        /* Este va a ser el método 0 */
      
        if(metodo == 0 && T >= 3) {
              
              r = 0.0;
              pos_final_pelota = interpolarSplines(recorrido_pelota);
              
              /* Sólo actualizo vector de posiciones de la pelota si cayó en rango */
              if(pos_final_pelota < arco_sup && pos_final_pelota > arco_inf) {
                  ultimas_mediciones.push_back(pos_final_pelota);
              }
              tam = ultimas_mediciones.size();
              
              /* No considero las mediciones que difieren mucho de las últimas 3 calculadas */ 
              if(ultimas_mediciones.size() >= 4 
                 && dist(ultimas_mediciones[tam-1], ultimas_mediciones[tam-2]) > 10 
                 && dist(ultimas_mediciones[tam-1], ultimas_mediciones[tam-3]) > 10
                 && dist(ultimas_mediciones[tam-1], ultimas_mediciones[tam-4]) > 10
                 && dist(ultimas_mediciones[tam-2], ultimas_mediciones[tam-3]) < 10
                 && dist(ultimas_mediciones[tam-2], ultimas_mediciones[tam-4]) < 10
                 && dist(ultimas_mediciones[tam-3], ultimas_mediciones[tam-4]) < 10) {
              
                    ultimas_mediciones.pop_back();
                    pos_final_pelota = ultimas_mediciones[ultimas_mediciones.size() - 1];
              }
              
              medida = fabs(pos_final_pelota - pos_ant_pelota);
              
              /* Si Splines calculó bien (al menos si la probabilidad es alta) me muevo a ese lugar */
              r = mover_arquero_caso1(pos_actual_arquero, pos_final_pelota, arco_inf, arco_sup, mu);
              
              /* Si Splines calculó cualquier cosa me muevo al medio o cero, dependiendo de T */
              //~ if(medida > 100 || pos_final_pelota > arco_sup || pos_final_pelota < arco_inf) {
                  //~ if(T < 5) {
                    //~ r = 0;
                  //~ 
                  //~ } else if(rant != 0 && T<5) {
                    //~ r = rant;
                  //~ 
                  //~ }else{
                    //~ r = mover_arquero_caso2(pos_actual_arquero, temp_punto.y, arco_inf, arco_sup, mu);
                  //~ 
                  //~ }
              //~ }
              
              /* Si Splines calculó medianamente aceptable me muevo hacia esa zona */
              if(medida >=20 && medida <=40 && pos_actual_arquero + 4 < arco_sup 
                                            && pos_actual_arquero - 4 > arco_inf)   {
                  if(pos_final_pelota > pos_ant_pelota) {
                      r = 4;
                  }else{
                      r = -4;}
              }
              
              /* Escribo en archivos */
              pos_actual_arquero += r;
              str = std::to_string(r);
              path_arq << str << endl;
              
              /* Actualizo info. que uso para toma de decisiones */
              pos_ant_pelota = pos_final_pelota;
              
              /* Escribo por pantalla */
              cout << pos_actual_arquero <<endl;
              
          } else {
            
              r = mover_arquero_caso2(pos_actual_arquero, temp_punto.y, arco_inf, arco_sup, mu);
              pos_actual_arquero += r;
              str = std::to_string(r);
              path_arq << str << endl;
          }
          
          /* Actualizo el ultimo valor de r */
          rant = r;
            
      
            /* Si la pelota llega a un jugador, limpio toda la información de la jugada que tengo */
      
            if(pertenece(jugadores, temp_punto) && T >= 3) {    
                    
                  A.clear();
                  x.clear(); x.push_back(temp_punto.x);
                  y.clear(); y.push_back(temp_punto.y);
                  recorrido_pelota.clear(); recorrido_pelota.push_back(temp_punto);
                  ultimas_mediciones.clear();
                  T = 1;
                  
                  r = mover_arquero_caso2(pos_actual_arquero, temp_punto.y, arco_inf, arco_sup, mu);
                  
                  pos_actual_arquero += r;
                  
                  str = std::to_string(r);
                  path_arq << str << endl;
            }
    }
  
  
    /* Este va a ser el método 1 */
      
    vector <double> ax, ay;
      
    if(metodo == 1) {
        cuadrados_minimos_QR(A, x, y, ax, ay);
        pos_final_pelota = estimar_posicion_final_pelota(ax, ay);
        mover_arquero_QR(pos_final_pelota, argv[3]);
      }
    for(int i=0; i<ultimas_mediciones.size(); i++) {
        cout << ultimas_mediciones[i] << endl;
    }
    
    return 0;
}
