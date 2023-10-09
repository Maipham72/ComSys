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

         static string vm_push( string segment, int offset);
         static string vm_pop( string segment, int offset);

         static string vm_add();
         static string vm_sub();
         static string vm_neg();

         static string vm_eq();
         static string vm_gt();
         static string vm_lt();
         static string vm_and();
         static string vm_or();
         static string vm_not();

        static string vm_label( string label);
        static string vm_goto( string label);
        static string vm_if( string label);

        static string vm_function( string function_name, int n_vars);
        static string vm_call( string function_name, int n_args);
        static string vm_return();
    
    
    static void write( string vmCode);
    string registerName( string segment, int offset);
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
    map< string, VMSegments> map_segments;
    stringstream ASM;

};


#endif /* VMTRANSLATOR_H */