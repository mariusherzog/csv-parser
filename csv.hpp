#ifndef CSV_HPP
#define CSV_HPP

#include <sstream>
#include <fstream>
#include <string>
#include <tuple>
#include <stdexcept>


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

      std::string readrawline()
      {
         std::string row;
         std::getline(csvf, row);
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
};


#endif // CSV_HPP
