#include "minishell.h"

static char *extract_var_name(const char *arg, int i)
{
    int start;
    int len;

    start = i;
    len = 0;
    if (ft_isalpha(arg[i]) || arg[i] == '_')
    {
        i++;
        len++;
        while (ft_isalnum(arg[i]) || arg[i] == '_')
        {
            i++;
            len++;
        }
    }
    if (len == 0)
        return NULL;
    return ft_strndup(&arg[start], len);
}

//recorrer argv y buscar $var_name
//si no existe, devolver el argumento original
char *expand_argument(const char *arg)
{
    int i;
    char *var_name;
    char *value;

    i = 0;
    while (arg[i])
    {
        if (arg[i] == '$')
        {
            i++;
            if (arg[i] == '?')//caso especial para $? hay que definir g_exit_status
                return ft_itoa(g_exit_status);
            else
            {
                var_name = extract_var_name(arg, i);
                if (!var_name)
                    return ft_strdup(arg);
                value = getenv(var_name);
                free(var_name);
                if (value)
                    return ft_strdup(value);
                else
                    return ft_strdup(arg); // Return original arg si no se encuentra la variable
            }
        }
        i++;
    }

}

//Llamar a expand_argument para cada argumento de argv
int expand_variables(char **argv)
{
    size_t i = 0;
    char *expanded;

    if (!argv)
        return 0;
    while (argv[i])
    {
        expanded = expand_argument(argv[i]);
        if (!expanded)
            return 1;
        free(argv[i]);
        argv[i] = expanded;
        i++;
    }
    return 0;
}