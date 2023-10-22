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

    while (have("keyword","void")) {
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

      if (have("keyword", "int") || have("keyword", "char") || have("keyword", "boolean") || have("identifier", "")) {
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
        subroutineBody->addChild(current());
        next();

        if (have("keyword","int") || have("keyword","char") || have("keyword","boolean") || have("identifier","")) {
            subroutineBody->addChild(current());
            next();
        } else {
            throw ParseException();
        }

        if (current()->getType() == "identifier") {
            subroutineBody->addChild(current());
            next();
        } else {
            throw ParseException();
        }

        if (have("symbol", ";")) {
            subroutineBody->addChild(current());
            next();
        } else {
            throw ParseException();
        }
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
    ParseTree* varDecTree = new ParseTree("VarDec", "");

    while (have("keyword", "var")) {
        // "var" keyword
        varDecTree->addChild(current());
        next(); // Advance to the next token

        // Variable data type
        varDecTree->addChild(current());
        next(); // Advance to the next token

        // Get all variable names for multiple variable declaration
        while (true) {
            if (have("identifier", "")) {
                varDecTree->addChild(current());
                next(); // Advance to the next token
            } else {
                break;
            }

            if (have("symbol", ",")) {
                varDecTree->addChild(current());
                next(); // Advance to the next token
            } else {
                break;
            }
        }

        // Semicolon ";"
        if (have("symbol", ";")) {
            varDecTree->addChild(current());
            next(); // Advance to the next token
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

    while ((current()->getType() == "keyword") && (have("keyword", "let") || have("keyword", "if") || have("keyword", "while") || have("keyword", "do") || have("keyword", "return"))) {

        if (have("keyword", "let")) {
            ParseTree* letTree = compileLet();
            statements->addChild(letTree);
        } else if (have("keyword", "if")) {
            ParseTree* ifTree = compileIf();
            statements->addChild(ifTree);
        } else if (have("keyword", "while")) {
            ParseTree* whileTree = compileWhile();
            statements->addChild(whileTree);
        } else if (have("keyword", "do")) {
            ParseTree* doTree = compileDo();
            statements->addChild(doTree);
        } else if (have("keyword", "return")) {
            ParseTree* returnTree = compileReturn();
            statements->addChild(returnTree);
        } else {
            throw ParseException();
        }

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
        next(); // Advance to the next token
    } else {
        throw ParseException();
    }

    if (current()->getType() == "identifier") {
        letStatementTree->addChild(current());
        next(); // Advance to the next token
    } else {
        throw ParseException();
    }

    if (have("symbol", "=")) {
        letStatementTree->addChild(current());
        next(); // Advance to the next token
    } else {
        throw ParseException();
    }

    // Parse the expression
    ParseTree* expressionTree = compileExpression();
    if (expressionTree != nullptr) {
        letStatementTree->addChild(expressionTree);
    } else {
        throw ParseException();
    }

    if (have("symbol", ";")) {
        letStatementTree->addChild(current());
        next(); // Advance to the next token
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

    // Parse the expression
    ParseTree* expressionTree = compileExpression();
    if (expressionTree != nullptr) {
        ifStatementTree->addChild(expressionTree);
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

    // Parse statements inside if block
    while (!have("symbol", "}")) {
        ParseTree* statement = compileStatements();
        if (statement != nullptr) {
            ifStatementTree->addChild(statement);
        } else {
            throw ParseException();
        }
    }

    if (have("symbol", "}")) {
        ifStatementTree->addChild(current());
        next(); // Advance to the next token
    } else {
        throw ParseException();
    }

    if (have("keyword", "else")) {
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

    // Parse statements inside else block
    while (!have("symbol", "}")) {
        ParseTree* statement = compileStatements();
        if (statement != nullptr) {
            ifStatementTree->addChild(statement);
        } else {
            throw ParseException();
        }
    }

    if (have("symbol", "}")) {
        ifStatementTree->addChild(current());
        next(); // Advance to the next token
    } else {
        throw ParseException();
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

    // Parse the expression
    ParseTree* expressionTree = compileExpression();
    if (expressionTree != nullptr) {
        whileStatementTree->addChild(expressionTree);
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

    // Parse statements inside while block
    while (!have("symbol", "}")) {
        ParseTree* statement = compileStatements();
        if (statement != nullptr) {
            whileStatementTree->addChild(statement);
        } else {
            throw ParseException();
        }
    }

    if (have("symbol", "}")) {
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

    if (have("keyword", "do")) {
        doStatementTree->addChild(current());
        next(); // Advance to the next token
    } else {
        throw ParseException();
    }

    // Parse the subroutine call
    ParseTree* expressionTree = compileExpression();
    if (expressionTree != nullptr) {
        doStatementTree->addChild(expressionTree);
    } else {
        throw ParseException();
    }

    if (have("symbol", ";")) {
        doStatementTree->addChild(current());
        next(); // Advance to the next token
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
        next(); // Advance to the next token
    } else {
        throw ParseException();
    }

    // Parse the expression
    ParseTree* expressionTree = compileExpression();
    if (expressionTree != nullptr) {
        returnStatementTree->addChild(expressionTree);
    } else {
        throw ParseException();
    }

    if (have("symbol", ";")) {
        returnStatementTree->addChild(current());
        next(); // Advance to the next token
    } else {
        throw ParseException();
    }

    return returnStatementTree;
}

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
