/**
 * @file test_CircularBuffer.cpp
 *
 * This implmentation adapted from
 * https://en.wikipedia.org/wiki/Circular_buffer. Per the web page,
 * the original content I copied was last edited his page was last edited on 6 September 2026, at 12:25 (UTC). 
 * and is licensed under the Creative Commons Attribution-ShareAlike 4.0 License
 * https://en.wikipedia.org/wiki/Wikipedia:Text_of_the_Creative_Commons_Attribution-ShareAlike_4.0_International_License
 *
 * The code has been adapted for embedded environments that do not support exceptions.
 * It uses C stdio instead of C++ std::println(). It also tests more functions than
 * the original code.
 *
 * On a system that supports the GNU C++ compiler, compile this with
 *      g++ -o test_CircularBuffer test_CircularBuffer.cpp
 */

 #include "CircularBuffer.tpp"
 #include <cstdio>
 int main() {
    CircularBuffer<int, 3> buffer;
    printf("push values 1-%ld into the buffer\r\n", buffer.getMaxSize());
    for (int val=0; val < buffer.getMaxSize(); val++) {
        if (!buffer.push(val+1)) {
            printf("unexpected full buffer\r\n");
            return -1;
        }
        printf("%d pushed\r\n", val+1);
    }
    if (!buffer.push(4)) {
        printf("pushing one more value failed as expected\r\n");
    }
    int value;
    if (!buffer.peek(value)) {
        printf("peek failed: unexpected empty buffer\r\n");
        return -1;
    }
    printf("first value in the buffer is now %d\r\n", value);

    if (!buffer.peekLast(value)) {
        printf("peek failed: unexpected empty buffer\r\n");
        return -1;
    }
    printf("last value in the buffer is %d\r\n", value);

    if (!buffer.pop(value)) {
        printf("unexpected empty buffer\r\n");
        return -1;
    }
    printf("make room by popping first value:%d\r\n", value); // prints 1

    printf("push 4\r\n");
    buffer.push(4);

    printf("pop out each value until the buffer is empty\r\n");
    while (!buffer.isEmpty()) {
        if (!buffer.pop(value)) {
            printf("unexpected empty buffer\r\n");
            return -1;
        }
        printf("%d\r\n", value);
    }
    if (buffer.isEmpty()) {
        printf("buffer is empty\r\n");
    }
    return 0;
}