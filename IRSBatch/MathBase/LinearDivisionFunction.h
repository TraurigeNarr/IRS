#pragma once

#include "IDivisionFunction.h"

class LinearDivisionFunction : public IDivisionFunction
{
  double m_coefficients[3];//w1*z1 + w2*z2 + w3; w3 = (1/2)*<z,z>
public:
  LinearDivisionFunction(ClassHandler* ip_handler);
  virtual ~LinearDivisionFunction();

  virtual double Calculate(double i_x) override;
  virtual double GetCoordinate(double i_x) override;
  virtual void UpdateCoefficients() override;

  virtual std::shared_ptr<IDivisionFunction> Minus(std::shared_ptr<IDivisionFunction> ih_second) override;

  virtual std::wostream& Write(std::wostream& io_stream) override;
};