/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expend_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salaoui <salaoui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:58:15 by salaoui           #+#    #+#             */
/*   Updated: 2024/12/18 10:38:43 by salaoui          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*after_dol_word(char *str, int l, int str_len)
{
	int		j;
	char	*word;

	word = malloc(sizeof(char) * str_len - l + 1);
	j = 0;
	while (str[l])
		word[j++] = str[l++];
	word[j] = '\0';
	return (word);
}

int	is_not_valid_expend(char *str, int i)
{
	if (str[i] == '$' && ft_isalnum(str[i + 1]) == 1)
		return (0);
	else
		return (1);
}

int	check_is_num(char *str, int i)
{
	if (str[i] && ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A'
				&& str[i] <= 'Z') || (str[i] >= '0' && str[i] <= '9')
			|| str[i] == 95))
		return (1);
	else
		return (0);
}

int	put_env_in_word(char *str, char *env_var, t_my_struc *norm)
{
	size_t	l;

	l = 0;
	norm->i++;
	while (check_is_num(str, norm->i) == 1)
		norm->i++;
	if (env_var != NULL)
	{
		while (l < ft_strlen(env_var))
			norm->word[norm->j++] = env_var[l++];
		l = norm->i;
		while (str[norm->i])
			norm->i++;
		norm->word[norm->j] = '\0';
	}
	else
	{
		while (str[norm->i])
			norm->word[norm->j++] = str[norm->i++];
		norm->word[norm->j] = '\0';
	}
	return (l);
}

char	*ft_join_words(char *word, char *str, int l)
{
	char	*command_rest;
	char	*hi;

	command_rest = after_dol_word(str, l, ft_strlen(str));
	hi = ft_strjoin(word, command_rest);
	free(command_rest);
	free(word);
	return (hi);
}
