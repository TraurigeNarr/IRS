#pragma once

#include <ostream>
#include <memory>

class ClassHandler;

class IDivisionFunction
{
protected:
  ClassHandler* mp_class_handler;
public:
  IDivisionFunction(ClassHandler* ip_handler)
    : mp_class_handler (ip_handler)
  {}
  virtual ~IDivisionFunction(){}

  virtual double Calculate(double i_x) = 0;
  virtual double GetCoordinate(double i_x) = 0;
  virtual void UpdateCoefficients() = 0;
  virtual std::shared_ptr<IDivisionFunction> Minus(std::shared_ptr<IDivisionFunction> ih_second) = 0;
  
  virtual std::wostream& Write(std::wostream& io_stream) = 0;
};