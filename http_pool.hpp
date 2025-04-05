#ifndef HTCW_HTTP_POOL
#define HTCW_HTTP_POOL
#include <stdint.h>
#include <stddef.h>
#include <memory.h>
namespace htcw {
    template<int Id,void*(*Allocator)(size_t)=::malloc,void(*Deallocator)(void*)=::free> class pool {
    public:
    private:
        static uint8_t* s_begin;
        static uint8_t* s_latest;
        static uint8_t* s_new;
        static size_t s_length;
    public:
        static bool initialize(size_t size) {
            if(size==0) {
                return false;
            }
            if(s_begin!=nullptr) {
                return false;
            }
            s_new = nullptr;
            s_latest = nullptr;
            s_length = 0;
            s_begin = (uint8_t*)Allocator(size);
            if(!s_begin) {
                return false;
            }
            s_new = s_begin;
            s_length = size;
            return true;
        }
        static void deinitialize() {
            if(s_begin) {
                Deallocator(s_begin);
                s_begin = nullptr;
                s_new = nullptr;
                s_latest = nullptr;
                s_length = 0;
            }
        }
        static void* allocate(size_t size) {
            if(!size || !s_begin) return nullptr;
            if((s_new-s_begin)+size>s_length) {
                return nullptr;
            }
            s_latest = s_new;
            s_new+=size;
            return s_latest;
        }
        static void deallocate(void* ptr) {
            // do nothing
        }
        static void* reallocate(void* ptr, size_t size) {
            if(!s_begin) return nullptr;
            if(!size) {
                return nullptr;
            }
            if(ptr!=s_latest) {
                return allocate(size);
            }
            size_t len = s_new - s_latest;
            size_t target_len = s_length-len+size;
            if(target_len>s_length) {
                return nullptr;
            }
            s_length = target_len;
            s_new = s_begin+target_len;
            return ptr;
        }
        static void deallocate_all() {
            s_new=s_begin;
            s_length = 0;
            s_latest = nullptr;
        }
    };
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