// *********************************************************************
// **
// ** Informática Gráfica
// ** Práctica 5  (implementaciones)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica5.hpp"
#include "grafo-escena.hpp"
#include "materiales.hpp"
#include "CamaraInter.hpp"
using namespace std ;

// COMPLETAR: práctica 5: declarar variables de la práctica 5 (static)
//    (escena, viewport, el vector de camaras, y las variables
//      que sirven para gestionar el modo arrastrar)
// ......

// Desplazamiento de la cámara actual en el eje Z
static const float DELTA = 5.0;
// viewport actual (se recalcula al inicializar y al fijar las matrices)
static Viewport viewport;
// true si se está en modo arrastrar, false si no
static bool modo_arrastrar = false ;
// número de cámaras
static constexpr int numCamaras = 4;
// vector de cámaras con vista como mínimo de alzado, planta y perfil
static CamaraInteractiva * camaras[numCamaras] = {nullptr, nullptr, nullptr, nullptr};
// cámara activa
static int camaraActiva = 0;
// posiciones anteriores del ratón
static int xant,
  yant;
// grafo de escena
static constexpr int numObjetos5 = 1;
static NodoGrafoEscena * objetos5[numObjetos5] = {nullptr};
static ColFuentesLuz * cf5 = nullptr;


// ---------------------------------------------------------------------

void P5_Inicializar(  int vp_ancho, int vp_alto )
{
   cout << "Creando objetos de la práctica 5 .... " << flush ;
   // COMPLETAR: práctica 5: inicializar las variables de la práctica 5 (incluyendo el viewport)
   // .......


   objetos5[0] = new EscenaP4();
   viewport = Viewport(0, 0, vp_ancho, vp_alto);
   float ratio = (float) vp_alto / vp_ancho;
   // Alzado
   camaras[0] = new CamaraInteractiva (true, ratio, 0, 0,
                                       {0,0,0}, true);
   // Perfil
   camaras[1] = new CamaraInteractiva (true, ratio, 90, 0,
                                       {0,0,0}, true);
   // Planta
   camaras[2] = new CamaraInteractiva (true, ratio, 0, 90,
                                       {0,0,0}, true);
   // Alzado ortográfica
   camaras[3] = new CamaraInteractiva (true, ratio, 0, 0,
                                       {0,0,0}, false);
   cout << "hecho." << endl << flush ;
}
// ---------------------------------------------------------------------

void P5_FijarMVPOpenGL( int vp_ancho, int vp_alto )
{

  // COMPLETAR: práctica 5: actualizar viewport, actualizar y activar la camara actual
  //     (en base a las dimensiones del viewport)
  // .......


  viewport.ancho = vp_ancho;
  viewport.alto = vp_alto;
  camaras[camaraActiva]->ratio_yx_vp = (float) vp_alto / vp_ancho;
  camaras[camaraActiva]->calcularViewfrustum(); // recalcular m_proy

  camaras[camaraActiva]->activar();


}
// ---------------------------------------------------------------------

void P5_DibujarObjetos( ContextoVis & cv )
{
  // COMPLETAR: práctica 5: activar las fuentes de luz y visualizar la escena
  //      (se supone que la camara actual ya está activada)
  // .......

  // Se supone que la cámara ya está activada

  if (cf5 != nullptr)
    cf5->activar();

  if (objetos5[0] != nullptr)
    objetos5[0]->visualizarGL(cv);


}

// ---------------------------------------------------------------------

bool P5_FGE_PulsarTeclaCaracter(  unsigned char tecla )
{
   bool result = true ;
   bool examinar;

   switch ( toupper( tecla ) )
   {
      case 'C':
        camaraActiva = (camaraActiva + 1) % numCamaras;
        camaras[camaraActiva]->activar();
        cout << "práctica 5: nueva cámara activa es " << camaraActiva << endl;

        break ;

      case 'V':
         examinar = camaras[camaraActiva]->examinar;
         if (examinar)
           camaras[camaraActiva]->modoPrimeraPersona();
         else
            camaras[camaraActiva]->modoExaminar();
         cout << "práctica 5: modo de la cámara " << camaraActiva
              << " cambiado a " << (examinar ? "primera persona" : "examinar") << endl;

         break ;

      case '+':
         camaras[camaraActiva]->desplaZ(DELTA);
         cout << "práctica 5: desplazamiento en el eje Z positivo" << endl;

         break;

      case '-':
        camaras[camaraActiva]->desplaZ(-DELTA);
        cout << "práctica 5: desplazamiento en el eje Z negativo" << endl;

         break;

      default:
         result = false ;
         break ;
	}

return result ;

}
// ---------------------------------------------------------------------

bool P5_FGE_PulsarTeclaEspecial(  int tecla  )
{

  bool result = true ;

  switch ( tecla )
    {
    case GLFW_KEY_LEFT:
      camaras[camaraActiva]->moverHV(-DELTA, 0);
      cout << "práctica 5: desplazamiento hacia la izquierda" << endl;

      break;
    case GLFW_KEY_RIGHT:
      camaras[camaraActiva]->moverHV(DELTA, 0);
      cout << "práctica 5: desplazamiento hacia la derecha" << endl;

      break;
    case GLFW_KEY_UP:
      camaras[camaraActiva]->moverHV(0, DELTA);
      cout << "práctica 5: desplazamiento hacia arriba" << endl;

      break;
    case GLFW_KEY_DOWN:
      camaras[camaraActiva]->moverHV(0, -DELTA);
      cout << "práctica 5: desplazamiento hacia abajo" << endl;

      break;
    default:
      result = false ;
      break ;
    }

  return result ;

}
// ---------------------------------------------------------------------
// se llama al hacer click con el botón izquierdo

void P5_ClickIzquierdo( int x, int y )
{

   // COMPLETAR: práctica 5: visualizar escena en modo selección y leer el color del pixel en (x,y)


   // 1. crear un 'contextovis' apropiado
   // .....
  ContextoVis contexto;
  contexto.modoSeleccionFBO = true;

  glClearColor(0, 0, 0, 1); // color de fondo
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // limpiar pantalla

   // 2. visualizar en modo selección (sobre el backbuffer)
   // ....
  objetos5[0]->visualizarGL( contexto );

   // 3. leer el color del pixel, si es 0 no se hace nada
   // .....
  int ident_leido = LeerIdentEnPixel(x, y);
  if ( ident_leido <= 0 ) {
    cout << "Ningun objeto seleccionado" << endl;
    return;
  }

   // 4. buscar el objeto en el grafo de escena e informar del mismo
   // .....
  Objeto3D* p_obj;
  Tupla3f centro;
  if ( !objetos5[0]->buscarObjeto( ident_leido, MAT_Ident(),
                          &p_obj, centro ) ) {
    cout << "ERROR: Objeto no encontrado en el arbol. ";
    cout << "Identificador: " << ident_leido << endl;
    return;
  }

  cout << "Objeto fijado con identificador: " << ident_leido << endl;
  camaras[camaraActiva]->modoExaminar( centro );
}
// ---------------------------------------------------------------------
// se llama al mover el botón en modo arrastrar

void P5_InicioModoArrastrar( int x, int y )
{
   // COMPLETAR: práctica 5: activar bool de modo arrastrar, registrar (x,y) de inicio del modo arrastrar
   // .....
  modo_arrastrar = true;
  xant = x;
  yant = y;
}
// ---------------------------------------------------------------------
// se llama al subir el botón derecho del ratón

void P5_FinModoArrastrar()
{
   // COMPLETAR: práctica 5: desactivar bool del modo arrastrar
   // .....
  modo_arrastrar = false;
}
// ---------------------------------------------------------------------
// se llama durante el modo arrastrar

void P5_RatonArrastradoHasta( int x, int y )
{
   // COMPLETAR: práctica 5: calcular desplazamiento desde inicio de modo arrastrar, actualizar la camara (moverHV)
   // .....

   // mover cámara
  camaras[camaraActiva]->moverHV(x - xant, y - yant);

  // registrar última posición
  xant = x;
  yant = y;
}
// ---------------------------------------------------------------------
// pulsar/levantar botón del ratón, específico de la práctica 5

bool P5_FGE_ClickRaton( int boton, int estado, int x, int y )
{
   //cout << "P5_FGE_ClickRaton" << endl ;
   if ( estado == GLFW_PRESS && boton == GLFW_MOUSE_BUTTON_LEFT  )
      P5_ClickIzquierdo( x, viewport.alto-y );
   else if ( estado == GLFW_PRESS && boton == GLFW_MOUSE_BUTTON_RIGHT )
      P5_InicioModoArrastrar( x, viewport.alto-y );
   else if ( estado == GLFW_RELEASE && boton == GLFW_MOUSE_BUTTON_RIGHT )
      P5_FinModoArrastrar();
   else
      return false ;

   return true ;
}
// ---------------------------------------------------------------------
bool P5_FGE_RatonMovidoPulsado( int x, int y )
{
   if ( modo_arrastrar )
   {
      P5_RatonArrastradoHasta( x, viewport.alto-y );
      return true ;
   }

   return false ;
}
// ---------------------------------------------------------------------

bool P5_FGE_Scroll( int direction )
{
   // COMPLETAR: práctica 5: acercar/alejar la camara (desplaZ)
   // .....
  camaras[camaraActiva]->desplaZ(direction * DELTA);
  return true ;
}
// ---------------------------------------------------------------------

void FijarColorIdent( const int ident ) { // 0 ≤ ident < 2^24
  // COMPLETADO: fijar color actual de OpenGL usando 'ident' (glColor3ub)
  const unsigned char
    byteR = ( ident ) % 0x100U,             // rojo = byte menos significativo
    byteG = ( ident / 0x100U ) % 0x100U,    // verde = byte intermedio
    byteB = ( ident / 0x10000U ) % 0x100U;  // azul = byte más significativo
  glColor3ub( byteR, byteG, byteB );
}

//---------------

int LeerIdentEnPixel( int xpix, int ypix ) {
  // COMPLETADO:leer el identificador codificado en el color del pixel (x,y)
  unsigned char bytes[3] ; // para guardar los tres bytes
  // leer los 3 bytes del frame-buffer
  glReadPixels( xpix, ypix, 1, 1, GL_RGB,GL_UNSIGNED_BYTE, (void *)bytes);
  // reconstruir el indentificador y devolverlo:
  return bytes[0] + ( 0x100U* bytes[1] ) + ( 0x10000U* bytes[2] ) ;
}
