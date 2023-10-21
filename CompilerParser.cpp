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
    ParseTree* classTree = new ParseTree("class", "");

    if (have("keyword", "class")) {
        classTree->addChild(current());
        next(); // advanceTokenIfPossible
    } else {
        throw ParseException();
    }

    if (current()->getType() == "identifier") {
        std::string className = current()->getValue();
        classTree->addChild(current());
        next(); // advanceTokenIfPossible
    } else {
        throw ParseException();
    }

    // {
    if (have("symbol", "{")) {
        classTree->addChild(current());
        next(); 
    } else {
        throw ParseException();
    }

    // class level subroutines (methods, constructors, functions)
    while (!(have("symbol", "}"))) {
        ParseTree* subroutineTree = compileSubroutine();
        if (subroutineTree != NULL) {
            classTree->addChild(subroutineTree);
        } else {
            throw ParseException();
        }
    }

    if (have("symbol", "}")) {
        classTree->addChild(current());
        next(); 
    } else {
        throw ParseException();
    }

    return classTree;
}

// std::vector<std::string> CompilerParser::getVarNames() {
//     // e.g. var int x,y;
//     std::vector<std::string> varNames;

//     // varName
//     varNames.push_back(current()->getValue());
//     next(); // advanceTokenIfPossible

//     while (!have("symbol", ";")) {
//         // ,
//         if (have("symbol", ",")) {
//             next(); // advanceTokenIfPossible
//         }

//         // varName
//         varNames.push_back(current()->getValue());
//         next(); // advanceTokenIfPossible
//     }

//     // ;
//     if (have("symbol", ";")) {
//         next(); // advanceTokenIfPossible
//     } else {
//         throw ParseException();
//     }

//     return varNames;
// }
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

    // Add keyword
    if (have("keyword", "static") || have("keyword", "field")) {
        classVarDecTree->addChild(new ParseTree("keyword", current()->getValue()));
        next(); // Advance to the next token
    } else {
        throw ParseException(); // Or handle the unexpected case accordingly
    }

    // Add variable data type
    if (current()->getType() == "keyword" || current()->getType() == "identifier") {
        classVarDecTree->addChild(new ParseTree(current()->getType(), current()->getValue()));
        next(); // Advance to the next token
    } else {
        throw ParseException(); // Or handle the unexpected case accordingly
    }

    // Extract variable names
    while (!have("symbol", ";")) {
        if (have("identifier", "")) {
            classVarDecTree->addChild(new ParseTree("identifier", current()->getValue()));
            next(); // Advance to the next token
        } else if (have("symbol", ",")) {
            classVarDecTree->addChild(new ParseTree("symbol", current()->getValue()));
            next(); // Advance to the next token
        } else {
            throw ParseException(); // Or handle the unexpected case accordingly
        }
    }

    // Add semicolon
    if (have("symbol", ";")) {
        classVarDecTree->addChild(new ParseTree("symbol", current()->getValue()));
        next(); // Advance to the next token
    } else {
        throw ParseException(); // Or handle the unexpected case accordingly
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

    while (!have("symbol", "}")) {
        if (have("keyword", "return")) {
            statements->addChild(compileReturn());
        } else if (have("keyword", "let")) {
            statements->addChild(compileLet());
        } else if (have("keyword", "while")) {
            statements->addChild(compileWhile());
        } else if (have("keyword", "if")) {
            statements->addChild(compileIf());
        } else if (have("keyword", "do")) {
            statements->addChild(compileDo());
        }
    }

    return statements;
}

/**
 * Generates a parse tree for a let statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileLet() {
    ParseTree* letTree = new ParseTree("LetStatement", "");

    if (have("keyword", "let")) {
        letTree->addChild(current());
        next(); // advanceTokenIfPossible

        if (have("identifier", "")) {
            letTree->addChild(current());
            next(); // advanceTokenIfPossible

            if (have("symbol", "[")) {
                letTree->addChild(current());
                next(); // advanceTokenIfPossible

                ParseTree* expressionTree = compileExpression();
                letTree->addChild(expressionTree);

                if (have("symbol", "]")) {
                    letTree->addChild(current());
                    next(); // advanceTokenIfPossible
                } else {
                    throw ParseException();
                }

                if (have("symbol", "=")) {
                    letTree->addChild(current());
                    next(); // advanceTokenIfPossible

                    expressionTree = compileExpression();
                    letTree->addChild(expressionTree);

                    if (have("symbol", ";")) {
                        letTree->addChild(current());
                        next(); // advanceTokenIfPossible
                    } else {
                        throw ParseException();
                    }
                } else {
                    throw ParseException();
                }
            } else if (have("symbol", "=")) {
                letTree->addChild(current());
                next(); // advanceTokenIfPossible

                ParseTree* expressionTree = compileExpression();
                letTree->addChild(expressionTree);

                if (have("symbol", ";")) {
                    letTree->addChild(current());
                    next(); // advanceTokenIfPossible
                } else {
                    throw ParseException();
                }
            } else {
                throw ParseException();
            }
        } else {
            throw ParseException();
        }
    } else {
        throw ParseException();
    }

    return letTree;
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
