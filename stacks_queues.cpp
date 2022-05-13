/*
Justin Koe.  stacks_queues program that parses an input text file containing commands creating / reading from stacks and queues of int, double, and string, returns an output text file with output of the input commands.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <iterator>

template <typename T>
class SimpleList {
    private:
        struct Node {
            T data;
            Node *next;
            
            Node( const T &d, Node *n = NULL ) : data( d ), next( n ) {}
        };

        Node *start;
        Node *end;
        std::string name;

    protected:
        void push_front( const T &val );
        void push_back( const T &val );
        T pop_front();

    public:
        SimpleList(std::string name) {
            start = NULL;
            end = NULL;
            this->name = name;
        }
        std::string get_name() {
            return this->name;
        }
        bool check_empty();
        virtual void push(const T &val) = 0;
        virtual T pop() = 0; 
};

template <typename T>
void SimpleList<T>::push_front( const T &val ) {
    Node *front = new Node(val);
    Node *old = this->start;
    if(this -> start == NULL) { // empty
      this -> start = front;
      this -> end = front;
    }
    else {
      this -> start = front;
      front -> next = old;
    }
}

template <typename T>
void SimpleList<T>::push_back( const T &val ) {
    Node *back = new Node(val);
    if(this -> start == NULL) { // empty
      this -> end = back;
      this -> start = back;
    }
    else {
      this -> end -> next = back;
      this -> end = back;
    }
}

template <typename T>
T SimpleList<T>::pop_front() {
    Node *front = this->start;
    Node *new_front = front->next;
    if (this->start == NULL) {
        this->end = NULL;
        return 0;
    }
    else {
        T ret = front->data;
        this->start = new_front;
        delete front;
        return ret;
    }
}

template <typename T>
bool SimpleList<T>::check_empty() {
    if(this->start == NULL) {
        return true;
    }
    return false;
}

template <typename T>
class Stack : public SimpleList<T> {
    public:
        Stack(std::string name) : SimpleList<T>(name) {};
        void push(const T &val);
        T pop();
};

template <typename T>
void Stack<T>::push(const T &val) {
    this->push_front(val);
}

template <typename T>
T Stack<T>::pop() {
    return this->pop_front();
}

template <typename T>
class Queue : public SimpleList<T> {
    public:
    Queue(std::string name) : SimpleList<T>(name) {};
        void push(const T &val);
        T pop();
};

template <typename T>
void Queue<T>::push(const T &val) {
    this->push_back(val);
}

template <typename T>
T Queue<T>::pop() {
    return this->pop_front();
}

// SEARCH FUNCTION:  templated search inside of list of SimpleList<> for a specified name
template <typename T> 
SimpleList<T>* name_search(std::list<SimpleList<T> *> sl_list, const std::string &name) {
    typename std::list<SimpleList<T> *>::iterator itr;
    for(itr = sl_list.begin(); itr != sl_list.end(); itr++) {
        if((*itr) -> get_name() == name) {
            return (*itr);
        }
    }
    return NULL;
}

void process_file(const std::string &file_in, const std::string &file_out) {
    
    std::list<SimpleList<int> *> listSLi; // all integer stacks and queues
    std::list<SimpleList<double> *> listSLd; // all double stacks and queues
    std::list<SimpleList<std::string> *> listSLs; // all string stacks and queues

    SimpleList<int> *pSLi;
    SimpleList<double> *pSLd;
    SimpleList<std::string> *pSLs;

    std::string line, s_val;
    int i_val;
    double d_val;

    std::string command, name, type;

    std::ifstream ifile (file_in);
    std::ofstream ofile (file_out);

    if (ifile.is_open()) {
        while(std::getline (ifile, line)) {
            std::stringstream ss;
            ss << line;
            ss >> command >> name;

            if (command.compare("create") == 0) {
                ss >> type;
                ofile << "PROCESSING COMMAND: " << command << " " << name << " " << type << std::endl;
                if (name.at(0) == 'i') {
                    if (name_search(listSLi, name) != NULL) {
                        ofile << "ERROR: This name already exists!" << std::endl;
                    }
                    else {
                        if (type.compare("stack") == 0) {
                            pSLi = new Stack<int>(name);
                            listSLi.push_front(pSLi);
                        }
                        else {
                            pSLi = new Queue<int>(name);
                            listSLi.push_front(pSLi);
                        }
                    }
                }
                else if (name.at(0) == 'd') {
                    if (name_search(listSLd, name) != NULL) {
                        ofile << "ERROR: This name already exists!" << std::endl;
                    }
                    else {
                        if (type.compare("stack") == 0) {
                            pSLd = new Stack<double>(name);
                            listSLd.push_front(pSLd);
                        }
                        else {
                            pSLd = new Queue<double>(name);
                            listSLd.push_front(pSLd);
                        }
                    }
                }
                else {
                    if (name_search(listSLs, name) != NULL) {
                        ofile << "ERROR: This name already exists!" << std::endl;
                    }
                    else {
                        if (type.compare("stack") == 0) {
                            pSLs = new Stack<std::string>(name);
                            listSLs.push_front(pSLs);
                        }
                        else {
                            pSLs = new Queue<std::string>(name);
                            listSLs.push_front(pSLs);
                        }
                    }
                }
            }
            else if (command.compare("push") == 0) {
                if (name.at(0) == 'i') {
                    ss >> i_val;
                    ofile << "PROCESSING COMMAND: " << command << " " << name << " " << i_val << std::endl;
                    pSLi = name_search(listSLi, name);
                    if (pSLi == NULL) {
                        ofile << "ERROR: This name does not exist!" << std::endl;
                    }
                    else {
                        pSLi->push(i_val);
                    }
                }
                else if (name.at(0) == 'd') {
                    ss >> d_val;
                    ofile << "PROCESSING COMMAND: " << command << " " << name << " " << d_val << std::endl;
                    pSLd = name_search(listSLd, name);
                    if(pSLd == NULL) {
                        ofile << "ERROR: This name does not exist!" << std::endl;
                    }
                    else {
                        pSLd->push(d_val);
                    }
                }
                else {
                    ss >> s_val;
                    ofile << "PROCESSING COMMAND: " << command << " " << name << " " << s_val << std::endl;
                    pSLs = name_search(listSLs, name);
                    if(pSLs == NULL) {
                        ofile << "ERROR: This name does not exist!" << std::endl;
                    }
                    else {
                        pSLs->push(s_val);
                    }
                }
            }
            else { // pop
                ofile << "PROCESSING COMMAND: " << command << " " << name << std::endl;
                
                if (name.at(0) == 'i') {
                    pSLi = name_search(listSLi, name);
                    if (pSLi == NULL) {
                        ofile << "ERROR: This name does not exist!" << std::endl;
                    }
                    else {
                        if(pSLi->check_empty() == true) {
                            ofile << "ERROR: This list is empty!" << std::endl;
                        }
                        else {
                            i_val = pSLi->pop();
                            ofile << "Value popped: " << i_val << std::endl;
                        }
                    }
                }
                else if (name.at(0) == 'd') {
                    pSLd = name_search(listSLd, name);
                    if (pSLd == NULL) {
                        ofile << "ERROR: This name does not exist!" << std::endl;
                    }
                    else {
                        if(pSLd->check_empty() == true) {
                            ofile << "ERROR: This list is empty!" << std::endl;
                        }
                        else {
                            d_val = pSLd->pop();
                            ofile << "Value popped: " << d_val << std::endl;
                        }
                    }
                }
                else {
                    pSLs = name_search(listSLs, name);
                    if (pSLs == NULL) {
                        ofile << "ERROR: This name does not exist!" << std::endl;
                    }
                    else {
                        if(pSLs->check_empty()  == true) {
                            ofile << "ERROR: This list is empty!" << std::endl;
                        }
                        else {
                            s_val = pSLs->pop();
                            ofile << "Value popped: " << s_val << std::endl;
                        }
                    }
                }
            }
        }
    }
    ifile.close();
    ofile.close();
}

int main() {
    std::string file_in, file_out;
    std::cout << "Enter name of input file: "; 
    std::cin >> file_in;
    std::cout << "Enter name of output file: ";
    std::cin >> file_out;

    process_file(file_in, file_out); 
}



