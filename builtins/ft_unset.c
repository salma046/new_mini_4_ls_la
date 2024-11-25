#include "../minishell.h"

void removeNode(t_env** head, char *valueToRemove) {
	t_env*	current = *head;
	t_env*	prev = NULL;

	while (current != NULL) {
		if (!ft_strcmp(valueToRemove, current->key))
		{
			if (prev == NULL)
				*head = current->next;
			else
				prev->next = current->next;
			free(current);
			return;
		}
		prev = current;
		current = current->next;
	}
}

// int	check_valid_key(t_env *envir, t_token *tooken)
// {
// 	t_env	*tmp;
// 	int		is_valid;

// 	is_valid = 0;
// 	tmp = envir;
// 	if (data.tokens->next_token == NULL)
// 		return (tmp);
// 	while (tmp)
// 	{
// 		if (tmp->next != NULL && !ft_strcmp(tooken->data, tmp->next->key))
// 		{
// 			is_valid = 1;
// 		}
// 		tmp = tmp->next;
// 	}
// 	if (tooken->data_type == WORD && )
// }

// You need to free it .
t_env	*ft_env_unset(t_minishell data)
{
	t_env	*tmp;
	char	*test;
	// t_env	*to_checkk;
	t_token	*temp_tokens;

	temp_tokens = data.tokens;
	tmp = data.envir;
	if (data.tokens->next_token == NULL)
		return (tmp);
	// while (check_valid_key(temp_tokens) == 1)
	while (tmp)
	{
		test = temp_tokens->next_token->data;
		if (tmp->next != NULL && !ft_strcmp(test, tmp->next->key))
		{
			removeNode(&data.envir, test);
		}
		tmp = tmp->next;
	}
	// to_checkk = data.envir;
	// while (to_checkk)
	// {
	// 	to_checkk->test = temp_tokens->next_token->data;
	// 	// printf("\nJust to check that test is LESS ->: %s\n", tmp->test);
	// 	printf("kkkkkkkkk:%s", to_checkk->key);
	// 	printf("<%c>", to_checkk->equal);
	// 	printf("-->%s\n", to_checkk->value);
	// 	to_checkk = to_checkk->next;
	// }
	return (tmp);
}

void	ft_unset(t_env *env_list, t_minishell data)
{
	(void)data;
	(void)env_list;
	printf("\033[36m-------  I'm in the unset -------\033[0m \n");
	data.envir = ft_env_unset(data);
	printf("\033[0;33m-----------------------------\033[0m\n");
}
