#include "ft_ssl.h"

/// @brief Creates a new input node.
/// Allocates a new t_input structure, duplicates the provided value,
/// and initializes all members.
/// @param type Type of the input (stdin, string, or file).
/// @param value Value associated with the input.
/// @return A pointer to the newly created node, or NULL if a memory
/// allocation fails.
t_input *new_input(t_input_type type, const char *value)
{
	t_input *new;
	
	new = malloc(sizeof(t_input));
	if (!new)
		return NULL;
	if (type == INPUT_STDIN)
		new->value = NULL;
	else
	{
		new->value = ft_strdup(value);
		if (!new->value)
		{
			free(new);
			return (NULL);
		}
	}
	new->type = type;
	new->next = NULL;
	return new;
}

/// @brief Appends an input node to the end of the input list.
/// If the list is empty, the new node becomes the head of the list.
/// Otherwise, it is linked to the end of the existing list.
/// @param options Structure containing the input list.
/// @param new Input node to append.
void add_input(t_options *options, t_input *new)
{
	t_input *current;

	if (!options->inputs)
	{
		options->inputs = new;
		return;
	}
	current = options->inputs;
	while (current->next)
		current = current->next;
	current->next = new;
}

/// @brief Frees an entire input list.
/// Frees each node's associated value, then releases the node itself.
/// @param inputs Head of the input list to free.
void free_inputs(t_input *inputs)
{
	t_input *tmp;

	while (inputs)
	{
		tmp = inputs->next;
		free(inputs->value);
		free(inputs);
		inputs = tmp;
	}
}