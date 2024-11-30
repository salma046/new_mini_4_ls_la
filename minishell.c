#include "minishell.h"

t_minishell	g_minishell;

int main3(t_minishell data, char **env)
{
    t_token *temp_tokens;

    temp_tokens = data.tokens;
    
    while (temp_tokens)
    {
        if (ft_check_building(temp_tokens))
        {
			if (!ft_strcmp(temp_tokens->data, "env") && temp_tokens->data)
				ft_env(data);
			if (!ft_strcmp(temp_tokens->data , "unset") && temp_tokens->data)
				ft_unset(NULL,  data);
            check_command(temp_tokens, data.export_env, data.envir);
        }
        else
        {
			// printf("");
			printf("\033[32m-->in:%s\033[0\n", temp_tokens->data);
            int retu = ft_execute(temp_tokens, env);
			if (retu == 127)
				return 1;
        }
		printf("\033[36m-->out:%s\033[0m\n", temp_tokens->data);
        temp_tokens = temp_tokens->next_token;
    }
    return (0);

}

void handle_sigint(int sig)
{
	(void)sig;
	printf("\n\033[1;35m Minishell~$ \033[0m");
}

int	main(int ac, char *av[], char **env)
{
	signal(SIGINT, handle_sigint);
	if (ac > 2)
		return (1);
	(void)av;
	g_minishell.envirement = env;
	g_minishell.envir = mk_env(g_minishell.envirement);
	g_minishell.export_env = mk_env_4expo(g_minishell.envirement);

	while (1)
	{
		g_minishell.command = readline("\033[1;35m Minishell~$ \033[0m");
		if (!g_minishell.command)
		{
			printf("Quiting minishell!\n");
			free_env_list(g_minishell.envir);
			free_env_list(g_minishell.export_env);
			clear_history();
			exit(1);
		}
		add_history(g_minishell.command);		
		g_minishell.tokens = ft_tokenize(g_minishell);
		free(g_minishell.command);
		if (!g_minishell.tokens)
			continue ;
		g_minishell.tokens = rm_qotes(g_minishell.tokens);
		g_minishell.tokens = parsing(g_minishell);
		if (main_heredoc(g_minishell.tokens) < 0)
			continue ;
		if (ft_check_redirections(&g_minishell, g_minishell.tokens) < 0)
			continue ;

		main3(g_minishell, g_minishell.envirement);

		g_minishell.nodes = mk_nodes(g_minishell.tokens);
		// tmp_node = g_minishell.nodes;
		// while (tmp_node)
		// {
		// 	j = 0;
		// 	i = 0;
		// 	printf("----------------------------------------------------------\n");
		// 	while(tmp_node->cmd[j])
		// 	{
		// 		printf("the node \033[32m%d\033[0m cmds n* %d is :\033[32m %s\033[0m\n",
		// 		i, j, tmp_node->cmd[j]);
		// 		j++;
		// 	}
		// 	while(tmp_node->redir)
		// 	{
		// 		printf("the redir file name is: %s\n",
		// 			tmp_node->redir->file);
		// 		printf("the redir type is: %d\n",
		// 			tmp_node->redir->red_type);
		// 		tmp_node->redir = tmp_node->redir->next;
		// 	}
		//  	printf("----------------------------------------------------------\n");
		// 	tmp_node = tmp_node->next_node;
		// 	i++;
		// }
		free_node_list(g_minishell.nodes);
	}
}