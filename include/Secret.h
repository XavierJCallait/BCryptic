#ifndef SECRET_H
#define SECRET_H

class Secret {

public:
    explicit Secret(size_t n);

    ~Secret();

    Secret(const Secret&) = delete;
    Secret& operator=(const Secret&) = delete;

    Secret(Secret&& other) noexcept;
    Secret& operator=(Secret&& other) noexcept;

    unsigned char *data() { return pointer; }
    const unsigned char *data() const { return pointer; }
    size_t size() const { return data_length; }

private:
    void secure_free();
    unsigned char *pointer;
    size_t data_length;
};

#endif