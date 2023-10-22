
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
    if (have("keyword", "class")) {
        ParseTree* classTree = compileClass();
        return classTree;
    } else {
        throw ParseException();
    }

}

/**
 * Generates a parse tree for a single class
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileClass() {
       ParseTree* classTree = new ParseTree("class", "");

    if (have("keyword", "class")) {
        classTree->addChild(current());
        next(); // Advance to the next token
    } else {
        throw ParseException();
    }

    if (current()->getType() == "identifier") {
        classTree->addChild(current());
        next(); // Advance to the next token
    } else {
        throw ParseException();
    }

    if (have("symbol", "{")) {
        classTree->addChild(current());
        next(); // Advance to the next token
    } else {
        throw ParseException();
    }

    while (have("keyword", "static") || have("keyword", "field")) {
        ParseTree* classVarDecs = compileClassVarDec();
        classTree->addChild(classVarDecs);
    } 

    while (have("keyword", "function") || have("keyword", "method") || have("keyword", "constructor")) {
        ParseTree* subroutine = compileSubroutine();
        classTree->addChild(subroutine);
    }


    if (have("symbol", "}")) {
        classTree->addChild(current());
        next(); // Advance to the next token
    } else {
        throw ParseException();
    }

    return classTree;
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
    ParseTree* classVarDecTree = new ParseTree("classVarDec", "");

   if (have("keyword", "static") || have("keyword", "field")) {
        classVarDecTree->addChild(current());
        next(); // Advance to the next token
    } else {
        throw ParseException();
    }

    if (have("keyword", "int") || have("keyword", "char") || have("keyword", "boolean") || have("identifier", "")) {
        classVarDecTree->addChild(current());
        next(); // Advance to the next token
    } else {
        throw ParseException();
    }

    if (current()->getType() == "identifier") {
        classVarDecTree->addChild(current());
        next(); // Advance to the next token
    } else {
        throw ParseException();
    }

    while (have("symbol", ",")) {
        classVarDecTree->addChild(current());
        next(); // Advance to the next token

        if (current()->getType() == "identifier") {
            classVarDecTree->addChild(current());
            next(); // Advance to the next token
        } else {
            throw ParseException();
        }
    }

    // if (have("symbol",",")) {
    //     classVarDecTree->addChild(current());
    //     next(); // Advance to the next token
    // } else {
    //     throw ParseException();
    // }

    // if (current()->getType() == "identifier") {
    //     classVarDecTree->addChild(current());
    //     next(); // Advance to the next token
    // } else {
    //     throw ParseException();
    // }

    if (have("symbol", ";")) {
        classVarDecTree->addChild(current());
        next(); // Advance to the next token
    } else {
        throw ParseException();
    }

    return classVarDecTree;
}

/**
 * Generates a parse tree for a method, function, or constructor
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutine() {
    ParseTree* subroutine = new ParseTree("subroutine", "");

    if (have("keyword", "function") || have("keyword", "method") || have("keyword", "constructor")) {
        subroutine->addChild(current());
        next();
    } else {
        throw ParseException();
    }

    while (have("keyword","void") || have("keyword","int") || have("keyword","char") || have("keyword","boolean") || have("identifier","")) {
        subroutine->addChild(current());
        next();
    }

    if(current()->getType() == "identifier") {
        subroutine->addChild(current());
        next();
    } else {
        throw ParseException();
    }

    while (have("identifier", "new")) {
        subroutine->addChild(current());
        next();
    }

    if (have("symbol","(")) {
        subroutine->addChild(current());
        next();
    } else {
        throw ParseException();
    }
    
    while (have("keyword", "int") || have("keyword", "char") || have("keyword", "boolean") || have("identifier", "")) {
        ParseTree* parameterList = compileParameterList();
        subroutine->addChild(parameterList);
    }

    if (have("symbol",")")) {
        subroutine->addChild(current());
        next();
    } else {
        throw ParseException();
    }

    while (have("symbol", "{")) {
        ParseTree* subRoutineBody = compileSubroutineBody();
        subroutine->addChild(subRoutineBody);
    }

    return subroutine;
}

/**
 * Generates a parse tree for a subroutine's parameters
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileParameterList() {
   ParseTree* parameterList = new ParseTree("parameterList", "");

    if (have("keyword", "int") || have("keyword", "char") || have("keyword", "boolean") || have("identifier", "") || (current()->getType() == "identifier")) {
        parameterList->addChild(current());
        next(); // Advance to the next token
    } else {
        throw ParseException();
    }

    if (current()->getType() == "identifier") {
        parameterList->addChild(current());
        next(); // Advance to the next token
    } else {
        throw ParseException();
    }

    while (have("symbol", ",")) {
        parameterList->addChild(current());
        next(); // Advance to the next token

        if (have("keyword", "int") || have("keyword", "char") || have("keyword", "boolean") || have("identifier", "") || (current()->getType() == "identifier")) {
        parameterList->addChild(current());
        next(); // Advance to the next token
    } else {
        throw ParseException();
    }

    if (current()->getType() == "identifier") {
        parameterList->addChild(current());
        next(); // Advance to the next token
    } else {
        throw ParseException();
    }

    }
    return parameterList;
}

/**
 * Generates a parse tree for a subroutine's body
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutineBody() {
    ParseTree* subroutineBody = new ParseTree("subroutineBody", "");

    if (have("symbol","{")) {
        subroutineBody->addChild(current());
        next();
    } else {
        throw ParseException();
    }

    while (have("keyword","var")) {
        ParseTree* varDec = compileVarDec();
        subroutineBody->addChild(varDec);
    }

    while (have("keyword", "let") || have("keyword", "if") || have("keyword", "while") || have("keyword", "do") || have("keyword", "return")) {
        ParseTree* statements = compileStatements();
        subroutineBody->addChild(statements);
    }

    if (have("symbol","}")) {
        subroutineBody->addChild(current());
        next();
    } else {
        throw ParseException();
    }

    return subroutineBody;
}

/**
 * Generates a parse tree for a subroutine variable declaration
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileVarDec() {
    ParseTree* varDecTree = new ParseTree("varDec", "");

    while (have("keyword","var")) {
        varDecTree->addChild(current());
        next();

        if (have("keyword","int") || have("keyword","char") || have("keyword","boolean") || have("identifier","") || (current()->getType() == "identifier")) {
            varDecTree->addChild(current());
            next();
        } else {
            throw ParseException();
        }

        if (current()->getType() == "identifier") {
            varDecTree->addChild(current());
            next();
        } else {
            throw ParseException();
        }

        while (have("symbol", ",")) {
            varDecTree->addChild(current());
            next();

            if (current()->getType() == "identifier") {
                varDecTree->addChild(current());
                next();
            } else {
                break;
            }
        }

        if (have("symbol", ";")) {
            varDecTree->addChild(current());
            next();
        } else {
            throw ParseException();
        }
    }

    return varDecTree;
}

/**
 * Generates a parse tree for a series of statements
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileStatements() {
    ParseTree* statements = new ParseTree("statements", "");

    if (have("keyword", "let")) {
        ParseTree* letStatement = compileLet();
        statements->addChild(letStatement);
    }

    if (have("keyword", "if")) {
        ParseTree* ifStatement = compileIf();
        statements->addChild(ifStatement);
    }

    if (have("keyword", "do")) {
        ParseTree* doStatement = compileDo();
        statements->addChild(doStatement);
    }

    if (have("keyword", "while")) {
        ParseTree* whileStatement = compileWhile();
        statements->addChild(whileStatement);
    }

    if (have("keyword", "return")) {
        ParseTree* returnStatement = compileReturn();
        statements->addChild(returnStatement);
    }


    return statements;
}

/**
 * Generates a parse tree for a let statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileLet() {
ParseTree* letStatementTree = new ParseTree("letStatement", "");

    if (have("keyword", "let")) {
        letStatementTree->addChild(current());
        next();
    } else {
        throw ParseException();
    }

    if (current()->getType() == "identifier") {
        letStatementTree->addChild(current());
        next();
    } else {
        throw ParseException();
    }

    while (have("symbol","[")) {
        letStatementTree->addChild(current()); 
        next();

        if(have("keyword", "skip")) {
            letStatementTree->addChild(current());
            next();
        } else {
            break;
        }
        if(have("symbol", "]")) {
            letStatementTree->addChild(current());
            next();
        } else {
            break;
        }
    }


    if (have("symbol", "=")) {
        letStatementTree->addChild(current());
        next();
    } else {
        throw ParseException();
    }

    if (have("keyword", "skip")) {
        ParseTree* expressionTree = compileExpression();
        letStatementTree->addChild(expressionTree);
        next();
    } else {
        throw ParseException();
    }

    if (have("symbol", ";")) {
        letStatementTree->addChild(current());
        next();
    } else {
        throw ParseException();
    }

    return letStatementTree;
}

/**
 * Generates a parse tree for an if statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileIf() {
    ParseTree* ifStatementTree = new ParseTree("ifStatement", "");

    if (have("keyword", "if")) {
        ifStatementTree->addChild(current());
        next(); // Advance to the next token
    } else {
        throw ParseException();
    }

    if (have("symbol", "(")) {
        ifStatementTree->addChild(current());
        next(); // Advance to the next token
    } else {
        throw ParseException();
    }

    if (have("keyword", "skip")) {
        ParseTree* expressionTree = compileExpression();
        ifStatementTree->addChild(expressionTree);
        next();
    } else {
        throw ParseException();
    }
   
    if (have("symbol", ")")) {
        ifStatementTree->addChild(current());
        next(); // Advance to the next token
    } else {
        throw ParseException();
    }

    if (have("symbol", "{")) {
        ifStatementTree->addChild(current());
        next(); // Advance to the next token
    } else {
        throw ParseException();
    }

    if(current()->getType() == "statements") {
        ParseTree* statementsTree = compileStatements();
        ifStatementTree->addChild(statementsTree);
        next();
    } else {
        throw ParseException();
    }
    
    if(have("symbol", "}")) {
        ifStatementTree->addChild(current());
        next(); // Advance to the next token
    } else {
        throw ParseException();
    }

    while (have("keyword", "else")) {
        ifStatementTree->addChild(current());
        next(); // Advance to the next token
    
        if (have("symbol", "{")) {
            ifStatementTree->addChild(current());
            next(); // Advance to the next token
        } else {
            throw ParseException();
        }

        if(have("symbol","}")) {
            ifStatementTree->addChild(current());
            next(); // Advance to the next token
        } else {
            throw ParseException();
        }
    }

    return ifStatementTree;
}

/**
 * Generates a parse tree for a while statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileWhile() {
   ParseTree* whileStatementTree = new ParseTree("whileStatement", "");

    if (have("keyword", "while")) {
        whileStatementTree->addChild(current());
        next(); // Advance to the next token
    } else {
        throw ParseException();
    }

    if (have("symbol", "(")) {
        whileStatementTree->addChild(current());
        next(); // Advance to the next token
    } else {
        throw ParseException();
    }

    if (have("keyword", "skip")) {
        ParseTree* expressionTree = compileExpression();
        whileStatementTree->addChild(expressionTree);
        next();
    } else {
        throw ParseException();
    }

    if (have("symbol", ")")) {
        whileStatementTree->addChild(current());
        next(); // Advance to the next token
    } else {
        throw ParseException();
    }

    if (have("symbol", "{")) {
        whileStatementTree->addChild(current());
        next(); // Advance to the next token
    } else {
        throw ParseException();
    }

    if(have("symbol", "}")) {
        whileStatementTree->addChild(current());
        next(); // Advance to the next token
    } else {
        throw ParseException();
    }

    return whileStatementTree;
}

/**
 * Generates a parse tree for a do statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileDo() {
    ParseTree* doStatementTree = new ParseTree("doStatement", "");

    if(have("keyword", "do")) {
        doStatementTree->addChild(current());
        next();
    } else {
        throw ParseException();
    }

    if (have("keyword", "skip")) {
        doStatementTree->addChild(current());
        next();
    } else {
        throw ParseException();
    }

    if (have("symbol", ";")) {
        doStatementTree->addChild(current());
        next();
    } else {
        throw ParseException();
    }

    return doStatementTree;
}

/**
 * Generates a parse tree for a return statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileReturn() {
    ParseTree* returnStatementTree = new ParseTree("returnStatement", "");

    if (have("keyword", "return")) {
        returnStatementTree->addChild(current());
        next();
    } else {
        throw ParseException();
    }

    if (have("symbol", ";")) {
        returnStatementTree->addChild(current());
        next();
    } else {
        throw ParseException();
    }


    return returnStatementTree;
}

/**
 * Generates a parse tree for an expression
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpression() {
    ParseTree* expressionTree = new ParseTree("expression", "");

    if (have("keyword", "skip")) {
        expressionTree->addChild(current());
    } else {
        throw ParseException();
    }

    return expressionTree;
 }

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
