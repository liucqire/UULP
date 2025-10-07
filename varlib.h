#define MAXVARS 200

struct var {
    char *str;
    int global;
};

extern struct var tab[MAXVARS];


int VLstore(char *name, char *val); 
char *VLlookup(char *name); 
int VLexport(char *name);
void VLlist();
int VLenviron2table(char **env);
char **VLtable2environ();
