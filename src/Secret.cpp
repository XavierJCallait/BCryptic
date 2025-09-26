#include <sodium.h>
#include <stdexcept>

#include "Secret.h"

Secret::Secret(size_t n) : pointer(nullptr), data_length(n) {
    pointer = static_cast<unsigned char *> (sodium_malloc(n));
    if (!pointer) {
        throw std::bad_alloc();
    }
}

Secret::~Secret() {
    secure_free();
}

Secret::Secret(Secret&& other) noexcept : pointer(other.pointer), data_length(other.data_length) {
    other.pointer = nullptr;
    other.data_length = 0;
}

Secret& Secret::operator=(Secret&& other) noexcept {
    if (this != &other) {
        secure_free();
        pointer = other.pointer;
        data_length = other.data_length;
        other.pointer = nullptr;
        other.data_length = 0;
    }
    return *this;
}

void Secret::secure_free() {
    if (pointer) {
        sodium_memzero(pointer, data_length);
        sodium_free(pointer);
        pointer = nullptr;
        data_length = 0;
    }
};