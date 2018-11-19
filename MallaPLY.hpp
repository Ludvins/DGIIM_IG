// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase MallaPLY
// **
// *********************************************************************

#ifndef IG_MALLAPLY_HPP
#define IG_MALLAPLY_HPP

#include <vector>          // usar std::vector
#include <string>
#include <cmath>

#include "MallaInd.hpp"   // declaración de 'Objeto3D'
// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class MallaPLY : public MallaInd
{

   public:
      // crea una malla de revolucion
      MallaPLY( const std::string & nombre_arch ) ;

  Tupla3f size(){
    float x = 0.0;
    float y = 0.0;
    float z = 0.0;

    for( auto it : vertices){
      x = std::max((float)fabs(it[0]), x);
      y = std::max((float)fabs(it[1]), y);
      z = std::max((float)fabs(it[2]), z);

    }


    return {x , y , z};

  }
  
} ;

#endif
