#include "LinearDivisionFunction.h"
#include "ClassHandler.h"

#include "math/Vector3D.h"

//////////////////////////////////////////////////////////////////////////

LinearDivisionFunction::LinearDivisionFunction(ClassHandler* ip_handler)
  : IDivisionFunction(ip_handler)
{
  m_coefficients[0] = m_coefficients[1] = m_coefficients[2] = 0.0;
}

LinearDivisionFunction::~LinearDivisionFunction()
{

}

double LinearDivisionFunction::GetCoordinate(double i_x)
{
  double y = (-i_x*(m_coefficients[0]/m_coefficients[1])) + m_coefficients[2]/m_coefficients[1];
  return y;
}

double LinearDivisionFunction::Calculate(double i_x)
{
  double result = 0.0;
  UpdateCoefficients();

  return result;
}

void LinearDivisionFunction::UpdateCoefficients()
{
  Vector3D center = mp_class_handler->GetCenter();
  m_coefficients[0] = center[0];
  m_coefficients[1] = center[1];
  m_coefficients[2] = center.DotProduct(center)/2.;
}

std::shared_ptr<IDivisionFunction> LinearDivisionFunction::Minus(std::shared_ptr<IDivisionFunction> ih_second)
{
  LinearDivisionFunction* p_second = dynamic_cast<LinearDivisionFunction*>(ih_second.get());
  if (nullptr == p_second)
    return nullptr;

  std::shared_ptr<LinearDivisionFunction> p_function = std::make_shared<LinearDivisionFunction>(this->mp_class_handler);

  this->UpdateCoefficients();
  p_second->UpdateCoefficients();

  for (size_t i = 0; i < 3; ++i)
    p_function->m_coefficients[i] = this->m_coefficients[i] - p_second->m_coefficients[i];

  return p_function;
}

std::wostream& LinearDivisionFunction::Write(std::wostream& io_stream)
{
  UpdateCoefficients();
  return io_stream << "d(x1,x2) = " << m_coefficients[0]
  << "*x1 + " << m_coefficients[1]
  << "*x2 + " << m_coefficients[2];
}