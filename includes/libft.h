/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guphilip <guphilip@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:07:07 by guphilip          #+#    #+#             */
/*   Updated: 2025/05/21 15:26:32 by guphilip         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <ctype.h>
# include <limits.h>
# include <stdarg.h>
# include <stdint.h>
# include <stdbool.h>

# define RESET "\033[0m"
# define BOLD_YELLOW "\033[1;33m"
# define BOLD_MAGENTA "\033[1;35m"
# define BOLD_RED "\033[1;31m"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 100
# endif

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

// LIBFT
size_t	ft_strspn(const char *str, const char *accept);

void	ft_putchar(char c);

int		ft_isalpha(int c);

int		ft_isdigit(int c);

int		ft_isalnum(int c);

int		ft_isascii(int c);

int		ft_isprint(int c);

int 	ft_isspace(char c);

int		ft_str_is_whitespace(char *str);

size_t	ft_strlen(const char *s);

void	*ft_memset(void *s, int c, size_t n);

void	ft_bzero(void *s, size_t n);

void	*ft_memcpy(void *dest, const void *src, size_t n);

void	*ft_memmove(void *dest, const void *src, size_t n);

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);

size_t	ft_strlcpy(char *dest, const char *src, size_t size);

size_t	ft_strlcat(char *dest, const char *src, size_t size);

int		ft_toupper(int c);

int		ft_tolower(int c);

char	*ft_strchr(const char *s, int c);

char	*ft_strrchr(const char *s, int c);

char	*ft_strdup(const char *s);

int		ft_strncmp(const char *s1, const char *s2, size_t n);

void	*ft_memchr(const void *s, int c, size_t n);

int		ft_memcmp(const void *s1, const void *s2, size_t n);

char	*ft_strnstr(const char *str, const char *to_find, size_t n);

long	ft_strtol(const char *nptr, char **endptr, int base);

int		ft_atoi(const char *nptr);

void	*ft_calloc(size_t nmemb, size_t size);

char	*ft_strdup(const char *s);

char	*ft_substr(char const *s, unsigned int start, size_t len);

char	*ft_strjoin(char const *s1, char const *s2);

char	*free_join(char *s1, char *s2, bool free_s1, bool free_s2);

char	*ft_strtrim(char const *s1, char const *set);

char	**ft_split(char const *s, char c);

char	**split_free(char *s, char c, bool free_s);

void	free_double_tab(char **split);

int		ft_strcmp(char *s1, char *s2);

void	my_strcat(char **dst_ptr, const char *src);

char	*ft_itoa(int n);

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));

void	ft_striteri(char *s, void (*f)(unsigned int, char *));

void	ft_putchar_fd(char c, int fd);

void	ft_putendl_fd(char *s, int fd);

void	ft_putnbr_fd(int n, int fd);

void	ft_putstr_fd(char *s, int fd);

t_list	*ft_lstnew(void *content);

void	ft_lstadd_front(t_list **alst, t_list *new);

int		ft_lstsize(t_list *lst);

t_list	*ft_lstlast(t_list *lst);

void	ft_lstadd_back(t_list **lst, t_list *new);

void	ft_lstdelone(t_list *lst, void (*del)(void *));

void	ft_lstclear(t_list **lst, void (*del)(void *));

void	ft_lstiter(t_list *lst, void (*f)(void *));

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

// PRINTF

# define L_HEX_CHARSET "0123456789abcdef"
# define U_HEX_CHARSET "0123456789ABCDEF"

int		ft_print_c(int fd, char c);

int		ft_printf(const char *str, ...);

int		ft_print_s(int fd, char *s);

int		ft_print_p(int fd, uintptr_t ptr);

int		check_base(char *base);

int		ft_print_d(int fd, int n);

int		ft_print_i(int fd, int n);

int		ft_print_u(int fd, unsigned int n);

int		ft_print_x(int fd, unsigned int n, int format);

int		fd_printf(int fd, const char *str, ...);

// GET_NEXT_LINE

char	*get_next_line(int fd);

char	*ft_strdup(const char *s);

size_t	ft_strlen(const char *s);

char	*ft_substr(char const *s, unsigned int start, size_t len);

char	*ft_strjoin(char const *s1, char const *s2);

#endif
