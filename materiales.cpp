// *********************************************************************
// **
// ** Gestión de materiales y texturas (implementación)
// **
// ** Copyright (C) 2014 Carlos Ureña
// **
// ** This program is free software: you can redistribute it and/or modify
// ** it under the terms of the GNU General Public License as published by
// ** the Free Software Foundation, either version 3 of the License, or
// ** (at your option) any later version.
// **
// ** This program is distributed in the hope that it will be useful,
// ** but WITHOUT ANY WARRANTY; without even the implied warranty of
// ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// ** GNU General Public License for more details.
// **
// ** You should have received a copy of the GNU General Public License
// ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
// **
// *********************************************************************


#include "matrices-tr.hpp"
#include "materiales.hpp"

using namespace std ;

const bool trazam = false ;

//*********************************************************************

PilaMateriales::PilaMateriales()
{
   actual = nullptr ;
}
// -----------------------------------------------------------------------------

void PilaMateriales::activarMaterial( Material * material )
{
   if ( material != actual )
   {
      actual = material ;
      if ( actual != nullptr )
         actual->activar();
   }
}
// -----------------------------------------------------------------------------

void PilaMateriales::activarActual()
{
   if ( actual != nullptr )
      actual->activar() ;
}
// -----------------------------------------------------------------------------

void PilaMateriales::push(  )
{
   pila.push_back( actual );
}
// -----------------------------------------------------------------------------

void PilaMateriales::pop(  )
{
   if ( pila.size() == 0 )
   {
      cout << "error: intento de hacer 'pop' de un material en una pila de materiales vacía." << endl << flush ;
      exit(1);
   }

   Material * anterior = pila [pila.size()-1] ;
   pila.pop_back();
   activarMaterial( anterior );  // cambia 'actual'
 }

//**********************************************************************

Textura::Textura( const std::string & nombreArchivoJPG )
{
   // DONE: práctica 4: inicializar todas las variables
   // .....

  enviada = false;
  glGenTextures(1, &ident_textura);
  imagen = new jpg::Imagen(nombreArchivoJPG);
  modo_gen_ct = mgct_desactivada;

}

// ---------------------------------------------------------------------

//----------------------------------------------------------------------

void Textura::enviar()
{
   // DONE: práctica 4: enviar la imagen de textura a la GPU
   // .......

  if (modo_gen_ct == mgct_coords_objeto)
    {
      glTexGenfv ( GL_S, GL_OBJECT_PLANE, coefs_s );
      glTexGenfv ( GL_T, GL_OBJECT_PLANE, coefs_t );
    }

  if (modo_gen_ct == mgct_coords_ojo)
    {
      glTexGenfv ( GL_S, GL_EYE_PLANE, coefs_s );
      glTexGenfv ( GL_T, GL_EYE_PLANE, coefs_t );
    }

  gluBuild2DMipmaps(
                    GL_TEXTURE_2D,
                    GL_RGB,
                    (GLsizei)imagen->tamX(),
                    (GLsizei)imagen->tamY(),
                    GL_RGB,
                    GL_UNSIGNED_INT,
                    imagen->leerPixels()
                    );

  enviada = true;

}

//----------------------------------------------------------------------

Textura::~Textura( )
{
   using namespace std ;
   cout << "destruyendo textura...imagen ==" << imagen << endl ;
   if ( imagen != NULL )
      delete imagen ;

   imagen = NULL ;
   cout << "hecho (no hecho!)" << endl << flush ;
}

//----------------------------------------------------------------------
// por ahora, se asume la unidad de texturas #0
void Textura::activar(  )
{
   // DONE: práctica 4: enviar la textura a la GPU (solo la primera vez) y activarla
   // .......

  const GLint TEXTURE_MODE[] = { GL_OBJECT_LINEAR, GL_EYE_LINEAR };
  const GLenum TEXTURE_RELATIVE[] = { GL_OBJECT_PLANE, GL_EYE_PLANE };

  glEnable( GL_TEXTURE_2D );
  glBindTexture( GL_TEXTURE_2D, ident_textura );


  if (modo_gen_ct != mgct_desactivada)
    {
      glEnable( GL_TEXTURE_GEN_S );
      glEnable( GL_TEXTURE_GEN_T );

      glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, TEXTURE_MODE[modo_gen_ct-1]);
      glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, TEXTURE_MODE[modo_gen_ct-1]);
      glTexGenfv(GL_S, TEXTURE_RELATIVE[modo_gen_ct-1], coefs_s);
      glTexGenfv(GL_T, TEXTURE_RELATIVE[modo_gen_ct-1], coefs_t);
    }

  else
    {
      glDisable( GL_TEXTURE_GEN_S );
      glDisable( GL_TEXTURE_GEN_T );
    }


}
// *********************************************************************

Material::Material()
{
   iluminacion = false ;
   tex = NULL ;
   coloresCero() ;
}
// ---------------------------------------------------------------------

Material::Material( const std::string & nombreArchivoJPG )
{
   iluminacion    = false ;
   tex            = new Textura( nombreArchivoJPG ) ;

   coloresCero();

   del.emision   = VectorRGB(0.0,0.0,0.0,1.0);
   del.ambiente  = VectorRGB( 0.0, 0.0, 0.0, 1.0);
   del.difusa    = VectorRGB( 0.5, 0.5, 0.5, 1.0 );
   del.especular = VectorRGB( 1.0, 1.0, 1.0, 1.0 );

   tra.emision   = VectorRGB(0.0,0.0,0.0,1.0);
   tra.ambiente  = VectorRGB( 0.0, 0.0, 0.0, 1.0);
   tra.difusa    = VectorRGB( 0.2, 0.2, 0.2, 1.0 );
   tra.especular = VectorRGB( 0.2, 0.2, 0.2, 1.0 );

}

// ---------------------------------------------------------------------
// crea un material usando textura y coeficientes: ka,kd,ks y exponente
// (la textura puede ser NULL, la ilum. queda activada)

Material::Material( Textura * text, float ka, float kd, float ks, float exp )
:  Material()
{
   // DONE: práctica 4: inicializar material usando text,ka,kd,ks,exp
   // .....

  iluminacion = true;
  ponerNombre("Material con textura e iluminación") ;
  tex = text;

  del.ambiente    =
    tra.ambiente  = VectorRGB ( ka, ka, ka, 1.0);

  del.especular   =
    tra.especular = VectorRGB ( ks, ks, ks, 1.0);

  del.difusa    =
    tra.difusa  = VectorRGB( kd, kd, kd, 1.0 );

  del.exp_brillo = tra.exp_brillo = exp;

 }

// ---------------------------------------------------------------------
// crea un material con un color único para las componentes ambiental y difusa
// en el lugar de textura (textura == NULL)
Material::Material( const Tupla3f & colorAmbDif, float ka, float kd, float ks, float exp )
{
   // DONE: práctica 4: inicializar material usando colorAmbDif,ka,kd,ks,exp
   // .....

  iluminacion = true;
  tex = nullptr;
  ponerNombre("Material color plano e iluminación.") ;


  coloresCero();

  //color = colorAmbDif;

   glColor3fv(colorAmbDif); //Establecemos el color que queremos utilizar a partir de ahora.
   glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT); //Hacemos que el color ambiental y el difuso sean
   glColorMaterial(GL_FRONT_AND_BACK,GL_DIFFUSE); //ese color que hemos establecido.
   
   //Esto de aquí es referido a la luz del material, no al color.
   tra.ambiente=del.ambiente  = VectorRGB( ka, ka, ka, 1.0);
   tra.difusa=del.difusa    = VectorRGB( kd, kd, kd, 1.0 );
   tra.especular=del.especular = VectorRGB( ks, ks, ks, 1.0 );
   tra.exp_brillo=del.exp_brillo = exp;

}
// ---------------------------------------------------------------------

Material::Material( const float r, const float g, const float b )
{
   // DONE: práctica 4: inicializar material usando un color plano sin iluminación
   // .....

  ponerNombre("Material color plano.");
  coloresCero();
  color = {r, g, b, 1.0};
  iluminacion = false;
  tex = nullptr;

}

//----------------------------------------------------------------------

void Material::coloresCero()
{
   const VectorRGB ceroRGBopaco(0.0,0.0,0.0,1.0);

   color         =

   del.emision   =
   del.ambiente  =
   del.difusa    =
   del.especular =

   tra.emision   =
   tra.ambiente  =
   tra.difusa    =
   tra.especular = ceroRGBopaco ;

   del.exp_brillo =
   tra.exp_brillo = 1.0 ;
}
//----------------------------------------------------------------------

Material::~Material()
{
   if ( tex != nullptr )
   {  delete tex ;
      tex = nullptr ;
   }
}
//----------------------------------------------------------------------

void Material::ponerNombre( const std::string & nuevo_nombre )
{
   nombre_mat = nuevo_nombre ;
}
//----------------------------------------------------------------------

std::string Material::nombre() const
{
   return nombre_mat ;
}
//----------------------------------------------------------------------

void Material::activar(  )
{
   // TODO: práctica 4: activar un material
   // .....

  if (iluminacion)
    {
      glDisable(GL_COLOR_MATERIAL);
      glMaterialfv(GL_FRONT, GL_EMISSION, del.emision);
      glMaterialfv(GL_FRONT, GL_AMBIENT, del.ambiente);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, del.difusa);
      glMaterialfv(GL_FRONT, GL_SPECULAR, del.especular);
      glMaterialf(GL_FRONT, GL_SHININESS, del.exp_brillo);

      glColorMaterial(GL_FRONT, GL_EMISSION);
      glColorMaterial(GL_FRONT, GL_AMBIENT);
      glColorMaterial(GL_FRONT, GL_DIFFUSE);
      glColorMaterial(GL_FRONT, GL_SPECULAR);
      glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

      glMaterialfv(GL_BACK, GL_EMISSION, tra.emision);
      glMaterialfv(GL_BACK, GL_AMBIENT, tra.ambiente);
      glMaterialfv(GL_BACK, GL_DIFFUSE, tra.difusa);
      glMaterialfv(GL_BACK, GL_SPECULAR, tra.especular);
      glMaterialf(GL_BACK, GL_SHININESS, tra.exp_brillo);

      glColorMaterial(GL_BACK, GL_EMISSION);
      glColorMaterial(GL_BACK, GL_AMBIENT);
      glColorMaterial(GL_BACK, GL_DIFFUSE);
      glColorMaterial(GL_BACK, GL_SPECULAR);
      glColorMaterial(GL_BACK, GL_AMBIENT_AND_DIFFUSE);

      glEnable(GL_COLOR_MATERIAL);

      glEnable(GL_LIGHTING);
    }

  else
    {

      //Si no hay luces

      glDisable(GL_LIGHTING);
    }

  if (tex == NULL)
    glDisable( GL_TEXTURE_2D );
  else
    tex->activar();

}

//**********************************************************************

FuenteLuz::FuenteLuz( GLfloat p_longi_ini, GLfloat p_lati_ini, const VectorRGB & p_color )
{
   //CError();

   if ( trazam )
      cout << "Creando fuente de luz." <<  endl << flush ;

   // inicializar parámetros de la fuente de luz
   longi_ini = p_longi_ini ;
   lati_ini  = p_lati_ini  ;
   longi = longi_ini ;
   lati  = lati_ini ;

   col_ambiente  = p_color ;
   col_difuso    = p_color ;
   col_especular = p_color ;

   ind_fuente = -1 ; // la marca como no activable hasta que no se le asigne indice

   //CError();
}

//----------------------------------------------------------------------

void FuenteLuz::activar()
{
   // TODO: práctica 4: activar una fuente de luz (en GL_LIGHT0 + ind_fuente)
   // .....

  glEnable (GL_LIGHT0 + ind_fuente);

  glLightfv (GL_LIGHT0 + ind_fuente, GL_AMBIENT, (GLfloat*) &col_ambiente);
  glLightfv (GL_LIGHT0 + ind_fuente, GL_DIFFUSE, (GLfloat*) &col_difuso);
  glLightfv (GL_LIGHT0 + ind_fuente, GL_SPECULAR, (GLfloat*) &col_especular);

  const float ejeZ[4] = {0.0, 0.0, 1.0, 0.0 };
  glMatrixMode (GL_MODELVIEW );
  glPushMatrix() ;

  glLoadIdentity();
  glRotatef (lati, 0.0, 1.0, 0.0);
  glRotatef (longi, -1.0, 0.0, 0.0 );

  glLightfv (GL_LIGHT0 + ind_fuente, GL_POSITION, ejeZ);

  glPopMatrix();

}

//----------------------------------------------------------------------

bool FuenteLuz::gestionarEventoTeclaEspecial( int key )
{
   bool actualizar = true ;
   const float incr = 3.0f ;

   switch( key )
   {
      case GLFW_KEY_RIGHT:
         longi = longi+incr ;
         break ;
      case GLFW_KEY_LEFT:
         longi = longi-incr ;
         break ;
      case GLFW_KEY_UP:
         lati = std::min( lati+incr, 90.0f) ;
         break ;
      case GLFW_KEY_DOWN:
         lati = std::max( lati-incr, -90.0f ) ;
         break ;
      case GLFW_KEY_HOME:
         lati  = lati_ini ;
         longi = longi_ini ;
         break ;
      default :
         actualizar = false ;
         cout << "tecla no usable para la fuente de luz." << endl << flush ;
   }

   //if ( actualizar )
   //   cout << "fuente de luz cambiada: longi == " << longi << ", lati == " << lati << endl << flush ;
   return actualizar ;
}

//**********************************************************************

ColFuentesLuz::ColFuentesLuz()
{
   max_num_fuentes = -1 ;
}
//----------------------------------------------------------------------

void ColFuentesLuz::insertar( FuenteLuz * pf )  // inserta una nueva
{
   assert( pf != nullptr );
   assert (vpf.size() <= 8);

   pf->ind_fuente = vpf.size() ;
   vpf.push_back( pf ) ;

}
//----------------------------------------------------------------------
// activa una colección de fuentes de luz

void ColFuentesLuz::activar( unsigned id_prog )
{
   // TODO: práctica 4: activar una colección de fuentes de luz
   // .....

  glEnable(GL_RESCALE_NORMAL);
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
  glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
  glEnable ( GL_LIGHTING );

  for( int i = 0; i < vpf.size(); i++)
      vpf[i]->activar();

  for ( int i = vpf.size(); i < 8; i++)
      glDisable( GL_LIGHT0 + i);

}

//----------------------------------------------------------------------
FuenteLuz * ColFuentesLuz::ptrFuente( unsigned i )
{
   assert(i < vpf.size()) ;
   return vpf[i] ;
}
//----------------------------------------------------------------------

ColFuentesLuz::~ColFuentesLuz()
{
   for( unsigned i = 0 ; i < vpf.size() ; i++ )
   {
      assert( vpf[i] != NULL );
      delete vpf[i] ;
      vpf[i] = NULL ;
   }
}

// *************************************************************************
// *************************************************************************
// *************************************************************************
// *************************************************************************
// *************************************************************************


MaterialLata::MaterialLata() : Material() {

  coloresCero();
  iluminacion = true;

  del.emision = tra.emision = Tupla4f(0.7, 0.7, 0.7, 1);
  del.ambiente = tra.ambiente = Tupla4f(0.1, 0.1, 0.1, 1);
  del.difusa = tra.difusa = Tupla4f(0.1, 0.1, 0.1, 1);
  del.especular = tra.especular = Tupla4f(1, 1, 1, 1);
  del.exp_brillo = tra.exp_brillo = 3;
  tex = new Textura( "../imgs/lata-coke.jpg" ) ;

  ponerNombre("Material CocaCola.");
}


MaterialTapasLata::MaterialTapasLata() {

  del.emision = tra.emision = Tupla4f(0.7, 0.7, 0.7, 1);
  del.ambiente = tra.ambiente = Tupla4f(0.1, 0.1, 0.1, 1);
  del.difusa = tra.difusa = Tupla4f(0.1, 0.1, 0.1, 1);
  del.especular = tra.especular = Tupla4f(1, 1, 1, 1);
  del.exp_brillo = tra.exp_brillo = 3;

}

MaterialPeonMadera::MaterialPeonMadera(){
  tex = new Textura("../imgs/text-madera.jpg");
  // Duda: diferencia entre poner 1 y 2 aquí
  
  // s proporcional a la X, t a la Y

  del.emision = tra.emision = Tupla4f(0.6, 0.6, 0.6, 1);
  del.ambiente = tra.ambiente = Tupla4f(0.1, 0.1, 0.1, 1);
  del.difusa = tra.difusa = Tupla4f(0.8, 0.8, 0.8, 1);
  del.especular = tra.especular = Tupla4f(0.9, 0.9, 0.9, 1);
  del.exp_brillo = tra.exp_brillo = 5;

}
