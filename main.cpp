#include <iostream>
#include <string>
#include <vector>

#include "csv.hpp"



// small program to test the class


struct testy
{
      int x;
      std::string y;
      char z;

      testy(int x, std::string y, char z):
         x(x), y(y), z(z)
      {}
};


int main(int argc, char *argv[])
{
   // data type for each field
   csv<int, std::string, char> f("../csv/tcsv.csv");

   std::vector<testy> testies;
   while (f.has_dataset()) {
      std::tuple<int, std::string, char> t = f.readline();
      testies.push_back(testy(std::get<0>(t),
                        std::get<1>(t),
                        std::get<2>(t)));
   }

   for (testy &t : testies) {
      std::cout << t.x << " " << t.y << " " << t.z << "\n";
   }

}
