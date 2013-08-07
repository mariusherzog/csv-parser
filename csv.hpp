#ifndef CSV_HPP
#define CSV_HPP

#include <sstream>
#include <fstream>
#include <string>
#include <tuple>
#include <stdexcept>
#include <limits>


template <typename T, typename... Ts>
class csv
{
   public:
      explicit csv(std::string file, char sep = ',', bool header = false):
         csvf(file),
         header(header),
         sep(sep)
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

      std::tuple<T, Ts...> readline()
      {
         std::tuple<T, Ts...> row;
         std::string buf = raw_readline();
         std::stringstream s(buf);

         filltuple(row, s);
         return row;
      }

      std::string raw_readline()
      {
         csvf.clear();
         std::string row;
         std::getline(csvf, row);
         return row;
      }

      void move_to(unsigned int l)
      {
         csvf.clear();
         csvf.seekg(0, std::ios_base::beg);
         if (header) {
            csvf.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
         }
         for (unsigned int i=0; i<l; ++i) {
            csvf.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
         }
      }

      void append(std::tuple<T, Ts...> t)
      {
         std::stringstream s;
         fillstream(t, s);
         std::string row;
         s >> row;
         csv<std::string>::append(row);
      }

      void append(std::string s)
      {
         csvf.clear(csvf.rdstate() & ~std::istream::failbit);
         std::streampos befpos = csvf.tellg();
         csvf.seekg(0, std::ios_base::end);

         csvf << s << "\n";

         csvf.seekg(befpos, std::ios_base::beg);
      }


   private:
      std::fstream csvf;
      std::string headerns;
      bool header;
      char sep;



      csv& operator=(const csv &rhs);
      csv(const csv &rhs);

      template<std::size_t I = 0, typename... Tp>
      inline typename std::enable_if<I == sizeof...(Tp), void>::type
      filltuple(std::tuple<Tp...>&, std::stringstream&)
      { }

      template<std::size_t I = 0, typename... Tp>
      inline typename std::enable_if<(I < sizeof...(Tp)), void>::type
      filltuple(std::tuple<Tp...>& t, std::stringstream& s)
      {
         std::string col;
         std::getline(s, col, sep);

         std::stringstream rowstream(col);
         rowstream >> std::get<I>(t);
         filltuple<I+1, Tp...>(t, s);
      }


      template<std::size_t I = 0, typename... Tp>
      inline typename std::enable_if<I == sizeof...(Tp), void>::type
      fillstream(std::tuple<Tp...>&, std::stringstream& s)
      {
         s << "\n";
      }

      template<std::size_t I = 0, typename... Tp>
      inline typename std::enable_if<(I < sizeof...(Tp)), void>::type
      fillstream(std::tuple<Tp...>& t, std::stringstream& s)
      {
         s << std::get<I>(t) << sep;
         fillstream<I+1, Tp...>(t, s);
      }

};


#endif // CSV_HPP
