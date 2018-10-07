// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

#ifndef IG_MALLAIND_HPP
#define IG_MALLAIND_HPP

#include <vector>          // usar std::vector

#include "Objeto3D.hpp"   // declaración de 'Objeto3D'
// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class MallaInd : public Objeto3D
{

protected:

  std::vector <Tupla3f> vertices;
  std::vector <Tupla3i> caras;
  std::vector <Tupla3f> nor_ver;
  std::vector <Tupla3f> nor_tri;
  std::vector <Tupla3f> col_ver;
  std::vector <Tupla3f> col_tri;

  GLuint id_vbo_ver = 0;
  GLuint id_vbo_tri = 0;
  GLuint id_vbo_col_ver = 0;

  unsigned tam_ver = 0;
  unsigned tam_tri = 0;

  // calculo de las normales de esta malla
  void calcular_normales();

  // visualizar con 'draw elements', en modo inmediato
  void visualizarDE_MI( ContextoVis & cv );
  // visualizar con 'draw elements', en modo diferido (con VBOS)
  void visualizarDE_VBOs( ContextoVis & cv );

  // Establecer colores de los vertices
  virtual void setColorVertices();

  // Inicializar VBO
  void crearVBOs ( );

public:
  // crea una malla vacía (nombre: "malla indexada nueva vacía")
  MallaInd();
  // crea una malla vacía con un nombre concreto:
  MallaInd( const std::string & nombreIni );
  // visualizar el objeto con OpenGL
  virtual void visualizarGL( ContextoVis & cv ) ;
} ;
// ---------------------------------------------------------------------

class Cubo : public MallaInd
{
   public:
  Cubo(Tupla3f origen, float lado);
};
// ---------------------------------------------------------------------

class Tetraedro : public MallaInd
{
   public:
      Tetraedro();
};


#endif
