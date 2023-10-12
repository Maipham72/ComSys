#include "VMTranslator.h"

using namespace std;

/**
 * VMTranslator constructor
 */
VMTranslator::VMTranslator() {
  // Your code here
}
/**
 * VMTranslator destructor
 */
VMTranslator::~VMTranslator() {
  // Your code here
}

/** Generate Hack Assembly code for a VM push operation */
string VMTranslator::vm_push(string segment, int offset) {
  string result = "";
  if (segment == "constant") {
    result += "@" + to_string(offset) + "\n";
    result += "D=A\n";
  } else if (segment == "static") {
    result += "@" + to_string(16 + offset) + "\n";
    result += "D=M\n";
  } else if (segment == "this") {
    result += "@THIS\n";
    result += "D=M\n";
    result += "@" + to_string(offset) + "\n";
    result += "A=D+A\n";
    result += "D=M\n";
  } else if (segment == "that") {
    result += "@THAT\n";
    result += "D=M\n";
    result += "@" + to_string(offset) + "\n";
    result += "A=D+A\n";
    result += "D=M\n";
  } else if (segment == "temp") {
    result += "@" + to_string(5 + offset) + "\n";
    result += "D=M\n";
  } else if (segment == "pointer") {
    result += "@" + to_string(3 + offset) + "\n";
    result += "D=M\n";
  } else if (segment == "argument") {
    result += "@ARG\n";
    result += "D=M\n";
    result += "@" + to_string(offset) + "\n";
    result += "A=D+A\n";
    result += "D=M\n";
  } else if (segment == "local") {
    result += "@LCL\n";
    result += "D=M\n";
    result += "@" + to_string(offset) + "\n";
    result += "A=D+A\n";
    result += "D=M\n";
  }

  result += "@SP\n";
  result += "A=M\n";
  result += "M=D\n";
  result += "@SP\n";
  result += "M=M+1\n";

  return result;
}

/** Generate Hack Assembly code for a VM pop operation */
string VMTranslator::vm_pop(string segment, int offset) {
  string result = "";

  if (segment == "static") {
    result += "@SP\n";
    result += "AM=M-1\n";
    result += "D=M\n";
    result += "@" + to_string(16 + offset) + "\n";
    result += "D=A+D\n";
    result += "@R13\n";
    result += "M=D\n";
    result += "@R13\n";
    result += "A=M";
    result += "M=D\n";

  } else if (segment == "this") {
    result += "@SP\n";
    result += "AM=M-1\n";
    result += "D=M\n";
    result += "@THIS\n";
    result += "D=M\n";
    result += "@" + to_string(offset) + "\n";
    result += "D=A+D\n";
    result += "@R13\n";
    result += "M=D";
    result += "@SP\n";
    result += "AD=M\n";
    result += "@R13\n";
    result += "A=M";
    result += "M=D\n";

  } else if (segment == "that") {
    result += "@SP\n";
    result += "AM=M-1\n";
    result += "D=M\n";
    result += "@THAT\n";
    result += "D=M\n";
    result += "@" + to_string(offset) + "\n";
    result += "D=A+D\n";
    result += "@R13\n";
    result += "M=D";
    result += "@SP\n";
    result += "AD=M\n";
    result += "@R13\n";
    result += "A=M";
    result += "M=D\n";

  } else if (segment == "temp") {
    result += "@SP\n";
    result += "AM=M-1\n";
    result += "D=M\n";
    result += "@" + to_string(5 + offset) + "\n";
    result += "M=D\n";

  } else if (segment == "pointer") {
    result += "@SP\n";
    result += "AM=M-1\n";
    result += "D=M\n";
    result += "@" + to_string(3 + offset) + "\n";
    result += "D=A+D\n";
    result += "@R13\n";
    result += "M=D\n";
    result += "@R13\n";
    result += "A=M";
    result += "M=D\n";


  } else if (segment == "argument") {
    result += "@SP\n";
    result += "AM=M-1\n";
    result += "D=M\n";
    result += "@ARG\n";
    result += "D=M\n";
    result += "@" + to_string(offset) + "\n";
    result += "D=A+D\n";
    result += "@R13\n";
    result += "M=D";
    result += "@SP\n";
    result += "A=M\n";
    result += "D=M\n";
    result += "@R13\n";
    result += "A=M";
    result += "M=D\n";


  } else if (segment == "local") {
    result += "@SP\n";
    result += "AM=M-1\n";
    result += "D=M\n";
    result += "@LCL\n";
    result += "D=M\n";
    result += "@" + to_string(offset) + "\n";
    result += "D=A+D\n";
    result += "@R13\n";
    result += "M=D";
    result += "@SP\n";
    result += "AD=M\n";
    result += "@R13\n";
    result += "A=M";
    result += "M=D\n";

  }
  return result;
}

/** Generate Hack Assembly code for a VM add operation */
string VMTranslator::vm_add() {
  string result;
  result += "@SP\n";
  result += "AM=M-1\n";
  result += "D=M\n";
  result += "@SP\n";
  result += "AM=M-1\n";
  result += "M=D+M\n";
  result += "@SP\n";
  result += "M=M+1\n";
  return result;
}

/** Generate Hack Assembly code for a VM sub operation */
string VMTranslator::vm_sub() {
  string result;
  result += "@SP\n";
  result += "AM=M-1\n";
  result += "D=M\n";
  result += "A=A-1\n";
  result += "M=M-D\n";
  return result;
}

/** Generate Hack Assembly code for a VM neg operation */
string VMTranslator::vm_neg() {
  string result;
  result += "@SP\n";
  result += "A=M\n";
  result += "A=A-1\n";
  result += "M=-M\n";
  return result;
}

/** Generate Hack Assembly code for a VM eq operation */
string VMTranslator::vm_eq() {
  string result = "";
  static int label = 0;
  result += "@SP\n";
  result += "AM=M-1\n";
  result += "D=M\n";
  result += "@SP\n";
  result += "AM=M-1\n";
  result += "D=M-D\n";
  result += "@TRUE" + to_string(label) + "\n";
  result += "D;JEQ\n";
  result += "D=0\n";
  result += "@FALSE" + to_string(label) + "\n";
  result += "0;JMP\n";
  result += "(TRUE" + to_string(label) + ")\n";
  result += "D=-1\n";
  result += "(FALSE" + to_string(label) + ")\n";
  result += "@SP\n";
  result += "A=M\n";
  result += "M=D\n";
  result += "@SP\n";
  result += "M=M+1\n";

  label++;
  return result;
}

/** Generate Hack Assembly code for a VM gt operation */
string VMTranslator::vm_gt() {
  string result = "";
  static int label = 0;
  result += "@SP\n";
  result += "AM=M-1\n";
  result += "D=M\n";
  result += "@SP\n";
  result += "AM=M-1\n";
  result += "D=M-D\n";
  result += "@TRUE" + to_string(label) + "\n";
  result += "D;JGT\n";
  result += "D=0\n";
  result += "@FALSE" + to_string(label) + "\n";
  result += "0;JMP\n";
  result += "(TRUE" + to_string(label) + ")\n";
  result += "D=-1\n";
  result += "(FALSE" + to_string(label) + ")\n";
  result += "@SP\n";
  result += "A=M\n";
  result += "M=D\n";
  result += "@SP\n";
  result += "M=M+1\n";

  label++;
  return result;
}

/** Generate Hack Assembly code for a VM lt operation */
string VMTranslator::vm_lt() {
  string result = "";
  static int label = 0;
  result += "@SP\n";
  result += "AM=M-1\n";
  result += "D=M\n";
  result += "@SP\n";
  result += "AM=M-1\n";
  result += "D=M-D\n";
  result += "@TRUE" + to_string(label) + "\n";
  result += "D;JLT\n";
  result += "D=0\n";
  result += "@FALSE" + to_string(label) + "\n";
  result += "0;JMP\n";
  result += "(TRUE" + to_string(label) + ")\n";
  result += "D=-1\n";
  result += "(FALSE" + to_string(label) + ")\n";
  result += "@SP\n";
  result += "A=M\n";
  result += "M=D\n";
  result += "@SP\n";
  result += "M=M+1\n";

  label++;
  return result;
}

/** Generate Hack Assembly code for a VM and operation */
string VMTranslator::vm_and() {
  string result = "";
  result += "@SP\n";
  result += "AM=M-1\n";
  result += "D=M\n";
  result += "A=A-1\n";
  result += "M=D&M\n";

  return result;
}

/** Generate Hack Assembly code for a VM or operation */
string VMTranslator::vm_or() {
  string result = "";

  result += "@SP\n";
  result += "AM=M-1\n";
  result += "D=M\n";
  result += "A=A-1\n";
  result += "M=D|M\n";

  return result;
}

/** Generate Hack Assembly code for a VM not operation */
string VMTranslator::vm_not() {
  string result = "";

  result += "@SP\n";
  result += "A=M\n";
  result += "A=A-1\n";
  result += "M=!M\n";

  return result;
}

/** Generate Hack Assembly code for a VM label operation */
string VMTranslator::vm_label(string label) {
  string result = "";
  result += "(" + label + ")\n";
  return result;
}

/** Generate Hack Assembly code for a VM goto operation */
string VMTranslator::vm_goto(string label) {
  string result = "";
  result += "@" + label + "\n";
  result += "0;JMP\n";
  return result;
}

/** Generate Hack Assembly code for a VM if-goto operation */
string VMTranslator::vm_if(string label) {
  string result = "";
  result += "@SP\n";
  result += "AM=M-1\n";
  result += "D=M\n";
  result += "@" + label + "\n";
  result += "D;JNE\n";
  return result;
}

/** Generate Hack Assembly code for a VM function operation */
string VMTranslator::vm_function(string function_name, int n_vars) {
  string result = "";
  result += "(" + function_name + ")\n";
  for (int i = 0; i < n_vars; i++) {
    result += "@SP\n";
    result += "A=M\n";
    result += "M=0\n";
    result += "@SP\n";
    result += "M=M+1\n";
  }
  return result;
}

/** Generate Hack Assembly code for a VM call operation */
string VMTranslator::vm_call(string function_name, int n_args) {
  string result = "";

  static int index = 0;

  string return_Address = "RETURN_ADDRESS" +  to_string(index);
  result += "@" + return_Address + "\n";
  result += "D=A\n";
  result += "@SP\n";
  result += "AM=M+1\n";
  result += "A=A-1\n";
  result += "M=D\n";

  result += "@LCL\n";
  result += "D=M\n";
  result += "@SP\n";
  result += "AM=M+1\n";
  result += "A=A-1\n";
  result += "M=D\n";

  result += "@ARG\n";
  result += "D=M\n";
  result += "@SP\n";
  result += "AM=M+1\n";
  result += "A=A-1\n";
  result += "M=D\n";

  result += "@SP\n";
  result += "D=M\n";
  result += "@5\n";
  result += "D=D-A\n";
  result += "@" + to_string(n_args) + "\n";
  result += "D=D-A\n";
  result += "@ARG\n";
  result += "M=D\n";
  result += "@SP\n";
  result += "D=M\n";
  result += "@LCL\n";
  result += "M=D\n";

  result += "@THIS\n";
  result += "D=M\n";
  result += "@SP\n";
  result += "AM=M+1\n";
  result += "A=A-1\n";
  result += "M=D\n";

  result += "@THAT\n";
  result += "D=M\n";
  result += "@SP\n";
  result += "AM=M+1\n";
  result += "A=A-1\n";
  result += "M=D\n";

  result += "@" + function_name + "\n";
  result += "0;JMP\n";
  result += "(" + return_Address + ")\n";

  index++;

  return result;
}

/** Generate Hack Assembly code for a VM return operation */
string VMTranslator::vm_return() {
  string result = "";
  result += "@LCL\n";
  result += "D=M\n";
  result += "@R13\n";
  result += "M=D\n";

  // result += "@R13\n";
  // result += "D=M\n";
  result += "@5\n";
  result += "A=D-A\n";
  result += "D=M\n";
  result += "@R14\n";
  result += "M=D\n";

  result += "@ARG\n";
  result += "D=M+1\n";
  result += "@SP\n";
  result += "M=D\n";

  result += "@SP\n";
  result += "AM=M-1\n";
  result += "D=M\n";
  result += "@ARG";
  result += "A=M\n";
  result += "M=D\n";

  result += "@R13\n";
  result += "D=M-1\n";
  result += "A=D\n";
  result += "M=D\n";
  result += "D=M\n";
  result += "@THAT\n";
  result += "M=D\n";

  result += "@R13\n";
  result += "D=M-1\n";
  result += "A=D\n";
  result += "M=D\n";
  result += "D=M\n";
  result += "@THIS\n";
  result += "M=D\n";

  result += "@R13\n";
  result += "D=M-1\n";
  result += "A=D\n";
  result += "M=D\n";
  result += "D=M\n";
  result += "@ARG\n";
  result += "M=D\n";

  result += "@R13\n";
  result += "D=M-1\n";
  result += "A=D\n";
  result += "M=D\n";
  result += "D=M\n";
  result += "@LCL\n";
  result += "M=D\n";

  result += "@R14\n";
  result += "A=M\n";
  result += "0;JMP\n";
  return result;
}