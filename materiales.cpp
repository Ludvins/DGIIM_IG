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
  imagen = new jpg::Imagen(nombreArchivoJPG);
  modo_gen_ct = mgct_desactivada;

}

// ---------------------------------------------------------------------

//----------------------------------------------------------------------

void Textura::enviar()
{
   // DONE: práctica 4: enviar la imagen

  glGenTextures(1, &ident_textura);
  glBindTexture(GL_TEXTURE_2D, ident_textura);

  gluBuild2DMipmaps
    (
     GL_TEXTURE_2D,
     GL_RGB,
     imagen->tamX(),
     imagen->tamY(),
     GL_RGB,
     GL_UNSIGNED_BYTE,
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
void Textura::activar(  )
{
   // DONE: práctica 4: enviar la textura a la GPU (solo la primera vez) y activarla
   // .......
  glEnable(GL_TEXTURE_2D);
  if (!enviada)
    enviar();
  else
    glBindTexture(GL_TEXTURE_2D, ident_textura);

  // Aprobechamos la correspondencia entre enum y entero, de forma que TEXTURE_MODE[modo_gen_ct] nos da el tipo.
  const GLint TEXTURE_MODE[] = { GL_OBJECT_LINEAR, GL_EYE_LINEAR };
  const GLenum TEXTURE_RELATIVE[] = { GL_OBJECT_PLANE, GL_EYE_PLANE };

  if (modo_gen_ct != mgct_desactivada)
    {
      glEnable( GL_TEXTURE_GEN_S );
      glEnable( GL_TEXTURE_GEN_T );

      glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, TEXTURE_MODE[modo_gen_ct]);
      glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, TEXTURE_MODE[modo_gen_ct]);
      glTexGenfv(GL_S, TEXTURE_RELATIVE[modo_gen_ct], coefs_s);
      glTexGenfv(GL_T, TEXTURE_RELATIVE[modo_gen_ct], coefs_t);
    }

  else
    {
      glDisable( GL_TEXTURE_GEN_S );
      glDisable( GL_TEXTURE_GEN_T );
    }

}

TexturaXY::TexturaXY( const std::string & nom )
  : Textura(nom)
{
  modo_gen_ct = mgct_coords_ojo;  // por ejemplo

  coefs_s[0] = 1.0;
  coefs_s[1] = coefs_s[2] = coefs_s[3] = 0.0;

  coefs_t[1] = 1.0;
  coefs_t[0] = coefs_t[2] = coefs_t[3] = 0.0;
}

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
   del.exp_brillo = 1.0;
   tra.exp_brillo = 1.0;

   ponerNombre("material con textura e iluminación por defecto");
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

  color =
    del.emision =
    tra.emision = {0.0, 0.0, 0.0, 1.0};

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

   tex = NULL;
  iluminacion = true;

  del.emision =
    tra.emision = {0.0, 0.0, 0.0, 1.0};

  del.ambiente =
    tra.ambiente = VectorRGB(ka * colorAmbDif(R), ka * colorAmbDif(G), ka * colorAmbDif(B), 1.0);

  del.difusa =
    tra.difusa = VectorRGB(kd * colorAmbDif(R), kd * colorAmbDif(G), kd * colorAmbDif(B), 1.0);

  del.especular =
    tra.especular = {ks, ks, ks, 1.0};

  del.exp_brillo =
    tra.exp_brillo = exp;

  ponerNombre("material INCREIBLE");

}
// ---------------------------------------------------------------------

Material::Material( const float r, const float g, const float b )
{
   // DONE: práctica 4: inicializar material usando un color plano sin iluminación
   // .....

  ponerNombre("Material color plano.");
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


  if (tex == nullptr)
    glDisable( GL_TEXTURE_2D );
  else
    tex->activar();


  if(iluminacion)
    {
      glEnable(GL_LIGHTING);
      //Nótese que en nuestro caso, del=tra siempre pero como lo tenemos separado
      //en dos partes, aquí también lo haremos por si pide separarlo en el examen.
      glMaterialfv(GL_FRONT,GL_EMISSION,del.emision);
      glMaterialfv(GL_FRONT,GL_AMBIENT,del.ambiente);
      glMaterialfv(GL_FRONT,GL_DIFFUSE,del.difusa);
      glMaterialfv(GL_FRONT,GL_SPECULAR,del.especular);
      glMaterialf(GL_FRONT,GL_SHININESS,del.exp_brillo);

      glMaterialfv(GL_BACK,GL_EMISSION,tra.emision);
      glMaterialfv(GL_BACK,GL_AMBIENT,tra.ambiente);
      glMaterialfv(GL_BACK,GL_DIFFUSE,tra.difusa);
      glMaterialfv(GL_BACK,GL_SPECULAR,tra.especular);
      glMaterialf(GL_BACK,GL_SHININESS,tra.exp_brillo);

      glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
    }
  else
    {
      glDisable(GL_LIGHTING);
      glColor4fv(color);
    }


}

//**********************************************************************

FuenteLuz::FuenteLuz( const VectorRGB & p_color )
{
   //CError();

   if ( trazam )
      cout << "Creando fuente de luz." <<  endl << flush ;

   col_ambiente  = p_color ;
   col_difuso    = p_color ;
   col_especular = p_color ;

   ind_fuente = -1 ; // la marca como no activable hasta que no se le asigne indice

   //CError();
}


//----------------------------------------------------------------------


void FuenteLuzDireccional::variarAngulo (unsigned angulo, float incremento){
  if (angulo == 0)
    longi += incremento;
  else
    lati += incremento;
}

bool FuenteLuzDireccional::gestionarEventoTeclaEspecial( int key )
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

   if ( actualizar )
      cout << "Fuente de luz cambiada: Longitud: " << longi << ", Latitud: " << lati << endl << flush ;
   return actualizar ;
}

void FuenteLuzDireccional::activar()
{
  // COMPLETAR: práctica 4: activar una fuente de luz (en GL_LIGHT0 + ind_fuente)
  if(ind_fuente>=0){
    //Habilitamos
    glEnable(GL_LIGHT0+ind_fuente);

    //Colores de ambiente
    glLightfv(GL_LIGHT0+ind_fuente,GL_AMBIENT,col_ambiente);
    glLightfv(GL_LIGHT0+ind_fuente,GL_DIFFUSE,col_difuso);
    glLightfv(GL_LIGHT0+ind_fuente,GL_SPECULAR,col_especular);

    //Establecemos la dirección de la luz utilizando coordenadas polares.
    const Tupla4f ejeZ={0.0,0.0,1.0,0.0};
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glLoadIdentity();
    glRotatef(longi,0.0,1.0,0.0);
    glRotatef(lati,-1.0,0.0,0.0);

    glLightfv(GL_LIGHT0+ind_fuente,GL_POSITION,ejeZ);
    glPopMatrix();

  }
}

FuenteLuzDireccional::FuenteLuzDireccional(float alpha_inicial,float beta_inicial, const VectorRGB & p_color)
  : FuenteLuz(p_color),
  longi_ini(alpha_inicial),
  lati_ini(beta_inicial),
  longi(alpha_inicial),
  lati(beta_inicial)
  {}

FuenteLuzPosicional::FuenteLuzPosicional(const Tupla3f & posicion, const VectorRGB & p_color)
  : FuenteLuz(p_color), posicion(posicion){}

void FuenteLuzPosicional::activar()
  {
     // DONE: práctica 4: activar una fuente de luz (en GL_LIGHT0 + ind_fuente)
     if(ind_fuente >= 0){
       //Habilitamos
      glEnable(GL_LIGHT0+ind_fuente);

      //Colores
      glLightfv(GL_LIGHT0+ind_fuente, GL_AMBIENT, col_ambiente);
      glLightfv(GL_LIGHT0+ind_fuente, GL_DIFFUSE, col_difuso);
      glLightfv(GL_LIGHT0+ind_fuente, GL_SPECULAR,col_especular);


      glLightfv(
                GL_LIGHT0+ind_fuente,
                GL_POSITION,
                Tupla4f{ posicion(X), posicion(Y), posicion(Z), 1.0}
                );
    }

  }

bool FuenteLuzPosicional::gestionarEventoTeclaEspecial( int key )
  {
    cout << "tecla no usable para la fuente de luz." << endl << flush ;

     return false ;
  }

//**********************************************************************

ColFuentesLuz::ColFuentesLuz()
{
   max_num_fuentes = 8 ;
}
//----------------------------------------------------------------------

void ColFuentesLuz::insertar( FuenteLuz * pf )  // inserta una nueva
{
   assert( pf != nullptr );
   if (vpf.size() == max_num_fuentes)
     {
       cout << "No se pueden insertar mas fuentes de luz" << endl;
       return;
     }

   pf->ind_fuente = vpf.size() ;
   vpf.push_back( pf ) ;

}
//----------------------------------------------------------------------
// activa una colección de fuentes de luz

void ColFuentesLuz::activar( )
{
  glEnable(GL_LIGHTING);
  glEnable(GL_NORMALIZE);

  for(unsigned i=0; i<vpf.size();i++){
    vpf[i]->activar();
  }

  for(unsigned i=vpf.size(); i<max_num_fuentes;i++){
    glDisable(GL_LIGHT0+i);
  }

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



MaterialLata::MaterialLata()
  : Material(
             new Textura("../imgs/lata-coke.jpg"),
             0.0,
             0.6,
             0.4,
             7
             )
{}

MaterialTapasLata::MaterialTapasLata()
  : Material(
             nullptr,
             0.0,
             0.6,
             0.4,
             40)
{}

MaterialPeonMadera::MaterialPeonMadera()
  : Material(
             new TexturaXY("../imgs/text-madera.jpg"),
             0.0,
             .6,
             .4,
             8
             )
{}

MaterialPeonBlanco::MaterialPeonBlanco()
  : Material(
             {1.0, 1.0, 1.0},
             0.0,
             1.0,
             0.0,
             1.0
             )
{}
MaterialPeonNegro::MaterialPeonNegro()
  : Material(
             {0.01,0.01,0.01},
             0.0,
             .9,
             .1,
             8
             )
{}

ColeccionFuenteP4::ColeccionFuenteP4()
{
  const VectorRGB color1 = {1.0, 1.0, 1.0, 5.0};
  const VectorRGB color2 = {0.4, 0.4, 0.4, 1.0};

  insertar(new FuenteLuzDireccional(-10.0, 30.0, color1));
  insertar(new FuenteLuzPosicional({0.0, 20.0, 0.0}, color2));
}
