// task_2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <vector>

class Observer {
public:
	virtual void Warning(const std::string& message) {};
	virtual void Error(const std::string& message) {};
	virtual void FatalError(const std::string& message) {};
	virtual ~Observer() = default;
};

class Observable {
public:
	void addObserver(Observer* observer) {
		observers_.push_back(observer);
	}

	void removeObserver(Observer* observer) {
		auto it = remove(observers_.begin(), observers_.end(), observer);
		observers_.erase(it, observers_.end());
	}

	void warning() {
		for (auto obs : observers_) {
			obs->Warning("Warning!");
		}
	}

	void error() {
		for (auto obs : observers_) {
			obs->Error("Error!");
		}
	}

	void fatalError() const {
		for (auto obs : observers_) {
			obs->FatalError("Fatal Error!");
		}
	}

private:
	std::vector <Observer*> observers_;
};


class WarningObserver : public Observer {
public:
	void Warning(const std::string& message) override {
		std::cout << "Warning: " << message << "\n";
	}
};

class ErrorObserver : public Observer {
private:
	std::ofstream write;
public:
	ErrorObserver(const std::string& filename) {
		write.open(filename, std::ofstream::app);
		if (!write.is_open()) {
			std::cout << "Can't open the file: " << filename << "!\n";
		}
	}

	~ErrorObserver() {
		write.close();
	}

	void Error(const std::string& message) override {
		write << "Error: " << message << "\n";
	}
};

class FatalErrorObserver : public Observer {
private:
	std::ofstream write;
public:
	FatalErrorObserver(const std::string& filename) {
		write.open(filename, std::ofstream::app);
		if (!write.is_open()) {
			std::cout << "Can't open the file: " << filename << "!\n";
		}
	}

	~FatalErrorObserver() {
		write.close();
	}

	void FatalError(const std::string& message) override {
		std::cout << "Fatal error: " << message << "\n";
		write << "Fatal error: " << message << "\n";
	}
};

int main() {

	Observable Obs;
	WarningObserver warning;
	ErrorObserver error("error_file.txt");
	FatalErrorObserver fatal("fatal_file.txt");

	Obs.addObserver(&warning);
	Obs.addObserver(&error);
	Obs.addObserver(&fatal);

	Obs.warning();
	Obs.error();
	Obs.fatalError();

	return 0;
}