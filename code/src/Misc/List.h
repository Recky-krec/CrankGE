#pragma once

#include <initializer_list>
#include <iostream>

namespace crank
{

template<typename Elem>
struct Link
{
    Link* prev;
    Link* succ;
    Elem val;
};

template<typename Elem>
class List
{
public:

    class iterator // bidirectional iterator
    {

    private:
        Link<Elem>* curr;

    public:
        iterator(Link<Elem>* p) : curr(p) {}
        iterator& operator++(){ curr = curr->succ; return *this; }
        iterator& operator--(){ curr = curr->prev; return *this; }
        bool operator==(const iterator& it) const { return curr == it.curr; }
        bool operator!=(const iterator& it) const { return curr != it.curr; }
        Elem& operator*() { return curr->val; }

        friend class List<Elem>;

    };

public:
    List() : first(new Link<Elem>), last(first) {}

    List(std::initializer_list<Elem> il)
        : first(new Link<Elem>), last(first)
    {
        for(auto it = il.begin(); it!=il.end(); it++)
            push_back(*it);
    }

    ~List()
    {
        delete first;
        delete last;
    }

    iterator begin() const { return iterator(first); }
    iterator end()   const { return iterator(last); }

    void push_back(const Elem& v)
    {
        if(begin() == end())
        {
            first->val = v;
            last = new Link<Elem>;
            last->prev = first;
            first->succ = last;
            return;
        }

        auto* l = new Link<Elem>;
        l->val = v;
        l->prev = last->prev;
        l->succ = last;
        last->prev->succ = l;
        last->prev = l;
    }

    void push_front(const Elem& v)
    {
        if(begin() == end())
        {
            first->val = v;
            last = new Link<Elem>;
            last->prev = first;
            first->succ = last;
            return;
        }

        auto* l = new Link<Elem>;
        l->succ = first;
        l->val = v;
        first->prev = l;
        first = l;
    }

    void pop_front()
    {
        if(begin() == end()) return;
        Link<Elem>* next = first->succ;
        next->prev = nullptr;

        delete first;
        first = next;
    }

    void pop_back()
    {
        if(begin() == end()) return;
        Link<Elem>* prev = last->prev;
        prev->succ = nullptr;

        delete last;
        last = prev;
    }

    iterator insert(iterator p, const Elem& v)
    {
        if(begin() == end())
        {
            first->val = v;
            last = new Link<Elem>;
            last->prev = first;
            first->succ = last;
            return iterator(first);
        }

        if(p == end()) return end();

        auto* l = new Link<Elem>;
        l->val = v;
        l->prev = p.curr;
        l->succ = p.curr->succ;
        p.curr->succ->prev = l;
        p.curr->succ = l;
        return iterator(l);
    }

    /*iterator erase(iterator p)
    {
        if(begin() == end()) return end();
        p.curr->succ->prev = p.curr->prev;
        p.curr->prev->succ = p.curr->succ;

        iterator next(p.curr->succ);
        delete p.curr;
        return next;
    }*/


    //Elem& front()
    //{
    //}

    //Elem& back()
    //{
    //}

private:
    Link<Elem>* first;
    Link<Elem>* last; // one beyond the last link
};

} // namespace crank

