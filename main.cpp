#include <algorithm>
#include <chrono>
#include <cstdio>
#include <functional>
#include <future>
#include <iostream>
#include <vector>

using namespace std;

class later {
public:
    template <class callable, class... arguments>
    later(int after, bool async, callable&& f, arguments&& ... args)
    {
        std::function<typename std::result_of<callable(arguments...)>::type()> task(
            std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));
        if(async) {
            std::thread([after, task]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(after));
                task();
            }).detach();
        }
        else {
            std::this_thread::sleep_for(std::chrono::milliseconds(after));
            task();
        }
    }
};

void test1(void) {}

// void test2(void)
// {}

// void test2(int a)
// {
//  printf("%i\n", a);
// }

class MemoryBlock {
public:
    // Simple constructor that initializes the resource.
    explicit MemoryBlock(size_t length)
        : _length(length), _data(new int[length])
    {
        std::cout << "In MemoryBlock(size_t). length = " << _length << "."
                  << std::endl;
    }

    // Destructor.
    ~MemoryBlock()
    {
        std::cout << "In ~MemoryBlock(). length = " << _length << ".";
        if(_data != nullptr) {
            std::cout << " Deleting resource.";
            // Delete the resource.
            delete[] _data;
        }
        std::cout << std::endl;
    }

    // Copy constructor.
    MemoryBlock(const MemoryBlock& other)
        : _length(other._length), _data(new int[other._length])
    {
        std::cout << "In MemoryBlock(const MemoryBlock&). length = "
                  << other._length << ". Copying resource." << std::endl;
        std::copy(other._data, other._data + _length, _data);
    }

    // Copy assignment operator.
    MemoryBlock& operator=(const MemoryBlock& other)
    {
        std::cout << "In operator=(const MemoryBlock&). length = " << other._length
                  << ". Copying resource." << std::endl;
        if(this != &other) {
            // Free the existing resource.
            delete[] _data;
            _length = other._length;
            _data = new int[_length];
            std::copy(other._data, other._data + _length, _data);
        }
        return *this;
    }

    // Retrieves the length of the data resource.
    size_t Length() const
    {
        return _length;
    }

private:
    size_t _length;  // The length of the resource.
    int* _data;      // The resource.
};

int main()
{
    for(auto i = 0; i < 1; i++) {
        printf("Move constructor example [c++11] \n");
        printf("\n");
        // Create a vector object and add a few elements to it.
        vector<MemoryBlock> v;
        v.push_back(MemoryBlock(25000000));
        printf("\n");
        v.insert(v.begin() + 1, MemoryBlock(50000000));
        printf("\n");
        later later_test1(100, false, &test1);
        printf("\n");
        v.push_back(MemoryBlock(75000000));
        //    Insert a new element into the second position of the vector.
        //    later later_test2(100, false, &test2);
    }
    cout << "Complete." << endl;
    printf("\n");
    return 0;
}
