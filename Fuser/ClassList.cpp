#include <iostream>
#include <vector>
using namespace std;

class ClassList {
    std::string profile;
    std::vector<std::string> methods;
    std::vector<std::string> fields; 
    std::vector<std::string> inheritance; 
    std::vector<std::string> dependencies;
    int size;
  public:
    void setProfile (std::string);
    std::string getProfile() const;
    vector<std::string> getMethods() const;
    vector<std::string> getFields() const;
    vector<std::string> getInheritance() const;
    vector<std::string>	 getDependency() const;
    int getSize();
    void setMethod(std::string);
    void setInheritance(std::string);
    void setSize(int);
    void setField(std::string);
    void setDependency(std::string);

};

void ClassList::setProfile (std::string prof) {
  profile = prof;
}

std::string ClassList::getProfile () const {
  return profile;
}

std::vector<std::string> ClassList::getMethods() const{
	return methods; 
}

std::vector<std::string> ClassList::getFields() const{
	return methods; 
}

std::vector<std::string> ClassList::getInheritance() const{
	return inheritance; 
}

std::vector<std::string> ClassList::getDependency() const{
	return dependencies; 
}

int ClassList::getSize(){
	return size; 
}

void ClassList::setMethod (std::string d){
	methods.push_back(d);
}

void ClassList::setField (std::string d){
	fields.push_back(d);
}

void ClassList::setInheritance (std::string d){
	inheritance.push_back(d);
}

void ClassList::setDependency (std::string d){
	methods.push_back(d);
}

void ClassList::setSize(int i){
	size = i;
}

// int main () {
//   std::cout << "Hello world!" << std::endl;
//   ClassList pr;
//   pr.Profile("fish");
//   pr.setSize(1);
//   pr.setMethod("method1");
//   std::cout << pr.getSize() << std::endl;
//   for( std::vector<string>::const_iterator i = pr.getMethods().begin(); i != pr.getMethods().end(); ++i)
//     std::cout << *i << ' ' << std::endl;

//   return 0;
// }