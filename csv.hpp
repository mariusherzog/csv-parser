#ifndef CSV_HPP
#define CSV_HPP

#include <sstream>
#include <fstream>
#include <string>
#include <tuple>
#include <stdexcept>

template <typename T, typename... Ts>
class csv
{
   public:
      explicit csv(std::string file, char sep = ',', bool header = false):
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

      std::tuple<T, Ts...> readline()
      {
         std::tuple<T, Ts...> row;
         std::string buf;

         csvf.seekg(-1, std::ios_base::cur);
         std::getline(csvf, buf);
         std::stringstream s(buf);

         fill(row, s);
         return row;
      }

      bool has_dataset()
      {
         std::string buf;
         std::getline(csvf, buf);
         csvf.seekg(-buf.length(), std::ios_base::cur);
         return !(buf.empty());
      }

      std::string headernames() const
      {
         return headerns;
      }


   private:
      std::fstream csvf;
      char sep;
      bool header;
      std::string headerns;

      template<std::size_t I = 0, typename... Tp>
      inline typename std::enable_if<I == sizeof...(Tp), void>::type
      fill(std::tuple<Tp...>&, std::stringstream&) { }

      template<std::size_t I = 0, typename... Tp>
      inline typename std::enable_if<(I < sizeof...(Tp)), void>::type
      fill(std::tuple<Tp...>& t, std::stringstream& s)
      {
         std::string col;
         std::getline(s, col, sep);

         std::stringstream rowstream(col);
         rowstream >> std::get<I>(t);
         fill<I+1, Tp...>(t, s);
      }

      csv& operator=(const csv &rhs);
      csv(const csv &rhs);

};

#endif // CSV_HPP
