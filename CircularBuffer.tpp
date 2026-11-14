/**
 * @file CircularBuffer.tpp
 *
 * This file contains a circular buffer template implmentation adapted from
 * https://en.wikipedia.org/wiki/Circular_buffer. Per the web page,
 * the original content I copied was last edited his page was last edited on 6 September 2026, at 12:25 (UTC). 
 * and is licensed under the Creative Commons Attribution-ShareAlike 4.0 License
 * https://en.wikipedia.org/wiki/Wikipedia:Text_of_the_Creative_Commons_Attribution-ShareAlike_4.0_International_License
 *
 * The template has been adapted for embedded environments that do not support exceptions.
 * Failed method calls return true if successful and false otherwise.
 */
#include <cstddef>
#include <array>
template <typename T /* buffer item type*/, size_t N /* maximum number of items in the buffer */>
class CircularBuffer {
private:
    std::array<T, N> data{};
    size_t head = 0;
    size_t tail = 0;
    size_t size = 0;
public:
    /**
     * @brief clear the buffer
     */
    void reset() {
        head = 0;
        tail = 0;
        size = 0;
    }

    /**
     * @return true if the buffer is empty
     */
    bool isEmpty() const noexcept {
        return size == 0;
    }

    /**
     * @return true if the buffer is full
     */
    bool isFull() const noexcept {
        return size == N;
    }

    /**
     * @return the maximum number of items that fit in the buffer
     */
    size_t getMaxSize() const noexcept {
        return N;
    }

    /**
     * @return the number of items currently in the buffer
     */
    size_t getSize() const noexcept {
        return size;
    }

    /**
     * @brief store a value at the end of the buffer
     *
     * @param value the value to enqueue
     * @return true if value is successfully enqueued
     * @return false if the buffer is full
     */
    bool push(const T& value) {
        if (isFull()) {
            return false;
        }
        data[tail] = value;
        tail = (tail + 1) % N;
        ++size;
        return true;
    }

    /**
     * @brief set value to the front of the buffer and then remove it from the buffer
     *
     * @param value will contain the value retrieved from the buffer if pop() returns true
     * @return false if the buffer is empty
     * @return true if value contains valid data
     */
    bool pop(T& value) {
        if (isEmpty()) {
            return false;
        }
        value = data[head];
        head = (head + 1) % N;
        --size;
        return true;
    }

    /**
     * @brief set value to the front of the buffer
     *
     * @param value will contain the value retrieved from the front of the buffer if peek() returns true
     * @return false if the buffer is empty
     * @return true if value contains valid data
     */
    bool peek(T& value) const {
        if (isEmpty()) {
            return false;
        }
        value = data[head];
        return true;
    }

    /**
     * @brief set value to the end of the buffer
     *
     * @param value will contain the value retrieved from the end of the buffer if peekLast() returns true
     * @return false if the buffer is empty
     * @return true if value contains valid data
     */
    bool peekLast(T& value) const {
        if (isEmpty()) {
            return false;
        }
        size_t idx = (tail == 0) ? (N - 1) : (tail - 1);

        value = data[idx];
        return true;
    }
};

