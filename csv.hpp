#ifndef CSV_HPP
#define CSV_HPP

#include <sstream>
#include <fstream>
#include <string>
#include <tuple>
#include <stdexcept>
#include <limits>


template <typename T, typename... Ts> class csv;

template <>
class csv<std::string>
{
   public:
      explicit csv(std::string file, char sep = ',', bool header = false):
         csvf(file),
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

      std::string readline()
      {
         csvf.clear();
         std::string row;
         std::getline(csvf, row);
         return row;
      }

      std::string readline(unsigned int l)
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

   protected:
      inline char delim()
      {
         return sep;
      }


   private:
      std::fstream csvf;
      char sep;
      std::string headerns;

      csv& operator=(const csv &rhs);
      csv(const csv &rhs);

};

template <typename T, typename... Ts>
class csv: public csv<std::string>
{
   public:
      explicit csv(std::string file, char sep = ',', bool header = false):
         csv<std::string>(file, sep, header)
      {
      }

      std::tuple<T, Ts...> readline()
      {
         std::tuple<T, Ts...> row;
         std::string buf = csv<std::string>::readline();
         std::stringstream s(buf);

         fill(row, s);
         return row;
      }

      std::tuple<T, Ts...> readline(unsigned int l)
      {
         std::tuple<T, Ts...> row;
         std::string buf = csv<std::string>::readline(l);
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


#endif // CSV_HPP
