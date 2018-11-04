#include <string>
#include <functional>
#include <matrizg.hpp>

#ifndef GRADO_LIBERTAD_HPP
#define GRADO_LIBERTAD_HPP

class Objeto3D ;


// tipo usado para las funciones que calculan una matriz a partir de un flotante
typedef std::function< Matriz4f( float )> TFuncionCMF  ;

// -----------------------------------------------------------------------------

class Parametro
{
   private:
   // COMPLETAR: práctica 3: definir variables de instancia privadas de un parámetro

  const std::string descripcion;  // Descripcion del grado de libertad (para seguimiento)
  Matriz4f* const ptr_mat;
  const TFuncionCMF fun_calculo_matriz;
  const bool acotado;
  const float c;
  const float s;
  const float f;

  static constexpr float incremento = 0.1;
  static constexpr float aceleracion = 0.1;

  float valor_norm = 0.0; // p
  float velocidad = 0.1;

   public:
   // crear grado de libertad no acotado
   Parametro
   (
      const std::string&   p_descripcion,
      Matriz4f*            p_ptr_mat,
      TFuncionCMF          p_fun_calculo_matriz,
      bool                 p_acotado,
      float                p_c,
      float                p_s,
      float                p_f
    );

   void  actualizar_matriz();
   void  siguiente_cuadro();      // actualizar valor y matriz al siguiente frame
   void  reset();                 // vuelve al estado inicial
   void  incrementar();           // incrementar el valor
   void  decrementar() ;          // decrementar el valor
   void  acelerar();              // acelerar (aumentar velocidad normalizada)
   void  decelerar();             // decelerar (disminuir la velocidad normalizada)
   float leer_valor_actual();     // devuelve el valor actual (escalado, no normalizado)
   float leer_velocidad_actual(); // devuelve velocidad o frecuencia actual
   std::string
         leer_descripcion();
};

#endif
