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
    ParseTree* program = new ParseTree("program","");
    ParseTree* classTree = compileClass();
    program->addChild(classTree);
    return program;
}

/**
 * Generates a parse tree for a single class
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileClass() {
    ParseTree* classTree = new ParseTree("class","");
    mustBe("keyword","class");
    Token* className = mustBe("identifier","");
    mustBe("symbol","{");
    ParseTree* classVarDecs = compileClassVarDec();
    ParseTree* subroutines = compileSubroutine();
    mustBe("symbol","}");
    classTree->addChild(className);
    classTree->addChild(classVarDecs);
    classTree->addChild(subroutines);
    return classTree;
}

/**
 * Generates a parse tree for a static variable declaration or field declaration
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileClassVarDec() {
    /**
 * Generates a parse tree for a static variable declaration or field declaration
 * @return a ParseTree
 */

    ParseTree* classVarDec = new ParseTree("classVarDec","");
    if (have("keyword","static")){
        mustBe("keyword","static");
    }
    else if (have("keyword","field")){
        mustBe("keyword","field");
    }
    else {
        throw ParseException();
    }
    Token* type = mustBe("keyword","int");
    if (have("keyword","int")){
        mustBe("keyword","int");
    }
    else if (have("keyword","char")){
        mustBe("keyword","char");
    }
    else if (have("keyword","boolean")){
        mustBe("keyword","boolean");
    }
    else if (have("identifier","")){
        mustBe("identifier","");
    }
    else {
        throw ParseException();
    }
    Token* varName = mustBe("identifier","");
    while (have("symbol",",")){
        mustBe("symbol",",");
        Token* varName = mustBe("identifier","");
    }
    mustBe("symbol",";");
    classVarDec->addChild(type);
    classVarDec->addChild(varName);
    return classVarDec;

}

/**
 * Generates a parse tree for a method, function, or constructor
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutine() {
    return NULL;
}

/**
 * Generates a parse tree for a subroutine's parameters
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileParameterList() {
    return NULL;
}

/**
 * Generates a parse tree for a subroutine's body
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutineBody() {
    return NULL;
}

/**
 * Generates a parse tree for a subroutine variable declaration
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileVarDec() {
    return NULL;
}

/**
 * Generates a parse tree for a series of statements
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileStatements() {
    return NULL;
}

/**
 * Generates a parse tree for a let statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileLet() {
    return NULL;
}

/**
 * Generates a parse tree for an if statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileIf() {
    return NULL;
}

/**
 * Generates a parse tree for a while statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileWhile() {
    return NULL;
}

/**
 * Generates a parse tree for a do statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileDo() {
    return NULL;
}

/**
 * Generates a parse tree for a return statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileReturn() {
    return NULL;
}

/**
 * Generates a parse tree for an expression
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpression() {
    return NULL;
}

/**
 * Generates a parse tree for an expression term
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileTerm() {
    return NULL;
}

/**
 * Generates a parse tree for an expression list
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpressionList() {
    return NULL;
}

/**
 * Advance to the next token
 */
void CompilerParser::next(){
    if (tokens.empty()==false){
        tokens.pop_front();
    }

    return;
}

/**
 * Return the current token
 * @return the Token
 */
Token* CompilerParser::current(){
    if (tokens.empty()==false){
        return tokens.front();
    }
    else {
        return NULL;
    }
}

/**
 * Check if the current token matches the expected type and value.
 * @return true if a match, false otherwise
 */
bool CompilerParser::have(std::string expectedType, std::string expectedValue){
    Token* currToken = current();
    if (currToken != nullptr && currToken->getType() == expectedType && currToken->getValue() == expectedValue){
        return true;
    }
    return false;
}

/**
 * Check if the current token matches the expected type and value.
 * If so, advance to the next token, returning the current token, otherwise throw a ParseException.
 * @return the current token before advancing
 */
Token* CompilerParser::mustBe(std::string expectedType, std::string expectedValue){
    Token* currToken = current();
    if (currToken != nullptr && currToken->getType() == expectedType && currToken->getValue() == expectedValue){
        next();
        return currToken;
    }
    else {
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
