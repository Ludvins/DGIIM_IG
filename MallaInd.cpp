// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

#include <aux.hpp>
#include <tuplasg.hpp>
#include "MallaInd.hpp"   // declaración de 'ContextoVis'

// *****************************************************************************
// funciones auxiliares


// *****************************************************************************
// métodos de la clase MallaInd.

MallaInd::MallaInd()
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre("Malla indexada, anónima");

}
// -----------------------------------------------------------------------------

MallaInd::MallaInd( const std::string & nombreIni )
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre(nombreIni) ;

}

// -----------------------------------------------------------------------------
// calcula las dos tablas de normales
void MallaInd::calcular_normales()
{
   // COMPLETAR: en la práctica 2: calculo de las normales de la malla
   // .......

  std::cout << "Calcular normales:" << std::endl;
  

}


// -----------------------------------------------------------------------------
void MallaInd::visualizarDE_MI( ContextoVis & cv )
{
   // COMPLETAR: en la práctica 1: visualizar en modo inmediato (glDrawElements)
   // ...........

  // Habilitar uso de un array de vértices.
  glEnableClientState( GL_VERTEX_ARRAY );

  // Especificar el puntero a tabla de coordenadas de vértices
  glVertexPointer( 3, GL_FLOAT, 0, vertices.data());

  // Dibujar usando vértices indexados. Parámetros.
  // - Tipo de primitiva
  // - Número de índices.
  // - Tipo de índices.
  // - Puntero a tabla de triángulos.
  glDrawElements(GL_TRIANGLES, caras.size()*3, GL_UNSIGNED_INT, caras.data());

  // Deshabilitar el array de vértices
  glDisableClientState( GL_VERTEX_ARRAY);


  // VERSION CON BEGIN-END.
  //   glBegin (GL_TRIANGLES);
  //   for ( int i = 0; i < tabla_caras.size(); i++)
  //     for ( int j = 0; j < 3; j++)
  //       {
  //         unsigned ind_ver = tabla_caras[i][j];
  //         glVertex3fv (tabla_vertices[ind_ver]);
  //       }

  //  glEnd();


}

// ----------------------------------------------------------------------------


GLuint VBO_Crear (GLuint tipo, GLuint size, GLvoid* p)
{

  assert ( tipo == GL_ARRAY_BUFFER || tipo == GL_ELEMENT_ARRAY_BUFFER);
  GLuint id_vbo;
  glGenBuffers ( 1, &id_vbo ); // Crear nuevo VBO con identificador en id_vbo
  glBindBuffer ( tipo, id_vbo ); //Activar el VBO usando su identificador
  glBufferData ( tipo , size, p, GL_STATIC_DRAW ); // Transferencia RAM -> GPU

  glBindBuffer ( tipo, 0) ; // Desactivar VBO

  return id_vbo;
}


void MallaInd::crearVBOs ()
{

  tam_ver = sizeof(float)*3*vertices.size();
  tam_tri = sizeof(unsigned)*3*caras.size();

  // GL_ARRAY_BUFFER - Vertex attributes
  // GL_ELEMENT_ARRAY_BUFFER - Vertex array indices.
  id_vbo_ver = VBO_Crear( GL_ARRAY_BUFFER, tam_ver, vertices.data());
  id_vbo_tri = VBO_Crear( GL_ELEMENT_ARRAY_BUFFER, tam_tri, caras.data()) ;

}

// ----------------------------------------------------------------------------
void MallaInd::visualizarDE_VBOs( ContextoVis & cv )
{
   // COMPLETAR: práctica 1: visualizar en modo diferido,
   //                        usando VBOs (Vertex Buffer Objects)
   // ..........

  // Inicializa VBO (solo una vez).

  crearVBOs();

  glBindBuffer ( GL_ARRAY_BUFFER, id_vbo_ver); // Activar VBO
  glVertexPointer ( 3, GL_FLOAT, 0, 0 ); // Seleccionamos el formato y el offset
  glBindBuffer ( GL_ARRAY_BUFFER, 0 ); // Desactivamos el VBO
  glEnableClientState ( GL_VERTEX_ARRAY ); // Activamos uso de VA.

  // Visualizamos con glDrawElements
  glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, id_vbo_tri );
  glDrawElements ( GL_TRIANGLES, caras.size()*3L, GL_UNSIGNED_INT, NULL );
  glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );

  // Desactivamos el array de vértices.
  glDisableClientState (GL_VERTEX_ARRAY);

}

// -----------------------------------------------------------------------------

void MallaInd::visualizarGL( ContextoVis & cv )
{
   // COMPLETAR: práctica 1: visualizar en modo inmediato o en modo diferido (VBOs),
   // (tener en cuenta el modo de visualización en 'cv' (alambre, sólido, etc..))
   //
   // .............

  GLenum mode;

  switch(cv.modoVis){
  case modoSolido:
    mode = GL_FILL;
    break;
  case modoPuntos:
    mode = GL_POINT;
    break;
  case modoAlambre:
  default:
    mode = GL_LINE;
  }

  glPolygonMode (GL_FRONT_AND_BACK, mode);

  cv.usarVBOs ? visualizarDE_VBOs(cv) : visualizarDE_MI(cv);

}
// *****************************************************************************

// *****************************************************************************

Cubo::Cubo( Tupla3f origen, float lado)
:  MallaInd( "Malla Cubo" )
{

  float origenx = origen[0],
    origeny = origen[1],
    origenz = origen[2];

  // Rojo, Verde, Azul
  Tupla3f cero = origen;
  Tupla3f uno = {origenx + lado, origeny, origenz};
  Tupla3f dos = {origenx + lado, origeny,origenz + lado};
  Tupla3f tres = {origenx, origeny, origenz + lado};
  Tupla3f cuatro = {origenx, origeny + lado, origenz};
  Tupla3f cinco = {origenx + lado, origeny + lado, origenz};
  Tupla3f seis = {origenx + lado,origeny + lado, origenz + lado};
  Tupla3f siete = {origenx, origeny + lado, origenz + lado};
  vertices = {cero, uno, dos, tres, cuatro, cinco, seis, siete};
  caras = {
                 {0,1,2} ,{0,2,3},
                 {0,1,4}, {1,4,5},
                 {0,3,7}, {0,4,7},
                 {2,1,5}, {2,5,6},
                 {2,3,7}, {2,7,6},
                 {4,5,6}, {4,6,7}
  };

 
// *****************************************************************************
}


Tetraedro::Tetraedro( )
:  MallaInd( "Malla Tetraedro")
{

  float origen = 0.0;
  float lado = 1.0;
  float altura = 1.0;

  // Numeración de la base en sentido horario.

  Tupla3f cero = {origen, origen, origen};
  Tupla3f uno = {lado, origen , origen};
  Tupla3f dos = {origen, origen, lado};
  Tupla3f punta = {origen, altura, origen};
  vertices = {cero, uno, dos, punta};
  caras = {{0,1,2},{0,1,3},{1,2,3},{0,2,3}};



}
// *****************************************************************************



