#include "VMTranslator.h"

using namespace std;

/**
 * VMTranslator constructor
 */
VMTranslator::VMTranslator() {
  // Your code here
  ASM.str(string());
  symbolCounter = 0;
  map_segments.clear();
  map_segments.insert(pair<string, VMSegments>("local", local));
  map_segments.insert(pair<string, VMSegments>("argument", argument));
  map_segments.insert(pair<string, VMSegments>("this", thiss));
  map_segments.insert(pair<string, VMSegments>("that", that));
  map_segments.insert(pair<string, VMSegments>("constant", constant));
  map_segments.insert(pair<string, VMSegments>("static", staticc));
  map_segments.insert(pair<string, VMSegments>("pointer", pointer));
  map_segments.insert(pair<string, VMSegments>("temp", temp));
}

void VMTranslator::write(string vmCode) {
  if (vmCode.find("(") == string::npos) ASM << "\t";
  ASM << vmCode << endl;
}

/**
 * VMTranslator destructor
 */
VMTranslator::~VMTranslator() {
  // Your code here
  map_segments.clear();
}

string VMTranslator::registerName(string segment, int offset) {
  string registerStr;
  if (segment == "local") {
    registerStr = "LCL";
  }
  if (segment == "argument") {
    registerStr = "ARG";
  }
  if (segment == "this") {
    registerStr = "THIS";
  }
  if (segment == "that") {
    registerStr = "THAT";
  }
  if (segment == "static") {
    registerStr = "16";
  }
  if (segment == "pointer") {
    registerStr = "R" + to_string(3 + offset);
  }
  if (segment == "temp") {
    registerStr = "R" + to_string(5 + offset);
  }
  return registerStr + to_string(offset);
}

/** Generate Hack Assembly code for a VM push operation */
string VMTranslator::vm_push(string segment, int offset) {
  ASM.str(string());
  string index = to_string(offset);
  string registerStr = registerName(segment, offset);
  switch (map_segments[segment]) {
    case constant:
      write("@" + index + " " + segment + " " + index);
      write("D=A");
      write("@SP");
      write("A=M");
      write("M=D");
      write("@SP");
      write("M=M+1");
      break;

    case staticc:
    case temp:
    case pointer:
      write("@" + registerStr + " " + segment + " " + index);
      write("D=A");
      write("@" + index);
      write("A=D+A");
      write("D=M");
      write("@SP");
      write("A=M");
      write("M=D");
      write("@SP");
      write("M=M+1");
      break;

    case argument:
    case local:
    case thiss:
    case that:
      write("@" + registerStr + " " + segment + " " + index);
      write("D=M");
      write("@" + index);
      write("A=D+A");
      write("D=M");
      write("@SP");
      write("A=M");
      write("M=D");
      write("@SP");
      write("M=M+1");
      break;

    default:
      break;
  }
  return ASM.str() + "\n";
}

/** Generate Hack Assembly code for a VM pop operation */
string VMTranslator::vm_pop(string segment, int offset) {}

/** Generate Hack Assembly code for a VM add operation */
string VMTranslator::vm_add() {
  ASM.str(string());
  write("@SP // add");
  write("AM=M-1");
  write("D=M");
  write("A=A-1");
  write("M=D+M");
  return ASM.str() + "\n";
}

/** Generate Hack Assembly code for a VM sub operation */
string VMTranslator::vm_sub() {
    ASM.str(string());
    write("@SP // sub");
    write("AM=M-1");
    write("D=M");
    write("A=A-1");
    write("M=M-D");
    return ASM.str() + "\n";
}

/** Generate Hack Assembly code for a VM neg operation */
string VMTranslator::vm_neg() {
  string assemblyCode =
      "@SP\n"
      "M=M-1\n"
      "A=M\n"   // Set A to point to the top element
      "M=-M\n"  // Negate the value of the top element

      // Increment SP to account for the result
      "@SP\n"
      "M=M+1\n";
  return assemblyCode;
}

/** Generate Hack Assembly code for a VM eq operation */
string VMTranslator::vm_eq() {
  string assemblyCode =
      "@SP\n"
      "AM=M-1\n"
      "D=M\n"  // Store the value of the top element in D

      // Decrement SP again to access the second top element
      "@SP\n"
      "AM=M-1\n"
      "D=M-D\n"  // Subtract the top element from the second top element
      "@LabelTrue\n"
      "D;JEQ\n"
      "D=0\n"
      "@LabelFalse\n"
      "0;JMP\n"
      "(LabelTrue)\n"
      "D=-1\n"
      "(LabelFalse)\n"  // Set to true (-1)
      "@SP\n"
      "A=M\n"
      "M=D\n"

      // Increment SP to account for the result
      "@SP\n"
      "M=M+1\n";

  return assemblyCode;
}

/** Generate Hack Assembly code for a VM gt operation */
string VMTranslator::vm_gt() {
  string assemblyCode =
      "@SP\n"
      "M=M-1\n"
      "A=M\n"  // Set A to point to the top element
      "D=M\n"  // Store the value of the top element in D

      // Decrement SP again to access the second top element
      "@SP\n"
      "M=M-1\n"
      "A=M\n"    // Set A to point to the second top element
      "D=M-D\n"  // Subtract the top element from the second top element

      // Set the result to true (-1) if D is greater than 0, otherwise, set it
      // to false (0)
      "M=0\n"
      "@GT_TRUE\n"
      "D;JGT\n"
      "@SP\n"
      "A=M\n"
      "M=-1\n"  // Set to true (-1)
      "(GT_TRUE)\n"

      // Increment SP to account for the result
      "@SP\n"
      "M=M+1\n";

  return assemblyCode;
}

/** Generate Hack Assembly code for a VM lt operation */
string VMTranslator::vm_lt() { return ""; }

/** Generate Hack Assembly code for a VM and operation */
string VMTranslator::vm_and() { return ""; }

/** Generate Hack Assembly code for a VM or operation */
string VMTranslator::vm_or() { return ""; }

/** Generate Hack Assembly code for a VM not operation */
string VMTranslator::vm_not() { return ""; }

/** Generate Hack Assembly code for a VM label operation */
string VMTranslator::vm_label(string label) { return ""; }

/** Generate Hack Assembly code for a VM goto operation */
string VMTranslator::vm_goto(string label) { return ""; }

/** Generate Hack Assembly code for a VM if-goto operation */
string VMTranslator::vm_if(string label) { return ""; }

/** Generate Hack Assembly code for a VM function operation */
string VMTranslator::vm_function(string function_name, int n_vars) {
  return "";
}

/** Generate Hack Assembly code for a VM call operation */
string VMTranslator::vm_call(string function_name, int n_args) { return ""; }

/** Generate Hack Assembly code for a VM return operation */
string VMTranslator::vm_return() { return ""; }