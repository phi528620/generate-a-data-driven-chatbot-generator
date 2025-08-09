#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Intent {
    string intentName;
    vector<string> phrases;
    vector<string> responses;
};

struct Entity {
    string entityName;
    vector<string> values;
};

struct ChatbotConfig {
    vector<Intent> intents;
    vector<Entity> entities;
};

ChatbotConfig loadConfig(const string& configFile) {
    ChatbotConfig config;
    ifstream file(configFile);
    string line;

    while (getline(file, line)) {
        if (line.substr(0, 6) == "Intent") {
            Intent intent;
            intent.intentName = line.substr(7);
            while (getline(file, line) && line != "") {
                intent.phrases.push_back(line);
            }
            while (getline(file, line) && line != "") {
                intent.responses.push_back(line);
            }
            config.intents.push_back(intent);
        } else if (line.substr(0, 6) == "Entity") {
            Entity entity;
            entity.entityName = line.substr(7);
            while (getline(file, line) && line != "") {
                entity.values.push_back(line);
            }
            config.entities.push_back(entity);
        }
    }

    return config;
}

void generateChatbot(const ChatbotConfig& config, const string& outputDir) {
    srand(time(0));

    for (const Intent& intent : config.intents) {
        string filename = outputDir + "/" + intent.intentName + ".cpp";
        ofstream file(filename);

        file << "#include <iostream>\n";
        file << "#include <string>\n";
        file << "#include <vector>\n";
        file << "\n";
        file << "class " << intent.intentName << " {\n";
        file << "public:\n";
        file << "    std::string respond(const std::string& input) {\n";
        file << "        std::vector<std::string> phrases = {";
        for (size_t i = 0; i < intent.phrases.size(); ++i) {
            file << "\"" << intent.phrases[i] << "\"";
            if (i < intent.phrases.size() - 1) {
                file << ", ";
            }
        }
        file << "};\n";
        file << "        std::vector<std::string> responses = {";
        for (size_t i = 0; i < intent.responses.size(); ++i) {
            file << "\"" << intent.responses[i] << "\"";
            if (i < intent.responses.size() - 1) {
                file << ", ";
            }
        }
        file << "};\n";
        file << "        for (const std::string& phrase : phrases) {\n";
        file << "            if (input.find(phrase) != std::string::npos) {\n";
        file << "                return responses[rand() % responses.size()];\n";
        file << "            }\n";
        file << "        }\n";
        file << "        return \"I didn't understand that.\";\n";
        file << "    }\n";
        file << "};\n";

        file.close();
    }
}

int main() {
    ChatbotConfig config = loadConfig("config.txt");
    generateChatbot(config, "generated_chatbots");

    return 0;
}