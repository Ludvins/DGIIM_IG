// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase MallaRevol
// **
// *********************************************************************

#ifndef IG_MALLAREVOL_HPP
#define IG_MALLAREVOL_HPP

#include <vector>          // usar std::vector
#include <string>

#include "MallaInd.hpp"   // declaración de 'Objeto3D'
// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class MallaRevol : public MallaInd
{
   private:

   unsigned
      nper , // numero de perfiles
      nvp  ; // numero de vertices por perfil

   protected:

  void crearMallaRevol( const std::vector<Tupla3f>& perfil_original,
              const unsigned nperfiles,
              const bool     crear_tapas,
              const bool     cerrar_malla ) ;

   public:
      // crea una malla de revolucion
      MallaRevol( const std::string & nombre_arch,
                  const unsigned nperfiles,
                  const bool     crear_tapas,
                  const bool     cerrar_malla ) ;

  MallaRevol() = default;


} ;

class Cilindro : public MallaRevol
{

public:
  Cilindro ( const int num_verts_per, //Numero de vertices del perfil original
             const unsigned nperfiles, //Numero de perfiles
             const bool crear_tapas,
             const bool cerrar_malla
             );
} ;


class Cono : public MallaRevol
{

public:
  Cono ( const int num_verts_per, //Numero de vertices del perfil original
         const unsigned nperfiles, //Numero de perfiles
         const bool crear_tapas,
         const bool cerrar_malla
         );
} ;


class Esfera : public MallaRevol
{

public:
  Esfera ( const int num_verts_per, //Numero de vertices del perfil original
           const unsigned nperfiles, //Numero de perfiles
           const bool crear_tapas,
           const bool cerrar_malla
           );
} ;

#endif
