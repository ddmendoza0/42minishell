#include "minishell.h"

/*
 * UPDATE ENVIRONMENT VARIABLES
 * Updates PWD after successful directory change
 */
static int update_pwd_var(t_shell* shell, char* new_pwd)
{
    char* pwd_var;

    if (!shell || !new_pwd)
        return 0;

    // Create PWD=new_path string
    pwd_var = malloc(strlen("PWD=") + strlen(new_pwd) + 1);
    if (!pwd_var)
        return 0;
    sprintf(pwd_var, "PWD=%s", new_pwd);

    // Update PWD in environment
    if (!set_env_var(shell, pwd_var))
    {
        free(pwd_var);
        return 0;
    }
    free(pwd_var);

    // Update shell's cwd
    if (shell->cwd)
        free(shell->cwd);
    shell->cwd = ft_strdup(new_pwd);

    return 1;
}

/*
 * GET CURRENT WORKING DIRECTORY
 * Gets the current working directory after chdir
 */
static char* get_current_directory(void)
{
    return getcwd(NULL, 0);
}

/*
 * SIMPLIFIED CD BUILTIN FUNCTION
 * Only handles relative or absolute paths
 */
int builtin_cd(char** argv, t_shell* shell)
{
    char* target_path;
    char* new_pwd;

    if (!shell)
        return EXIT_FAILURE;

    // Check if path argument is provided
    if (!argv[1])
    {
        print_error("minishell: cd", NULL, "missing argument");
        return set_exit_status(shell, EXIT_FAILURE);
    }

    // Check for too many arguments
    if (argv[2])
    {
        print_error("minishell: cd", NULL, "too many arguments");
        return set_exit_status(shell, EXIT_FAILURE);
    }

    target_path = argv[1];

    // Actually change directory
    if (chdir(target_path) != 0)
    {
        int error_type = errno;
        return cd_error(shell, target_path, error_type);
    }

    // Get the actual current directory after chdir
    new_pwd = get_current_directory();
    if (!new_pwd)
    {
        return handle_error(shell, ERR_MEMORY, "getting current directory");
    }

    // Update PWD environment variable
    if (!update_pwd_var(shell, new_pwd))
    {
        free(new_pwd);
        return handle_error(shell, ERR_MEMORY, "updating PWD variable");
    }

    free(new_pwd);
    return set_exit_status(shell, EXIT_SUCCESS);
}

/*
 * ENVIRONMENT VARIABLE SETTER
 * This function updates or adds a variable in shell->env
 */
int set_env_var(t_shell* shell, char* var_assignment)
{
    char* var_name;
    char* equals_pos;
    int var_name_len;
    int i;
    int env_count;
    char** new_env;

    if (!shell || !var_assignment)
        return 0;

    equals_pos = ft_strchr(var_assignment, '=');
    if (!equals_pos)
        return 0;

    var_name_len = equals_pos - var_assignment;
    var_name = ft_strndup(var_assignment, var_name_len);
    if (!var_name)
        return 0;

    // Look for existing variable
    i = 0;
    while (shell->env[i])
    {
        if (ft_strncmp(shell->env[i], var_name, var_name_len) == 0 &&
            shell->env[i][var_name_len] == '=')
        {
            // Replace existing variable
            free(shell->env[i]);
            shell->env[i] = ft_strdup(var_assignment);
            free(var_name);
            return (shell->env[i] != NULL);
        }
        i++;
    }

    // Add new variable
    env_count = i;
    new_env = malloc(sizeof(char*) * (env_count + 2));
    if (!new_env)
    {
        free(var_name);
        return 0;
    }

    // Copy existing environment
    i = 0;
    while (i < env_count)
    {
        new_env[i] = shell->env[i];
        i++;
    }

    // Add new variable
    new_env[env_count] = ft_strdup(var_assignment);
    new_env[env_count + 1] = NULL;

    if (!new_env[env_count])
    {
        free(new_env);
        free(var_name);
        return 0;
    }

    // Replace old environment
    free(shell->env);
    shell->env = new_env;
    free(var_name);

    return 1;
}