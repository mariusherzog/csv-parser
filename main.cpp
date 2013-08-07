#include <iostream>
#include <string>
#include <vector>
#include <tuple>

#include "csv.hpp"

// small program to test the class


struct testy
{
      int x;
      std::string y;
      double z;

      testy(int x, std::string y, double z):
         x(x), y(y), z(z)
      {}
};


int main(int, char)
{

   // data type for each field
   csv<int, std::string, double> f("../csv/tcsv.csv", ';', true);

   std::vector<testy> testies;
   while (f.has_dataset()) {
      std::tuple<int, std::string, double> t = f.readline();
      testies.push_back(testy(std::get<0>(t),
                        std::get<1>(t),
                        std::get<2>(t)));
   }

   for (testy &t : testies) {
      std::cout << t.x << " " << t.y << " " << t.z << "\n";
   }

   f.move_to(0);
   while (f.has_dataset()) {
      std::cout << f.raw_readline() << "\n";
   }

}
