#include <cassert>
#include <cmath>
#include "Parametro.hpp"
#include "Objeto3D.hpp"

// -----------------------------------------------------------------------------

Parametro::Parametro(
   const std::string &  p_descripcion,
   Matriz4f *           p_ptr_mat,
   TFuncionCMF          p_fun_calculo_matriz,
   bool                 p_acotado,
   float                p_c,
   float                p_s,
   float                p_f
  )
  :descripcion(p_descripcion),
   ptr_mat(p_ptr_mat),
   fun_calculo_matriz(p_fun_calculo_matriz),
   acotado(p_acotado),
   c(p_c),
   s(p_s),
   f(p_f)
{
   // DONE: práctica 3: inicializar un parámetro
   // ....

}
// -----------------------------------------------------------------------------

void Parametro::actualizar_matriz(){ 
  *ptr_mat = fun_calculo_matriz(leer_valor_actual());
}

void Parametro::siguiente_cuadro()
{
   // DONE: práctica 3: actualizar el valor y la matriz para el siguiente cuadro
   // ....

  valor_norm += velocidad;
  actualizar_matriz();

}
// -----------------------------------------------------------------------------

void Parametro::reset()
{
   // DONE: práctica 3: hace reset de valor y velocidad
   // ....

  valor_norm = 0.0;
  velocidad = 0.1;
  actualizar_matriz();

}
// -----------------------------------------------------------------------------
void Parametro::incrementar()
{
   // DONE: práctica 3: incrementa el valor
   // ....

  valor_norm += incremento;
  actualizar_matriz();
}

// -----------------------------------------------------------------------------

void Parametro::decrementar()
{
   // DONE: práctica 3: decrementa el valor
   // ....

  valor_norm -= incremento;
  actualizar_matriz();

}
// -----------------------------------------------------------------------------
void Parametro::acelerar()
{
   // DONE: práctica 3: incrementa la velocidad
   // ....

  velocidad += aceleracion;

}
// -----------------------------------------------------------------------------
void Parametro::decelerar()
{
   // DONE: práctica 3: decrementa la velocidad
   // ....

  velocidad -= aceleracion;
  if (velocidad < 0.0) velocidad = 0.0;

}
// -----------------------------------------------------------------------------

float Parametro::leer_valor_actual()
{
   // DONE: práctica 3: devuelve el valor actual
   // ....

  return acotado
    ? c + s * sin(2*M_PI*f*valor_norm)
    : c + s * valor_norm;


}

// -----------------------------------------------------------------------------
float Parametro::leer_velocidad_actual()
{
   //  DONE: práctica 3: devuelve la velocidad actual
   // ....

  return velocidad;

}
// -----------------------------------------------------------------------------

std::string Parametro::leer_descripcion()
{
   // DONE: práctica 3: devuelve la descripción
   // ....

  return descripcion;

}
// -----------------------------------------------------------------------------
