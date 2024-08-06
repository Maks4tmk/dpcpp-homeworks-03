// task_3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>

enum class Type {
    Warning,
    Error,
    FatalError,
    Unknown
};

class LogMessage {
private:
    Type type_;
    std::string text_;

public:
    LogMessage(Type type, const std::string text) :type_(type), text_(text) {}

    Type type()const {
        return type_;
    }

    const std::string& message() const {
        return text_;
    }
};

class LogHandler {
protected:
    LogHandler* nextHandler;

public:
    LogHandler(LogHandler* handler = nullptr) : nextHandler(handler) {};

    virtual ~LogHandler() = default;

    void setNext(LogHandler* handler) {
        nextHandler = handler;
    }

    virtual void handleMessage(const LogMessage& message) = 0;
};

class Warning : public LogHandler {
public:
    void handleMessage(const LogMessage& message) override {
        if (message.type() == Type::Warning) {
            std::cout << "Log: " << message.message() << "\n";
        }
        else {
            nextHandler->handleMessage(message);
        }
    }
};

class Error : public LogHandler {
private:
    std::ofstream write;

public:
    Error(const std::string& filename) {
        write.open(filename, std::ofstream::app);
        if (!(write.is_open())) {
            std::cout << "Can't open the file: " << filename << "!\n";
        }
    }

    ~Error() {
        write.close();
    }

    void handleMessage(const LogMessage& message) override {
        if (message.type() == Type::Error) {
            write << "Log: " << message.message() << "\n";
        }
        else {
            nextHandler->handleMessage(message);
        }
    }
};

class FatalError : public LogHandler {
public:
    void handleMessage(const LogMessage& message) override {
        if (message.type() == Type::FatalError) {
            throw std::runtime_error(message.message());
        }
        else {
            nextHandler->handleMessage(message);
        }
    }
};

class Unknown : public LogHandler {
public:
    void handleMessage(const LogMessage& message) override {
        if (message.type() == Type::Unknown) {
            std::cout << "Log: " << message.message() << "\n";
        }
        else {
            nextHandler->handleMessage(message);
        }
    }
};

int main() {
    LogMessage log1(Type::Error, "Error!");
    LogMessage log2(Type::Warning, "Warning!");
    LogMessage log3(Type::Unknown, "Unknown message!");
    LogMessage log4(Type::FatalError, "Fatal error, programm closed!");

    FatalError fatalErrorHandler;
    Error errorHandler("file.txt");
    Warning warningHandler;
    Unknown unknownHandler;

    fatalErrorHandler.setNext(&errorHandler);
    errorHandler.setNext(&warningHandler);
    warningHandler.setNext(&unknownHandler);

    fatalErrorHandler.handleMessage(log1);
    fatalErrorHandler.handleMessage(log2);

    try {
        fatalErrorHandler.handleMessage(log3);
        fatalErrorHandler.handleMessage(log4);
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
