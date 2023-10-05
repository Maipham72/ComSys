#include "VMTranslator.h"

#include <string>

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
  string assemblyCode;

  if (segment == "constant") {
    // Push a constant value onto the stack
    assemblyCode =
        "@" + to_string(offset) +
        "\n"
        "D=A\n"  // Load the constant value into D
        "@SP\n"
        "A=M\n"
        "M=D\n"  // Store the value at the current stack pointer location
        "@SP\n"
        "M=M+1\n";  // Increment the stack pointer
  } else if (segment == "local" || segment == "argument" || segment == "this" ||
             segment == "that") {
    // Push from a memory segment (local, argument, this, or that)
    string segmentPointer;
    if (segment == "local") {
      segmentPointer = "LCL";
    } else if (segment == "argument") {
      segmentPointer = "ARG";
    } else if (segment == "this") {
      segmentPointer = "THIS";
    } else if (segment == "that") {
      segmentPointer = "THAT";
    }

    // Calculate the address to access in the selected segment
    assemblyCode = "@" + segmentPointer +
                   "\n"
                   "D=M\n"  // Load the segment base address into D
                   "@" +
                   to_string(offset) +
                   "\n"
                   "A=D+A\n"  // Calculate the target address: segment base
                              // address + offset
                   "D=M\n"    // Load the value at the target address into D

                   // Push the value onto the stack
                   "@SP\n"
                   "A=M\n"
                   "M=D\n"
                   "@SP\n"
                   "M=M+1\n";  // Increment the stack pointer
  }
  // Add more cases for other memory segments (e.g., static, pointer, temp)

  return assemblyCode;
  // string result;
  // string index = to_string(offset);

  // // if (segment == "constant") {
  // result = "@" + index + " " + segment + " " + index + "\n" + "D=A\n" +
  //          "@SP\n" + "A=M\n" + "M=D\n" + "@SP\n" + "M=M+1\n";
  // //}
  // return result;
}

/** Generate Hack Assembly code for a VM pop operation */
string VMTranslator::vm_pop(string segment, int offset) {
  string assemblyCode;

  if (segment == "local" || segment == "argument" || segment == "this" ||
      segment == "that") {
    // Calculate the target address in the selected segment
    string segmentPointer;
    if (segment == "local") {
      segmentPointer = "LCL";
    } else if (segment == "argument") {
      segmentPointer = "ARG";
    } else if (segment == "this") {
      segmentPointer = "THIS";
    } else if (segment == "that") {
      segmentPointer = "THAT";
    }

    // Calculate the target address: *(segmentPointer + offset)
    assemblyCode = "@" + segmentPointer +
                   "\n"
                   "D=M\n"  // D = segment base address
                   "@" +
                   to_string(offset) +
                   "\n"
                   "A=D+A\n"  // A = segment base address + offset

                   // Store the target address in a temporary location (R13)
                   "D=A\n"
                   "@R13\n"
                   "M=D\n"

                   // Decrement SP to access the top element
                   "@SP\n"
                   "M=M-1\n"
                   "A=M\n"  // Set A to point to the top element

                   // Pop the value from the stack into D
                   "D=M\n"

                   // Retrieve the target address from R13
                   "@R13\n"
                   "A=M\n"

                   // Store the value from D into the target address
                   "M=D\n";
  }
  // Add more cases for other memory segments (e.g., static, pointer, temp)

  return assemblyCode;
  // string result;
  // string index = to_string(offset);

  // result = "@" + index + " " + segment + " " + index + "\n" + "D=A\n" + "@" +
  //          index + "\n" + "D=D+A\n" + "@R13\n" + "M=D\n" + "@SP\n" +
  //          "AM = M-1\n" + "D=M\n" + "@R13\n" + "A=M\n" + "M=D\n";
  // return result;
}

/** Generate Hack Assembly code for a VM add operation */
string VMTranslator::vm_add() {
  string result;
  result =
      "@SP\n"
      "AM=M-1\n"
      "D=M\n"
      "A=A-1\n"
      "M=M+D\n";
  return result;
}

/** Generate Hack Assembly code for a VM sub operation */
string VMTranslator::vm_sub() {
  string result;
  result =
      "@SP\n"
      "AM=M-1\n"
      "D=M\n"
      "A=A-1\n"
      "M=M-D\n";
  return result;
}

/** Generate Hack Assembly code for a VM neg operation */
string VMTranslator::vm_neg() {
  string result;
  result =
      "@SP\n"
      "A=M-1\n"
      "M=-M\n";
  return result;
}

/** Generate Hack Assembly code for a VM eq operation */
string VMTranslator::vm_eq() {
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

      // Set the result to true (-1) if D is 0, otherwise, set it to false (0)
      "M=0\n"
      "@EQUAL_TRUE\n"
      "D;JEQ\n"
      "@SP\n"
      "A=M\n"
      "M=-1\n"  // Set to true (-1)
      "(EQUAL_TRUE)\n"

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