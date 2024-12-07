#include <fstream>
#include <sstream>
#include <iomanip>
#include "CSVparser.hpp"
#include <iostream>
#include <ctime>
#include <sstream>

namespace csv {
    // Function to parse a date string in the format "MM/DD/YYYY" and return a std::tm object
    std::tm parseDate(const std::string& dateStr) {
      std::tm tm = {};
      std::istringstream ss(dateStr);
      ss >> std::get_time(&tm, "%m/%d/%Y");
      return tm;
  }

   // Constructor for teh Parser class
  Parser::Parser(const std::string &data, const DataType &type, char sep)
    : _type(type), _sep(sep)
  {
      std::string line;
      if (type == eFILE)
      {
        _file = data;
        std::ifstream ifile(_file.c_str());
        if (ifile.is_open())
        {
            while (ifile.good())
            {
                getline(ifile, line);
                if (line != "")
                    _originalFile.push_back(line);
            }
            ifile.close();

            if (_originalFile.size() == 0)
              throw Error(std::string("No Data in ").append(_file));
            
            parseHeader();
            parseContent();
        }
        else
            throw Error(std::string("Failed to open ").append(_file));
      }
      else
      {
        std::istringstream stream(data);
        while (std::getline(stream, line))
          if (line != "")
            _originalFile.push_back(line);
        if (_originalFile.size() == 0)
          throw Error(std::string("No Data in pure content"));

        parseHeader();
        parseContent();
      }
  }

  // Destructor for the Parser class
  Parser::~Parser(void)
  {
     std::vector<Row *>::iterator it;

     for (it = _content.begin(); it != _content.end(); it++)
          delete *it;
  }

  // Function to parse the header of the CSV file
  void Parser::parseHeader(void)
  {
      std::stringstream ss(_originalFile[0]);
      std::string item;

      while (std::getline(ss, item, _sep))
          _header.push_back(item);
  }

  // Function to parse the content of the CSV file
  void Parser::parseContent(void)
  {
     std::vector<std::string>::iterator it;
     
     it = _originalFile.begin();
     it++; // skip header

     for (; it != _originalFile.end(); it++)
     {
         bool quoted = false;
         int tokenStart = 0;
         unsigned int i = 0;

         Row* row = new Row(_header);

         for (; i != it->length(); i++)
         {
             if (it->at(i) == '"')
                 quoted = ((quoted) ? (false) : (true));
             else if (it->at(i) == ',' && !quoted)
             {
                 row->push(it->substr(tokenStart, i - tokenStart));
                 tokenStart = i + 1;
             }
         }

         // end
         row->push(it->substr(tokenStart, it->length() - tokenStart));

         // Debug statement to check row size
         // std::cout << "Row size: " << row->size() << ", Expected: " << _header.size() << std::endl;

         // if value(s) missing
         if (row->size() != _header.size()) {
             std::cerr << "Row size mismatch. Skipping malformed row: " << *it << std::endl;
             delete row;
             continue;
         }
         _content.push_back(row);
     }
  }

  // Function to get a row by its position
  Row &Parser::getRow(unsigned int rowPosition) const
  {
      if (rowPosition < _content.size())
          return *(_content[rowPosition]);
      throw Error("can't return this row (doesn't exist)");
  }

  // Overloaded operator to get a row by its position
  Row &Parser::operator[](unsigned int rowPosition) const
  {
      return Parser::getRow(rowPosition);
  }

  // Function to get the number of rows
  unsigned int Parser::rowCount(void) const
  {
      return _content.size();
  }

  // Function to get the number of columns
  unsigned int Parser::columnCount(void) const
  {
      return _header.size();
  }

  // Function to get the header
  std::vector<std::string> Parser::getHeader(void) const
  {
      return _header;
  }

  // Function to get a specific header element by its position
  const std::string Parser::getHeaderElement(unsigned int pos) const
  {
      if (pos >= _header.size())
        throw Error("can't return this header (doesn't exist)");
      return _header[pos];
  }

  // Function to delete a row by its position
  bool Parser::deleteRow(unsigned int pos)
  {
    if (pos < _content.size())
    {
      delete *(_content.begin() + pos);
      _content.erase(_content.begin() + pos);
      return true;
    }
    return false;
  }

  // Funcmtion to add a row at a specific position
  bool Parser::addRow(unsigned int pos, const std::vector<std::string> &r)
  {
    Row *row = new Row(_header);

    for (auto it = r.begin(); it != r.end(); it++)
      row->push(*it);
    
    if (pos <= _content.size())
    {
      _content.insert(_content.begin() + pos, row);
      return true;
    }
    return false;
  }

  // Function to synchronize the content with the file
  void Parser::sync(void) const
  {
    if (_type == DataType::eFILE)
    {
      std::ofstream f;
      f.open(_file, std::ios::out | std::ios::trunc);

      // Write header
      unsigned int i = 0;
      for (auto it = _header.begin(); it != _header.end(); it++)
      {
        f << *it;
        if (i < _header.size() - 1)
          f << ",";
        else
          f << std::endl;
        i++;
      }
     
      // Write content
      for (auto it = _content.begin(); it != _content.end(); it++)
        f << **it << std::endl;
      f.close();
    }
  }

  // Function to get the file name
  const std::string &Parser::getFileName(void) const
  {
      return _file;    
  }
  
  /*
  ** ROW
  */


  // Constructor for the Row class
  Row::Row(const std::vector<std::string> &header)
      : _header(header) {}

  // Destructor for the Row class
  Row::~Row(void) {}

  // Function to get the size of the row
  unsigned int Row::size(void) const
  {
    return _values.size();
  }

  // Function to add value to the row
  void Row::push(const std::string &value)
  {
    _values.push_back(value);
  }

  // Function to set a value in the row by key
  bool Row::set(const std::string &key, const std::string &value) 
  {
    std::vector<std::string>::const_iterator it;
    int pos = 0;

    for (it = _header.begin(); it != _header.end(); it++)
    {
        if (key == *it)
        {
          _values[pos] = value;
          return true;
        }
        pos++;
    }
    return false;
  }

  // Overloaded operator to get a value by its position
  const std::string Row::operator[](unsigned int valuePosition) const
  {
       if (valuePosition < _values.size())
           return _values[valuePosition];
       throw Error("can't return this value (doesn't exist)");
  }

  // Overloaded operator to get a value by key
  const std::string Row::operator[](const std::string &key) const
  {
      std::vector<std::string>::const_iterator it;
      int pos = 0;

      for (it = _header.begin(); it != _header.end(); it++)
      {
          if (key == *it)
              return _values[pos];
          pos++;
      }
      
      throw Error("can't return this value (doesn't exist)");
  }

  // Overloaded operator to output a row to an ostream
  std::ostream &operator<<(std::ostream &os, const Row &row)
  {
      for (unsigned int i = 0; i != row._values.size(); i++)
          os << row._values[i] << " | ";

      return os;
  }

  // Overloaded operator to output a row to an ofstream
  std::ofstream &operator<<(std::ofstream &os, const Row &row)
  {
    for (unsigned int i = 0; i != row._values.size(); i++)
    {
        os << row._values[i];
        if (i < row._values.size() - 1)
          os << ",";
    }
    return os;
  }
}
