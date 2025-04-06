#ifndef HTCW_POOL_HPP
#define HTCW_POOL_HPP
#include <stdint.h>
#include <stddef.h>
#include <memory.h>
namespace htcw {
    /// @brief A memory pool
    /// @tparam Id A unique id for this pool
    /// @tparam Allocator The allocator to use to allocate memory for the pool
    /// @tparam Deallocator The deallocator used to free memory for the pool
    template<int Id,void*(*Allocator)(size_t)=::malloc,void(*Deallocator)(void*)=::free> class pool {
    public:
        /// @brief The id of the pool
        constexpr static const int id = Id;
    private:
        static bool s_own;
        static uint8_t* s_begin;
        static uint8_t* s_latest;
        static uint8_t* s_new;
        static size_t s_length;
    public:
        /// @brief Initialize and allocate the buffer using the configured allocator
        /// @param capacity The capacity of the pool to allocate, in bytes
        /// @return True if successful, otherwise false
        static bool initialize(size_t capacity) {
            if(capacity==0) {
                return false;
            }
            if(s_begin!=nullptr) {
                return false;
            }
            s_own = false;
            s_new = nullptr;
            s_latest = nullptr;
            s_length = 0;
            s_begin = (uint8_t*)Allocator(capacity);
            if(!s_begin) {
                return false;
            }
            s_new = s_begin;
            s_length = capacity;
            s_own = true;
            return true;
        }
        /// @brief Initialize the pool with a passed in memory block
        /// @param block The block of memory to use
        /// @param size The size of the block in bytes
        /// @return True if successful, otherwise false
        static bool initialize(void* block, size_t size) {
            if(size==0) {
                return false;
            }
            if(s_begin!=nullptr) {
                return false;
            }
            s_own = false;
            s_new = nullptr;
            s_latest = nullptr;
            s_length = 0;
            s_begin = (uint8_t*)block;
            if(!s_begin) {
                return false;
            }
            s_new = s_begin;
            s_length = size;
            return true;
        }
        /// @brief Indicates the capacity of the pool in bytes
        /// @return The total number of bytes in the pool
        static size_t capacity() {
            return s_length;
        }
        /// @brief Indicates the number of bytes used
        /// @return The count of used bytes
        static size_t bytes_used() {
            return s_new-s_begin;
        }
        /// @brief Indicates the number of bytes free
        /// @return The count of free bytes
        static size_t bytes_free() {
            return capacity()-bytes_used();
        }
        /// @brief Deinitializes the pool and deallocates any memory that may have been allocated
        static void deinitialize() {
            if(s_begin) {
                if(s_own) {
                    Deallocator(s_begin);
                }
                s_own = false;
                s_begin = nullptr;
                s_new = nullptr;
                s_latest = nullptr;
                s_length = 0;
                
            }
        }
        /// @brief Allocates some memory from the pool
        /// @param size The size in bytes
        /// @return A pointer to the memory, or nullptr if unsuccessful
        static void* allocate(size_t size) {
            if(!size || !s_begin) return nullptr;
            if((s_new-s_begin)+size+sizeof(size_t)>s_length) {
                return nullptr;
            }
            s_latest = s_new;
            s_new+=(size+sizeof(size_t));
            (*((size_t*)s_latest))=size;
            return s_latest+sizeof(size_t);
        }
        /// @brief Attempts to deallocate memory from the pool if possible
        /// @param ptr The pointer to attempt to deallocate (not guaranteed)
        static void deallocate(void* ptr) {
            if(s_begin==nullptr || s_latest==nullptr) {
                return;
            }
            if(ptr==((uint8_t*)s_latest)+sizeof(size_t)) {
                // we can reclaim this memory
                // if it's the only entry, early out:
                if(s_latest==s_begin) {
                    s_latest=nullptr;
                    s_new = s_begin;
                    return;
                }
                uint8_t* p = s_begin;
                uint8_t* op=p;
                while(p<s_new) {
                    op=p;
                    size_t sz = *(size_t*)p;
                    size_t totsz = sz + sizeof(size_t);
                    p+=totsz;
                }
                s_new = p;
                s_latest = op;
            }
        }
        /// @brief Attempts to reallocate memory from the pool if possible
        /// @param ptr The pointer to reallocate
        /// @param size The new size in bytes
        /// @return A pointer to the reallocated data, or nullptr if unsuccessful
        static void* reallocate(void* ptr, size_t size) {
            if(!s_begin) return nullptr;
            if(!size) {
                deallocate(ptr);
                return nullptr;
            }
            if(ptr==nullptr) {
                return allocate(size);
            }
            if(ptr!=s_latest+sizeof(size_t)) {
                void* newp = allocate(size);
                if(newp==nullptr) {
                    return nullptr;
                }
                memmove(newp,ptr,*(size_t*)(((uint8_t*)ptr)-sizeof(size_t)));
                return newp;
            }
            size_t len = s_new - s_latest+sizeof(size_t);
            size_t new_len = (bytes_used()-len+size);
            if(new_len>s_length) {
                return nullptr;
            }
            s_new = s_begin+new_len;
            *(size_t*)(((uint8_t*)ptr)-sizeof(size_t))=size;
            return ptr;
        }
        /// @brief Deallicates all the pointers in the pool 
        static void deallocate_all() {
            s_new=s_begin;
            s_latest = nullptr;
        }
    };
    template<int Id,void*(*Allocator)(size_t),void(*Deallocator)(void*)>
    bool pool<Id,Allocator,Deallocator>::s_own = false;
    template<int Id,void*(*Allocator)(size_t),void(*Deallocator)(void*)>
    uint8_t* pool<Id,Allocator,Deallocator>::s_begin = nullptr;
    template<int Id,void*(*Allocator)(size_t),void(*Deallocator)(void*)>
    uint8_t* pool<Id,Allocator,Deallocator>::s_latest = nullptr;
    template<int Id,void*(*Allocator)(size_t),void(*Deallocator)(void*)>
    uint8_t* pool<Id,Allocator,Deallocator>::s_new = nullptr;
    template<int Id,void*(*Allocator)(size_t),void(*Deallocator)(void*)>
    size_t pool<Id,Allocator,Deallocator>::s_length = 0;
}
#endif