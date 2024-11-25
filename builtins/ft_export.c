#include "../minishell.h"

void	sort_env(t_senv *node)
{
	t_senv	*next_node;
	char	*tmp_key;
	char	*tmp_value;

	if (node == NULL || node->next == NULL)
		return ;
	next_node = node->next;
	while (next_node)
	{
		if (ft_strcmp(node->skey, next_node->skey) > 0)
		{
			tmp_key = node->skey;
			node->skey = next_node->skey;
			next_node->skey = tmp_key;
			tmp_value = node->svalue;
			node->svalue = next_node->svalue;
			next_node->svalue = tmp_value;
		}
		next_node = next_node->next;
	}
	sort_env(node->next);
}
int is_special_char(char c)
{
        return (c >= 33 && c <= 47) ||  
           (c >= 58 && c <= 64) ||  
           (c >= 91 && c <= 96) ||  
           (c >= 123 && c <= 126);}

int has_doubled_special_chars(char *token)
{
    if (!token)
        return 0;

    int i = 0;
    while (token[i] != '\0' && token[i] != '=')
    {
        if (token[i + 1] != '\0' && 
            is_special_char(token[i]) && 
            is_special_char(token[i + 1]))
        {
            return 1;  
        }
        i++;
    }
    return 0;  
}


int ft_check(char **env, char *key, char *value)
{
    int i = 0;
    size_t key_len;
    
    if (!env || !key || !value)
        return 0;
    
    key_len = ft_strlen(key);
    
    while (env[i] != NULL)
    {
        if (ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
        {
            free(env[i]);
            size_t new_len = key_len + ft_strlen(value) + 2;  
            env[i] = malloc(new_len);
            if (!env[i])
            {
                perror("malloc");
                return -1;  
            }
            
            ft_strlcpy(env[i], key, new_len);
            ft_strlcat(env[i], "=", new_len);
            ft_strlcat(env[i], value, new_len);
            return 1;  
        }
        i++;
    }
    
    return 0; 
}

void	ft_env_export_once(t_token *data, char **env, int active)
{
	t_senv	*head;
	t_senv	*last_node;
	int		i;
	t_senv	*cmd_env;
	char	*equal_env;
	t_senv	*current;

	(void)data;
	head = NULL;
	last_node = NULL;
	i = 0;
	while (env[i] != NULL)
	{
		cmd_env = (t_senv *)malloc(sizeof(t_senv));
		if (!cmd_env)
			exit(1);
		equal_env = strchr(env[i], '=');
		if (equal_env != NULL)
		{
			cmd_env->skey = strndup(env[i], equal_env- env[i]);
			cmd_env->svalue = strndup(equal_env + 1, strlen(env[i]+ 1));
=======
			cmd_env->skey = strndup(env[i], equal_env - env[i]);
			cmd_env->svalue = strndup(equal_env + 1, strlen(env[i] + 1));
			cmd_env->sequal = '=';
			cmd_env->next = NULL;
		}
		if (last_node == NULL)
			head = cmd_env;
		else
			last_node->next = cmd_env;
		last_node = cmd_env;
		i++;
	}

	if (active == 1)
		sort_env(head);
	current = head;

	while (current)
	{
		if (active == 1)
			printf("declare -x %s=%s\n", current->skey, current->svalue);
		current = current->next;
	}
}

void	ft_adding_in_export_once(t_senv *head, int active)
{
	t_senv *current = head;

	while (current) 
	{
		if (current->svalue)
			printf("declare -x %s=\"%s\"\n", current->skey, current->svalue);
		else if (active == 1 || active == 0)
			printf("\033[1;35mdeclare -x %s\033[0m\n", current->skey);
		current = current->next;
	}
}
void key_without_equal(t_token *data, char **env, int active) {
    t_senv *head = NULL;
    t_senv *last_node = NULL;

    int i = 0;
    (void)active;
    while (env[i] != NULL) {
        t_senv *new_node = (t_senv *)malloc(sizeof(t_senv));
        if (!new_node)
            exit(1);

        char *key_value = env[i];
        new_node->skey = ft_strdup(key_value); 
        new_node->svalue = NULL;              
        new_node->next = NULL;

        if (last_node == NULL)
            head = new_node;
        else
            last_node->next = new_node;
        last_node = new_node;
        i++;
    }
   
    while (data && data->next_token) {
        t_senv *new_export = (t_senv *)malloc(sizeof(t_senv));
        if (!new_export)
            exit(1);

        new_export->skey = ft_strdup(data->next_token->data);
        new_export->svalue = NULL;
        new_export->next = NULL;

        if (last_node == NULL)
            head = new_export;
        else
            last_node->next = new_export;
        last_node = new_export;

        data = data->next_token; 
    }
    ft_adding_in_export_once(head, active);
   
    while (head) {
        t_senv *tmp = head;
        head = head->next;
        free(tmp->skey);
        free(tmp->svalue);
        free(tmp);
    }
}

 
void ft_add_to_export_arg(t_token *tokens, char **env)
{
    t_token *current_token;
    t_env   *expo;
    char    *token_data;
    char    *splitVar;
    size_t  len;
    char    *new_env_str;
    int     i;
    
    if (!tokens || !tokens->next_token || !env)
        return;
    
    current_token = tokens->next_token;
    while (current_token != NULL)
    {
        token_data = current_token->data;
        
        if (!token_data)
        {
            current_token = current_token->next_token;
            continue;
        }
        
        if (ft_strcmp(token_data, "=") == 0 ||
            token_data[0] == '=' ||
            ft_isdigit(token_data[0]) ||
            is_special_char(token_data[0]) ||
            has_doubled_special_chars(token_data))
        {
            printf("bash: export: `%s': not a valid identifier\n", token_data);
            current_token = current_token->next_token;
            continue;
        }

        i = 0;
        while (token_data[i] != '\0' && token_data[i] != '=')
        {
            if (is_special_char(token_data[i]))
            {
                printf("bash: export: `%s': not a valid identifier\n", token_data);
                current_token = current_token->next_token;
                break;
            }
            i++;
        }

        if (token_data[i] != '\0' && token_data[i] != '=')
            continue;
        
        expo = malloc(sizeof(t_env));
        if (!expo)
        {
            perror("malloc");
            return;
        }
        
        splitVar = ft_strchr(token_data, '=');
        if (!splitVar)
        {
           key_without_equal(tokens, env, 0);
        //    printf("%s", splitVar);
            free(expo);
            current_token = current_token->next_token;
            continue;
        }
        printf("\033[1;35mthis is expo: %s\033[0m", splitVar);
        expo->key = ft_strndup(token_data, splitVar - token_data);
        expo->value = ft_strdup(splitVar + 1);
        
        if (!expo->key || !expo->value)
        {
            free(expo->key);
            free(expo->value);
            free(expo);
            current_token = current_token->next_token;
            continue;
        }
        
        int check_result = ft_check(env, expo->key, expo->value);
        
        if (check_result == 0)
        {
            i = 0;
            while (env[i] != NULL)
                i++;
            
            len = ft_strlen(expo->key) + ft_strlen(expo->value) + 2;
            new_env_str = malloc(len);
            if (!new_env_str)
            {
                free(expo->key);
                free(expo->value);
                free(expo);
                current_token = current_token->next_token;
                continue;
            }
            
            ft_strlcpy(new_env_str, expo->key, len);
            ft_strlcat(new_env_str, "=", len);
            ft_strlcat(new_env_str, expo->value, len);
            
            env[i] = new_env_str;
            env[i + 1] = NULL;
        }
        
        free(expo->key);
        free(expo->value);
        free(expo);
        
        current_token = current_token->next_token;
    }
=======
int	ft_check(char **env, char *key, char *value, int active)
{
	int		i;
	size_t	key_len;
	size_t	new_len;

	i = 0;
	active = 0;
	if (!env || !key || !value)
		return (0);
	key_len = ft_strlen(key);
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
		{
			active = 1;
			free(env[i]);
			new_len = key_len + ft_strlen(value) + 2;
			env[i] = malloc(new_len);
			if (!env[i])
			{
				perror("malloc");
				return (1);
			}
			ft_strlcpy(env[i], key, new_len);
			ft_strlcat(env[i], "=", new_len);
			ft_strlcat(env[i], value, new_len);
			return (0);
		}
		i++;
	}
	return (0);
}
void	ft_add_to_export_arg(t_token *tokens, char **env)
{
	t_token	*current_token;
	t_env	*expo;
	char	*token_data;
	char	*splitVar;
	size_t	len;
	char	*new_env_str;
	int		i;
	int		active;

	active = 0;
	if (!tokens || !tokens->next_token || !env)
		return ;
	current_token = tokens->next_token;
	while (current_token != NULL)
	{
		token_data = current_token->data;
		if (ft_strcmp(token_data, "=") == 0)
		{
			printf("bash: export: `%s': not a valid identifier\n", token_data);
			current_token = current_token->next_token;
			continue ;
		}
		if (token_data[0] == '=')
		{
			printf("bash: export: `%s': not a valid identifier\n", token_data);
			current_token = current_token->next_token;
			continue ;
		}
		if (ft_isdigit(token_data[0]))
		{
			printf("bash: export: `%s': not a valid identifier\n", token_data);
			current_token = current_token->next_token;
			continue ;
		}
		expo = malloc(sizeof(t_env));
		if (!expo)
		{
			perror("malloc");
			return ;
		}
		splitVar = ft_strchr(token_data, '=');
		if (!splitVar)
		{
			free(expo);
			current_token = current_token->next_token;
			continue ;
		}
		expo->key = ft_strndup(token_data, splitVar - token_data);
		expo->value = ft_strdup(splitVar + 1);
		if (!expo->key || !expo->value)
		{
			free(expo->key);
			free(expo->value);
			free(expo);
			current_token = current_token->next_token;
			continue ;
		}
		i = 0;
		while (env[i] != NULL)
			i++;
		len = ft_strlen(expo->key) + ft_strlen(expo->value) + 2;
		new_env_str = malloc(len);
		if (!new_env_str)
		{
			free(expo->key);
			free(expo->value);
			free(expo);
			current_token = current_token->next_token;
			continue ;
		}
		ft_strlcpy(new_env_str, expo->key, len);
		ft_strlcat(new_env_str, "=", len);
		ft_strlcat(new_env_str, expo->value, len);
		if (!ft_check(env, expo->key, expo->value, active))
		{
			printf("\033[0;33m-------->HAdi raha %d<----------\033[0m", active);
		}
		env[i] = new_env_str;
		env[i + 1] = NULL;
		free(expo->key);
		free(expo->value);
		free(expo);
		current_token = current_token->next_token;
	}
}

void	ft_export(t_token *tokens, char **env)
{
    int active = 0;
	
    if (!tokens || !env) 
        return;
	
    active = 1;
    if ((tokens->next_token == NULL && tokens->data)) 
    {
        // ft_env_export_once(tokens, env, active);
        printf("--");
    }
    else
    {
        active = 0;
        ft_add_to_export_arg(tokens, env);
        ft_env_export_once(tokens, env, active);
    }
    
}