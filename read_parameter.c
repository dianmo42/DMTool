#include "variables.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { N_I, N_R } Vtype;

typedef struct {
    char *v_name;
    void *v_ptr;
    Vtype v_type;
    int v_len, v_status;
} NameList;

#define NameI(x)                                                               \
    {                                                                          \
#x, &x, N_I, sizeof(x) / sizeof(int)                                   \
    }
#define NameR(x)                                                               \
    {                                                                          \
#x, &x, N_R, sizeof(x) / sizeof(double)                                \
    }

#define NP_I ((int *)(namelist[k].v_ptr) + j)
#define NP_R ((double *)(namelist[k].v_ptr) + j)

int read_parameter() {
    int ifile, j, k, match, ok;
    char buff[80], *token;
    fn_dump = (char *)malloc(256 * sizeof(char));

    NameList namelist[] = {
        NameR(dt),   NameI(Nstep), NameI(Nskip), NameI(ilevel), NameI(nthread),
        NameI(irdf), NameR(rcut),  NameI(nbin),  NameI(ifluc),  NameI(issf),
        NameI(imsd), NameI(isisf), NameR(qmax),  NameI(iovlp),  NameR(a0)
    };

    // reset status of namelists
    for (k = 0; k < sizeof(namelist) / sizeof(NameList); ++k)
        namelist[k].v_status = 0;
    ok = 1;
    ifile = 0;

    FILE *fp_para = fopen("parameter.md", "r");
    while (1) {
        fgets(buff, 80, fp_para);
        if (feof(fp_para))
            break;
        token = strtok(buff, " \t\n");
        if (!token)
            break;

        if (strcmp(token, "xyzfile") == 0) {
            token = strtok(NULL, ", \t\n");
            strcpy(fn_dump, token);
            ifile = 1;
        }

        match = 0;
        for (k = 0; k < sizeof(namelist) / sizeof(NameList); ++k) {
            if (strcmp(token, namelist[k].v_name) == 0) {
                match = 1;
                if (namelist[k].v_status == 0) {
                    namelist[k].v_status = 1;
                    for (j = 0; j < namelist[k].v_len; ++j) {
                        token = strtok(NULL, ", \t\n");
                        if (token) {
                            switch (namelist[k].v_type) {
                            case N_I:
                                *NP_I = atol(token);
                                break;
                            case N_R:
                                *NP_R = atof(token);
                                break;
                            default:
                                break;
                            }
                        } else {
                            namelist[k].v_status = 2;
                            ok = 0;
                        }
                    }
                    token = strtok(NULL, ", \t\n");
                    if (token) {
                        namelist[k].v_status = 3;
                        ok = 0;
                    }
                    break;
                } else {
                    namelist[k].v_status = 4;
                    ok = 0;
                }
            }
        }
        if (!match)
            ok = 0;
    }
    if (!ifile)
        ok = 0;
    fclose(fp_para);

    for (k = 0; k < sizeof(namelist) / sizeof(NameList); ++k) {
        if (namelist[k].v_status != 1)
            ok = 0;
    }

    // check namelist
    for (k = 0; k < sizeof(namelist) / sizeof(NameList); ++k) {
        switch (namelist[k].v_status) {
        case 0:
            printf("%s: no data\n", namelist[k].v_name);
            break;
        case 1:
            break;
        case 2:
            printf("%s: missing data\n", namelist[k].v_name);
            break;
        case 3:
            printf("%s: extra data\n", namelist[k].v_name);
            break;
        case 4:
            printf("%s: multiply defined\n", namelist[k].v_name);
            break;
        default:
            break;
        }
    }

    return (ok);
}