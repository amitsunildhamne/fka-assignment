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

std::map<std::string, double> VarMap;
std::map<std::string, bool> VarPending; //1 = done/can be computed; 0: cannot

class Element
{
  public:
    int isNum; //isNum = 1 is Number
    std::string VarName;
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
          this->VarName = token;
          this->isNum = 0;
          std::cout<<"Is Alphabet: "<<this->str<<std::endl;
          VarPending[token] = true;
       }
       else
       {
          this->isComputed = 1;
          this->value = std::stod(token);
          this->VarName = "";
          this->isNum = 1;
          std::cout<<"Is Num: "<<this->value<<std::endl;
       }
    }

     Element(std::string token, double value)
    {
       this->isComputed = 1;
       this->value      = value;
       this->VarName = token;
       this->isNum = 0;
       std::cout<<"Is Alphabet: "<<this->str<<std::endl;
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

      Equation(std::string *str)
      {
         this->solved    = 0;
         this->variables = 0;
         this->numbers   = 0;
         this->eqn_str = str;
         this->varMap = varMap;
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
               if ( VarMap.find(token) == VarMap.end() )
               {
                   Element *element = new Element(token);
               }
               else
               {
                    Element *element = new Element(token, VarMap[token]);
               }

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
          for(auto itr = this->lhs.begin(); itr != this->lhs.end(); ++itr)
          {
             res &= itr->first->isComputed;
          }
          for(auto itr = this->rhs.begin(); itr != this->rhs.end(); ++itr)
          {
              res &= itr->first->isComputed;
          }
          this->solved = res;
          return res;
      }

     Status SolveEqn()
     {
         int result;
         int status = FAIL;
         if ( (lhs.size() == 1) &&
              (rhs.size() >= 1)  )
         {
             if (lhs[0]->isNum == 0)
             {
                status = compute(rhs, result);
                if (status == SUCCESS)
                {
                   VarMap[lhs[0]->VarName] = result;
                   lhs[0]->isComputed = 1;
                   this->solved = 1;
                   VarPending[lhs[0]->VarName] = false;
                } 
             }
           else
           {
               
           }
         }
         else if ( (lhs.size() >= 1) && 
                    rhs.size() == 1)
         {
             status = compute(lhs, result);
             if (status == SUCCESS)
             {
                 VarMap[rhs[0]->VarName] = result;
                 rhs[0]->isComputed = 1;
                 this->solved = 1;
                 VarPending[rhs[0]->VarName] = false;
                 
             }

         }
         else if ( (lhs.size() > 1) &&
                    (rhs.size() > 1 ))
         {
             std::cout<<"Incapable of solving equations with mix of Variable";
             std::cout<<" and Numbers on One Side"<<std::endl;
         }
     }
            
};

class Equations
{
  public:
   std::vector<Equation *> Eqn;
   std::map<Equation *, bool>EqPending;

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
         Equation *equation = new Equation(&line, &varMap);
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
