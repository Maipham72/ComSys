#include <string>
#include <iostream>

using namespace std;

string vm_call(string function_name, int n_args){

string asm_code = "";
    asm_code += "@LCL\n";
    asm_code += "D=M\n";
    asm_code += "@FRAME\n";
    asm_code += "M=D\n";
    asm_code += "@5\n";
    asm_code += "A=D-A\n";
    asm_code += "D=M\n";
    asm_code += "@RET\n";
    asm_code += "M=D\n";
    asm_code += "@SP\n";
    asm_code += "AM=M-1\n";
    asm_code += "D=M\n";
    asm_code += "@ARG\n";
    asm_code += "A=M\n";
    asm_code += "M=D\n";
    asm_code += "@ARG\n";
    asm_code += "D=M+1\n";
    asm_code += "@SP\n";
    asm_code += "M=D\n";

    // THAT = *(FRAME-1), THIS = *(FRAME-2), ARG = *(FRAME-3), LCL = *(FRAME-4)
    string segments[] = {"THAT", "THIS", "ARG", "LCL"};
    for(int i = 0; i < 4; i++){
        asm_code += "@FRAME\n";
        asm_code += "D=M-1\n";
        asm_code += "AM=D\n";
        asm_code += "D=M\n";
        asm_code += "@" + segments[i] + "\n";
        asm_code += "M=D\n";
    }

    asm_code += "@RET\n";
    asm_code += "A=M\n";
    asm_code += "0;JMP\n";

    return asm_code;

}

int main() {
  std::cout << vm_call("ArrMax", 0);
}
