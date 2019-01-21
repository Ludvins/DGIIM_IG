// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Práctica 1  (implementación)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica1.hpp"
#include "Objeto3D.hpp"
#include "MallaInd.hpp"

static unsigned      objetoActivo       = 0 ; // objeto activo: cubo (0), tetraedro (1), otros....
static constexpr int numObjetos         = 2 ; // numero de objetos creados para esta práctica
static Objeto3D *    objeto[numObjetos] = { nullptr, nullptr }; // punteros a los objetos

using namespace std ;

// ---------------------------------------------------------------------
// Función para implementar en la práctica 1 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e
// incializado OpenGL.

void P1_Inicializar(  )
{
   cout << "Creando objetos de la práctica 1 .... " << flush ;
   // COMPLETAR: práctica 1: asignar valores a 'objeto[0]' y 'objeto[1]'
   // (punteros a mallas)

   objeto[0] = new Cubo();

   cout << "Primer objeto creado." << endl << flush;

   objeto[1] = new Tetraedro();

   cout << "hecho." << endl << flush ;
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

bool P1_FGE_PulsarTeclaCaracter( unsigned char tecla )
{
   if ( toupper(tecla) != 'O')
      return false ;

   objetoActivo = (objetoActivo+1) % numObjetos ;
   cout << "práctica 1: nuevo objeto activo es: " << objetoActivo ;
   if ( objeto[objetoActivo] != nullptr )
      cout << " (" << objeto[objetoActivo]->leerNombre() << ")." << endl ;
   else
      cout << " (objeto no creado)" << endl ;
   return true ;
}


// ---------------------------------------------------------------------
// Función a implementar en la práctica 1  para dibujar los objetos

void P1_DibujarObjetos( ContextoVis & cv )
{
   // COMPLETAR: práctica 1: invocar a 'visualizarGL' para el objeto activo actual
   // ...

  objeto[objetoActivo]->visualizarGL(cv);


}



// void Ej2(int n){
//   float alpha = 360.0 / n;

//   Matriz4f m = MAT_Rotacion(alpha, 0,0,1);

//   auto p = Tupla3f(1,0,0);

//   std::vector<Tupla3f> v;
//   for(unsigned i = 0; i<n; ++i){
//     v.push_back(p);
//     p = m*p;
//   }

//   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//   glEnableClientState(GL_VERTEX_ARRAY);
//   glColor3f(0,0,1);
//   glVertexPointer(3, GL_FLOAT, 0, v.data());
//   glDrawArrays(GL_POLYGON,0 ,n);
//   glDisableClientState(GL_VERTEX_ARRAY);

//   auto u = (1.0 - 0.1)/n;

//   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

//   for(unsigned i=0; i< n ; ++i){
//     auto r = 1.0 - i*u;
//     auto p0 = v[i%n] * r;
//     auto p1 = v[(i+1)%n] * r;
//     glColor3f(0.5,0.8,1.0);
//     glBegin(GL_TRIANGLES);
//     glVertex3fv(p0);
//     glVertex3fv(p1);
//     glVertex3fv(Tupla3f(0.0,0.0,0.0));
//     glEnd();

//     glColor3f(0,0,1);
//     glBegin(GL_LINES);
//     glVertex3fv(p0);
//     glVertex3fv(p1);
//     glEnd();
//   }

//   glDisable(GL_DEPTH_TEST);

// }
