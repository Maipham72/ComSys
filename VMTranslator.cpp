#include "VMTranslator.h"

using namespace std;

/**
 * VMTranslator constructor
 */
VMTranslator::VMTranslator() {
  // Your code here
  // ASM.str(string());
  // symbolCounter = 0;
  // map_segments.clear();
  // map_segments.insert(pair<string, VMSegments>("local", local));
  // map_segments.insert(pair<string, VMSegments>("argument", argument));
  // map_segments.insert(pair<string, VMSegments>("this", thiss));
  // map_segments.insert(pair<string, VMSegments>("that", that));
  // map_segments.insert(pair<string, VMSegments>("constant", constant));
  // map_segments.insert(pair<string, VMSegments>("static", staticc));
  // map_segments.insert(pair<string, VMSegments>("pointer", pointer));
  // map_segments.insert(pair<string, VMSegments>("temp", temp));
}

// void VMTranslator::result += string vmCode) {
//   VMTranslator vmTranslator;
//   if (vmCode.find("(") == string::npos) vmTranslator.ASM << "\t";
//   vmTranslator.ASM << vmCode << endl;
// }

/**
 * VMTranslator destructor
 */
VMTranslator::~VMTranslator() {
  // Your code here
  // map_segments.clear();
}

// string VMTranslator::registerName(string segment, int offset) {
//   string registerStr;
//   if (segment == "local") {
//     registerStr = "LCL";
//   }
//   if (segment == "argument") {
//     registerStr = "ARG";
//   }
//   if (segment == "this") {
//     registerStr = "THIS";
//   }
//   if (segment == "that") {
//     registerStr = "THAT";
//   }
//   if (segment == "static") {
//     registerStr = "16";
//   }
//   if (segment == "pointer") {
//     registerStr = "R" + to_string(3 + offset);
//   }
//   if (segment == "temp") {
//     registerStr = "R" + to_string(5 + offset);
//   }
//   return registerStr + to_string(offset);
// }

/** Generate Hack Assembly code for a VM push operation */
string VMTranslator::vm_push(string segment, int offset) {
  // VMTranslator vmTranslator;
  // vmTranslator.ASM.str(string());
  // string index = to_string(offset);
  // string registerStr = vmTranslator.registerName(segment, offset);
  // switch (vmTranslator.map_segments[segment]) {
  //   case constant:
  //     result += "@" + index + " " + segment + " " + index);
  //     result += "D=A");
  //     result += "@SP");
  //     result += "A=M");
  //     result += "M=D");
  //     result += "@SP");
  //     result += "M=M+1");
  //     break;

  //   case staticc:
  //   case temp:
  //   case pointer:
  //     result += "@" + registerStr + " " + segment + " " + index);
  //     result += "D=A");
  //     result += "@" + index);
  //     result += "A=D+A");
  //     result += "D=M");
  //     result += "@SP");
  //     result += "A=M");
  //     result += "M=D");
  //     result += "@SP");
  //     result += "M=M+1");
  //     break;

  //   case argument:
  //   case local:
  //   case thiss:
  //   case that:
  //     result += "@" + registerStr + " " + segment + " " + index);
  //     result += "D=M");
  //     result += "@" + index);
  //     result += "A=D+A");
  //     result += "D=M");
  //     result += "@SP");
  //     result += "A=M");
  //     result += "M=D");
  //     result += "@SP");
  //     result += "M=M+1");
  //     break;

  //   default:
  //     break;
  // }
  // return vmTranslator.ASM.str() + "\n";
}

/** Generate Hack Assembly code for a VM pop operation */
string VMTranslator::vm_pop(string segment, int offset) {
  
}

/** Generate Hack Assembly code for a VM add operation */
string VMTranslator::vm_add() {
  string result;
  result += "@SP\n";
  result += "AM=M-1\n";
  result += "D=M\n";
  result += "A=A-1\n";
  result += "M=D+M\n";
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
string VMTranslator::vm_label(string label) { 
  string result;
  result += "(" + label + ")\n";
  return result; 
}

/** Generate Hack Assembly code for a VM goto operation */
string VMTranslator::vm_goto(string label) { 
  string result;
  result += "@" + label + "\n";
  result += "0;JMP\n";
  return result; 
}

/** Generate Hack Assembly code for a VM if-goto operation */
string VMTranslator::vm_if(string label) {
  string result;
  result += "@SP\n";
  result += "AM=M-1\n";
  result += "D=M\n";
  result += "@" + label + "\n";
  result += "D;JNE\n";
  return result; 
  }

/** Generate Hack Assembly code for a VM function operation */
string VMTranslator::vm_function(string function_name, int n_vars) {
  string result;
  result += "(" + function_name + ")\n";
  for (int i = 0; i < n_vars; i++) {
    result += "@SP\n";
    result += "AM=M+1\n";
    result += "A=A-1\n";
    result += "M=0\n";
  }
  return result;
}

/** Generate Hack Assembly code for a VM call operation */
string VMTranslator::vm_call(string function_name, int n_args) { 
  string result;
  result += "@RETURN_ADDRESS_" + function_name + "\n";
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

  result += "@" + function_name + "\n";
  result += "0;JMP\n";

  return result; 
}

/** Generate Hack Assembly code for a VM return operation */
string VMTranslator::vm_return() { return ""; }