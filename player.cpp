#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include "player.h"
#include "environment.h"

using namespace std;

const double masinf=9999999999.0, menosinf=-9999999999.0;

// Constructor
Player::Player(int jug){
    jugador_=jug;
}

// Actualiza el estado del juego para el jugador
void Player::Perceive(const Environment & env){
    actual_=env;
}

double Puntuacion(int jugador, const Environment &estado){
    double suma=0;

    for (int i=0; i<7; i++)
      for (int j=0; j<7; j++){
         if (estado.See_Casilla(i,j)==jugador){
            if (j<3)
               suma += j;
            else
               suma += (6-j);
         }
      }

    return suma;
}


// Funcion de valoracion para testear Poda Alfabeta
double ValoracionTest(const Environment &estado, int jugador){
    int ganador = estado.RevisarTablero();

    if (ganador==jugador)
       return 99999999.0; // Gana el jugador que pide la valoracion
    else if (ganador!=0)
            return -99999999.0; // Pierde el jugador que pide la valoracion
    else if (estado.Get_Casillas_Libres()==0)
            return 0;  // Hay un empate global y se ha rellenado completamente el tablero
    else
          return Puntuacion(jugador,estado);
}

// ------------------- Los tres metodos anteriores no se pueden modificar
double ValoracionHorizontalVertical(const Environment &estado, int jugador){

    double valor=0;
    //en linea de 3
    for(int i=0;i<7;i++){
        for(int j=0;j<5;j++){
            if( (estado.See_Casilla(i,j)%3==estado.See_Casilla(i,j+1))%3 && (estado.See_Casilla(i,j+1)%3==estado.See_Casilla(i,j+2)%3) ){
                if(estado.See_Casilla(i,j)%3==jugador){
                    valor+=-3000;
                }else if(estado.See_Casilla(i,j)%3==(jugador%2)+1){
                    valor+=3000;
                }
            }
            if( (estado.See_Casilla(j,i)%3==estado.See_Casilla(j+1,i)%3) && (estado.See_Casilla(j+1,i)%3==estado.See_Casilla(j+2,i)%3) ){
                if(estado.See_Casilla(j,i)%3==jugador){
                    valor+=-3000;
                }else if(estado.See_Casilla(j,i)%3==(jugador%2)+1){
                    valor+=3000;
                }
            }
        }
        for(int j=0;j<6;j++){
            if(estado.See_Casilla(i,j)%3==estado.See_Casilla(i,j+1)%3){
                if(estado.See_Casilla(i,j)%3==jugador){
                    valor+=-150;
                }else if(estado.See_Casilla(i,j)%3==(jugador%2)+1){
                    valor+=150;
                }
            }
            if(estado.See_Casilla(j,i)%3==estado.See_Casilla(j+1,i)%3){
                if(estado.See_Casilla(j,i)%3==jugador){
                    valor+=-150;
                }else if(estado.See_Casilla(j,i)%3==(jugador%2)+1){
                    valor+=150;
                }
            }
        }

    }

    return valor;
}

double ValoracionDiagonal(const Environment &estado, int jugador){

    double valor=0;
    //diagonales hacia arriba de 3
    for(int i=6; i>1; i--){
        for(int j=0;j<5;j++){
            if( (estado.See_Casilla(j,i)%3==estado.See_Casilla(j+1,i-1)%3) && (estado.See_Casilla(j+1,i-1)%3==estado.See_Casilla(j+2,i-2)%3) ){
                if(estado.See_Casilla(j,i)%3==jugador){
                    valor+=-3000;
                }else if(estado.See_Casilla(j,i)%3==(jugador%2)+1){
                    valor+=3000;
                }
            }
        }
    }

    //diagonales hacia arriba de 2
    for(int i=6; i>0; i--){
        for(int j=0;j<6;j++){
            if( (estado.See_Casilla(j,i)%3==estado.See_Casilla(j+1,i-1)%3)){
                if(estado.See_Casilla(j,i)%3==jugador){
                    valor+=-150;
                }else if(estado.See_Casilla(j,i)%3==(jugador%2)+1){
                    valor+=150;
                }
            }
        }
    }

    //diagonales hacia abajo de 3
    for(int i=0; i<5; i++){
        for(int j=0;j<5;j++){
            if( (estado.See_Casilla(j,i)%3==estado.See_Casilla(j+1,i+1)%3) && (estado.See_Casilla(j+1,i+1)%3==estado.See_Casilla(j+2,i+2)%3) ){
                if(estado.See_Casilla(j,i)%3==jugador){
                    valor+=-3000;
                }else if(estado.See_Casilla(j,i)%3==(jugador%2)+1){
                    valor+=3000;
                }
            }
        }
    }

    //diagonales hacia abajo de 2
    for(int i=0; i<6; i++){
        for(int j=0;j<6;j++){
            if( (estado.See_Casilla(j,i)%3==estado.See_Casilla(j+1,i+1)%3)){
                if(estado.See_Casilla(j,i)%3==jugador){
                    valor+=-150;
                }else if(estado.See_Casilla(j,i)%3==(jugador%2)+1){
                    valor+=150;
                }
            }
        }
    }

    return valor;

}


// Funcion heuristica
double Valoracion(const Environment &estado, int jugador){

    double valor=0;

    if(estado.RevisarTablero() == jugador){//he ganado
        valor= masinf;
    }else if(estado.RevisarTablero()==0){
        valor+=ValoracionHorizontalVertical(estado,jugador);
        valor+=ValoracionDiagonal(estado,jugador);
        valor+=rand() % 100;
    }else{
        valor= menosinf;
    }
    return valor;
}




// Esta funcion no se puede usar en la version entregable
// Aparece aqui solo para ILUSTRAR el comportamiento del juego
// ESTO NO IMPLEMENTA NI MINIMAX, NI PODA ALFABETA
void JuegoAleatorio(bool aplicables[], int opciones[], int &j){
    j=0;
    for (int i=0; i<8; i++){
        if (aplicables[i]){
           opciones[j]=i;
           j++;
        }
    }
}






// Invoca el siguiente movimiento del jugador
Environment::ActionType Player::Think(){
    const int PROFUNDIDAD_MINIMAX = 4;  // Umbral maximo de profundidad para el metodo MiniMax
    const int PROFUNDIDAD_ALFABETA = 8; // Umbral maximo de profundidad para la poda Alfa_Beta

    Environment::ActionType accion; // acci�n que se va a devolver
    bool aplicables[8]; // Vector bool usado para obtener las acciones que son aplicables en el estado actual. La interpretacion es
                        // aplicables[0]==true si PUT1 es aplicable
                        // aplicables[1]==true si PUT2 es aplicable
                        // aplicables[2]==true si PUT3 es aplicable
                        // aplicables[3]==true si PUT4 es aplicable
                        // aplicables[4]==true si PUT5 es aplicable
                        // aplicables[5]==true si PUT6 es aplicable
                        // aplicables[6]==true si PUT7 es aplicable
                        // aplicables[7]==true si BOOM es aplicable



    double valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alfa=menosinf, beta=masinf; // Cotas de la poda AlfaBeta

    int n_act; //Acciones posibles en el estado actual


    n_act = actual_.possible_actions(aplicables); // Obtengo las acciones aplicables al estado actual en "aplicables"
    int opciones[10];

    // Muestra por la consola las acciones aplicable para el jugador activo
    //actual_.PintaTablero();
    cout << " Acciones aplicables ";
    (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
    for (int t=0; t<8; t++)
      if (aplicables[t])
         cout << " " << actual_.ActionStr( static_cast< Environment::ActionType > (t)  );
    cout << endl;

    /*
    //--------------------- COMENTAR Desde aqui
    cout << "\n\t";
    int n_opciones=0;
    JuegoAleatorio(aplicables, opciones, n_opciones);

    if (n_act==0){
      (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
      cout << " No puede realizar ninguna accion!!!\n";
      //accion = Environment::actIDLE;
    }
    else if (n_act==1){
           (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
            cout << " Solo se puede realizar la accion "
                 << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[0])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[0]);

         }
         else { // Hay que elegir entre varias posibles acciones
            int aleatorio = rand()%n_opciones;
            cout << " -> " << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[aleatorio])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[aleatorio]);
         }

    //--------------------- COMENTAR Hasta aqui


    Environment estados_siguientes[8];
    int n_estados_siguientes;
    n_estados_siguientes=actual_.GenerateAllMoves(estados_siguientes);//calcular la lista de estados siguientes
    //iterarpor la lista de estadpos siguientes
    bool hayJugadaGanadora = false;
    for(int i=0; i<n_estados_siguientes && !hayJugadaGanadora; i++){

      Environment e = estados_siguientes[i];

      if(e.RevisarTablero() == jugador_){//he ganado
        int n_action=e.Last_Action(jugador_);//pillo la ultima accion
        accion = static_cast< Environment::ActionType >( n_action );//lo paso a actionType en vez de numero
        hayJugadaGanadora=true;

        cout<<"Se ha encontrado jugada ganadora " <<actual_.ActionStr(accion) <<endl;
      }
    }
    */

    //--------------------- AQUI EMPIEZA LA PARTE A REALIZAR POR EL ALUMNO ------------------------------------------------


    // Opcion: Poda AlfaBeta
    // NOTA: La parametrizacion es solo orientativa
    Environment estados_siguientes[8];
    int n_estados_siguientes;
    n_estados_siguientes=actual_.GenerateAllMoves(estados_siguientes);//calcular la lista de estados siguientes
    //iterarpor la lista de estadpos siguientes
    bool hayJugadaGanadora = false;
    for(int i=0; i<n_estados_siguientes && !hayJugadaGanadora; i++){

        Environment e = estados_siguientes[i];

        if(e.RevisarTablero() == jugador_){//he ganado
            int n_action=e.Last_Action(jugador_);//pillo la ultima accion
            accion = static_cast< Environment::ActionType >( n_action );//lo paso a actionType en vez de numero
            hayJugadaGanadora=true;

            cout<<"Se ha encontrado jugada ganadora " <<actual_.ActionStr(accion) <<endl;
        }
    }
    if(!hayJugadaGanadora)valor=Poda_AlfaBeta(actual_, jugador_, PROFUNDIDAD_ALFABETA, 0, accion,alfa,beta);
    cout << "Valor MiniMax: " << valor << "  Accion: " << actual_.ActionStr(accion) << endl;

    return accion;

}

double Player::Poda_AlfaBeta(Environment actual, int jugador, int PROFUNDIDAD_ALFABETA, int profundidad, Environment::ActionType &accion,double alfa, double beta){//Max pares Min impares
                                                                                                                                        //alfa max beta min
    if(profundidad==PROFUNDIDAD_ALFABETA || actual.RevisarTablero()==jugador || actual.RevisarTablero()==(jugador%2)+1){ //nodo hoja
        return Valoracion(actual,jugador);
    }
    Environment estados_siguientes[8];
    int n_estados_siguientes=actual.GenerateAllMoves(estados_siguientes); //calcular la lista de estados siguientes
    //double best = (profundidad%2==0)?menosinf:masinf;

	double max_peso=menosinf, min_peso=masinf;	//MODIFICADO


    if(profundidad%2==0){
        double peso;
        for(int i=0; i<n_estados_siguientes; i++){
            peso=0;
            peso=Poda_AlfaBeta(estados_siguientes[i],jugador,PROFUNDIDAD_ALFABETA,profundidad+1,accion,alfa,beta);

            if(peso>alfa){
                alfa=peso;
            }

			if(peso>max_peso){
				max_peso=peso;
				if(profundidad==0){
					accion = static_cast< Environment::ActionType > ( estados_siguientes[i].Last_Action(jugador) );
				}
			}

            // Alpha Beta Pruning
            if (beta <= alfa){
                break;
            }
        }
		return max_peso;

    }else{
        double peso;
        for(int i=0; i<n_estados_siguientes; i++){
            peso=0;
            peso=Poda_AlfaBeta(estados_siguientes[i],jugador,PROFUNDIDAD_ALFABETA,profundidad+1,accion,alfa,beta);

            if(peso<beta){
                beta=peso;
            }
			if(min_peso>peso){
				min_peso=peso;
			}

            // Alpha Beta Pruning
            if (beta <= alfa){
                break;
            }
        }
		return min_peso;

    }

}
