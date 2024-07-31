    #include<iostream>
using namespace std;

/////////////////////////////////////////////////////////////
//
// Code of Generic Singly Linear
//
/////////////////////////////////////////////////////////////

template <class T>
struct node
{
    T data;
    struct node *next;
    struct node *prev;

};

template <class T>
class SinglyLL
{
    private:
        struct node<T> * First;
        int iCount;

    public:
        SinglyLL();

        void Display();
        int Count();

        void InsertFirst(T No);
        void InsertLast(T No);
        void InsertAtPos(T No, int iPos);

        void DeleteFirst();
        void DeleteLast();
        void DeleteAtPos(int iPos);
};

template <class T>
SinglyLL<T>::SinglyLL()
{
    cout<<"Inside Constructor\n";
    First = NULL;
    iCount = 0;
}

template <class T>
void SinglyLL<T>::Display()
{
    struct node<T> * temp = First;

    while(temp != NULL)
    {
        cout<<"| "<<temp->data<<"|-> ";
        temp = temp -> next;
    }
    cout<<"NULL\n";
}

template <class T>
int SinglyLL<T>::Count()
{
    return iCount;
}

template <class T>
void SinglyLL<T>::InsertFirst(T No)
{
    struct node<T> * newn = NULL;

    newn = new node<T>;    // malloc

    newn->data = No;
    newn->next = NULL;

    if(First == NULL) // if(iCount == 0)
    {
        First = newn;
    }
    else
    {
        newn->next = First;
        First = newn;
    }
    iCount++;
}

template <class T>
void SinglyLL<T>::InsertLast(T No)
{
    struct node<T> * newn = NULL;
    struct node<T> * temp = First;

    newn = new node<T>;    // malloc

    newn->data = No;
    newn->next = NULL;

    if(First == NULL) // if(iCount == 0)
    {
        First = newn;
    }
    else
    {
        while(temp->next != NULL)
        {
            temp = temp -> next;
        }
        temp -> next = newn;
    }
    iCount++;
}

template <class T>
void SinglyLL<T>::InsertAtPos(T No, int iPos)
{
    struct node<T> * newn = NULL;
    int i = 0;
    struct node<T> * temp;

    if((iPos < 1) || (iPos > iCount+1))
    {
        cout<<"Invalid position\n";
        return;
    }

    if(iPos == 1)
    {
        InsertFirst(No);
    }
    else if(iPos == iCount+1)
    {
        InsertLast(No);
    }
    else 
    {
        temp = First;
        
        newn = new node<T>;
        newn->data = No;
        newn->next = NULL;

        for(i = 1; i < iPos-1; i++)
        {
            temp = temp -> next;
        }

        newn->next = temp->next;
        temp -> next = newn;
        iCount++;
    }
}

template <class T>
void SinglyLL<T>::DeleteFirst()
{
    struct node<T> * temp = First;

    if(First == NULL)
    {
        cout<<"LL is empty\n";
        return;
    }
    else if(First -> next == NULL)
    {
        delete First;
        First = NULL;
    }
    else
    {
        First = First -> next;
        delete temp;
    }
    iCount--;
}

template <class T>
void SinglyLL<T>::DeleteLast()
{
    struct node<T> * temp = First;

    if(First == NULL)
    {
        cout<<"LL is empty\n";
        return;
    }
    else if(First -> next == NULL)
    {
        delete First;
        First = NULL;
    }
    else
    {
        while(temp->next->next != NULL)
        {
            temp = temp ->next;
        }
        delete temp->next;
        temp->next = NULL;
    }
    iCount--;
}

template <class T>
void SinglyLL<T>::DeleteAtPos(int iPos)
{
    int i = 0;
    struct node<T> * temp1;
    struct node<T> * temp2;

    if((iPos < 1) || (iPos > iCount))
    {
        cout<<"Invalid position\n";
        return;
    }

    if(iPos == 1)
    {
        DeleteFirst();
    }
    else if(iPos == iCount)
    {
        DeleteLast();
    }
    else 
    {
        temp1 = First;

        for(i = 1; i < iPos-1; i++)
        {
            temp1 = temp1 -> next;
        }

        temp2 = temp1->next;

        temp1->next = temp2->next;
        delete temp2;

        iCount--;
    }    
}


/////////////////////////////////////////////////////////////
//
// Code of Generic Singly Circular
//
/////////////////////////////////////////////////////////////


template <class T>
class SinglyCL
{
    private: 
    struct node<T>* First;
    struct node<T>* Last;
    int iCount;

    public:
    SinglyCL();

    void Display();
    int Count();

    void InsertFirst (T No);
    void InsertLast(T No);
    void InsertAtPos(T No, int iPos);

    void DeleteFirst();
    void DeleteLast();
    void DeleteAtPos(int iPos);
};

template <class T>
SinglyCL<T>::SinglyCL()
{
    First = NULL;
    Last = NULL;
    iCount = 0;
} 

template <class T>
void SinglyCL<T>::InsertFirst(T No)
{
    struct node<T>* newn = NULL;

    newn = new struct node<T>;

    newn->data = No;
    newn->next = NULL;

    if((First == NULL) && (Last == NULL))
    {
        First = newn;
        Last = newn;
    }
    else
    {
        newn->next = First;
        First = newn;
    }
        Last->next = First;
        iCount++;

}

template <class T>
void SinglyCL<T>:: InsertLast(T No)
{
    struct node<T>* newn = NULL;

    newn = new struct node<T>;

    newn->data = No;
    newn->next = NULL;

    if((First == NULL) && (Last == NULL))
    {
        First = newn;
        Last = newn;
    }
    else
    {
        Last -> next = newn;
        Last = newn;
    }
        Last->next = First;
        iCount++;

}

template <class T>
void SinglyCL<T>::Display()
{
    if((First == NULL) && (Last == NULL))
    {
        cout<<"LinkedList is empty"<<endl;
        return;
    }

    struct node<T>* temp = First;
    do
    {
        cout<<"| "<<temp->data<<" |->";
        temp = temp -> next;
    }while(temp != First);

    cout<<endl;
}

template <class T>
int SinglyCL<T>::Count()
{
    return iCount;
}

template <class T>
void SinglyCL<T>::DeleteFirst()
{
    if((First == NULL) && (Last == NULL)) 
    {
        return;
    }
    else if(First == Last)   
    {
        delete (First);
        First = NULL;
        Last = NULL;
    }
    else    
    {
        struct node<T>* temp = First;
        First = First -> next;
        delete (temp);
        Last -> next = First;
    }
    iCount--;
}

template <class T>
void SinglyCL<T>:: DeleteLast()
{
    struct node<T>* temp = NULL;

    if((First == NULL) && (Last == NULL)) 
    {
        return;
    }
    else if(First == Last)   
    {
        delete (First);
        First = NULL;
        Last = NULL;
    }
    else     
    {
        temp = First;

        while(temp ->next != Last)
        {
            temp = temp -> next;
        }

        delete(Last);
        Last = temp;

        Last->next = First;
    }
    iCount--;
}

template <class T>
void SinglyCL<T>::InsertAtPos(T No, int iPos)
{
    int i = 0;
    int iLength = 0;
    struct node<T>* temp = NULL;
    struct node<T>* newn = NULL;

    iLength = Count();

    if((iPos < 1) || (iPos > iLength + 1))
    {
        cout<<"Invalid position"<<endl;
        return;
    }

    if(iPos == 1)
    {
        InsertFirst(No);
    }
    else if(iPos == iLength + 1)
    {
        InsertLast(No);
    }
    else
    {
        temp = First;
        
        newn = new struct node<T>;
        newn->data = No;
        newn->next = NULL;

        for(i = 1 ; i < iPos-1 ; i++)
        {
            temp = temp->next;
        }

        newn->next = temp -> next;
        temp -> next = newn;
        iCount++;
    }

}

template <class T>
void SinglyCL<T>::DeleteAtPos(int iPos)
{
    int i = 0;
    int iLength = 0;
    struct node<T>* temp1 = NULL;
    struct node<T>* temp2 = NULL;

    iLength = Count();

    if((iPos < 1) || (iPos > iLength))
    {
        cout<<"Invalid position"<<endl;
        return;
    }

    if(iPos == 1)
    {
        DeleteFirst();
    }
    else if(iPos == iLength)
    {
        DeleteLast();
    }
    else
    {
        temp1 = First;

        for(i = 1; i < iPos-1; i++)
        {
            temp1 = temp1 -> next;
        }

        temp2 = temp1->next;

        temp1 -> next = temp2->next;
        delete temp2;
        iCount--;
    }

}


/////////////////////////////////////////////////////////////
//
// Code of Generic Doubly Circular
//
/////////////////////////////////////////////////////////////

template <class T>
class DoublyCL
{
    private:
        struct node<T> * First;
        struct node<T> * Last;
        int iCount;

    public:
        DoublyCL();

        void Display();
        int Count();

        void InsertFirst(T No);
        void InsertLast(T No);
        void InsertAtPos(T No, int iPos);

        void DeleteFirst();
        void DeleteLast();
        void DeleteAtPos(int iPos);
};

template <class T>
DoublyCL<T>:: DoublyCL()
{
    First = NULL;
    Last = NULL;
    iCount = 0;
}

template <class T>
void DoublyCL<T>::Display()
{
    if(First == NULL && Last == NULL)
    {
        cout<<"Linked List is emprty\n";
        return;
    }

    cout<<"<=> ";
    do
    {
        cout<<"| "<<First->data <<"| <=> ";
        First = First -> next;
    }while(Last -> next != First);

    cout<<"\n";
}

template <class T>
int DoublyCL<T>::Count()
{
    return iCount;
}

template <class T>
void DoublyCL<T>::InsertFirst(T No)
{
    struct node<T> * newn = NULL;

    newn = new node<T>;

    newn->data = No;
    newn->next = NULL;
    newn->prev = NULL;

    if((First == NULL) && (Last == NULL))
    {
        First = newn;
        Last = newn;
    }
    else
    {
        newn->next = First;
        First->prev = newn;
        First = newn;
    }
    Last -> next = First;
    First -> prev = Last;

    iCount++;
}

template <class T>
void DoublyCL<T>::InsertLast(T No)
{
    struct node<T> * newn = NULL;

    newn = new node<T>;

    newn->data = No;
    newn->next = NULL;
    newn->prev = NULL;

    if((First == NULL) && (Last == NULL))
    {
        First = newn;
        Last = newn;
    }
    else
    {
        Last ->next = newn;
        newn->prev = Last;
        Last = newn;
    }
    Last -> next = First;
    First -> prev = Last;

    iCount++;    
}

template <class T>
void DoublyCL<T>::InsertAtPos(T No, int iPos)
{
    struct node<T> * temp = NULL;
    struct node<T> * newn = NULL;

    int i = 0;

    if(iPos < 1 || iPos > iCount+1)
    {
        cout<<"Invalid postion\n";
        return;
    }

    if(iPos == 1)
    {
        InsertFirst(No);
    }
    else if(iPos == iCount+1)
    {
        InsertLast(No);
    }
    else
    {
        newn = new node<T>;

        newn->data = No;
        newn->next = NULL;
        newn->prev = NULL;

        temp = First;

        for(i = 1; i < iPos -1; i++)
        {
            temp = temp -> next;
        }

        newn->next = temp->next;
        temp->next->prev = newn;

        temp->next = newn;
        newn->prev = temp;

        iCount++;
    }
}

template <class T>
void DoublyCL<T>::DeleteFirst()
{
    if(First == NULL && Last == NULL)   // Empty LL
    {
        return;
    }
    else if(First == Last)   // Single node
    {
        delete First;
        First = NULL;
        Last = NULL;
    }
    else    // More than one node
    {
        First = First -> next;
        delete Last->next;
        First -> prev = Last;
        Last -> next = First;
    }
    iCount--;
}

template <class T>
void DoublyCL<T>::DeleteLast()
{
    if(First == NULL && Last == NULL)   // Empty LL
    {
        return;
    }
    else if(First == Last)   // Single node
    {
        delete First;
        First = NULL;
        Last = NULL;
    }
    else    // More than one node
    {
        Last = Last -> prev;
        delete First -> prev;

        Last -> next = First;
        First -> prev = Last;
    }
    iCount--;    
}

template <class T>
void DoublyCL<T>::DeleteAtPos(int iPos)
{
    struct node<T> * temp = NULL;

    int i = 0;

    if(iPos < 1 || iPos > iCount)
    {
        cout<<"Invalid postion\n";
        return;
    }

    if(iPos == 1)
    {
        DeleteFirst();
    }
    else if(iPos == iCount)
    {
        DeleteLast();
    }
    else
    {
        temp = First;

        for(i = 1; i < iPos -1; i++)
        {
            temp = temp -> next;
        }

        temp->next = temp->next->next;
        delete temp->next->prev;
        temp->next->prev = temp;

        iCount--;
    }    
}


/////////////////////////////////////////////////////////////
//
// Code of Generic Doubly Linear
//
/////////////////////////////////////////////////////////////

template <class T>
class DoublyLL
{
    private:
    int iCount;
    struct node<T>* First;
    struct node<T>* Last;

    public:
    DoublyLL();

    void Display();
    int Count();

    void InsertFirst(T No);
    void InsertLast(T No);
    void InsertAtPos(T No, int iPos);

    void DeleteFirst();
    void DeleteLast();
    void DeleteAtPos(int iPos);

};

template <class T>
DoublyLL<T>::DoublyLL()
{
    iCount = 0;
    First = NULL;
    Last = NULL;
}

template <class T>
void DoublyLL<T>::Display()
{
    struct node<T>* temp = First;

    cout<<"\n"<<"NULL<=>";
    while(temp != NULL)
    {
        cout<<"| "<<temp->data<<"| <=> ";
        temp = temp->next;
    }
    cout<<"NULL"<<endl;
}

template <class T>
int DoublyLL<T>::Count()
{
    return iCount;
}

template <class T>
void DoublyLL<T>::InsertFirst(T No)
{
    struct node<T>* newn = NULL;

    newn = new node<T>;

    newn->data = No;
    newn->next = NULL;
    newn->prev = NULL;  

    if(First == NULL)
    {
        First = newn;
        Last = newn;
    }
    else
    {
        newn->next = First;
        First -> prev = newn;    

        First = newn;
    }
    iCount++;
}

template <class T>
void DoublyLL<T>::InsertLast(T No)
{
    struct node<T>* newn = NULL;
    struct node<T>* temp = NULL;

    newn = new node<T>;

    newn->data = No;
    newn->next = NULL;
    newn->prev = NULL;  

    if(First == NULL)
    {
        First = newn;
        Last = newn;
    }
    else
    {
        temp = First;

        while(temp->next != NULL)
        {
            temp = temp -> next;
        }

        temp->next = newn;
        newn->prev = temp; 
        Last = newn;    
    }
    iCount++;
}

template <class T>
void DoublyLL<T>::InsertAtPos(T No, int iPos)
{
    int iLength = 0;
    int i = 0;

    struct node<T>* newn = NULL;
    struct node<T>* temp = NULL;

    iLength = Count();

    if((iPos < 1) || (iPos > iLength+1))
    {
        cout<<"Invalid Position"<<"\n";
        return;
    }

    if(iPos == 1)
    {
        InsertFirst(No);
    }
    else if(iPos == iLength + 1)
    {
        InsertLast(No);
    }
    else
    {
        newn = new node<T>;

        newn->data = No;
        newn->next = NULL;
        newn->prev = NULL; 

        temp = First;

        for(i = 1; i < iPos -1; i++)
        {
            temp = temp -> next;
        }

        newn->next = temp->next;
        temp->next->prev = newn;   
        temp->next = newn;
        newn->prev = temp;  
        iCount++;       
    }
}

template <class T>
void DoublyLL<T>::DeleteFirst()
{
    if(First == NULL)  
    {
        cout<< "Unable to delete the node as LL is empty\n";
        return;
    }
    else if(First->next == NULL) 
    {
        delete First;
        First = NULL;
        Last = NULL;
        
        iCount--;
    }
    else    
    {
        struct node<T>* temp = First;
        First = First->next;
        First->prev = NULL;
        delete temp;       
          
        iCount--;     
    }
}

template <class T>
void DoublyLL<T>::DeleteLast()
{
    struct node<T>* temp = NULL;

    if(First == NULL) 
    {
        cout<<"Unable to delete the node as LL is empty"<<"\n";
        return;
    }
    else if(First->next == NULL) 
    {
        delete First;
        First = NULL;
        Last = NULL;

        iCount--;
    }
    else    
    {
        temp = Last;

        Last = Last->prev;
        Last->next = NULL;
        delete temp;
      
        iCount--;
    }
}

template <class T>
void DoublyLL<T>::DeleteAtPos(int iPos)
{
    int iLength = 0;
    int i = 0;

    struct node<T>* temp = NULL;
    struct node<T>* temp2 = NULL;

    iLength = Count();

    if((iPos < 1) || (iPos > iLength))
    {
        cout<<"\nInvalid Position"<<endl;
        return;
    }

    if(iPos == 1)
    {
        DeleteFirst();
    }
    else if(iPos == iLength)
    {
        DeleteLast();
    }
    else
    {
        temp = First;

        for(i = 1; i < iPos-1; i++)
        {
            temp = temp -> next;
        }

        temp2 = temp->next;
        temp->next = temp ->next->next;
        temp->next->prev = temp;
        delete temp2;

        iCount--;
    }    
}

/////////////////////////////////////////////////////////////
//
// Code of Generic Stack
//
/////////////////////////////////////////////////////////////

template <class T>
class Stack
{
    private:
        struct node<T>* First;
        int iCount;

    public:
        Stack();
        ~Stack();
        void Display();
        int Count();
        void Push(T No);  
        T Pop();   
};

template <class T>
Stack<T>:: Stack()
{
    First = NULL;
    iCount = 0;
}

template <class T>
Stack<T>::~Stack()
{
    struct  node<T>* temp;

    while(First != NULL)
    {
        temp = First;
        First = First->next;
        delete temp;
    }
}

template <class T>
void Stack<T>::Display()
{
    cout<<"Elements inside stack are : \n";
    struct node<T>* temp = First;

    while(temp != NULL)
    {
        cout<<temp->data<<"\n";
        temp = temp -> next;
    }

}

template <class T>
int Stack<T>::Count()
{
    return iCount;
}

template <class T>
void Stack<T>::Push(T No)
{
    struct node<T>* newn = NULL;

    newn = new node<T>;

    newn->data = No;
    newn->next = NULL;

    if(First == NULL)
    {
        First = newn;
    }
    else
    {
        newn->next = First;
        First = newn;
    }
    iCount++;
}

template <class T>
T Stack<T>::Pop()
{
    T iValue = 0;
    struct node<T>* temp = NULL;

    if(First == NULL)
    {
        cout<<"Unable to pop the element as stack is empty\n";
        return T();
    }
    else
    {
        temp = First;

        iValue = First->data;
        First = First->next;

        delete temp;
        
        iCount--;
    }

    return iValue;
}

/////////////////////////////////////////////////////////////
//
// Code of Generic Queue
//
/////////////////////////////////////////////////////////////

template <class T>
class Queue
{
    private:
        struct node<T>* First;
        int iCount;

    public:
        Queue();
        void Display();
        int Count();
        void EnQueue(T No);  // InsertLast()
        T DeQueue();   // DeleteFirst()
};

template <class T>
Queue<T>:: Queue()
{
    First = NULL;
    iCount = 0;
}

template <class T>
void Queue<T>::Display()
{
    cout<<"Elements of Queue are : \n";
    struct node<T>* temp = First;

    while(temp != NULL)
    {
        cout<<temp->data<<"\t";
        temp = temp -> next;
    }
    cout<<"\n";
}

template <class T>
int Queue<T>::Count()
{
    return iCount;
}

template <class T>
void Queue<T>::EnQueue(T No)
{
    struct node<T>* newn = NULL;
    struct node<T>* temp = NULL;

    newn = new node<T>;

    newn->data = No;
    newn->next = NULL;

    if(First == NULL)
    {
        First = newn;
    }
    else
    {
        temp = First;

        while(temp ->next != NULL)
        {
            temp = temp -> next;
        }

        temp->next = newn;
    }
    iCount++;
}

template <class T>
T Queue<T>::DeQueue()
{
    T iValue;
    struct node<T>*  temp = NULL;

    if(First == NULL)
    {
        cout<<"Unable to remove the element as queue is empty\n";
        return T();
    }
    else
    {
        temp = First;

        iValue = First -> data;
        First = First -> next;
        delete temp;
        
        iCount--;
    }

    return iValue;
}


int main()
{

    /////////////////////////////////////////////////////////////////////////////////////
    //
    // code of main Linear LinkedList
    //
    //////////////////////////////////////////////////////////////////////////////////////

    cout<<"-------------------------------- LinkdList of Integers-------------------- --------------\n";

    SinglyLL<int> *LLiobj = new SinglyLL<int>();

    int iRet = 0;
    char cRet = '\0';
    float fRet = 0.0f;
    double dRet = 0.0;

    LLiobj->InsertFirst(51);
    LLiobj->InsertFirst(21);
    LLiobj->InsertFirst(11);
    LLiobj->Display();
    iRet = LLiobj->Count();
    cout<<"Number of elemensts after InsertFirst : "<<iRet<<endl;

    LLiobj->InsertLast(101);
    LLiobj->InsertLast(111);
    LLiobj->InsertLast(121);   
    LLiobj->Display();
    iRet = LLiobj->Count();
    cout<<"Number of elemensts after InsertLast : "<<iRet<<endl;

    LLiobj->InsertAtPos(105,5);
    LLiobj->Display();
    iRet = LLiobj->Count();
    cout<<"Number of elemensts after InsertAtPos : "<<iRet<<endl;

    LLiobj->DeleteFirst();    
    LLiobj->Display();
    iRet = LLiobj->Count();
    cout<<"Number of elemensts after DeleteFirst : "<<iRet<<endl;

    LLiobj->DeleteLast();    
    LLiobj->Display();
    iRet = LLiobj->Count();
    cout<<"Number of elemensts after DeleteLast : "<<iRet<<endl;

    LLiobj->DeleteAtPos(5);
    LLiobj->Display();
    iRet = LLiobj->Count();
    cout<<"Number of elemensts after DeleteAtPos : "<<iRet<<endl;

    delete LLiobj;


    return 0;
}
