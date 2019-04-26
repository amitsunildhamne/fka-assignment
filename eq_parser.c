#include <map>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
typedef enum
{
   SUCCESS = 0,
   FAIL = -1,
   PENDING = -2
} Status;

typedef enum
{
    LHS = 0,
    RHS = 1
} Side;

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
          std::cout<<"Is Alphabet: "<<this->VarName<<std::endl;
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
       std::cout<<"Is Alphabet: "<<this->VarName;
       std::cout<<" With Value: "<<this->value<<std::endl;
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
      }

      Status ProcessEqn()
      {
         std::string token;
         Side whichSide = LHS;
         std::stringstream ss(*eqn_str);
         while( getline(ss, token, ' ') )
         {
//            std::cout<<token<<std::endl;

            if ( token == "=")
            {
               whichSide = RHS;
               continue;
            }
            else
            {
               Element *element;
               if ( VarMap.find(token) == VarMap.end() )
               {
                   element = new Element(token);
               }
               else
               {
                   element = new Element(token, VarMap[token]);
               }

               if( whichSide == LHS )
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
          for(int itr = 0; itr > this->lhs.size(); ++itr)
          {
             res &= this->lhs[itr]->isComputed;
          }
          for(int itr = 0; itr > this->rhs.size(); ++itr)
          {
              res &= this->rhs[itr]->isComputed;
          }
          this->solved = res;
          return res;
      }

     Status compute(Side side, double &result)
     {
        result = 0.00;
        std::cout<<"Computed"<<std::endl;
        return SUCCESS;
     }
     Status SolveEqn()
     {
         double result;
         Status status = FAIL;
         if ( (lhs.size() == 1) &&
              (rhs.size() >= 1)  )
         {
            status = compute(RHS, result);
            if (status == SUCCESS)
            {
               VarMap[lhs[0]->VarName] = result;
               lhs[0]->isComputed = 1;
               this->solved = 1;
               VarPending[lhs[0]->VarName] = false;
            }
         }
         else if ( (lhs.size() >= 1) && 
                    rhs.size() == 1)
         {
            status = compute(LHS, result);
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
             //To be worked on
         }
         else
         {
             std::cout<<"Wrong Format"<<std::endl;
         }
         return status;
     }
            
};

class Equations
{
  public:
   std::vector<Equation *> Eqn;
   std::vector<Equation *>EqPending;

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
         EqPending.push_back(equation);
         equation->ProcessEqn();
         //std::cout<<line<<std::endl;
      }
      
      eqFile.close();
      return SUCCESS;
   }

   Status SolveEquations()
   {
       Status ret_val = SUCCESS;
       int i = 0;
       while(true) //Keeps looping until all equations have been solved
       {           //Loop breaks if Equations are unsolveable
           int prev_size = this->EqPending.size();
           Status status = this->EqPending[i]->SolveEqn();
           if (status = SUCCESS)
           {
               Equation *store_prev_element = NULL;
               if (i > 0)
               {
                  store_prev_element = this->EqPending[i-1]; // We store previous element to find 
                                                             //our position after deleting the current element
               }
               this->EqPending.erase( std::remove(this->EqPending.begin(),
                                                  this->EqPending.end(), this->Eqn[i]),
                                      this->EqPending.end() );
               if (i > 0)
               {
                  auto itr = std::find(this->EqPending.begin(), this->EqPending.end(), store_prev_element);
                  i = std::distance(this->EqPending.begin(), itr);
               }
               std::cout<<"EqPending Size: "<<this->EqPending.size()<<std::endl;
           }
           else if (status == FAIL)
           {
              ret_val = FAIL;
              break;
           }

           if(prev_size == this->EqPending.size() || this->EqPending.size() == 0)
           {
               if (this->EqPending.size() > 0)
               {
                   ret_val = FAIL;
               }
               break;
           }
       }
       return ret_val;
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
  std::cout<<"Status Solve Equations: "<<eqns->SolveEquations()<<std::endl;
  std::cout<<status<<std::endl;
  return 0;
}
