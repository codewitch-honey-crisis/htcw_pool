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
        static size_t capacity() {
            return s_length;
        }
        static size_t bytes_used() {
            return s_new-s_begin;
        }
        static size_t bytes_free() {
            return capacity()-bytes_used();
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
            if((s_new-s_begin)+size+sizeof(size_t)>s_length) {
                return nullptr;
            }
            s_latest = s_new;
            s_new+=(size+sizeof(size_t));
            (*((size_t*)s_latest))=size;
            return s_latest+sizeof(size_t);
        }
        static void deallocate(void* ptr) {
            if(s_begin==nullptr) {
                return;
            }
            if(ptr==((uint8_t*)s_latest)+sizeof(size_t)) {
                // we can reclaim this memory
                uint8_t* b = s_begin;
                size_t used = bytes_used();
                size_t len = *(size_t*)s_latest;
                s_new = b+(used-(len+sizeof(size_t)));
                uint8_t* p = (uint8_t*)b;
                uint8_t* op = p;
                size_t remaining = s_new-b;
                while(remaining && p) {
                    op=p;
                    size_t l = *(size_t*)p;
                    size_t lo = (l+sizeof(size_t));
                    p+=lo;
                    remaining-=l;
                }
                s_latest = op;
            }
        }
        static void* reallocate(void* ptr, size_t size) {
            if(!s_begin) return nullptr;
            if(!size) {
                return nullptr;
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
        static void deallocate_all() {
            s_new=s_begin;
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