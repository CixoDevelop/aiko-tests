#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <aiko.h>

/** \fn print_process 
 * This process receiving string from message box, string would be
 * allocated by malloc, and print it on screen.
 */
void print_process(
    kernel_instance_t *kernel, 
    kernel_pid_t pid, 
    message_box_t *message, 
    void *parameter
) {
    if (!message_box_is_readable(message)) return;

    char *received = message_box_receive(message);

    printf("From print process: ");
    printf("%s", received);
    printf("\n");

    free (received);
}

/** \fn send_process
 * This process would load data from keyboard and send it to printing process.
 */
void send_process(
    kernel_instance_t *kernel,
    kernel_pid_t pid,
    message_box_t *message,
    void *parameter
) {
    char *typed = malloc(30 * sizeof(char));

    printf("Type text to print by printing process: ");
    scanf("%29s",  typed);
    kernel_process_message_box_send(kernel, 0x00, typed);
}

/** \fn main
 * This function create kernel, two process, and run it.
 */
int main() {
    kernel_instance_t kernel[1];

    kernel_create(kernel, 2);
    kernel_create_process(kernel, 0x00, REACTIVE, print_process, NULL);
    kernel_create_process(kernel, 0x01, CONTINUOUS, send_process, NULL);

    kernel_scheduler(kernel);

    return 0x00;
}

