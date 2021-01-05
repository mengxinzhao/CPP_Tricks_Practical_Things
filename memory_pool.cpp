#include <iomanip>
#include <iostream>
#include <memory>

// compile with g++ -g -std=c++14 -o memory_pool memory_pool.cpp
// -Wno-pointer-arith

template <typename T>
std::string int_to_hex(T i) {
  std::stringstream stream;
  stream << "0x" << std::setfill('0') << std::setw(sizeof(T)) << std::hex << i;
  return stream.str();
}

struct Block {
  void *next;
};

// not a thread safe in place fixed size memory pool
class MemoryPool {
 public:
  MemoryPool(size_t blockSize, size_t numOfBlock) {
    // we want blockSize aligned to machine default unit address length
    size_t alignedBlockSize =
        (blockSize + sizeof(void *)) & (~(sizeof(void *) - 1));
    size_t rawBlockSize = alignedBlockSize + sizeof(void *);
    std::cout << "alignedBlockSize :" << alignedBlockSize << std::endl;
    std::cout << "rawBlockSize : " << rawBlockSize << std::endl;
    _rawSize = sizeof(Block *) + numOfBlock * rawBlockSize;
    _rawPtr = malloc(_rawSize);
    if (!_rawPtr) {
      throw std::runtime_error("No system memory available");
    }

    std::cout << "_rawPtr @ " << _rawPtr << std::endl;
    std::cout << "_rawSize: " << _rawSize << std::endl;
    // partition the block
    // first block
    _freeHead = (Block *)(_rawPtr + 1);
    std::cout << "_freeHead @ " << _freeHead << std::endl;
    std::cout << "Block 0 @ " << _freeHead << std::endl;

    auto ptr = _freeHead;
    for (size_t i = 1; i < numOfBlock; i++) {
      ptr->next = ptr + rawBlockSize / sizeof(void *);
      ptr = (Block *)ptr->next;
      std::cout << "Block " << i << " @" << ptr << std::endl;
    }

    // last free block
    ptr->next = nullptr;
  }

  ~MemoryPool() {
    if (_rawPtr) {
      free(_rawPtr);
      _rawPtr = nullptr;
    }
    _freeHead = nullptr;
  }

  void *alloc() {
    // use the 1st free block in the free list
    // return the actual block address
    if (_freeHead) {
      auto _rawPtr = _freeHead;
      void *returnAddr = (void *)_rawPtr + 1;

      // adjust _freeHead
      _freeHead = (Block *)_freeHead->next;
      std::cout << "==_freeHead @ " << _freeHead << std::endl;
      std::cout << "Allocated addr: " << returnAddr << std::endl;

      return returnAddr;
    } else
      return nullptr;
  }

  void dealloc(void **addr) {
    auto _ptr = (Block *)(*addr - 1);

    // range check
    if ((void *)_ptr < _rawPtr || (void *)_ptr > (_rawPtr + _rawSize))
      throw std::runtime_error(int_to_hex((unsigned long)_ptr) +
                               std::string(" doesn't belong to this pool"));

    // put it back to free listt
    _ptr->next = _freeHead;
    _freeHead = _ptr;
    std::cout << "--_freeHead @ " << _freeHead << std::endl;
    *addr = nullptr;
  }

  bool isUsedUp() { return !_freeHead; }

 private:
  void *_rawPtr;
  size_t _rawSize;
  Block *_freeHead;
};

int main() {
  auto pool = MemoryPool(63, 5);
  auto ptr1 = pool.alloc();
  auto ptr2 = pool.alloc();
  std::cout << "isUsedUp: " << pool.isUsedUp() << std::endl;
  pool.dealloc(&ptr2);
  pool.dealloc(&ptr1);
}
