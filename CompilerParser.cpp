#include "CompilerParser.h"

/**
 * Constructor for the CompilerParser
 * @param tokens A linked list of tokens to be parsed
 */
CompilerParser::CompilerParser(std::list<Token*> tokens) {
  this->tokens = tokens;
}

/**
 * Generates a parse tree for a single program
 * @return a ParseTree
 */

ParseTree* CompilerParser::compileProgram() {
  ParseTree* program = new ParseTree("program", "");

    if (have("keyword", "class")) {
        ParseTree* classTree = compileClass();
        program->addChild(classTree);
    } else {
        throw ParseException();
    }

    return program;

}

/**
 * Generates a parse tree for a single class
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileClass() {
    ParseTree* classTree = new ParseTree("Class", "");

    if (have("keyword", "class")) {
        classTree->addChild(current());
        next(); // advanceTokenIfPossible
    }

    if (current()->getType() == "identifier") {
        std::string className = current()->getValue();
        classTree->addChild(current());
        next(); // advanceTokenIfPossible
    }

    // {
    if (have("symbol", "{")) {
        classTree->addChild(current());
        next(); 
    }

    classTree->addChild(compileClassVarDec());

    // class level subroutines (methods, constructors, functions)
    while (!(have("symbol", "}"))) {
        classTree->addChild(compileSubroutine());
    }

    if (have("symbol", "}")) {
        classTree->addChild(current());
        next(); 
    } else {
        throw ParseException();
    }

    return classTree;
}

std::vector<std::string> CompilerParser::getVarNames() {
    // e.g. var int x,y;
    std::vector<std::string> varNames;

    // varName
    varNames.push_back(current()->getValue());
    next(); // advanceTokenIfPossible

    while (!have("symbol", ";")) {
        // ,
        if (have("symbol", ",")) {
            next(); // advanceTokenIfPossible
        }

        // varName
        varNames.push_back(current()->getValue());
        next(); // advanceTokenIfPossible
    }

    // ;
    if (have("symbol", ";")) {
        next(); // advanceTokenIfPossible
    } else {
        throw ParseException();
    }

    return varNames;
}
/**
 * Generates a parse tree for a static variable declaration or field declaration
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileClassVarDec() {
  /**
   * Generates a parse tree for a static variable declaration or field
   * declaration
   * @return a ParseTree
   */
  ParseTree* classVarDecTree = new ParseTree("ClassVarDec", "");

    while (have("keyword", "static") || have("keyword", "field")) {
        // variable symbol table kind
        std::string varKind = current()->getValue();
        classVarDecTree->addChild(current());
        next(); // advanceTokenIfPossible

        // variable data type
        std::string varDataType = current()->getValue();
        classVarDecTree->addChild(current());
        next(); // advanceTokenIfPossible

        // get all variable names for multiple variable declaration
        std::vector<std::string> varNames = getVarNames();
        for (const auto &varName : varNames) {
            classVarDecTree->addChild(new ParseTree("VarName", varName));
        }
    }

    return classVarDecTree;
}

/**
 * Generates a parse tree for a method, function, or constructor
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutine() {
//   ParseTree* subroutine = new ParseTree("subroutine", "");
//   if (have("keyword", "constructor")) {
//     mustBe("keyword", "constructor");
//   } else if (have("keyword", "function")) {
//     mustBe("keyword", "function");
//   } else if (have("keyword", "method")) {
//     mustBe("keyword", "method");
//   } else {
//     throw ParseException();
//   }
//   if (have("keyword", "void")) {
//     mustBe("keyword", "void");
//   } else if (have("keyword", "int")) {
//     mustBe("keyword", "int");
//   } else if (have("keyword", "char")) {
//     mustBe("keyword", "char");
//   } else if (have("keyword", "boolean")) {
//     mustBe("keyword", "boolean");
//   } else if (have("identifier", "")) {
//     mustBe("identifier", "");
//   } else {
//     throw ParseException();
//   }
//   Token* subroutineName = mustBe("identifier", "");
//   mustBe("symbol", "(");
//   ParseTree* parameterList = compileParameterList();
//   mustBe("symbol", ")");
//   ParseTree* subroutineBody = compileSubroutineBody();
//   subroutine->addChild(subroutineName);
//   subroutine->addChild(parameterList);
//   subroutine->addChild(subroutineBody);
return NULL;
}

/**
 * Generates a parse tree for a subroutine's parameters
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileParameterList() {
//   ParseTree* parameterList = new ParseTree("parameterList", "");
//   if (have("keyword", "int")) {
//     mustBe("keyword", "int");
//   } else if (have("keyword", "char")) {
//     mustBe("keyword", "char");
//   } else if (have("keyword", "boolean")) {
//     mustBe("keyword", "boolean");
//   } else if (have("identifier", "")) {
//     mustBe("identifier", "");
//   } else {
//     throw ParseException();
//   }
//   Token* varName = mustBe("identifier", "");
//   while (have("symbol", ",")) {
//     mustBe("symbol", ",");
//     if (have("keyword", "int")) {
//       mustBe("keyword", "int");
//     } else if (have("keyword", "char")) {
//       mustBe("keyword", "char");
//     } else if (have("keyword", "boolean")) {
//       mustBe("keyword", "boolean");
//     } else if (have("identifier", "")) {
//       mustBe("identifier", "");
//     } else {
//       throw ParseException();
//     }
//     Token* varName = mustBe("identifier", "");
//   }
//   parameterList->addChild(varName);
return NULL;
}

/**
 * Generates a parse tree for a subroutine's body
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutineBody() {
//   ParseTree* subroutineBody = new ParseTree("subroutineBody", "");
//   mustBe("symbol", "{");
//   ParseTree* varDecs = compileVarDec();
//   ParseTree* statements = compileStatements();
//   mustBe("symbol", "}");
//   subroutineBody->addChild(varDecs);
//   subroutineBody->addChild(statements);
return NULL;
}

/**
 * Generates a parse tree for a subroutine variable declaration
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileVarDec() {
//   ParseTree* varDec = new ParseTree("varDec", "");
//   if (have("keyword", "var")) {
//     mustBe("keyword", "var");
//   } else {
//     throw ParseException();
//   }
//   if (have("keyword", "int")) {
//     mustBe("keyword", "int");
//   } else if (have("keyword", "char")) {
//     mustBe("keyword", "char");
//   } else if (have("keyword", "boolean")) {
//     mustBe("keyword", "boolean");
//   } else if (have("identifier", "")) {
//     mustBe("identifier", "");
//   } else {
//     throw ParseException();
//   }
//   Token* varName = mustBe("identifier", "");
//   while (have("symbol", ",")) {
//     mustBe("symbol", ",");
//     Token* varName = mustBe("identifier", "");
//   }
//   mustBe("symbol", ";");
//   varDec->addChild(varName);
return NULL;
}

/**
 * Generates a parse tree for a series of statements
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileStatements() {
//   ParseTree* statements = new ParseTree("statements", "");
//   while (have("keyword", "let") || have("keyword", "if") ||
//          have("keyword", "while") || have("keyword", "do") ||
//          have("keyword", "return")) {
//     if (have("keyword", "let")) {
//       ParseTree* letTree = compileLet();
//       statements->addChild(letTree);
//     } else if (have("keyword", "if")) {
//       ParseTree* ifTree = compileIf();
//       statements->addChild(ifTree);
//     } else if (have("keyword", "while")) {
//       ParseTree* whileTree = compileWhile();
//       statements->addChild(whileTree);
//     } else if (have("keyword", "do")) {
//       ParseTree* doTree = compileDo();
//       statements->addChild(doTree);
//     } else if (have("keyword", "return")) {
//       ParseTree* returnTree = compileReturn();
//       statements->addChild(returnTree);
//     } else {
//       throw ParseException();
//     }
//   }
return NULL;
}

/**
 * Generates a parse tree for a let statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileLet() {
//   ParseTree* letTree = new ParseTree("letStatement", "");
//   mustBe("keyword", "let");
//   Token* varName = mustBe("identifier", "");
//   if (have("symbol", "[")) {
//     mustBe("symbol", "[");
//     ParseTree* expression = compileExpression();
//     mustBe("symbol", "]");
//     letTree->addChild(expression);
//   }
//   mustBe("symbol", "=");
//   ParseTree* expression = compileExpression();
//   mustBe("symbol", ";");
//   letTree->addChild(expression);
return NULL;
}

/**
 * Generates a parse tree for an if statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileIf() {
//   ParseTree* ifTree = new ParseTree("ifStatement", "");
//   mustBe("keyword", "if");
//   mustBe("symbol", "(");
//   ParseTree* expression = compileExpression();
//   mustBe("symbol", ")");
//   mustBe("symbol", "{");
//   ParseTree* statements = compileStatements();
//   mustBe("symbol", "}");
//   if (have("keyword", "else")) {
//     mustBe("keyword", "else");
//     mustBe("symbol", "{");
//     ParseTree* statements = compileStatements();
//     mustBe("symbol", "}");
//   }
//   ifTree->addChild(expression);
//   ifTree->addChild(statements);
return NULL;
}

/**
 * Generates a parse tree for a while statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileWhile() {
//   ParseTree* whileTree = new ParseTree("whileStatement", "");
//   mustBe("keyword", "while");
//   mustBe("symbol", "(");
//   ParseTree* expression = compileExpression();
//   mustBe("symbol", ")");
//   mustBe("symbol", "{");
//   ParseTree* statements = compileStatements();
//   mustBe("symbol", "}");
//   whileTree->addChild(expression);
//   whileTree->addChild(statements);
return NULL;
}

/**
 * Generates a parse tree for a do statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileDo() {
//   ParseTree* doTree = new ParseTree("doStatement", "");
//   mustBe("keyword", "do");
//   Token* subroutineName = mustBe("identifier", "");
//   if (have("symbol", ".")) {
//     mustBe("symbol", ".");
//     Token* subroutineName = mustBe("identifier", "");
//   }
//   mustBe("symbol", "(");
//   ParseTree* expressionList = compileExpressionList();
//   mustBe("symbol", ")");
//   mustBe("symbol", ";");
//   doTree->addChild(subroutineName);
//   doTree->addChild(expressionList);
return NULL;
}

/**
 * Generates a parse tree for a return statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileReturn() { return NULL; }

/**
 * Generates a parse tree for an expression
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpression() { return NULL; }

/**
 * Generates a parse tree for an expression term
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileTerm() { return NULL; }

/**
 * Generates a parse tree for an expression list
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpressionList() { return NULL; }

/**
 * Advance to the next token
 */
void CompilerParser::next() {
  if (tokens.empty() == false) {
    tokens.pop_front();
  }
  return;
}

/**
 * Return the current token
 * @return the Token
 */
Token* CompilerParser::current() {
  if (tokens.empty() == false) {
    return tokens.front();
  } else {
    return NULL;
  }
}

/**
 * Check if the current token matches the expected type and value.
 * @return true if a match, false otherwise
 */
bool CompilerParser::have(std::string expectedType, std::string expectedValue) {
  Token* currToken = current();
  if (currToken != nullptr && currToken->getType() == expectedType &&
      currToken->getValue() == expectedValue) {
    return true;
  }
  return false;
}

/**
 * Check if the current token matches the expected type and value.
 * If so, advance to the next token, returning the current token, otherwise
 * throw a ParseException.
 * @return the current token before advancing
 */
Token* CompilerParser::mustBe(std::string expectedType,
                              std::string expectedValue) {
  Token* currToken = current();
  if (currToken != nullptr && currToken->getType() == expectedType &&
      currToken->getValue() == expectedValue) {
    next();
    return currToken;
  } else {
    throw ParseException();
  }
  return NULL;
}

/**
 * Definition of a ParseException
 * You can use this ParseException with `throw ParseException();`
 */
const char* ParseException::what() {
  return "An Exception occurred while parsing!";
}
