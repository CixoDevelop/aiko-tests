#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <aiko.h>
#include <testlib.h>

/** \fn test_kernel_create
 * This test check kernel constructor.
 * @*kernel (void *) (kernel_instance_t *) Kernel instance to work on
 */
int test_kernel_create(void *kernel_pointer) {
    kernel_instance_t *kernel = kernel_pointer;

    kernel_create(kernel, 10);

    if (kernel->size != 10) return 0x01;
    if (kernel->last_changed != ERROR_PID) return 0x02;
    if (kernel->signal != (void *) (uintptr_t) (0x00)) return 0x03;
    
    for (kernel_pid_t count = 0; count < kernel->size; ++count) {
        if ((kernel->processes + count)->type != EMPTY) return 0x04;
    }

    kernel_remove(kernel);

    return 0x00;
}

/** \fn test_process_add
 * This test check kernel can create process.
 * @*kernel (void *) (kernel_instance_t *) Kernel instance to work on
 */
int test_process_add(void *kernel_pointer) {
    kernel_instance_t *kernel = kernel_pointer;

    kernel_create(kernel, 10);
    kernel_create_process(
        kernel, 
        0, 
        REACTIVE, 
        (void (*) (
            kernel_instance_t *, 
            kernel_pid_t, 
            message_box_t *, 
            void *)
        ) (uintptr_t) (0x20), 
        (void *) (uintptr_t) (0x40)
    );

    if ((kernel->processes + 0x00)->type != REACTIVE) return 0x01;
    if ((kernel->processes + 0x00)->worker != 
        (void (*) (void *, uint_t, message_box_t *, void *)) (uintptr_t) (0x20)
    ) {
        return 0x02;
    }
    if ((kernel->processes + 0x00)->parameter != (void *) (uintptr_t) (0x40)) {
        return 0x03;
    }
    
    kernel_remove(kernel);

    return 0x00;
}

/** \fn test_process_kill
 * This test check kernel can kill process.
 * @*kernel (void *) (kernel_instance_t *) Kernel instance to work on
 */
int test_process_kill(void *kernel_pointer) {
    kernel_instance_t *kernel = kernel_pointer;

    kernel_create(kernel, 10);
    kernel_create_process(
        kernel, 
        0, 
        REACTIVE, 
        (void (*) (
            kernel_instance_t *, 
            kernel_pid_t, 
            message_box_t *, 
            void *)
        ) (uintptr_t) (0x20), 
        (void *) (uintptr_t) (0x40)
    );
    kernel_kill_process(kernel, 0);

    if ((kernel->processes + 0x00)->type != EMPTY) return 0x01;
    
    kernel_remove(kernel);

    return 0x00;
}

/** \fn test_process_message
 * This test check kernel can working with messages.
 * @*kernel (void *) (kernel_instance_t *) Kernel instance to work on
 */
int test_process_message(void *kernel_pointer) {
    kernel_instance_t *kernel = kernel_pointer;

    kernel_create(kernel, 10);
    kernel_create_process(
        kernel, 
        0, 
        REACTIVE, 
        (void (*) (
            kernel_instance_t *, 
            kernel_pid_t, 
            message_box_t *, 
            void *)
        ) (uintptr_t) (0x20), 
        (void *) (uintptr_t) (0x40)
    );

    if (!kernel_is_process_message_box_sendable(kernel, 0)) return 0x01;
    
    kernel_process_message_box_send(kernel, 0, (void *) (uintptr_t) (0x50));

    if (kernel_is_process_message_box_sendable(kernel, 0)) return 0x02;
    if (
        message_box_receive((kernel->processes + 0x00)->message) 
        != (void *) (uintptr_t) (0x50)
    ) return 0x03;
    
    kernel_remove(kernel);

    return 0x00;
}

/** \fn test_kernel_signal
 * This test check kernel signals making and receiving.
 * @*kernel (void *) (kernel_instance_t *) Kernel instance to work on
 */
int test_kernel_signal(void *kernel_pointer) {
    kernel_instance_t *kernel = kernel_pointer;

    kernel_create(kernel, 10);
    kernel_trigger_signal(kernel, 0x20);
    kernel_create_process(
        kernel, 
        0, 
        SIGNAL, 
        (void (*) (
            kernel_instance_t *, 
            kernel_pid_t,
            message_box_t *, 
            void *)
        ) (uintptr_t) (0x20), 
        (void *) (uintptr_t) (0x40)
    );

    if ((kernel->processes + 0x00)->type != SIGNAL) return 0x01;
    if (kernel->signal != kernel_generate_signal_parameter(0x20)) return 0x02;

    kernel_remove(kernel);

    return 0x00;
}

/** \fn main
 * This function create test container and run it!
 */
int main() {
    test_t kernel_test[1];
    kernel_instance_t kernel[1];

    test_create(kernel_test);

    kernel_test->name = "kernel";

    test_unit_add(kernel_test, "creating", test_kernel_create, kernel);
    test_unit_add(kernel_test, "adding", test_process_add, kernel);
    test_unit_add(kernel_test, "killing", test_process_kill, kernel);
    test_unit_add(kernel_test, "sending", test_process_message, kernel);
    test_unit_add(kernel_test, "signalling", test_kernel_signal, kernel);
    
    return test_run(kernel_test);
}
