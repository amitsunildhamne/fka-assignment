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

struct Element
{
  int isNum; //isNum = 1 is Number
  std::string str;
  int isComputed;
  int value;
};

class Equation
{
    public:
      int solved;
      int variables;
      int numbers;
      std::string *eqn_str;
      std::string *lhs;
      std::string *rhs;
      std::map< Element *, int> VarMap;

      Equation(std::string *str)
      {
         this->solved    = 0;
         this->variables = 0;
         this->numbers   = 0;
         this->eqn_str = str;
      }

      Status BreakEqn()
      {
         
      } 
      Status ProcessEqn()
      {
         std::string token;
         std::stringstream ss(*eqn_str);
         while( getline(ss, token, ' ') )
         {
            std::cout<<token<<std::endl;
         }
         std::cout<<std::endl;
         return SUCCESS;
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
         result &= Eqn[i]->solved;
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
