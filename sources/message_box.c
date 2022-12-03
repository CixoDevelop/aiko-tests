#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <aiko.h>
#include <testlib.h>

/** \fn test_box_create
 * This test message box constructor.
 * @*box_pointer (void *) (message_box_t *) Message box for test
 */
int test_box_create(void *box_pointer) {
    message_box_t *box = box_pointer;

    message_box_create(box);

    if (message_box_is_readable(box)) return 0x01;
    if (!message_box_is_sendable(box)) return 0x02;

    return 0;
}

/** \fn test_box_send
 * This test message box sending.
 * @*box_pointer (void *) (message_box_t *) Message box for test
 */
int test_box_send(void *box_pointer) {
    message_box_t *box = box_pointer;
    
    message_box_create(box);
    message_box_send(box, (void *) (uintptr_t) (20));

    if (message_box_is_sendable(box)) return 0x01;
    if (!message_box_is_readable(box)) return 0x02;

    return 0;
}

/** \fn test_box_read
 * This test message box reading.
 * @*box_pointer (void *) (message_box_t *) Message box for test
 */
int test_box_read(void *box_pointer) {
    message_box_t *box = box_pointer;
    
    message_box_create(box);
    message_box_send(box, (void *) (uintptr_t) (20));

    void *from_test = message_box_receive(box);

    if (from_test != (void *) (uintptr_t) (20)) return 0x01;
    if (message_box_is_readable(box)) return 0x02;
    if (!message_box_is_sendable(box)) return 0x03;

    return 0;
}

/** \fn main
 * This function create test container and run it!
 */
int main() {
    test_t box_test[1];
    message_box_t box[1];

    test_create(box_test);
    
    box_test->name = "message box";

    test_unit_add(box_test, "creating", test_box_create, box);
    test_unit_add(box_test, "sending", test_box_send, box);
    test_unit_add(box_test, "reading", test_box_read, box);

    return test_run(box_test);
}
