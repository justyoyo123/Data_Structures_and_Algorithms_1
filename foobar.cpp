/*
Justin Koe.  Foobar program that parses an input text file containing the name of different foobars, foos, and bars.  Then, returns an output text file that shows that lists the name of all foobars, foos, and bars and their respective strengths.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

class Foobar
{
    public:
        Foobar(std::string foo_name)
        {
            name = foo_name;
            position = 0; 
        }
        void set_position(int pos) 
        {
            position = pos;
        }
        virtual int get_strength()
        {
            return position;
        }
        std::string get_name()
        {
            return name;
        }
    protected:
        int get_position()
        {
            return position;
        }
    private:
        int position;
        std::string name;
};

class Foo : public Foobar
{
    public:
        Foo(std::string name) : Foobar(name) {} 
        virtual int get_strength()
        {
            return Foobar::get_position() * 3;
        }
};

class Bar : public Foobar
{
    public:
        Bar(std::string name) : Foobar(name) {}
        virtual int get_strength()
        {
            return Foobar::get_position() + 15;
        }
};

void read_input(std::string file, std::vector<Foobar*> &vec)
{
    std::string line;
    std::ifstream myfile (file);
    if (myfile.is_open())
    {
        while ( std::getline (myfile,line) )
        {
            std::string foo_type,name;
            std::stringstream ss;
    
            ss << line; // convert line to string stream for easier parsing
            ss >> foo_type >> name;

            if (foo_type.compare("foobar") == 0)
            {
                Foobar* foobar = new Foobar(name);
                vec.push_back(foobar);
            }
            else if (foo_type.compare("foo") == 0)
            {
                Foo* foo = new Foo(name);
                vec.push_back(foo);
            }
            else
            {
                Bar* bar = new Bar(name);
                vec.push_back(bar);
            }
        }
        myfile.close();
    }
}

void set_positions(std::vector<Foobar*> &vec)
{
    for(int i = vec.size() - 1; i >= 0; i--)
    {
        (vec.at(i))->set_position(vec.size() - i);
    }
}

void output_strengths(std::string file, std::vector<Foobar*> &vec)
{
    std::ofstream myout;
    myout.open(file);
    if (myout.is_open())
    {
        //Since vector of pointers of Foobar (baseclass), take advantage of polymorphism
        for(auto& item: vec)
        {
            myout << item->get_name();
            myout << ' '; 
            myout << item->get_strength() << '\n';
        }
    }
    myout.close();
}

int main()
{
    std::string file_in, file_out;
    std::cout << "Enter the name of the input file: "; 
    std::cin >> file_in;
    std::cout << "Enter the name of the output file: ";
    std::cin >> file_out;
    std::vector<Foobar*> vec;
    read_input(file_in, vec); 
    set_positions(vec);
    output_strengths(file_out, vec);
}


