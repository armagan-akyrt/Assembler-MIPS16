#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<regex.h>
#include<string.h>

void* replace_commas(char *str);
char* decode_instruction(char** instr_list);
char* int_to_binary(int n, int size);

char instruction[17] = "";
struct machine_code 
{
    char format; // format of the instruction (R, I, or J)
    char *op; // operation (add, sub, etc.)
    char *rs; // source register
    char *rt; // target register
    char *rd; // destination register
    char *func;
    char *target_addr; // target address.
    char *address_imm; // address or immediate value
};

const char *file_name; // name of the assembly file, provided through command line args.

// this is an assembler file, will take input.txt in assembly format, and will generate an output.txt in machine code format.
int main(int argc, char const *argv[])
{
    if( argc == 2 ) 
    {
        printf("Selected file is:  %s\n", argv[1]);
        (file_name) = argv[1];
    }     
        
   else 
    {
        printf("Exactly one argument is expected.\n");
        return 1;
    }
      
    FILE *fp;
    FILE *fo = fopen("output.txt", "w");
    char *instr = NULL; // hold the instruction of the current line
    fp = fopen(file_name, "r");
    size_t len = 0;
    ssize_t read;
    char *delimiter = " "; // to clear comments
    char *delimiter_comments = "//";
    char **instr_list; // holds instructions seperately for better control.

    if (fp == NULL)
    {
        printf("Specified file could not found, exiting program...\n");
        return 1;
    }
    
    // instructions are retrieved here
    printf("Contents: \n");
    while ((read = getline(&instr, &len, fp)) != -1) 
    {
        //printf("Retrieved line of length %zu:\n", read);
        //printf("%s\n", instr);
        
        int count = 0;
        char *token = strtok(instr, delimiter_comments); // removes comments from instruction.
        replace_commas(instr);
        //printf("\n%s\n", instr);
        token = strtok(instr, delimiter); // retokenizes the string.
        while (token != NULL)
        {
            instr_list[count] = (char *)token;
            printf("Token no: %d, str: %s\n", count, token);
            token = strtok(NULL, delimiter);
            count++;
        }
        instr_list[count] = NULL;

/*         // debug, will remove later.
        size_t i = 0;
        printf("Commencing str test: \n");
        while (instr_list[i] != NULL)
        {
            
            printf("%ld: %s\n", i+1, instr_list[i]);
            i++;

        } */
        
        
        decode_instruction(instr_list);
        fprintf(fo, "%s\n", instruction); // print instruction out.
        strcpy(instruction, "");




    }
    
    printf("\n");
    

    fclose(fp);
    fclose(fo);

    if (instr)
        free(instr);

    return 0;
}

char* decode_instruction(char** instr_list) 
{

    struct machine_code instr_machine;

    
    for (size_t i = 0; instr_list[0][i]; i++)
    {
       instr_list[0][i] = tolower(instr_list[0][i]);
    }

    // to get format.
    if (strcmp(instr_list[0], "add") == 0)
    {
        instr_machine.format = 'R';
        instr_machine.op = "000";
        instr_machine.func = "0000";
    }
    else if (strcmp(instr_list[0], "sub") == 0)
    {
        instr_machine.format = 'R';
        instr_machine.op = "000";
        instr_machine.func = "0001";
    }
    else if (strcmp(instr_list[0], "and") == 0)
    {
        instr_machine.format = 'R';
        instr_machine.op = "000";
        instr_machine.func = "0010";
    }
    else if (strcmp(instr_list[0], "or") == 0)
    {
        instr_machine.format = 'R';
        instr_machine.op = "000";
        instr_machine.func = "0011";
    }
    else if (strcmp(instr_list[0], "slt") == 0)
    {
        instr_machine.format = 'R';
        instr_machine.op = "000";
        instr_machine.func = "0100";
    }
    else if (strcmp(instr_list[0], "jr") == 0)
    {
        instr_machine.format = 'R';
        instr_machine.op = "000";
        instr_machine.func = "1000";
    }
    else if (strcmp(instr_list[0], "slti") == 0)
    {
        instr_machine.format = 'I';
        instr_machine.op = "001";
        
    }
    else if (strcmp(instr_list[0], "j") == 0)
    {
        instr_machine.format = 'J';
        instr_machine.op = "010";
    }
    else if (strcmp(instr_list[0], "jal") == 0)
    {
        instr_machine.format = 'R';
        instr_machine.op = "011";
    }
    else if (strcmp(instr_list[0], "lw") == 0)
    {
        instr_machine.format = 'I';
        instr_machine.op = "100";
    }
    else if (strcmp(instr_list[0], "sw") == 0)
    {
        instr_machine.format = 'I';
        instr_machine.op = "101";
    }
    else if (strcmp(instr_list[0], "beq") == 0)
    {
        instr_machine.format = 'I';
        instr_machine.op = "110";
    }
    else if (strcmp(instr_list[0], "addi") == 0)
    {
        instr_machine.format = 'I';
        instr_machine.op = "111";
    }
    

    
    // according to format, generates instruction.
    if (instr_machine.format == 'R')
    {
        
        // remove first words, be it R or #, depending on the instruction.
        memmove(&instr_list[1][0], &instr_list[1][1], strlen(instr_list[1]) - 0); // rd
        
        memmove(&instr_list[2][0], &instr_list[2][1], strlen(instr_list[2]) - 0); // rs
        memmove(&instr_list[3][0], &instr_list[3][1], strlen(instr_list[3]) - 0);  // rt
        
        strcat(instruction, instr_machine.op);
        
        instr_machine.rs = int_to_binary(atoi(instr_list[1]), 3);
        strcat(instruction, instr_machine.rs);
        instr_machine.rt = int_to_binary(atoi(instr_list[2]), 3);
        strcat(instruction, instr_machine.rt);
        instr_machine.rd = int_to_binary(atoi(instr_list[3]), 3);
        strcat(instruction, instr_machine.rd);
        
        
        strcat(instruction, instr_machine.func);
        printf("INSTRUCTION: %s\n",instruction);
    }

    else if (instr_machine.format == 'I')
    {
        
        // remove first words, be it R or #, depending on the instruction.
        memmove(&instr_list[1][0], &instr_list[1][1], strlen(instr_list[1]) - 0); // rt
        memmove(&instr_list[2][0], &instr_list[2][1], strlen(instr_list[2]) - 0); // imm
        memmove(&instr_list[3][0], &instr_list[3][1], strlen(instr_list[3]) - 0); // rs  
        
        strcat(instruction, instr_machine.op);

        instr_machine.rs = int_to_binary(atoi(instr_list[3]), 3);
        strcat(instruction, instr_machine.rs);
        instr_machine.rt = int_to_binary(atoi(instr_list[1]), 3);
        strcat(instruction, instr_machine.rt);
        instr_machine.address_imm = int_to_binary(atoi(instr_list[2]), 7);
        strcat(instruction, instr_machine.address_imm);
        
        printf("INSTRUCTION: %s\n",instruction);
    }

    else if (instr_machine.format == 'J')
    {
        
        // remove first words, be it R or #, depending on the instruction.
        memmove(&instr_list[1][0], &instr_list[1][1], strlen(instr_list[1]) - 0); // target addr

        strcat(instruction, instr_machine.op);

        instr_machine.target_addr = int_to_binary(atoi(instr_list[1]), 13);
        
        strcat(instruction, instr_machine.target_addr);
        printf("INSTRUCTION: %s\n",instruction);
    }
    
    return instruction;

}

// removes all commas in a string.
void* replace_commas(char *str) 
{
    int i;
    for (i = 0; str[i] != '\0'; i++) {
        if (strchr(",", str[i])) {
            str[i] = ' ';
        }
    }
}

char* int_to_binary(int n, int size) 
{
    int i;
    static char binary[33];
    for (i = size-1; i >= 0; i--) {
        binary[i] = (n & 1) + '0';
        n >>= 1;
    }
    binary[size] = '\0';
    return binary;

}