#include "PSO.h"

double func(const std::vector<double>& args)
{
   double x = args[0];
   double y = args[1];
   return (x - 2) * (x - 2) + y * y + 1;
}

int main()
{
   PSO optimizer(
      { -10.0, -10.0 },
      { 10.0, 10.0 },
      { 0.72984, 2.05, 2.05 },
      100
   );
   optimizer.iterate(func, 40);
   optimizer.getState().report("f(x,y) = (x-2)^2 + (y-2)^2 + 1");

   std::cout << "f(2.0, 0.0) = " << func({ 2.0, 0.0 }) << std::endl;

   return 0;
}