#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Функция для проверки, является ли строка комментарием
bool isComment(const string& line) {
    return !line.empty() && line[0] == '#';
}

// Функция для разбиения строки на имя и значение параметра
pair<string, string> parseParameter(const string& line) {
    size_t pos = line.find('=');
    if (pos != string::npos) {
        string name = line.substr(0, pos);
        string value = line.substr(pos + 1);
        return make_pair(name, value);
    }
    else {
        // Ввозвращаем пустые строки если условие не выпонено
        return make_pair("", "");
    }
}

// Функция для поиска значения параметра по имени
string findParameterValue(const string& filename, const string& paramName) {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        if (!isComment(line)) {
            pair<string, string> param = parseParameter(line);
            if (param.first == paramName) {
                return param.second;
            }
        }
    }
    return ""; // Если параметр не найден
}

// Функция для проверки, содержится ли запрошенный параметр в комментариях
bool isParameterInComments(const string& filename, const string& paramName) {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        if (isComment(line) && line.find(paramName) != string::npos) {
            return true;
        }
    }
    return false;
}

// Функция для изменения значения параметра и сохранения новой конфигурации
void changeParameterValue(const string& filename, const string& paramName, const string& newValue) {
    vector<string> lines;
    ifstream file(filename);
    string line;
    bool parameterFound = false;
    while (getline(file, line)) {
        if (!isComment(line)) {
            pair<string, string> param = parseParameter(line);
            if (param.first == paramName) {
                line = param.first + "=" + newValue;
                parameterFound = true;
            }
        }
        else if (line.find(paramName) != string::npos) {
            // Если параметр найден в комментарии, заменяем его значение
            line = "#" + paramName + "=" + newValue;
            parameterFound = true;
        }
        lines.push_back(line);
    }

    // Если параметр не найден, добавляем его в конец файла
    if (!parameterFound) {
        lines.push_back(paramName + "=" + newValue);
    }

    ofstream newFile("new_" + filename);
    for (const string& l : lines) {
        newFile << l << endl;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    string filename, paramName, newValue;

    cout << "Введите имя файла (parametrs.txt): ";
    cin >> filename;

    cout << "Введите имя параметра: ";
    cin >> paramName;

    string paramValue = findParameterValue(filename, paramName);
    if (!paramValue.empty()) {
        cout << "Значение параметра " << paramName << ": " << paramValue << endl;
    }
    else if (isParameterInComments(filename, paramName)) {
        cout << "Параметр " << paramName << " найден в комментариях." << endl;
    }
    else {
        cout << "Параметр " << paramName << " не найден." << endl;
    }

    cout << "Введите новое значение параметра: ";
    cin >> newValue;

    changeParameterValue(filename, paramName, newValue);

    cout << "Новый файл сохранен." << endl;

    return 0;
}