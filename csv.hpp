#ifndef CSV_HPP
#define CSV_HPP

#include <sstream>
#include <fstream>
#include <string>
#include <tuple>
#include <stdexcept>
#include <limits>

#include <iostream>

class Acsv
{
   public:
      bool has_dataset()
      {
         std::string buf;
         std::getline(csvf, buf);
         csvf.seekg(-(buf.length()+1), std::ios_base::cur);
         return !(buf.empty());
      }

      std::string headernames() const
      {
         return headerns;
      }


   protected:
      explicit Acsv(std::string file, char sep, bool header):
         csvf(file),
         sep(sep),
         header(header)
      {
         if (!csvf.is_open()) {
            throw std::runtime_error("Could not open file!\n");
         }

         if (header) {
            std::getline(csvf, headerns);
         } else {
            headerns = "";
         }
      }

      inline std::string readrawline()
      {
         csvf.clear();
         std::string row;
         std::getline(csvf, row);
         return row;
      }

      inline std::string readrawline(unsigned int l)
      {
         csvf.clear();
         std::streampos befpos = csvf.tellg();
         csvf.seekg(0, std::ios_base::beg);
         for (unsigned int i=1; i<l; ++i) {
            csvf.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
         }

         std::string row;
         std::getline(csvf, row);

         csvf.clear();
         csvf.seekg(befpos, std::ios_base::beg);
         return row;
      }

      char delim()
      {
         return sep;
      }


   private:
      std::ifstream csvf;
      char sep;
      bool header;
      std::string headerns;

      Acsv& operator=(const Acsv &rhs);
      Acsv(const Acsv &rhs);
};



template <typename T, typename... Ts>
class csv: public Acsv
{
   public:
      explicit csv(std::string file, char sep = ',', bool header = false):
         Acsv(file, sep, header)
      {
      }

      std::tuple<T, Ts...> readline()
      {
         std::tuple<T, Ts...> row;
         std::string buf = readrawline();
         std::stringstream s(buf);

         fill(row, s);
         return row;
      }

      std::tuple<T, Ts...> readline(unsigned int l)
      {
         std::tuple<T, Ts...> row;
         std::string buf = readrawline(l);
         std::stringstream s(buf);

         fill(row, s);
         return row;
      }


   private:
      template<std::size_t I = 0, typename... Tp>
      inline typename std::enable_if<I == sizeof...(Tp), void>::type
      fill(std::tuple<Tp...>&, std::stringstream&)
      { }

      template<std::size_t I = 0, typename... Tp>
      inline typename std::enable_if<(I < sizeof...(Tp)), void>::type
      fill(std::tuple<Tp...>& t, std::stringstream& s)
      {
         std::string col;
         std::getline(s, col, delim());

         std::stringstream rowstream(col);
         rowstream >> std::get<I>(t);
         fill<I+1, Tp...>(t, s);
      }

};


template <>
class csv<std::string>: public Acsv
{
   public:
      explicit csv(std::string file, char sep = ',', bool header = false):
         Acsv(file, sep, header)
      {
      }

      std::string readline()
      {
         return readrawline();
      }

      std::string readline(unsigned int l)
      {
         return readrawline(l);
      }

};


#endif // CSV_HPP
