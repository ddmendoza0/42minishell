#include "minishell.h"

/*
 * UPDATE ENVIRONMENT VARIABLES
 * Updates PWD after successful directory change
 */
static int update_pwd_var(t_shell *shell, char *new_pwd)
{
    char    *pwd_var;

    if (!shell || !new_pwd)
        return (0);
    // Create PWD=new_path string
    pwd_var = malloc(strlen("PWD=") + strlen(new_pwd) + 1);
    if (!pwd_var)
        return (0);
    sprintf(pwd_var, "PWD=%s", new_pwd);
    // Update PWD in environment
    if (!set_env_var(shell, pwd_var))
    {
        free(pwd_var);
        return (0);
    }
    free(pwd_var);
    // Update shell's cwd
    if (shell->cwd)
        free(shell->cwd);
    shell->cwd = ft_strdup(new_pwd);
    return (1);
}

/*
 * GET CURRENT WORKING DIRECTORY
 * Gets the current working directory after chdir
 */
static char *get_current_directory(void)
{
    return (getcwd(NULL, 0));
}

/*
 * SIMPLIFIED CD BUILTIN FUNCTION
 * Only handles relative or absolute paths
 */
int builtin_cd(char **argv, t_shell *shell)
{
    char    *target_path;
    char    *new_pwd;

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
        return (handle_error(shell, ERR_MEMORY, "getting current directory"));
    }
    // Update PWD environment variable
    if (!update_pwd_var(shell, new_pwd))
    {
        free(new_pwd);
        return handle_error(shell, ERR_MEMORY, "updating PWD variable");
    }

    free(new_pwd);
    return (set_exit_status(shell, EXIT_SUCCESS));
}