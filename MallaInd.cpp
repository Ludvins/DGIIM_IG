// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

#include <aux.hpp>
#include <tuplasg.hpp>
#include "MallaInd.hpp"   // declaración de 'ContextoVis'


#define MODO_INMEDIATO 0

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
   ponerNombre(nombreIni);
}

// -----------------------------------------------------------------------------
// calcula las dos tablas de normales
void MallaInd::calcular_normales()
{
   // TODO: en la práctica 2: calculo de las normales de la malla
   // .......

  std::cout << "Calcular normales:" << std::endl;

  assert(!vertices.empty() && !caras.empty());

  nor_vertices=std::vector<Tupla3f>(vertices.size(),Tupla3f{0.0,0.0,0.0});

  for( auto cara : caras ){

    Tupla3f vertice_1 = vertices[cara[0]],
      vertice_2 = vertices[cara[1]],
      vertice_3 = vertices[cara[2]];

    Tupla3f base_1 = vertice_2 - vertice_1;
    Tupla3f base_2 = vertice_3 - vertice_1;


    // std::cout << "Vertice 1: " << vertice_1 << std::endl;
    // std::cout << "Vertice 2: " << vertice_2 << std::endl;
    // std::cout << "Vertice 3: " << vertice_3 << std::endl;

    // std::cout << "Base 1: " << base_1 << std::endl;
    // std::cout << "Base 2: " <<base_2 << std::endl;

    Tupla3f normal = base_1.cross(base_2).normalized();

    //std::cout << normal[0] << normal[1] << normal[2] << std::endl;

    nor_caras.push_back(normal);

    for( int j = 0 ; j < 3; j++){
      nor_vertices[cara[j]] = (nor_vertices[cara[j]] + normal).normalized();
    }

  }

}

void MallaInd::fijarColorNodo(const Tupla3f& color) {

  col_ver.clear();
  for (unsigned i = 0; i < num_ver; i++)
    col_ver.push_back(color);
}

void MallaInd::setColorVertices(std::vector<Tupla3f>* colores)
{
  if (colores != nullptr) {
    col_ver.clear();
    for (unsigned i = 0; i < num_ver; i++)
      col_ver.push_back(colores->at(i));
  }

  else if (col_ver.size() == 0) { // Gradiente
    for (unsigned i = 0; i < num_ver; i++)
      col_ver.push_back({0.1, 0.1, (float) (i+1) / num_ver});
  }
}

// -----------------------------------------------------------------------------

void MallaInd::visualizarDE_NT ( ContextoVis& cv ){

  assert(num_caras != 0 && !caras.empty());

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glVertexPointer (3, GL_FLOAT, 0, vertices.data());
  glTexCoordPointer (2, GL_FLOAT, 0, texturas.data());
  glNormalPointer (GL_FLOAT, 0, nor_vertices.data());

  glDrawElements(GL_TRIANGLES, 3*num_caras, GL_UNSIGNED_INT, caras.data());

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

}

// ----------------------------------------------------------------------------

void MallaInd::visualizarVBOs_NT( ContextoVis& cv)
{
  if (!vbo_creado) crearVBOs();
  if (texturas.empty())
    std::cout << "JAJA LMAO " << std::endl;

  if(!texturas.empty())
    {
    glBindBuffer(GL_ARRAY_BUFFER,id_vbo_tex);
    glTexCoordPointer(2,GL_FLOAT,0,0);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  }

  if(!nor_vertices.empty())
    {
    glBindBuffer(GL_ARRAY_BUFFER,id_vbo_nor_ver);
    glNormalPointer(GL_FLOAT,0,0);
    glEnableClientState(GL_NORMAL_ARRAY);
    }

  // visualizar (el mismo método ya visto)
  visualizarDE_VBOs(cv);

  // Desactivar punteros a tablas
  glDisableClientState ( GL_TEXTURE_COORD_ARRAY);

}

// -----------------------------------------------------------------------------
void MallaInd::visualizarDE_MI( ContextoVis & cv )
{
   // DONE: en la práctica 1: visualizar en modo inmediato (glDrawElements)
   // ...........

#if MODO_INMEDIATO
    glBegin (GL_TRIANGLES);
    for (auto cara : caras) 
      for ( int j = 0; j < 3; j++)
        {
          if (!col_ver.empty())
            glColor3fv(col_ver[cara[j]]);

          glVertex3fv (vertices[cara[j]]);
        }
   glEnd();

#else

  // Habilitar uso de un array de vértices.
  glEnableClientState( GL_VERTEX_ARRAY );

  // Especificar el puntero a tabla de coordenadas de vértices
  glVertexPointer( 3, GL_FLOAT, 0, vertices.data());

  // Especificar y habilitar puntero a colores
  glColorPointer(3, GL_FLOAT, 0, col_ver.data());
  glEnableClientState (GL_COLOR_ARRAY);

  // Dibujar usando vértices indexados. Parámetros.
  // - Tipo de primitiva
  // - Número de índices.
  // - Tipo de índices.
  // - Puntero a tabla de triángulos.
  glDrawElements(GL_TRIANGLES, caras.size()*3, GL_UNSIGNED_INT, caras.data());
  /* NOTA: Si quisiéramos usar glDrawArrays (por ejemplo, para el modo puntos).
  *  glDrawArrays(GL_TRIANGLES, 0, 3L * tabla_vertices.size());
  */

  // Deshabilitar el array de vértices
  glDisableClientState( GL_VERTEX_ARRAY);
  glDisableClientState( GL_COLOR_ARRAY);

#endif


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

  // GL_ARRAY_BUFFER - Vertex attributes
  // GL_ELEMENT_ARRAY_BUFFER - Vertex array indices.
  id_vbo_ver = VBO_Crear( GL_ARRAY_BUFFER,  sizeof(float)*3L*vertices.size(),vertices.data());
  id_vbo_caras = VBO_Crear( GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*3L*caras.size(), caras.data()) ;

  if (!col_ver.empty())
    id_vbo_col_ver = VBO_Crear (GL_ARRAY_BUFFER, sizeof(float)*3L*vertices.size(), col_ver.data());

  if (!nor_vertices.empty())
    id_vbo_nor_ver = VBO_Crear (GL_ARRAY_BUFFER,sizeof(float)*3L*vertices.size(), nor_vertices.data());

  if (!texturas.empty())
    id_vbo_tex = VBO_Crear (GL_ARRAY_BUFFER, sizeof(float)*2L*texturas.size(), texturas.data());

  vbo_creado = true;
}

// ----------------------------------------------------------------------------
void MallaInd::visualizarDE_VBOs( ContextoVis & cv )
{
   // DONE: práctica 1: visualizar en modo diferido,
   //                        usando VBOs (Vertex Buffer Objects)
   // ..........

  // Inicializa VBO (solo una vez).

  if (!vbo_creado)
    crearVBOs();

  if (!col_ver.empty())
    {
      glBindBuffer (GL_ARRAY_BUFFER, id_vbo_col_ver);
      glColorPointer ( 3, GL_FLOAT, 0, 0);
      glEnableClientState (GL_COLOR_ARRAY );
    }

  glBindBuffer ( GL_ARRAY_BUFFER, id_vbo_ver); // Activar VBO
  glVertexPointer ( 3, GL_FLOAT, 0, 0 ); // Seleccionamos el formato y el offset
  glBindBuffer ( GL_ARRAY_BUFFER, 0 ); // Desactivamos el VBO
  glEnableClientState ( GL_VERTEX_ARRAY ); // Activamos uso de VA.

  // Visualizamos con glDrawElements
  glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, id_vbo_caras );
  glDrawElements ( GL_TRIANGLES, num_caras*3L, GL_UNSIGNED_INT, NULL );
  glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );

  // Desactivamos el array de vértices.
  glDisableClientState (GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);

}

// -----------------------------------------------------------------------------

void MallaInd::visualizarGL( ContextoVis & cv )
{
   // DONE: práctica 1: visualizar en modo inmediato o en modo diferido (VBOs),
   // (tener en cuenta el modo de visualización en 'cv' (alambre, sólido, etc..))
   //
   // .............

  GLenum mode;

  num_ver = vertices.size();
  num_caras = caras.size();
 
  switch(cv.modoVis){
  case modoSolido:
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_SMOOTH);
    cv.usarVBOs ? visualizarDE_VBOs(cv) : visualizarDE_MI(cv);

    break;
  case modoPuntos:
    glPolygonMode (GL_FRONT_AND_BACK, GL_POINT);
    glShadeModel(GL_FLAT);
    cv.usarVBOs ? visualizarDE_VBOs(cv) : visualizarDE_MI(cv);

    break;
  case modoMateriales:
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_SMOOTH);
    cv.usarVBOs ? visualizarVBOs_NT(cv) : visualizarDE_NT(cv);
    break;

  case modoColorNodoPlano:
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_FLAT);
    cv.usarVBOs ? visualizarVBOs_NT(cv) : visualizarDE_NT(cv);
    break;

  case modoColorNodoSuave:
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_SMOOTH);
    cv.usarVBOs ? visualizarVBOs_NT(cv) : visualizarDE_NT(cv);
    break;

  case modoAlambre:
  default:
    glShadeModel(GL_FLAT);
    glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
    cv.usarVBOs ? visualizarDE_VBOs(cv) : visualizarDE_MI(cv);
    break;
}



}
// *****************************************************************************

// *****************************************************************************

Cubo::Cubo()
:  MallaInd( "Malla Cubo" )
{
  

  float origenx = 0.0,
    origeny = 0.0,
    origenz = 0.0;
  float lado = 1.0;

  // Rojo, Verde, Azul
  Tupla3f cero = {origenx, origeny ,origenz};
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
                 {1,0,4}, {1,4,5},
                 {0,3,7}, {4,0,7},
                 {2,1,5}, {2,5,6},
                 {3,2,7}, {7,2,6},
                 {4,6,5}, {4,7,6}
  };


  num_ver = vertices.size();
  num_caras = caras.size();
  setColorVertices();
}


Tetraedro::Tetraedro( )
:  MallaInd( "Malla Tetraedro")
{

  Tupla3f origen = {0.0, 0.0, 0.0};
  float lado = 1.0;
  float altura = 1.0;

  vertices = {
                    origen + lado * Tupla3f{1.0, 0.0, -1.0/sqrt(2)},
                    origen + lado * Tupla3f{0.0, 1.0, 1.0/sqrt(2)},
                    origen + lado * Tupla3f{-1.0, 0.0, -1.0/sqrt(2)},
                    origen + lado * Tupla3f{0.0, -1.0, 1.0/sqrt(2)}
  };

  caras = {
                 {3, 0, 1},
                 {2, 3, 1},
                 {1, 0, 2},
                 {2, 0, 3}
  };

  // Numeración de la base en sentido horario.
  num_ver = vertices.size();
  num_caras = caras.size();
  setColorVertices();
}
// *****************************************************************************



