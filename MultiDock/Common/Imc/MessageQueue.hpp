#pragma once
#include <queue>

template<class _Ty>
class CMessageQueue
{
public:
   typedef _Ty          value_type;
   typedef _Ty&         reference;
   typedef const _Ty&   const_reference;
   typedef std::queue<_Ty>  container_type;

   CMessageQueue()
   {
      m_pContainer = new container_type;
   };

   ~CMessageQueue()
   {
      delete m_pContainer;
   };

   bool empty() const
   {	// test if queue is empty
      return (m_pContainer->empty());
   }

   size_t size() const
   {	// return length of queue
      return (m_pContainer->size());
   }

   reference front()
   {	// return first element of mutable queue
      return (m_pContainer->front());
   }

   const_reference front() const
   {	// return first element of nonmutable queue
      return (m_pContainer->front());
   }

   reference back()
   {	// return last element of mutable queue
      return (m_pContainer->back());
   }

   const_reference back() const
   {	// return last element of nonmutable queue
      return (m_pContainer->back());
   }

   void push(const value_type& _Val)
   {	// insert element at beginning
      m_pContainer->push(_Val);
   }

   void pop()
   {	// erase element at end
      m_pContainer->pop();
   }

   void clear()
   {
      while(!empty())
      {
         m_pContainer->pop();
      }
   }
   // deletor can be a functor or a function witch
   // accept message queue element as argument
   template<typename Deleter>
   void clear(Deleter deleter)
   {
      // clear contiainer and do user defined 
      // delete operation on each element
      while( !empty() ) {
         _Ty item = m_pContainer->front();
         deleter(item);
         m_pContainer->pop();
      }
   }

   void swap(CMessageQueue<_Ty>& rhs)
   {  // exchange two message queue efficiently
      container_type * p = rhs.m_pContainer;
      rhs.m_pContainer = m_pContainer;
      m_pContainer = p;
   }

   CMessageQueue<_Ty>& operator=(CMessageQueue<_Ty>& rhs)
   {
      m_pContainer = rhs.m_pContainer;
   }

private:
   container_type * m_pContainer;
};
