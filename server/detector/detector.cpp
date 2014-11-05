#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>

#include "server/detector/feature_matcher.h"
#include "third_party/rapidjson/document.h"
#include "third_party/rapidjson/stringbuffer.h"
#include "third_party/rapidjson/writer.h"

using namespace rapidjson;
using namespace std;

// ./detector <reference_image_path> <database_path>
int main(int argc, const char** argv) {
    assert(argc == 3);
    const char* reference_image = argv[1];

    // Read the json database.
    fstream file_stream;
    file_stream.open(argv[2], fstream::in);
    stringstream database;
    database << file_stream.rdbuf();
    file_stream.close();

    Document document;
    document.Parse(database.str().c_str());

    SizeType best_index = 0;
    int best_score = 0;

    for (SizeType i = 0; i < document.Size(); ++i) {
        Value& entry = document[i];
        string template_path = entry["template"].GetString();
        int score = ComputeFeatureMatchScore(reference_image, template_path);
        if (score > best_score) {
            best_score = score;
            best_index = i;
        }
    }

    if (best_score > 0) {
        Value& match = document[best_index];

        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);

        writer.StartObject();
        writer.String("name");
        writer.String(match["name"].GetString());
        writer.String("description");
        writer.String(match["description"].GetString());
        writer.String("image");
        writer.String(match["image"].GetString());
        writer.EndObject();

        cout << buffer.GetString();
    }
    
    return 0;
}
