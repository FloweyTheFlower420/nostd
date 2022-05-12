#ifndef __NOSTDLIB_RBTREE_H__
#define __NOSTDLIB_RBTREE_H__
#include <type_traits>
#include <functional>

namespace std::detail
{
    template<typename T>
    class rbtree_node
    {
        rbtree_node* parent;
        rbtree_node* l;
        rbtree_node* r;
        T data;
    public:
        template<typename = enable_if_t<is_default_constructible_v<T>>>
        constexpr rbtree_node() : parent(nullptr), l(nullptr), r(nullptr), T() {}
        constexpr rbtree_node(rbtree_node* parent) : parent(parent), l(nullptr), r(nullptr), T() {}

        rbtree_node& left(rbtree_node* n)
        {
            l = n;
            return *this;
        }

        rbtree_node& right(rbtree_node* n)
        {
            r = n;
            return *this; 
        }

        constexpr rbtree_node* left() { return l; }
        constexpr rbtree_node* right() { return r; }
    };

    template <typename T, typename C = less<T>>
    class rbtree
    {
        rbtree_node<T>* root;
        size_t s;
        C cmp;
    public:


        [[nodiscard]] constexpr size_t size() { return s; }

        ~rbtree()
        {
            
        }
    };
}

#endif
