/**
 * @file test_CircularBuffer.cpp
 * @brief Demonstrate the CircularBuffer template using a USB MIDI packet class.
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

#include "../CircularBuffer.tpp"
#include "usb_midi_packet.h"
#include <cstdio>

// Make a 3-packet MIDI buffer class
class MidiBuffer3 : public CircularBuffer<Midi_packet, 3>
{
};

int main() {
    MidiBuffer3 buffer;
    Midi_packet packet;
    printf("sizeof(packet)=%lu; number of packets is %lu\r\n", sizeof(packet), buffer.getMaxSize());
    packet.setCin(0x9);
    packet.setCn(0x0);
    packet.setStatus(0x90);
    packet.setData2(0x7f);
    printf("push packets with note value 0-%ld into the buffer\r\n", buffer.getMaxSize()-1);
    for (uint8_t val=0; val < buffer.getMaxSize(); val++) {
        packet.setData1(val);
        if (!buffer.push(packet)) {
            printf("unexpected full buffer\r\n");
            return -1;
        }
        printf("0x%08x pushed\r\n", packet.getPacket());
    }

    packet.setData1(4);

    if (!buffer.push(packet)) {
        printf("pushing one more packet failed as expected\r\n");
    }
    if (!buffer.peek(packet)) {
        printf("peek failed: unexpected empty buffer\r\n");
        return -1;
    }
    printf("first packet value in the buffer is now 0x%08x\r\n", packet.getPacket());

    if (!buffer.peekLast(packet)) {
        printf("peek failed: unexpected empty buffer\r\n");
        return -1;
    }
    printf("last packet value in the buffer is 0x%08x\r\n", packet.getPacket());

    if (!buffer.pop(packet)) {
        printf("unexpected empty buffer\r\n");
        return -1;
    }
    printf("make room by popping first packet value:0x%08x\r\n", packet.getPacket()); // prints 1

    printf("push Note on, note=4, velocity=0x7F\r\n");
    packet.setData1(4);
    buffer.push(packet);

    printf("pop out each packet until the buffer is empty\r\n");
    while (!buffer.isEmpty()) {
        if (!buffer.pop(packet)) {
            printf("unexpected empty buffer\r\n");
            return -1;
        }
        printf("0x%08x\r\n", packet.getPacket());
    }
    if (buffer.isEmpty()) {
        printf("buffer is empty\r\n");
    }

    return 0;
}