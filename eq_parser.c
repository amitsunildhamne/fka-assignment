#include <map>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

typedef enum
{
   SUCCESS = 0,
   FAIL = -1
} Status;

class Element
{
  public:
    int isNum; //isNum = 1 is Number
    std::string str;
    int isComputed;
    double value;

    Element(std::string token)
    {
       char *p;
       double converted = std::strtod(token.c_str(), &p);
       if(*p)
       {
          this->isComputed = 0;
          this->value      = 0;
          this->str = token;
          this->isNum = 0;
          std::cout<<"Is Alphabet: "<<this->str<<std::endl;
       }
       else
       {
          this->isComputed = 1;
          this->value = std::stod(token);
          this->str = "";
          this->isNum = 1;
          std::cout<<"Is Num: "<<this->value<<std::endl;
       }
    }
};

class Equation
{
    public:
      int solved;
      int variables;
      int numbers;
      std::string *eqn_str;
      std::vector<Element *> lhs;
      std::vector<Element *> rhs;
      std::map< Element *, int> VarMap;

      Equation(std::string *str)
      {
         this->solved    = 0;
         this->variables = 0;
         this->numbers   = 0;
         this->eqn_str = str;
      }

      Status ProcessEqn()
      {
         std::string token;
         bool whichSide = 0; //lhs = 0, rhs = 1
         std::stringstream ss(*eqn_str);
         while( getline(ss, token, ' ') )
         {
//            std::cout<<token<<std::endl;

            if ( token == "=")
            {
               whichSide = 1;
               continue;
            }
            else
            {
               Element *element = new Element(token);
               if( whichSide == 0 )
               {
                  this->lhs.push_back(element);
               }
               else
               {
                  this->rhs.push_back(element);
               }
            }
         }
         std::cout<<std::endl;
         return SUCCESS;
      }

      bool isSolved()
      {
          int res = 1;
          for(auto itr = VarMap.begin(); itr != VarMap.end(); ++itr)
          {
             res &= itr->first->isComputed;
          }
          return res;
      }
};

class Equations
{
  public:
   std::vector<Equation *> Eqn;

   bool isComputeComplt()
   {
      bool result = 1;
      for (int i = 0; i < Eqn.size(); ++i)
      {
         result &= Eqn[i]->isSolved();
      }
      return result;
   }

   Status ProcessFile(std::string &fileName)
   {
      std::ifstream eqFile;
      eqFile.open(fileName);
      if ( eqFile.is_open() == false)
      {
         eqFile.clear();
         eqFile.open(fileName, std::ios::out);
         return FAIL;
      }

      std::string line;
      while (std::getline(eqFile, line, '\n'))
      {
         std::istringstream iss(line);
         Equation *equation = new Equation(&line);
         Eqn.push_back(equation);
         equation->ProcessEqn();
         //std::cout<<line<<std::endl;
      }
      
      eqFile.close();
      return SUCCESS;
   }

   ~Equations()
    {
       for(int i = 0; i < Eqn.size(); ++i)
       {
          Eqn[i]->~Equation();
          free(Eqn[i]);
       }
    }
};

int main()
{
  Status status;
  std::string fileName = "math1.txt";
  Equations *eqns = new Equations();
  status = eqns->ProcessFile(fileName);
  std::cout<<status<<std::endl;
  return 0;
}
