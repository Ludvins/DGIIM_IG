// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Práctica 4  (implementación)
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

void DibujarEscena();

string p4_peon_file = "../plys/peon.ply";
string p4_lata_file = "../plys/lata-pcue.ply";
string p4_lata_inf = "../plys/lata-pinf.ply";
string p4_lata_sup = "../plys/lata-psup.ply";
static unsigned objetoActivo4 = 0;
static constexpr int numObjetos4 = 4;
static NodoGrafoEscena* objetos4[numObjetos4] = {nullptr, nullptr, nullptr, nullptr};
const VectorRGB rojo = {1.0, 0.0, 0.0, 1.0};
ColeccionFuenteP4 p4_fuentes;
unsigned p4_fuente_activa = 0;
unsigned p4_angulo_actual = 0;

// MaterialCambiante* m = nullptr;
// Matriz4f rotacion = MAT_Rotacion(10, 0, 0, 1);
// ColFuentesLuz* col_test = nullptr;
// FuenteLuzDireccional* f1 = new FuenteLuzDireccional(45, 45, rojo);
// unsigned ident_f1 = 0;
// bool f1_activada = false;
// FuenteLuzPosicional* f2 = new FuenteLuzPosicional({ -1.825,1.825,1.825}, rojo);
// unsigned ident_f2 = 1;
// bool f2_activada = false;

// // ---------------------------------------------------------------------
// Función para implementar en la práctica 4 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e
// incializado OpenGL.

void P4_Inicializar(  )
{
  //col_test = new ColFuentesLuz();
  //col_test->insertar(f1);
  //col_test->insertar(f2);
  // m = new MaterialCambiante(
  //                           {0.314, 0.784, 0.471},
  //                           {1.0,0.8,0.0},
  //                           0.0,
  //                           0.0,
  //                           0.6,
  //                           0.6,
  //                           0.4,
  //                           0.4,
  //                           10,
  //                           10
  //                          );

   cout << "Creando objetos de la práctica 4 .... " << flush ;
   objetos4[0] = new EscenaP4();
   //objetos4[1] = new Test(m);
   //objetos4[2] = new Moneda();
   //objetos4[3] = new CuboColores();
   //objetos4[3] = new EsferaTest();

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
   // case 'L':
   //   glEnable(GL_LIGHTING);
   //   col_test->activar(ident_f1);
   //   f1_activada = true;
   //   res = true;
   //   break;

   // case 'J':
   //   glEnable(GL_LIGHTING);
   //   col_test->activar(ident_f2);
   //   f2_activada = true;
     // res = true;
   //   break;

   // case 'I':
   //   f2->posicion = rotacion*f2->posicion;
   //   res = true;
   //   break;
   // case 'H':
   //   m->cambiar();
   //   res = true;
   //   break;

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


   case 'G' :
     // DONE práctica 4: activar el siguiente ángulo (longitud o latitud)
     // ....
     // Conmutamos entre alpha y beta

     p4_angulo_actual = (p4_angulo_actual + 1) %2;
     std::cout << "Ángulo actual:";
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
   }

   return res ;
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

  p4_fuentes.activar();
  //fuente_test_1->activar();

  //if(f2_activada)
  //  col_test->activar(ident_f2);

  if (objetos4[objetoActivo4] != nullptr)
    objetos4[objetoActivo4]->visualizarGL(cv);

}

