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
        next(); // advance
    } else {
        throw ParseException();
    }

     if (current()->getType() == "identifier") {
        std::string identifierName = current()->getValue();
        classTree->addChild(current());
        next(); // advanceTokenIfPossible
    } else {
        throw ParseException();
    }

    if (have("symbol", "{")) {
        std::string symbol = current()->getValue();
        classTree->addChild(current());
        next(); 
    } else {
        throw ParseException();
    }

    // if (have("symbol", "{")) {
    //     classTree->addChild(mustBe("symbol", "{"));
    // } else {
    //     throw ParseException();
    // }

    while(current()->getType() == "keyword" && (current()->getValue() == "static" || current()->getValue() == "field")) {
         ParseTree* classVarDec = compileClassVarDec();
         if (classVarDec != nullptr) {
             classTree->addChild(classVarDec);
         } else {
             throw ParseException();
         }
     }

    while(current()->getType() == "keyword" && (current()->getValue() == "constructor" || current()->getValue() == "function" || current()->getValue() == "method")) {
        ParseTree* subroutine = compileSubroutine();
        if (subroutine != nullptr) {
            classTree->addChild(subroutine);
        } else {
            throw ParseException();
        }
    }

    if (have("symbol", "}")) {
        std::string cSymbol = current()->getValue();
        classTree->addChild(current());
        next(); 
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
        next();
    } else {
        throw ParseException();
    }

    if (current()->getType() == "identifier") {
        std::string identifierName = current()->getValue();
        classVarDecTree->addChild(current());
        next(); // advanceTokenIfPossible
    } else {
        throw ParseException();
    }

    while (current()->getType() == "symbol" && current()->getValue() == ",") {
        classVarDecTree->addChild(current());
        next(); // advanceTokenIfPossible

        if (current()->getType() == "identifier") {
            std::string identifierName = current()->getValue();
            classVarDecTree->addChild(current());
            next(); // advanceTokenIfPossible
        } else {
            throw ParseException();
        }
    }

    if (have("symbol", "}")) {
        std::string cSymbol = current()->getValue();
        classVarDecTree->addChild(current());
        next(); 
    } else {
        throw ParseException();
    }

    

    while (true) {
        if (have("symbol", ";")) {
            classVarDecTree->addChild(mustBe("symbol", ";"));
            break;
        } else if (have("symbol", ",")) {
            classVarDecTree->addChild(mustBe("symbol", ","));
            if (current()->getType() == "identifier") {
                classVarDecTree->addChild(mustBe("identifier", current()->getValue()));
            } else {
                throw ParseException();
            }
        } else {
            throw ParseException();
        }
    }

    return classVarDecTree;
}

/**
 * Generates a parse tree for a method, function, or constructor
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutine() {
    ParseTree* subroutine = new ParseTree("subroutine", "");

    // Parsing subroutine type
    if (have("keyword", "constructor")) {
        mustBe("keyword", "constructor");
    } else if (have("keyword", "function")) {
        mustBe("keyword", "function");
    } else if (have("keyword", "method")) {
        mustBe("keyword", "method");
    } else {
        throw ParseException();
    }

    // Parsing return type
    if (have("keyword", "void")) {
        mustBe("keyword", "void");
    } else if (have("keyword", "int")) {
        mustBe("keyword", "int");
    } else if (have("keyword", "char")) {
        mustBe("keyword", "char");
    } else if (have("keyword", "boolean")) {
        mustBe("keyword", "boolean");
    } else if (have("identifier", "")) {
        mustBe("identifier", "");
    } else {
        throw ParseException();
    }

    Token* subroutineName = mustBe("identifier", "");
    mustBe("symbol", "(");
    // Parse parameter list
    ParseTree* parameterList = compileParameterList();
    mustBe("symbol", ")");
    // Parse subroutine body
    ParseTree* subroutineBody = compileSubroutineBody();

    // Adding children to the parse tree
    subroutine->addChild(subroutineName);
    subroutine->addChild(parameterList);
    subroutine->addChild(subroutineBody);

    return subroutine;
}

/**
 * Generates a parse tree for a subroutine's parameters
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileParameterList() {
   ParseTree* parameterList = new ParseTree("parameterList", "");

    while (current() && !have("symbol", ")")) {
        // Process comma if encountered
        if (have("symbol", ",")) {
            next(); // advanceTokenIfPossible
        }

        // Parse variable type
        std::string varType = current()->getValue();
        parameterList->addChild(new ParseTree("varType", varType));
        next(); // advanceTokenIfPossible

        // Parse variable name
        if (current() && have("identifier", "")) {
            std::string varName = current()->getValue();
            parameterList->addChild(new ParseTree("varName", varName));
            next(); // advanceTokenIfPossible
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
