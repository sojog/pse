#include <iostream>

#include "third_party/rapidjson/stringbuffer.h"
#include "third_party/rapidjson/writer.h"

using namespace rapidjson;
using namespace std;

int main() {
    StringBuffer buffer;
    
    Writer<StringBuffer> writer(buffer);

    // Writing a dummy Mona Lisa output.
    writer.StartObject();
    writer.String("name");
    writer.String("Mona Lisa");
    writer.String("description");
    writer.String("Mona Lisa description goes here.");
    writer.String("image");
    writer.String("http://www.example.com");
    writer.EndObject();

    cout << buffer.GetString();

    return 0;
}
