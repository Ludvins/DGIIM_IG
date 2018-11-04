// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Práctica 2  (implementación)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica3.hpp"
#include "grafo-escena.hpp"
#include "MallaPLY.hpp"

using namespace std ;

// COMPLETAR: práctica 3: declarar variables de la práctica 3 (static)
// .................

static unsigned objetoActivo3 = 0;
static constexpr int numObjetos3 = 3;
static NodoGrafoEscenaParam* objetos3[numObjetos3] = {nullptr, nullptr, nullptr};
static Muneco* mu = nullptr;
static ejercicio1* ej = nullptr;
static ejercicio2* ej2 = nullptr;
static bool animaciones = false;
static unsigned parametro_activo = 0;

// ---------------------------------------------------------------------
// Función para implementar en la práctica 1 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e
// incializado OpenGL.

void P3_Inicializar(  )
{
   cout << "Creando objetos de la práctica 3 .... " << flush ;
   // DONE: práctica 3: inicialización de objetos de la práctica 3
   // .................

   mu = new Muneco();
   ej = new ejercicio1();
   ej2 = new ejercicio2();
   objetos3[0] = mu;
   objetos3[1] = ej;
   objetos3[2] = ej2;

   cout << "hecho." << endl << flush ;
}

// ---------------------------------------------------------------------
// Función invocada al pulsar una tecla con la práctica 1 activa:
// (si la tecla no se procesa en el 'main').
//
//  - devuelve 'true' si la tecla se usa en esta práctica para cambiar
//    entre el grado de libertad o incrementar/decrementar valores
//  - devuelve 'false' si la tecla no se usa en esta práctica (no ha
//    cambiado nada)

bool P3_FGE_PulsarTeclaCaracter( unsigned char tecla )
{
   bool res = false  ; // valor devuelto (true si se ha procesado tecla)

   switch ( toupper( tecla ) )
   {
      case 'O' :
         // DONE: práctica 3: activar siguiente objeto de la práctica
         //           (solo en el caso de que se incluyan varios objetos en la práctica)
         // ....

        objetoActivo3 = (objetoActivo3+1) % numObjetos3 ;
        cout << "Práctica 3: nuevo objeto activo es: " << objetoActivo3 ;
        if ( objetos3[objetoActivo3] != nullptr )
          cout << " (" << objetos3[objetoActivo3]->leerNombre() << ")." << endl ;
        else
          cout << " (objeto no creado)" << endl ;
        return true ;


        res = true;
         break ;

      case 'A' :
         // DONE: práctica 3: activar o desactivar animaciones
         // ....
        animaciones = !animaciones;

        if (animaciones){
          std::cout << "Activando animaciones." << std::endl;
          FijarFuncDesocupado(FGE_Desocupado);
        }
        else
          std::cout << "Desactivando animaciones." << std::endl;

        res = true;
         break ;

      case 'G' :
         // DONE: práctica 3: activar siguiente parámetro
         // ....

        parametro_activo = (parametro_activo + 1) % (objetos3[objetoActivo3]->numParametros());
        std::cout << "Parametro activo " << parametro_activo << std::endl;


        res = true;
         break ;

      case 'R' :
         // DONE: práctica 3: reset de animaciones
         // ....
        animaciones = false;

        for ( int i = 0; i < objetos3[objetoActivo3]->numParametros(); i++)
          objetos3[objetoActivo3]->leerPtrParametro(i)->reset();

        std::cout << "Resenteando animaciones" << std::endl;
        res = true;
        break ;

      case '>' :
         // DONE: práctica 3: acelerar o incrementar parámetro
         // ....

        if (!animaciones)
          objetos3[objetoActivo3]->leerPtrParametro(parametro_activo)->incrementar();
        else
          objetos3[objetoActivo3]->leerPtrParametro(parametro_activo)->acelerar();


          res = true;
         break ;

      case '<' :
         // DONE: práctica 3: decelerar o decrementar parámetro
         // ....

        if (!animaciones)
          objetos3[objetoActivo3]->leerPtrParametro(parametro_activo)->decrementar();
        else
          objetos3[objetoActivo3]->leerPtrParametro(parametro_activo)->decelerar();
        res = true;
        break ;

      default :
         break ;

   }
   return res ;
}

// ---------------------------------------------------------------------
// Función a implementar en la práctica 3  para dibujar los objetos
// se debe de usar el modo de dibujo que hay en el parámetro 'cv'
// (se accede con 'cv.modoVisu')

void P3_DibujarObjetos( ContextoVis & cv )
{
   // DONE: práctica 3: visualizar el objeto de la práctica 3
   // ....

  objetos3[objetoActivo3]->visualizarGL(cv);

}

//--------------------------------------------------------------------------

bool P3_FGE_Desocupado()
{
   // DONE: práctica 3: si las animaciones están activadas,
   // actualizar el objeto activo, forzar redibujado, devolver true.
   // si las animaciones están desactivadas, devolver false.
   // ....

  if (!animaciones)
    return false;

  objetos3[objetoActivo3]->siguienteCuadro();

  redibujar_ventana = true;

  return true;


}
