#include "ft_ssl.h"

/// @brief Increases the buffer capacity if necessary.
/// Grows the buffer until it can hold at least the requested number of
/// bytes. The buffer capacity is doubled at each step.
/// @param buffer Buffer to resize.
/// @param needed Minimum required capacity in bytes.
/// @return 0 on success, 1 if a memory allocation fails.
static int grow_buffer(t_buffer *buffer, size_t needed)
{
    size_t          new_capacity;

    new_capacity = buffer->capacity;
    if (new_capacity == 0)
        new_capacity = CHUNK_SIZE;
    while (new_capacity < needed)
        new_capacity *= 2;
    buffer->data = ft_realloc(buffer->data, buffer->len, new_capacity);
    if (!buffer->data)
        return 1;
    buffer->capacity = new_capacity;
    return 0;
}

/// @brief Reads the entire contents of a file descriptor.
/// Continuously reads data from the given file descriptor until EOF is
/// reached, growing the buffer as needed.
/// @param fd File descriptor to read from.
/// @param buffer Buffer receiving the read data.
/// @return 0 on success, 1 if a read or allocation error occurs.
int read_fd(int fd, t_buffer *buffer)
{
    ssize_t n;

    buffer->data = NULL;
    buffer->len = 0;
    buffer->capacity = 0;

    while (1)
    {
        if (buffer->len + CHUNK_SIZE > buffer->capacity)
        {
            if (grow_buffer(buffer, buffer->len + CHUNK_SIZE))
                return 1;
        }
        n = read(fd, buffer->data + buffer->len, CHUNK_SIZE);
        if (n < 0)
            return 1;
        if (n == 0)
            break ;
        buffer->len += (size_t)n;
    }
    // if (buffer->data[buffer->len - 1] == '\n')
    //     buffer->data[buffer->len - 1] = '\0'; // risk to fail hash, we'll see later
    return 0;
}

/// @brief Reads the data associated with an input.
/// Depending on the input type, either duplicates a string, reads from
/// standard input, or opens and reads a file into the buffer.
/// @param input Input to read.
/// @param buffer Buffer receiving the input data.
/// @return 0 on success, 1 if an error occurs.
int read_input(t_input *input, t_buffer *buffer)
{
    int fd;
    int ret;

    if (input->type == INPUT_STRING)
    {
        buffer->len = ft_strlen(input->value);
        buffer->capacity = buffer->len;
        buffer->data = malloc(buffer->len);
        if (!buffer->data)
            return error_malloc();
        ft_memcpy(buffer->data, input->value, buffer->len);
        return 0;
    }
    
    if (input->type == INPUT_STDIN)
        return read_fd(0, buffer);
    fd = open(input->value, O_RDONLY);
    if (fd < 0)
        return error_open(input->value);
    ret = read_fd(fd, buffer);
    close(fd);
    if (ret)
        return error_read(input->value);
    return 0;
}

/// @brief Releases the resources owned by a buffer.
/// Frees the allocated memory and resets the buffer members.
/// @param buffer Buffer to free.
void free_buffer(t_buffer *buffer)
{
    if (buffer->data)
        free(buffer->data);
    buffer->data = NULL;
    buffer->len = 0;
    buffer->capacity = 0;
}