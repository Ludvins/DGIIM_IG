// *********************************************************************
// **
// ** Informática Gráfica
// ** Práctica 4  (declaraciones públicas)
// **
// *********************************************************************

#ifndef IG_PRACTICA4_HPP
#define IG_PRACTICA4_HPP

#include "grafo-escena.hpp"

void P4_Inicializar(  ) ;
bool P4_FGE_PulsarTeclaCaracter(  unsigned char tecla ) ;
void P4_DibujarObjetos( ContextoVis & cv ) ;

class Lata : public NodoGrafoEscena
{
public:
  Lata();
};

#endif
