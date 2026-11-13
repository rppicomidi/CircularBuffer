/**
 * @file usb_midi_packet.h
 * @brief a C++ abstraction of the 4-byte USB MIDI 1.0 packet
 *
 * A USB MIDI 1.0 packet is 4 bytes long (see https://www.usb.org/sites/default/files/midi10.pdf)
 *     Byte 0           Byte 1           Byte 2           Byte 3
 * +----------------+----------------+----------------+----------------+
 * | CN    | CIN    | Status byte    | Data byte 1    | Data byte 2    |
 * +----------------+----------------+----------------+----------------+
 * Bit 0                                                             Bit 31
 * 
 * CN (Byte 0, Bits 3:0) is the virtual Cable Number 0x0-0xF
 * CIN (Byte 0, Bits 7:4) is the 4-bit Code Index Number 0x0-0xF
 * Note for system exclusive messages, Byte 1, the Status byte, may be
 * re-purposed to be a data byte, and at the end of system exclusive
 * messages, depending on the total message length, either Byte 1, Byte 2,
 * or Byte 3 may contain the 0xF7 EOX status byte.
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2026 rppicomidi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
#pragma once
#include <cstdint>
#include <cstddef>

// Store bytes in network byte order (big-endian)
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define MIDI_PACKET_BYTE0 3
#define MIDI_PACKET_BYTE1 2
#define MIDI_PACKET_BYTE2 1
#define MIDI_PACKET_BYTE3 0
#else
#define MIDI_PACKET_BYTE0 0
#define MIDI_PACKET_BYTE1 1
#define MIDI_PACKET_BYTE2 2
#define MIDI_PACKET_BYTE3 3
#endif
union Midi_packet {
    Midi_packet() : val{0} {}
    void setPacket(int v) { val = v; }
    void setPacket(uint8_t* buffer) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        for (ssize_t idx = sizeof(bytes) - 1; idx >= 0; idx--) {
#else
        for (size_t idx = 0; idx < sizeof(bytes); idx++) {
#endif
            bytes[idx] = buffer[idx];
        }
    }
    uint32_t getPacket() const { return val; }
    uint8_t* getPacketBuffer() {return bytes; }
    void setCinCn(uint8_t cinCn) {bytes[MIDI_PACKET_BYTE0] = cinCn; }
    void setCn(uint8_t cn) {
        bytes[MIDI_PACKET_BYTE0] &= 0xF0;
        bytes[MIDI_PACKET_BYTE0] |= cn;
    }
    void setCin(uint8_t cin) {
        bytes[MIDI_PACKET_BYTE0] &= 0xF;
        bytes[MIDI_PACKET_BYTE0] |= (cin << 4);
    }
    void setStatus(uint8_t status) {bytes[MIDI_PACKET_BYTE1] = status; }
    void setData1(uint8_t data1) {bytes[MIDI_PACKET_BYTE2] = data1;}
    void setData2(uint8_t data2) {bytes[MIDI_PACKET_BYTE3] = data2;}
    uint8_t getCinCn() const { return bytes[MIDI_PACKET_BYTE0]; }
    uint8_t getCn() const {return bytes[MIDI_PACKET_BYTE0] & 0xF;}
    uint8_t getCin() const {return (bytes[MIDI_PACKET_BYTE0] >> 4) & 0xF;}
    uint8_t getStatus() const {return bytes[MIDI_PACKET_BYTE1];}
    uint8_t getData1() const {return bytes[MIDI_PACKET_BYTE2];}
    uint8_t getData2() const {return bytes[MIDI_PACKET_BYTE3];}
    size_t getPacketSize() const { return sizeof(bytes); }
 private:
    uint32_t val;
    uint8_t bytes[4];
};
