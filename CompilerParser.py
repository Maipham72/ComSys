from ParseTree import *

class CompilerParser :

    def __init__(self,tokens):
        """
        Constructor for the CompilerParser
        @param tokens A list of tokens to be parsed
        """
        self.tokens = tokens
        self.current_token_index = 0
        
    

    def compileProgram(self):
        """
        Generates a parse tree for a single program
        @return a ParseTree that represents the program
        """
        if not self.tokens:
          raise ParseException("No tokens to parse!")
        
        if self.have("keyword","class"):
          return self.compileClass()
        else:
          raise ParseException("The program doesn't begin with a class")
    
    
    def compileClass(self):
      class_tree = ParseTree("Class", "")

      # Logic for parsing the class
      if self.current().type == "keyword" and self.current().value == "class":
          # Add the 'class' keyword to the class tree
          class_tree.add_child(ParseTree("keyword", "class"))
          self.next()  # Move to the next token

          # Check for the class name
          if self.current().type == "identifier":
              # Add the class name to the class tree
              class_tree.add_child(ParseTree("identifier", self.current().value))
              self.next()  # Move to the next token

              # Check for the opening curly brace
              if self.current().type == "symbol" and self.current().value == "{":
                  # Add the opening curly brace to the class tree
                  class_tree.add_child(ParseTree("symbol", "{"))
                  self.next()  # Move to the next token

                  # TODO: Add logic to handle class body

                  # Check for the closing curly brace
                  if self.current().type == "symbol" and self.current().value == "}":
                      # Add the closing curly brace to the class tree
                      class_tree.add_child(ParseTree("symbol", "}"))
                      self.next()  # Move to the next token
                  else:
                      # If the closing curly brace is missing, raise an exception
                      raise ParseException("Missing closing curly brace in class definition.")
              else:
                  # If the opening curly brace is missing, raise an exception
                  raise ParseException("Missing opening curly brace in class definition.")
          else:
              # If the class name is missing, raise an exception
              raise ParseException("Missing class name in class definition.")
      else:
          # If the 'class' keyword is missing, raise an exception
          raise ParseException("Missing 'class' keyword in class definition.")

      return class_tree
          # Return the generated parse tree for the class
            
    

    def compileClassVarDec(self):
        """
        Generates a parse tree for a static variable declaration or field declaration
        @return a ParseTree that represents a static variable declaration or field declaration
        """
        return None 
    

    def compileSubroutine(self):
        """
        Generates a parse tree for a method, function, or constructor
        @return a ParseTree that represents the method, function, or constructor
        """
        return None 
    
    
    def compileParameterList(self):
        """
        Generates a parse tree for a subroutine's parameters
        @return a ParseTree that represents a subroutine's parameters
        """
        return None 
    
    
    def compileSubroutineBody(self):
        """
        Generates a parse tree for a subroutine's body
        @return a ParseTree that represents a subroutine's body
        """
        return None 
    
    
    def compileVarDec(self):
        """
        Generates a parse tree for a variable declaration
        @return a ParseTree that represents a var declaration
        """
        return None 
    

    def compileStatements(self):
        """
        Generates a parse tree for a series of statements
        @return a ParseTree that represents the series of statements
        """
        return None 
    
    
    def compileLet(self):
        """
        Generates a parse tree for a let statement
        @return a ParseTree that represents the statement
        """
        return None 


    def compileIf(self):
        """
        Generates a parse tree for an if statement
        @return a ParseTree that represents the statement
        """
        return None 

    
    def compileWhile(self):
        """
        Generates a parse tree for a while statement
        @return a ParseTree that represents the statement
        """
        return None 


    def compileDo(self):
        """
        Generates a parse tree for a do statement
        @return a ParseTree that represents the statement
        """
        return None 


    def compileReturn(self):
        """
        Generates a parse tree for a return statement
        @return a ParseTree that represents the statement
        """
        return None 


    def compileExpression(self):
        """
        Generates a parse tree for an expression
        @return a ParseTree that represents the expression
        """
        return None 


    def compileTerm(self):
        """
        Generates a parse tree for an expression term
        @return a ParseTree that represents the expression term
        """
        return None 


    def compileExpressionList(self):
        """
        Generates a parse tree for an expression list
        @return a ParseTree that represents the expression list
        """
        return None 


    def next(self):
        """
        Advance to the next token
        """
        return self.tokens.next()


    def current(self):
        """
        Return the current token
        @return the token
        """
        return None


    def have(self,expectedType,expectedValue):
        """
        Check if the current token matches the expected type and value.
        @return True if a match, False otherwise
        """
        return False


    def mustBe(self,expectedType,expectedValue):
        """
        Check if the current token matches the expected type and value.
        If so, advance to the next token, returning the current token, otherwise throw/raise a ParseException.
        @return token that was current prior to advancing.
        """
        return None
    

if __name__ == "__main__":


    """ 
    Tokens for:
        class MyClass {
        
        }
    """
    tokens = []
    tokens.append(Token("keyword","class"))
    tokens.append(Token("identifier","MyClass"))
    tokens.append(Token("symbol","{"))
    tokens.append(Token("symbol","}"))

    parser = CompilerParser(tokens)
    try:
        result = parser.compileProgram()
        print(result)
    except ParseException:
        print("Error Parsing!")
