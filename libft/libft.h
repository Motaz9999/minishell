/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moodeh <moodeh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 18:04:10 by moodeh            #+#    #+#             */
/*   Updated: 2026/02/26 15:12:42 by moodeh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

# include <stdarg.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

size_t				ft_strlen(const char *str);
size_t				ft_strlcpy(char *dst, const char *src, size_t size);
size_t				ft_strlcat(char *dst, const char *src, size_t size);
int					ft_strncmp(const char *s1, const char *s2, size_t size);
int					ft_atoi(const char *str);
int					ft_isalpha(int c);
int					ft_isprint(int arg);
int					ft_isdigit(int dig);
int					ft_isalnum(int arg);
int					ft_toupper(int ch);
int					ft_tolower(int ch);
void				*ft_memset(void *s, int value, size_t num);
void				ft_bzero(void *s, size_t n);
void				*ft_memcpy(void *dest, const void *src, size_t size);
void				*ft_memchr(const void *str, int c, size_t n);
int					ft_memcmp(const void *s1, const void *s2, size_t n);
char				*ft_strnstr(const char *haystack, const char *needle,
						size_t n);
char				*ft_strchr(const char *s, int c);
char				*ft_strrchr(const char *s, int c);
void				*ft_memmove(void *dest, const void *src, size_t n);
int					ft_isascii(int asc);
void				*ft_calloc(size_t count, size_t size);
char				*ft_strdup(const char *s1);
char				*ft_substr(char const *s, unsigned int start, size_t len);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_strtrim(char const *s1, char const *set);
char				**ft_split(char const *s, char c);
char				*ft_itoa(int n);
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void				ft_striteri(char *s, void (*f)(unsigned int, char *));
void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char *s, int fd);
void				ft_putendl_fd(char *s, int fd);
void				ft_putnbr_fd(int n, int fd);
int					ft_strcmp(const char *s1, const char *s2);
t_list				*ft_lstnew(void *content);
int					ft_lstsize(t_list *lst);
t_list				*ft_lstlast(t_list *lst);
void				ft_lstadd_front(t_list **alst, t_list *new);
void				ft_lstadd_back(t_list **alst, t_list *new);
void				ft_lstdelone(t_list *lst, void (*del)(void *));
void				ft_lstclear(t_list **lst, void (*del)(void *));
void				ft_lstiter(t_list *lst, void (*f)(void *));
t_list				*ft_lstmap(t_list *lst, void *(*f)(void *),
						void (*del)(void *));

int					ft_put_hexa(unsigned int num, char form, int fd);
int					ft_putnbr_len(int n, int fd);
int					nbr_len(int n);
int					ft_putchar_len(char c, int fd);
int					ft_putstr_len(char *s, int fd);
size_t				ft_strlen(const char *str);
int					ft_printf(const char *format, ...);

char				*ft_strchr(const char *s, int c);
char				*ft_strdup(const char *s);
size_t				ft_strlen(const char *str);
size_t				ft_strlcpy(char *dest, const char *src, size_t size);
void				ft_free_all(void *first, ...);
void				ft_free_all2(void **first, ...);

/*
** t_lst_ops — teach the generic functions about YOUR node type
**
**   next_offset : offsetof(your_type, next)
**   dup         : malloc a new node, deep copy all fields, set next=NULL
**   del         : free all inner pointers (key, value...) but NOT the node
**                 pass NULL if your node has no inner pointers to free
*/

typedef struct s_lst_ops
{
	size_t			next_offset;
	void			*(*dup)(void *node);
	void			(*del)(void *node);
}					t_lst_ops;

void				*ft_lstcopy_generic(void *head, t_lst_ops ops);
void				ft_lstfree_generic(void *head, t_lst_ops ops);

#endif