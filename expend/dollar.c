/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salaoui <salaoui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 14:36:50 by salaoui           #+#    #+#             */
/*   Updated: 2024/12/18 00:10:49 by salaoui          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_dollar_lenth(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i] != '\0')
	{
		while (str[i] && str[i] != '\'' && is_not_valid_expend(str, i) == 1)
			i++;
		while (str[i] == '\'')
			skip_quo(str, &i, '\'');
		if (str[i] == '"')
			skip_2quo(str, &i, '"');
		while (str[i] && str[i] != '\'' && str[i] != '"' && str[i + 1] != '$'
			&& is_not_valid_expend(str, i) == 1)
			i++;
		if (str[i] == '$' && str[i + 1] == '$')
		{
			while (str[i + 1] == '$')
				i += 2;
		}
		if (str[i] == '$' && str[i + 1] != '$' && ft_isalnum(str[i + 1]) == 1)
			j = count_lenth2(str, &i);
	}
	return (j);
}

char	*remp_with_null(char *str)
{
	int		i;
	int		j;
	char	*word;

	word = allocate_4_nword(str, NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		fill_word_sgl_quot(word, str, &i, &j);
		if (str[i] != '\'' && str[i] != '"' && str[i] != '$' && str[i
				+ 1] == '$')
			word[j++] = str[i++];
		if (str[i] == '$' && str[i + 1] == '$')
			skip_double_signs(str, &i);
		if (valid_word(str, i) == 1)
			put_env_null_word(str, word, &i, &j);
	}
	word[j] = '\0';
	return (word);
}

char	*remp_with_value(char *str, char *env_var)
{
	t_my_struc	norm;
	size_t		l;
	char		*hi;

	norm.word = allocate_4_nword(str, env_var);
	norm.i = 0;
	norm.j = 0;
	while (str[norm.i])
	{
		fill_word_sgl_quot(norm.word, str, &norm.i, &norm.j);
		if (str[norm.i] != '\'' && str[norm.i] != '"'
			&& str[norm.i] != '$' && str[norm.i
				+ 1] == '$')
			norm.word[norm.j++] = str[norm.i++];
		if (str[norm.i] == '$' && str[norm.i + 1] == '$')
			skip_double_signs(str, &norm.i);
		if (valid_word(str, norm.i) == 1)
			l = put_env_in_word(str, env_var, &norm);
		else if (str[norm.i] && str[norm.i] != '$'
			&& str[norm.i - 1] != '$' && str[norm.i] != '"'
			&& str[norm.i] != '\'')
			norm.i++;
	}
	hi = ft_join_words(norm.word, str, l);
	return (hi);
}

char	*remplace_doll_str(char *data, char *env_var)
{
	char	*word;

	if (env_var == NULL)
		word = remp_with_null(data);
	else
		word = remp_with_value(data, env_var);
	free(data);
	return (word);
}

char	*ft_getenv(char *key, t_env *envir)
{
	t_env	*tmp;

	tmp = envir;
	while (tmp)
	{
		if (!strcmp(key, tmp->key))
		{
			return (tmp->value);
		}
		tmp = tmp->next;
	}
	return (NULL);
}
