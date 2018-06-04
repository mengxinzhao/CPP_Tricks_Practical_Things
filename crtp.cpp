
//In short, CRTP is when a class A has a base class which is a template specialization for the class A itself. E.g.
//
//template <class T>
//class X{...};
//class A : public X<A> {...};
//
// This actually gives the X template the ability to be a base class for its specializations.
// Using this technique, CRTP could be used to implement static polymorphism(which like dynamic polymorphism but without virtual function pointer table). This will drastically reduces code dups.
#include <iostream>
#include <fstream>

using namespace std;

template <class T>
class Writer
{
public:
    Writer()  { }
    ~Writer()  { }
    string name="A Generic Writer";
    
    void write(const char* str) const
    {
        static_cast<const T*>(this)->writeImpl(str); //here the magic is!!!
    }
};


class FileWriter : public Writer<FileWriter>
{
public:
    FileWriter() {  name = "FileWriter";}
    ~FileWriter() {  }
    
    //here comes the implementation of the write method on the subclass
    void writeImpl(string something) const
    {
        cout<<"FileWriter write something"<<endl;
    }
    
};


class ConsoleWriter : public Writer<ConsoleWriter>
{
public:
    ConsoleWriter() {name="ConsoleWriter"; }
    ~ConsoleWriter() { }
    
    void writeImpl(string something) const
    {
         cout<<"ConsoleWriter write something"<<endl;
    }
};

int main() {
    ConsoleWriter wr;
    cout<<wr.name<<endl;
    wr.write("aaa");
    FileWriter fwr;
    cout<<fwr.name<<endl;
    fwr.write("bbb");
    return 0;
}
