#ifndef VMTRANSLATOR_H
#define VMTRANSLATOR_H

#include <string>
#include <map>
#include <sstream>

using namespace std;

class VMTranslator {

    public:

        VMTranslator();
        ~VMTranslator();

        string vm_push(string segment, int offset);
        string vm_pop(string segment, int offset);

        string vm_add();
        string vm_sub();
        string vm_neg();

        string vm_eq();
        string vm_gt();
        string vm_lt();
        string vm_and();
        string vm_or();
        string vm_not();

        string vm_label(string label);
        string vm_goto(string label);
        string vm_if(string label);

        string vm_function(string function_name, int n_vars);
        string vm_call(string function_name, int n_args);
        string vm_return();
    
    
     void write(string vmCode);
    string registerName(string segment, int offset);
     int symbolCounter;
     enum VMSegments {
            local,
            argument,
            thiss,
            that,
            constant,
            staticc,
            pointer,
            temp
        };
     map<string, VMSegments> map_segments;
     stringstream ASM;

};

#endif /* VMTRANSLATOR_H */