#include <iostream>
#include <string>
#include <vector>

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




int main(int argc, char *argv[])
{
   // data type for each field
   {
   csv<int, std::string, double> f("../csv/tcsv.csv");

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
   }

   csv<std::string> g("../csv/tcsv.csv");
   while (g.has_dataset()) {
      std::cout << g.readline() << "\n";
   }

}
