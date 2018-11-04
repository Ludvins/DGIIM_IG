// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Práctica 2  (implementación)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica2.hpp"
#include "Objeto3D.hpp"
#include "MallaPLY.hpp"
#include "MallaRevol.hpp"

using namespace std ;

static unsigned objetoActivo2 = 0 ;
static constexpr int numObjetos2 = 6 ;

static Objeto3D* objetos2[numObjetos2] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
static MallaPLY* malla_ply = nullptr;
static MallaRevol* malla_revol = nullptr;
static MallaRevol* malla_revol2 = nullptr;
static Cilindro* cilindro = nullptr;
static Cono* cono = nullptr;
static Esfera* esfera = nullptr;

// ---------------------------------------------------------------------
// Función para implementar en la práctica 1 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e
// incializado OpenGL.

void P2_Inicializar(  )
{
   cout << "Creando objetos de la práctica 2 .... " << flush ;
   // COMPLETAR: práctica 2: inicializar objeto de revolución y objeto PLY (en 'objeto2')
   // ...

   malla_ply = new MallaPLY("../plys/ant.ply");
   malla_revol = new MallaRevol("../plys/peon.ply", 10, true, true);
   malla_revol2 = new MallaRevol("../plys/peon.ply", 10, true, false);
   cilindro = new Cilindro(2, 10, true, true);
   cono = new Cono(5, 100, true, true);
   esfera = new Esfera(100, 10, true, true);

   objetos2[0] = malla_ply;
   objetos2[1] = malla_revol;
   objetos2[2] = malla_revol2;
   objetos2[3] = cilindro;
   objetos2[4] = cono;
   objetos2[5] = esfera;
   cout << "Hecho." << endl << flush ;
}

// ---------------------------------------------------------------------
// Función invocada al pulsar una tecla con la práctica 1 activa:
// (si la tecla no se procesa en el 'main').
//
//  - devuelve 'true' si la tecla se usa en esta práctica para cambiar
//    entre el cubo, el tetraedro u otros objetos (cambia el valor de
//    'objeto_activo').
//  - devuelve 'false' si la tecla no se usa en esta práctica (no ha
//    cambiado nada)

bool P2_FGE_PulsarTeclaCaracter( unsigned char tecla )
{
   if ( toupper(tecla) != 'O')
      return false ;

   objetoActivo2 = (objetoActivo2+1) % numObjetos2 ;
   cout << "Práctica 2: nuevo objeto activo es: " << objetoActivo2 ;
   if ( objetos2[objetoActivo2] != nullptr )
      cout << " (" << objetos2[objetoActivo2]->leerNombre() << ")." << endl ;
   else
      cout << " (objeto no creado)" << endl ;
   return true ;
}

// ---------------------------------------------------------------------
// Función a implementar en la práctica 1  para dibujar los objetos
// se debe de usar el modo de dibujo que hay en el parámetro 'cv'
// (se accede con 'cv.modoVisu')

void P2_DibujarObjetos( ContextoVis & cv )
{
   // COMPLETAR: práctica 2: invocar a 'visualizarGL' para el objeto activo actual en 'objetos2'
   // ...

  objetos2[objetoActivo2]->visualizarGL(cv);



}
