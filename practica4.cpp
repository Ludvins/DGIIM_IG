// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Práctica 2  (implementación)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica3.hpp"
#include "grafo-escena.hpp"


using namespace std ;

// COMPLETAR: práctica 4: declaración de variables de la práctica 4 (static)
// ....

string p4_peon_file = "../plys/peon.ply";
string p4_lata_file = "../plys/lata-pcue.ply";
string p4_lata_inf = "../plys/lata-pinf.ply";
string p4_lata_sup = "../plys/lata-psup.ply";
uint p4_resolucion_objs = 4;
static unsigned objetoActivo4 = 0;
static constexpr int numObjetos4 = 2;
static NodoGrafoEscena* objetos4[numObjetos4] = {nullptr, nullptr};

ColFuentesLuz p4_fuentes;
int p4_angulo_actual = 0;
int p4_fuente_activa = 0;
bool p4_focos_activos = true;



// ---------------------------------------------------------------------
// Función para implementar en la práctica 4 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e
// incializado OpenGL.

void P4_Inicializar(  )
{

   cout << "Creando objetos de la práctica 4 .... " << flush ;

   p4_fuentes.insertar(new FuenteLuzDireccional(-10,30,Tupla4f{0.5,0.5,0.5,1.0}));
   p4_fuentes.insertar( new FuenteLuzPosicional( {10,10,10}, {0.1,0.1,0.1,0} ) );
   p4_fuentes.activar();
   objetos4[0] = new EscenaP4();

   cout << "hecho." << endl << flush ;
}

// ---------------------------------------------------------------------
// Función invocada al pulsar una tecla con la práctica 4 activa:
// (si la tecla no se procesa en el 'main').
//
//  - devuelve 'true' si la tecla se usa en esta práctica para cambiar
//    entre el grado de libertad o incrementar/decrementar valores
//  - devuelve 'false' si la tecla no se usa en esta práctica (no ha
//    cambiado nada)

bool P4_FGE_PulsarTeclaCaracter( unsigned char tecla )
{
   bool res = false  ; // valor devuelto: es true solo cuando se ha procesado alguna tecla

   switch ( toupper( tecla ) )
   {

   case 'O' :

     objetoActivo4 = (objetoActivo4+1) % numObjetos4 ;
     cout << "Práctica 4: nuevo objeto activo es: " << objetoActivo4;
     if ( objetos4[objetoActivo4] != nullptr )
       cout << " (" << objetos4[objetoActivo4]->leerNombre() << ")." << endl ;
     else
       cout << " (objeto no creado)" << endl ;

     res = true ;

     break ;

   case 'F' :
     p4_fuente_activa = (p4_fuente_activa + 1) % p4_fuentes.size();
     cout << "La fuente seleccionada es la numero " << p4_fuente_activa << endl;
     res = true;
     break;

   case 'L' :

     p4_focos_activos = !p4_focos_activos;

     if (p4_focos_activos) {
       cout << "Focos activados" << endl;
       p4_fuentes.activar();
     }
     else {
       cout << "Focos desactivados" << endl;
       p4_fuentes.desactivar();
     }
     res = true;
     break;
   case 'G' :
     // DONE práctica 4: activar el siguiente ángulo (longitud o latitud)
     // ....
     // Conmutamos entre alpha y beta

     p4_angulo_actual = (p4_angulo_actual + 1) %2;
     std::cout << "Ángulo actual: beta";
     p4_angulo_actual==0
       ? cout << " beta " << endl
       : cout << " alpha " << endl;

     res = true;
     break ;

   case '>' :
     // COMPLETAR: práctica 4: incrementar el ángulo activo
     // ....

     p4_angulo_actual == 0
       ? res = p4_fuentes.ptrFuente(p4_fuente_activa)->gestionarEventoTeclaEspecial(GLFW_KEY_UP)
       : res = p4_fuentes.ptrFuente(p4_fuente_activa)->gestionarEventoTeclaEspecial(GLFW_KEY_RIGHT);

     break;

   case '<' :
     // COMPLETAR: práctica 4: decrementar el ángulo activo
     // ....

     p4_angulo_actual == 0
       ? res = p4_fuentes.ptrFuente(p4_fuente_activa)->gestionarEventoTeclaEspecial(GLFW_KEY_DOWN)
       : res = p4_fuentes.ptrFuente(p4_fuente_activa)->gestionarEventoTeclaEspecial(GLFW_KEY_LEFT);

       break;

   default :
     break;
 
   return res ;

   }

}

// ---------------------------------------------------------------------
// Función a implementar en la práctica 4  para dibujar los objetos
// se debe de usar el modo de dibujo que hay en el parámetro 'cv'
// (se accede con 'cv.modoVisu')

void P4_DibujarObjetos( ContextoVis & cv )
{
   // COMPLETAR: práctica 4: visualizar objetos
   //     (requiere activar las fuentes de luz y luego dibujar el grafo de escena)
   // ....

  objetos4[objetoActivo4]->visualizarGL(cv);

}

