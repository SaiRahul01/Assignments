    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include<stdbool.h>
    #include <math.h>
    #include <ctype.h>
    #define MAX_TOKENS (1000)
    #define MAX_LINE_LENGTH (1000)
    #define MAX_LINES (1000)
    #define MAX_WORD_LENGTH (100)
    #define EXIT_SUCCESS 0
    #define max_no_of_vars 100
        #define max_var_len 20
     
        FILE *ofptr;
        bool var_dec = false;
        int no_of_vars = 0;
        char all_vars[max_no_of_vars][max_var_len];
        const char seperators[] = " \t\r\n\v\f;=(";
     
     
        void program_start()
        {
            fprintf(ofptr, "#include<stdio.h>\n");
            fprintf(ofptr, "int main(){\n");
        }
     
        void dec_of_var(char line[])
        {
            fprintf(ofptr,"%s\n",line);
            return;
        }
     
     
        void dec_of_assignment(char line[])
        {
            fprintf(ofptr, "%s\n", line);
            return;
        }
     
        void read_function(char line[])
        {
            char *token;
            token = strtok(line, seperators);
            token = strtok(NULL, seperators);
            fprintf(ofptr, "scanf(\"%%d\",&%s);\n", token);
        
            return;
        }
     
        void write_function(char line[])
        {
            char *token;
            token = strtok(line, seperators);
            token = strtok(NULL, seperators);
            if (token[0] >= '0' && token[0] <= '9')
            {
                fprintf(ofptr, "printf(\"constant = %%d\\n\",%s);\n", token);
            }
            else
            {
                fprintf(ofptr, "printf(\"%s = %%d\\n\",%s);\n", token, token);
            }
            return;
        }
     
        void for_loop(char line[])
        {
            fprintf(ofptr, "%s\n", line);
            return;
        }
     
        void program_end()
        {
            fprintf(ofptr, "return 0;\n");
            fprintf(ofptr, "}");
        }
        
        bool checking_var_names(char *token)
        {
            if (strcmp(token, "for") == 0 || strcmp(token, "int") == 0 || strcmp(token, "read") == 0 || strcmp(token, "write") == 0)
            {
                return false;
            }
            return true;
        }
     
        void store_var_name(char line[])
        {
            const char seperators[] = " \t\r\v\f,;\n";
            char *token = strtok(line, seperators);
            token = strtok(NULL, seperators);
            while (token != NULL)
            {
                if (checking_var_names(token))
                {
                    strcpy(all_vars[no_of_vars], token);
                    no_of_vars++;
                    token = strtok(NULL, seperators);
                }
                else
                {
                    printf("Program contains one of reserved words as variables (\"%s\"), exiting the program\n", token);
                    exit(0);
                }
            }
        }
     
        bool contain_semicolon(char line[])
        {
            if (strchr(line, ';') != NULL)
            {
                return true;
            }
            return false;
        }
        
        void replace_char(char line[])
        {
            int i = 0;
            while (line[i] != '\0')
            {
                if ((int)line[i] == 13 || line[i] == '\n' || line[i] == '\f' || line[i] == '\t' || line[i] == '\v')
                {
                    line[i] = ' ';
                }
                ++i;
            }
            return;
        }
     
        void get_line_to_semicolon(FILE *fp, char concatenatedString[], char line[])
        {
            strcat(concatenatedString, line);
            strcat(concatenatedString, " ");
            while (fscanf(fp, "%[^\n]\n", line) != EOF && !contain_semicolon(line))
            {
                strcat(concatenatedString, line);
                strcat(concatenatedString, " ");
            }
            strcat(concatenatedString, line);
            strcat(concatenatedString, " ");
            replace_char(concatenatedString);
            return;
        }
     
        bool var_exists(char *token)
        {
            for (int i = 0; i < no_of_vars; ++i)
            {
                if (strcmp(token, all_vars[i]) == 0)
                {
                    return true;
                }
            }
            return false;
        }
     
        void validity_of_expr(char line[])
        {
            char seperators[] = " \t\r\v\f,;\n(){}<+->*/=";
            char *token = strtok(line, seperators);
            bool oneflag = false;
            while (token != NULL)
            {
                if (strcmp(token, "int") == 0 || strcmp(token, "read") == 0 || strcmp(token, "write") == 0 || strcmp(token, "for") == 0)
                {
                    oneflag = true;
                }
                else if (token[0] >= '0' && token[0] <= '9')
                {
                    NULL;
                }
                else if (!var_exists(token))
                {
                    printf("Variable Undeclared");
                    exit(0);
                }
                token = strtok(NULL, seperators);
            }
            printf("\n");
        }
     
        bool contain_for_loop(char line[])
        {
            int countOfSemiColon = 0;
            int i = 0;
            while (line[i] != '\0' && line[i] != 13 && line[i] != '{')
            {
                if (line[i] == ';')
                {
                    countOfSemiColon++;
                }
                ++i;
            }
            int sizeOfline = i;
            return (countOfSemiColon == 2) && line[sizeOfline - 1] == ')';
        }
     
        int get_freq(char c, char line[])
        {
            int count = 0;
            int i = 0;
            while (line[i] != '\0' && line[i] != 13)
            {
                if (line[i] == c)
                {
                    ++count;
                }
                ++i;
            }
            return count;
        }
     
        void get_for_loop_closing(FILE *fp, char concatenatedString[], char line[])
        {
            strcat(concatenatedString, line);
            int numberOfOpeningBrackets = get_freq('(', line);
            int numberOfClosingBrackets = get_freq(')', line);
            int numberofSemiColon = get_freq(';', line);
            while (fscanf(fp, "%[^\n]\n", line) != EOF && (numberOfOpeningBrackets + get_freq('(', line) != numberOfClosingBrackets + get_freq(')', line)) && numberofSemiColon != 2)
            {
                strcat(concatenatedString, line);
            }
            strcat(concatenatedString, line);
            replace_char(concatenatedString);
            printf("%s\n", concatenatedString);
            return;
        }
     
     
    char tokens[MAX_LINES][MAX_TOKENS][MAX_WORD_LENGTH];
    int line_lengths[MAX_LINES];
    int NUM_LINES;
    char* variable_list[100];
    int num_variables;
     
    /**
    Reads the passed input file line by line.
    Removes all space type characters from the lines, including \n (Carriage Return). 
    Modifies the global tokens and num_tokens Vars. 
    */
     
    typedef struct Node{
        char* name;
        int n;
        struct Node* children[20];
    } Node;
     
     
    int tokenize(char* filename);
    int Start_Sym(Node*);
    int Prod(int lineno,Node* root);
    int Stat(int lineno,int tokenno,Node*);
    int Dec_Stat(int lineno,Node*);
    int Var_List(int lineno, int tokenno,Node* root);
    int Var(int lineno,int tokenno,Node* root);
    int Int_Const(int lineno,int tokenno,Node* root);
    int Read_Stat(int lineno,int tokenno,Node*);
    int Write_Stat(int lineno,int tokenno,Node*);
    int Assign_Stat(int lineno,int tokenno,int end,Node*);
    int For_Loop_Stat(int lineno,Node*);
    int Seq(int lineno,int tokenno,int endline,Node*);
    int Expr(int lineno, int tokenno, int end,Node*);
    int Term1(int lineno, int tokenno, int end,Node*);
    int Term2(int lineno, int tokenno, int end,Node*);
    int Factor(int lineno, int tokenno, int end,Node*);
     
    void printpreorder(Node* root,FILE* traversal);
     
     
    int tokenize(char* filename) {
     
        FILE* in_file = fopen(filename, "r");
        char line[MAX_TOKENS];
        int l=0;
        int w=0;
        int c=0;
        char last;
        char ch  = fgetc(in_file);
        while(ch != EOF){
            // printf("%c",ch);
            // if(ch == '\n'){
            //     printf("nl");
            // }
            if(ch != '\n')
                last = ch;
            if(ch == ' ' || ch == '\t'){
                if(strlen(tokens[l][w]) != 0){
                    w++;
                    c = 0;
                }
            }
            else if(ch == '\n'){
                
            }
            else if(ch == '{' || ch == '}' || ch == '(' || ch == ')'  || ch == '=' || ch == '+' || ch == '-' || ch == '>' || ch == '/' || ch == '*' || ch == ','){
                if(strlen(tokens[l][w]) != 0){
                    w++;
                    c = 0;
                }
                tokens[l][w][c] = ch;
                w++;
            }
            else if(ch == ';'){
                if(strlen(tokens[l][w]) != 0)
                    w++;
                line_lengths[l] = w;
                l++;
                w = 0;
                c = 0;
            }
            else{
                tokens[l][w][c] = ch;
                c++;
            }
            ch = fgetc(in_file);
        }
        printf("Last : %c\n",last);
        if(last == ';'){
            NUM_LINES = l;
            return 1;
        }
        else{
            NUM_LINES = l+1;
            return 0;
        }
    }
     
 
    void emul(int argc,char* argv){
         char line1[100];
            char line2[100];
            if (argc != 2)
            {
                printf("Insufficient Arguments");
                exit(0);
            }
        
            FILE *ifptr = fopen(argv, "r");
            ofptr = fopen("output.c", "w");
        
            char *token;
            program_start(ofptr);
            while (fscanf(ifptr, "%[^\n]\n", line1) != EOF)
            {
                if (strcmp(line1, "{") == 0)
                {
                    fprintf(ofptr, "{\n");
                    continue;
                }
                else if (strcmp(line1, "};") == 0)
                {
                    fprintf(ofptr, "};\n");
                    continue;
                }
                else if (strcmp(line1, "}") == 0)
                {
                    fprintf(ofptr, "};\n");
                    continue;
                }
                char concatenatedString[200] = "\0";
                if (line1[0] == 'f' && line1[1] == 'o' && line1[2] == 'r' && (line1[3] == '(') || (line1[3] == ' ') && line1[4] == '(')
                {
                    if (contain_for_loop(line1))
                    {
                        strcpy(concatenatedString, line1);
                    }
                    else
                    {
                        get_for_loop_closing(ifptr, concatenatedString, line1);
                    }
                }
                else
                {
                    if (contain_semicolon(line1))
                    {
                        strcpy(concatenatedString, line1);
                    }
                    else
                    {
                        get_line_to_semicolon(ifptr, concatenatedString, line1);
                    }
                }
        
                char line4[100];
                strcpy(line2, concatenatedString);
                strcpy(line4, concatenatedString);
                if (var_dec == true)
                {
                    validity_of_expr(line4);
                }
                token = strtok(concatenatedString, seperators);
                if (strcmp(token, "int") == 0)
                {
                    char line3[100];
                    strcpy(line3, line2);
                    store_var_name(line3);
                    dec_of_var(line2);
                    var_dec = true;
                }
                else if (strcmp(token, "read") == 0)
                {
                    read_function(line2);
                }
                else if (strcmp(token, "write") == 0)
                {
                    write_function(line2);
                }
                else if (strcmp(token, "for") == 0)
                {
                    for_loop(line2);
                }
                else
                {
                    dec_of_assignment(line2);
                }
                if (!var_dec)
                {
                    printf("Dec_Stat not in first line");
                    exit(0);
                }
            }
            fclose(ifptr);
            program_end(ofptr);
            fclose(ofptr);
 
 
            system("gcc output.c -o output.out");
            system("output.out");
 
 
 
 
 
    }
    int main(int argc, char** argv) {
     
        if (argc < 2) {
            perror("Pass the name of the input file as the first parameter. e.g.: ./simulator input.txt");
            exit(EXIT_FAILURE);
        }
     
        int res = tokenize(argv[1]);
        if(!res){
            printf("Tokenization : Unsuccessful\n");
        }
        else{
            printf("Tokenization : Successful\n");
        }
     
        for(int i=0;i<NUM_LINES;i++){
            for(int j=0;j<line_lengths[i];j++){
                printf("Token : %s | ",tokens[i][j]);
            }
        }
     
        struct Node* root = (Node*)malloc(sizeof(Node));
        root->name = "Start_Sym";
        root->n = 0;
        int result = Start_Sym(root);
     
        if(result == 1){
            printf("Start_Sym : Successful!\n");
        }
        else{
            printf("Start_Sym : Unsucessful!\n");
        }
     
     
        // preorder code 
        
        FILE* traversal = fopen("preorder.txt","w");
        printpreorder(root,traversal);
     
     
        emul(argc,argv[1]);
     
        return EXIT_SUCCESS;
    }
     
    void printpreorder(Node* root,FILE* traversal){
        if(root->n == 0){
            fprintf(traversal,"[");
            fprintf(traversal,"%s ",root->name);
            fprintf(traversal,"]");
            return;
        }
        
        fprintf(traversal,"[");
        fprintf(traversal,"%s ",root->name);
        for(int i=0;i<root->n;++i){
            printpreorder(root->children[i],traversal);
        }
        fprintf(traversal,"]");
    }
     
    int Start_Sym(Node* root){
     
        
        int l = 0;
        struct Node* node = (Node*)malloc(sizeof(Node));
        node->name = "Dec_Stat";
        node->n = 0;
        if(Dec_Stat(l,node)){
            printf("Dec_Stat: successful\n");
     
            root->n++;
            root->children[root->n-1] = node;

            struct Node* node4 = (Node*)malloc(sizeof(Node));
            node4->name = ";";
            node4->n = 0;
            root->n++;
            root->children[root->n-1] = node4;

     
            l++;
        }
        else{
            printf("Dec_Stat: unsuccessful\n");
            //return 0;
        }
        if(l == NUM_LINES && l == 1) return 1;
       
     
        struct Node* node1 = (Node*)malloc(sizeof(Node));
        node1->name = "Prod";
        node1->n = 0;
     
        if(Prod(l,node1)){
             
            printf("Prod: successful\n");
     
            root->n++;
            root->children[root->n-1] = node1;
            
            return 1;
        }
        else{
            printf("Prod: unsuccessful\n");
            return 0;
        }
     
        printf("\nded bro\n");
    }
     
    int Prod(int lineno,Node* root){
     
        
     
        struct Node* node = (Node*)malloc(sizeof(Node));
        node->name = "Prod";
        node->n = 0;
     
        struct Node* node1 = (Node*)malloc(sizeof(Node));
        node1->name = "Stat";
        node1->n=0;
     
     
          printf("\nim here\n");
        int lines = Stat(lineno,0,node1);
       
        if(lines == 0) return 0;
      
        if ((lineno + lines) >= NUM_LINES) {
            root->n++;
            root->children[root->n-1]=node1;
     
            return 1;
        }
        root->n++;
        root->children[root->n-1] = node1;
     
        if(Prod(lineno + lines,node)){
            root->n++;
            root->children[root->n-1] = node;
            return 1;
        }
        return 0;
    }
     
    int Stat(int lineno,int tokenno, Node* root){
        printf("In Stat\n");
        struct Node* node = (Node*)malloc(sizeof(Node));
        node->name = "Read_Stat";
        node->n = 0;
        struct Node* node1 = (Node*)malloc(sizeof(Node));
        node1->name = "Write_Stat";
        node1->n = 0;
        struct Node* node2 = (Node*)malloc(sizeof(Node));
        node2->name = "Assign_Stat";
        node2->n = 0;
        struct Node* node3 = (Node*)malloc(sizeof(Node));
        node3->name = "For_Loop_Stat";
        node3->n = 0;
     
        if(Read_Stat(lineno,tokenno,node)){
            root->n++;
            root->children[root->n-1] = node;
            struct Node* node4 = (Node*)malloc(sizeof(Node));
            node4->name = ";";
            node4->n = 0;
            root->n++;
            root->children[root->n-1] = node4;
            return 1;
        }
        else if(Write_Stat(lineno,tokenno,node1)){
            root->n++;
            root->children[root->n-1] = node1;
            struct Node* node5 = (Node*)malloc(sizeof(Node));
            node5->name = ";";
            node5->n = 0;
            root->n++;
            root->children[root->n-1] = node5;
            return 1;        
        }
        else if(Assign_Stat(lineno,tokenno,line_lengths[lineno]-1,node2)){
            root->n++;
            root->children[root->n-1] = node2;
            struct Node* node6 = (Node*)malloc(sizeof(Node));
            node6->name = ";";
            node6->n = 0;
            root->n++;
            root->children[root->n-1] = node6;
            printf("Assign_Stat in Stat : Successful\n");
            return 1;        
        }
        int lines = For_Loop_Stat(lineno,node3);
        if(lines > 0){
            root->n++;
            root->children[root->n-1] = node3;
            printf("For_Stat in Stat : Successful\n");
            struct Node* node7 = (Node*)malloc(sizeof(Node));
            node7->name = ";";
            node7->n = 0;
            root->n++;
            root->children[root->n-1] = node7;
            printf("Assign_Stat in Stat : Successful\n");
            return lines;
        }
        printf("Stat ended\n");
        return 0;
    }
     
     
    int Dec_Stat(int lineno,Node* root){
        //check if first token is int,
        if(line_lengths[lineno] < 2) return 0;
        if(strcmp(tokens[lineno][0],"int") == 0){
            
        struct Node* node = (Node*)malloc(sizeof(Node));
        node->name = "int";
        node->n = 0;
     
        struct Node* node1 = (Node*)malloc(sizeof(Node));
        node1->name = "Var_List";
        node1->n = 0;
     
            if(Var_List(lineno,1,node1)){
     
                root->n++;
                root->children[root->n-1] = node;
     
                 
     
                root->n++;
                root->children[root->n-1] = node1;
     
                
                return 1;
            }
            else{
                return 0;
            }
        }
        else{
            return 0;
        }
        //if it is then check next tokens is Var list function
        //if it is not return false 
    }
     
     
    int Var_List(int lineno, int tokenno,Node* root){
        //if(tokenno == size of line) check Var(lineno,tokenno)
        if(tokenno >= line_lengths[lineno]){
            return 0;
        }
        else if(tokenno == line_lengths[lineno]-1){
     
                struct Node* node = (Node*)malloc(sizeof(Node));
                node->name = "Var";
                node->n = 0;
     
                root->n++;
                root->children[root->n-1]=node;
            variable_list[num_variables] =  tokens[lineno][tokenno];
            num_variables++;
            return Var(lineno,tokenno,node);
        }
        else{
                
     
                
     
                struct Node* node1 = (Node*)malloc(sizeof(Node));
                node1->name = "Var";
                node1->n = 0;
     
                root->n++;
                root->children[root->n-1]=node1;
     
                struct Node* node2 = (Node*)malloc(sizeof(Node));
                node2->name = ",";
                node2->n = 0;
     
                root->n++;
                root->children[root->n-1]=node2;
     
                struct Node* node3 = (Node*)malloc(sizeof(Node));
                node3->name = "Var_List";
                node3->n = 0;
     
                root->n++;
                root->children[root->n-1]=node3;
            variable_list[num_variables] =  tokens[lineno][tokenno];
            num_variables++;
            return Var(lineno,tokenno,node1) && (strcmp(tokens[lineno][tokenno+1],",") == 0) && Var_List(lineno,tokenno+2,node3); 
        }
        //check Var(lineno,tokenno) if yes then check next token whether it is a ,
        //if this is true call vl(lineno,tokenno) if true return true
    }
     
    int Var(int lineno,int tokenno,Node* root){
        // [a-z]+ 
        printf("In Var : Line no : %d  Token no: %d\n",lineno,tokenno);
        char* s = tokens[lineno][tokenno];
        int len = strlen(s);
        if(len == 0)
        {
            return 0;
        }
        else if(strcmp(s,"for") == 0 || strcmp(s,"read") == 0 || strcmp(s,"write") == 0 || strcmp(s,"int") == 0) 
        {
            return 0;
        }
        else
        {
            for(int i = 0; i < len; i++)
            {
                if(!islower(s[i]))
                {
                    return 0;
                }
            }
        }
     
        int f = 0;
        for(int i=0;i<num_variables;i++){
            if(strcmp(s,variable_list[i]) == 0){
                f = 1;
                break;
            }
        }
     
        if(f == 0) return 0;
     
        struct Node* node = (Node*)malloc(sizeof(Node));
        node->name = s;
        node->n = 0;
        root->n++;
        root->children[root->n-1] = node;
     
        return 1;
    }
     
    int Int_Const(int lineno,int tokenno,Node* root){
        // regex = [0-9]+
        char *s = tokens[lineno][tokenno];
        printf("Intger const: %s\n",s);
        for(int i=0;i<strlen(s);i++){
            if(s[i]>'9' || s[i]<'0'){
                printf("Integer constant : unsuccessful\n");
                return 0;
            }
        }
     
        printf("Integer constant : successful\n");
     
        struct Node* node = (Node*)malloc(sizeof(Node));
        node->name = s;
        node->n = 0;
        root->n++;
        root->children[root->n-1] = node;
        return 1;
       
                          
    }
     
    int Read_Stat(int lineno,int tokenno,Node* root){
        printf("In read : Line no : %d  Token no: %d\n",lineno,tokenno);
        if(line_lengths[lineno]-tokenno != 2) return 0;
        if(strcmp(tokens[lineno][tokenno],"read") == 0){
            struct Node* node = (Node*)malloc(sizeof(Node));
            node->name = "read";
            node->n = 0;
            root->n++;
            root->children[root->n-1] = node;
            struct Node* node1 = (Node*)malloc(sizeof(Node));
            node1->name = "Var";
            node1->n = 0;
            // struct Node* node2 = (Node*)malloc(sizeof(Node));
            // node2->name = ";";
            // node2->n = 0;
            if(Var(lineno,tokenno+1,node1)){
                root->n++;
                root->children[root->n-1] = node1;            
                // root->n++;
                // root->children[root->n-1] = node2;            
                return 1;

            } 
            else{
                //printf("Read : Unsuccessful\n");
                return 0;
            }
        }
        else{
            //printf("Read : Unsuccessful\n");
            return 0;
        }
    }
     
    int Write_Stat(int lineno,int tokenno,Node* root){
        printf("In Expr : Line no : %d  Token no: %d\n",lineno,tokenno);
        if(line_lengths[lineno]-tokenno != 2) return 0;
        if(strcmp(tokens[lineno][tokenno],"write") == 0){
            struct Node* node = (Node*)malloc(sizeof(Node));
            node->name = "write";
            node->n = 0;
            root->n++;
            root->children[root->n-1] = node;
            struct Node* node1 = (Node*)malloc(sizeof(Node));
            node1->name = "Var";
            node1->n = 0;
            // struct Node* node2 = (Node*)malloc(sizeof(Node));
            // node2->name = ";";
            // node2->n = 0;
            if(Var(lineno,tokenno+1,node1)){
                root->n++;
                root->children[root->n-1] = node1; 
                // root->n++;
                // root->children[root->n-1] = node2;             
                return 1;
            }
            else return 0;
        }
        else{
            return 0;
        }
    }
     
     
    int Assign_Stat(int lineno,int tokenno,int end,Node* root){
        printf("In assignment : Line no : %d  Token no: %d End : %d\n",lineno,tokenno,end);
        if(end - tokenno + 1 < 3) return 0;
        struct Node* node = (Node*)malloc(sizeof(Node));
        node->name = "Var";
        node->n = 0;
        if(!Var(lineno,tokenno,node)) return 0;
        root->n++;
        root->children[root->n-1]=node;
        
        struct Node* node1 = (Node*)malloc(sizeof(Node));
        node1->name = "=";
        node1->n = 0;
        if(strcmp(tokens[lineno][tokenno + 1],"=") != 0) return 0;
        root->n++;
        root->children[root->n-1]=node1;
        
        struct Node* node2 = (Node*)malloc(sizeof(Node));
        node2->name = "Expr";
        node2->n = 0;
        if(!Expr(lineno,tokenno + 2,end,node2)) return 0;
        root->n++;
        root->children[root->n-1]=node2;
        return 1;
    }
     
    int For_Loop_Stat(int lineno,Node* root){
        printf("In for : Line no : %d  \n",lineno);
        int startline = lineno;
        int lines = 0;
        if(line_lengths[lineno] < 3) return 0; // --> for ( assignment
     
        struct Node* node1 = (Node*)malloc(sizeof(Node));
        node1->name = "for";
        node1->n = 0;
     
        if(strcmp(tokens[lineno][0],"for") != 0) return 0;
        
        root->n++;
        root->children[root->n-1]=node1; 
     
        struct Node* node2 = (Node*)malloc(sizeof(Node));
        node2->name = "(";
        node2->n = 0;
     
        if(strcmp(tokens[lineno][1],"(") != 0) return 0;
     
        root->n++;
        root->children[root->n-1]=node2;
     
        struct Node* node3 = (Node*)malloc(sizeof(Node));
        node3->name = "Assign_stat";
        node3->n = 0;
     
        if(!Assign_Stat(lineno,2,line_lengths[lineno]-1,node3)) return 0;
     
        root->n++;
        root->children[root->n-1]=node3;

        struct Node* node11 = (Node*)malloc(sizeof(Node));
        node11->name = ";";
        node11->n = 0;
        root->n++;
        root->children[root->n-1] = node11;
        printf("Assign_Stat in Stat : Successful\n");
     
        lineno++; 
        lines++;
        if(lineno >= NUM_LINES) return 0;
        struct Node* node10 = (Node*)malloc(sizeof(Node));
        node10->name = "Expr";
        node10->n = 0;
        root->n++;
        root->children[root->n-1]=node10;
        if(!Expr(lineno,0,line_lengths[lineno] - 1,node10)) return 0;

        struct Node* node12 = (Node*)malloc(sizeof(Node));
        node12->name = ";";
        node12->n = 0;
        root->n++;
        root->children[root->n-1] = node12;
        printf("Assign_Stat in Stat : Successful\n");
        
        lineno++;
        lines++;
        if(lineno >= NUM_LINES) return 0;
        if(line_lengths[lineno] < 2) return 0;
        int last_round = 0;
        int last_flower = 0;
        for(int i=0;i<=line_lengths[lineno]-1;i++){
            if(strcmp(tokens[lineno][i],")") == 0){
                if(last_flower == 0)
                    last_round = i;
            }
            else if(strcmp(tokens[lineno][i],"{") == 0){
                last_flower = 1;
            }
        }
        if(last_round == 0){
            printf("for : unsuccessful 1\n");
            return 0;
        } 
        struct Node* node4 = (Node*)malloc(sizeof(Node));
        node4->name = "Assign_stat";
        node4->n = 0;
        
        
        if(!Assign_Stat(lineno,0,last_round-1,node4)) return 0;
     
        root->n++;
        root->children[root->n-1]=node4;
     
        struct Node* node5 = (Node*)malloc(sizeof(Node));
        node5->name = ")";
        node5->n = 0;
        root->n++;
        root->children[root->n-1]=node5;
        
        if(last_round + 1 >= line_lengths[lineno]){
            printf("for : unsuccessful 2\n");
            return 0;
        }
        if(strcmp(tokens[lineno][last_round+1],"{") != 0){
            printf("for : unsuccessful 3\n");
            return 0;
        }
        
        struct Node* node6 = (Node*)malloc(sizeof(Node));
        node6->name = "{";
        node6->n = 0;
        root->n++;
        root->children[root->n-1]=node6;
     
     
        last_flower = 0;
        int next_for = 0;
        //printf("last round : %d,line length : %d\n",last_round,line_lengths[lineno]);
        if(last_round+2 == line_lengths[lineno] - 1){
            if(strcmp(tokens[lineno][last_round+2],"}") == 0){
                struct Node* node7 = (Node*)malloc(sizeof(Node));
                node7->name = "}";
                node7->n = 0;
                root->n++;
                root->children[root->n-1]=node7;
                return lines+1;
            }
        }
        for(int i=lineno+1;i<NUM_LINES;i++){
            if(line_lengths[i] == 1  && strcmp(tokens[i][0],"}") == 0){
                if(next_for == 0)
                    last_flower = i;
            }
            else if(strcmp(tokens[i][0],"for") == 0){
                next_for = 1;
            }
        }
        if(last_flower == 0){
            printf("for : unsuccessful 4\n");
            return 0;
        }
        if(last_round+2 >= line_lengths[lineno]){
            printf("for : unsuccessful 5\n");
            return 0;
        }
     
        struct Node* node8 = (Node*)malloc(sizeof(Node));
        node8->name = "Seq";
        node8->n = 0;
        
        if(!Seq(lineno,last_round+2,last_flower -1,node8)){
            printf("for : unsuccessful 6\n");
            return 0;
        }
     
        root->n++;
        root->children[root->n-1]=node8;
     
        struct Node* node9 = (Node*)malloc(sizeof(Node));
        node9->name = "}";
        node9->n = 0;
        root->n++;
        root->children[root->n-1]=node9;
     
        return last_flower-startline + 1;
    }
     
    int Seq(int lineno,int tokenno,int endline, Node* root){
        printf("In Seq : Line no : %d  Token no: %d End : %d\n",lineno,tokenno,endline);
        if(lineno > endline) return 1;
     
        struct Node* node1 = (Node*)malloc(sizeof(Node));
        node1->name = "Stat";
        node1->n = 0;
        struct Node* node2 = (Node*)malloc(sizeof(Node));
        node2->name = "Seq";
        node2->n = 0;
     
        if(!Stat(lineno,tokenno,node1)) return 0;
        root->n++;
        root->children[root->n-1]=node1;  
        
        if(!Seq(lineno+1,0,endline,node2)) return 0;
        if(lineno != endline){
            root->n++;
            root->children[root->n-1]=node2; 
        }
        return 1;
    }
     
     
    int Expr(int lineno, int tokenno, int end,Node* root){
        printf("In Expr : Line no : %d  Token no: %d End : %d\n",lineno,tokenno,end);
        if(tokenno > end) return 0;
        int lastsign = tokenno;
        int lastsym = 0; 
        for(int i=tokenno;i<end;i++){
            if(strcmp(tokens[lineno][i],">") == 0 ){
                lastsign = i;
                lastsym = 1;
            }
            else if((strcmp(tokens[lineno][i],"=") == 0 && strcmp(tokens[lineno][i+1],"=") == 0)){
                lastsign = i;
                lastsym = 2;
            }
        }
        if(lastsign == tokenno){
            struct Node* node = (Node*)malloc(sizeof(Node));
            node->name = "Term1";
            node->n = 0;
            if(!Term1(lineno,tokenno,end,node)) return 0;
            root->n++;
            root->children[root->n-1]=node;
            return 1;
        }
        else{
            struct Node* node1 = (Node*)malloc(sizeof(Node));
            node1->name = "Expr";
            node1->n = 0;
            if(!Expr(lineno,tokenno,lastsign-1,node1)) return 0;
            root->n++;
            root->children[root->n-1]=node1;
     
            if(lastsym == 1){
                struct Node* node3 = (Node*)malloc(sizeof(Node));
                node3->name = ">";
                node3->n = 0;
                root->n++;
                root->children[root->n-1]=node3;
            }
            else{
                struct Node* node4 = (Node*)malloc(sizeof(Node));
                node4->name = "==";
                node4->n = 0;
                root->n++;
                root->children[root->n-1]=node4;
            }
     
            struct Node* node2 = (Node*)malloc(sizeof(Node));
            node2->name = "Term1";
            node2->n = 0;
            if(!Term1(lineno,lastsign+1,end,node2)) return 0;
            root->n++;
            root->children[root->n-1]=node2;
            return 1;
        }
    }
     
    int Term1(int lineno, int tokenno, int end, Node* root){
        printf("In Term1 : Line no : %d  Token no: %d End : %d\n",lineno,tokenno,end);
        if(tokenno > end) return 0;
        int lastsign = tokenno;
        int lastsym = 0; 
        for(int i=tokenno;i<end;i++){
            if(strcmp(tokens[lineno][i],"+") == 0 ){
                lastsign = i;
                lastsym = 1;
            }
            else if((strcmp(tokens[lineno][i],"-") == 0)){
                lastsign = i;
                lastsym = 2;
            }
        }
        if(lastsign == tokenno){
            struct Node* node = (Node*)malloc(sizeof(Node));
            node->name = "Term2";
            node->n = 0;
            if(!Term2(lineno,tokenno,end,node)) return 0;
            root->n++;
            root->children[root->n-1]=node;        
            return 1;
        }
        else{
            struct Node* node1 = (Node*)malloc(sizeof(Node));
            node1->name = "Term1";
            node1->n = 0;        
            if(!Term1(lineno,tokenno,lastsign-1,node1)) return 0;
            root->n++;
            root->children[root->n-1]=node1;
            
            if(lastsym == 1){
                struct Node* node3 = (Node*)malloc(sizeof(Node));
                node3->name = "+";
                node3->n = 0;
                root->n++;
                root->children[root->n-1]=node3;
            }
            else{
                struct Node* node4 = (Node*)malloc(sizeof(Node));
                node4->name = "-";
                node4->n = 0;
                root->n++;
                root->children[root->n-1]=node4;
            }        
     
            struct Node* node2 = (Node*)malloc(sizeof(Node));
            node2->name = "Term2";
            node2->n = 0;
            if(!Term2(lineno,lastsign+1,end,node2)) return 0;
            root->n++;
            root->children[root->n-1]=node2;
            return 1;
        }
     
    }
     
    int Term2(int lineno, int tokenno, int end, Node* root){
        printf("In Term2 : Line no : %d  Token no: %d End : %d\n",lineno,tokenno,end);
        if(tokenno > end) return 0;
        int lastsign = tokenno;
        int lastsym = 0; 
        for(int i=tokenno;i<end;i++){
            if(strcmp(tokens[lineno][i],"*") == 0){
                lastsign = i;
                printf("lastsign assign : %d\n",lastsign);
                lastsym = 1;
                printf("lastsym assign : %d\n",lastsym);
            }
            else if(strcmp(tokens[lineno][i],"/") == 0){
                lastsign = i;
                lastsym = 2;
            }
        }
        printf("lastsign display : %d\n",lastsign);
        printf("lastsym display : %d\n",lastsym);
        if(lastsign == tokenno){
            struct Node* node = (Node*)malloc(sizeof(Node));
            node->name = "Factor";
            node->n = 0;
            if(!Factor(lineno,tokenno,end,node)) return 0;
            root->n++;
            root->children[root->n-1]=node;        
            return 1;
        }
        else{
            struct Node* node1 = (Node*)malloc(sizeof(Node));
            node1->name = "Term2";
            node1->n = 0;        
            if(!Term2(lineno,tokenno,lastsign-1,node1)) return 0;
            root->n++;
            root->children[root->n-1]=node1;
            
            if(lastsym == 1){
                printf("lastsym : *\n");
                struct Node* node3 = (Node*)malloc(sizeof(Node));
                node3->name = "*";
                node3->n = 0;
                root->n++;
                root->children[root->n-1]=node3;
            }
            else{
                struct Node* node4 = (Node*)malloc(sizeof(Node));
                node4->name = "/";
                node4->n = 0;
                root->n++;
                root->children[root->n-1]=node4;
            }        
     
            struct Node* node2 = (Node*)malloc(sizeof(Node));
            node2->name = "Factor";
            node2->n = 0;
            if(!Factor(lineno,lastsign+1,end,node2)) return 0;
            root->n++;
            root->children[root->n-1]=node2;
            return 1;
        }
    }
    int Factor(int lineno, int tokenno, int end, Node* root){
        printf("In Factor : Line no : %d  Token no: %d End : %d\n",lineno,tokenno,end);
        int l = end - tokenno + 1;
        if(l == 0) return 0;
        
        if(l == 1){
            struct Node* node = (Node*)malloc(sizeof(Node));
            node->name = "Var";
            node->n = 0;
            
            if(!Var(lineno,tokenno,node)){
                struct Node* node1 = (Node*)malloc(sizeof(Node));
                node1->name = "Int_Const";
                node1->n = 0;
                if(!Int_Const(lineno,tokenno,node1)) return 0;
                root->n++;
                root->children[root->n-1]=node1;
                return 1;
            }
            else{
                root->n++;
                root->children[root->n-1]=node;
                return 1;
            }
        }
        struct Node* node2 = (Node*)malloc(sizeof(Node));
        node2->name = "(";
        node2->n = 0;
        if(strcmp(tokens[lineno][tokenno],"(") != 0) return 0;
        root->n++;
        root->children[root->n-1]=node2;
     
        struct Node* node3 = (Node*)malloc(sizeof(Node));
        node3->name = "Expr";
        node3->n = 0;
        if(!Expr(lineno,tokenno+1,end-1,node3)) return 0;
        root->n++;
        root->children[root->n-1]=node3;
        
        struct Node* node4 = (Node*)malloc(sizeof(Node));
        node4->name = ")";
        node4->n = 0;
        if(strcmp(tokens[lineno][end],")") != 0) return 0;
        root->n++;
        root->children[root->n-1]=node4;
 
 
        return 1;
    }