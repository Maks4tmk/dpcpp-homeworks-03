// task_1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>



class LogCommand {
public:
    virtual ~LogCommand() = default;
    virtual void print(const std::string& message) = 0;
};



class ConsoleLogCommand : public LogCommand {
public:
    void print(const std::string& message) override {
        std::cout << message << std::endl;
    }
};



class FileLogCommand : public LogCommand {
public:
    FileLogCommand() {
        m_file.open("log.txt", std::fstream::app);
        if (!m_file.is_open()) {
            std::cout << "Не удается открыть файл log.txt!" << std::endl;
        }
    }

    ~FileLogCommand() {
        m_file.close();
    }

    void print(const std::string& message) override {
        m_file << message << std::endl;
    }

private:
    std::fstream m_file;
};



void print(LogCommand& command, const std::string& message) {
    command.print(message);
}



int main() {
    setlocale(LC_ALL, "ru");

    ConsoleLogCommand consoleCommand;
    FileLogCommand fileCommand;

    print(consoleCommand, "Привет консоль");
    print(fileCommand, "Привет фаил");

    return 0;
}

