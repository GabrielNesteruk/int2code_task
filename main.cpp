#include <iostream>
#include <string>
#include "result.h"

using namespace std;

enum class OperationStatus {
    OK = 0,
    FAIL = 1,
    SOMETHING = 2,
};

class CustomMoveType {
public:
    CustomMoveType() = default;
    ~CustomMoveType() = default;
    CustomMoveType(CustomMoveType& rhs) {
        cout << "In copy constructor" << endl;
    }
    CustomMoveType(CustomMoveType&& rhs) {
        cout << "In move constructor" << endl;
    }

    CustomMoveType& operator=(const CustomMoveType& rhs) = delete;
    CustomMoveType& operator=(CustomMoveType&& rhs) = delete;
};

int main() {
    Result<void, OperationStatus> result_with_void{OperationStatus::OK};
    if (result_with_void.IsValid()) {
        cout << "result_with_void OK" << endl;
    }

    Result<CustomMoveType, OperationStatus> result_1{CustomMoveType{}};
    result_1.ExtractValue();

    Result<CustomMoveType, OperationStatus> result_2{OperationStatus::FAIL};
    // result_2.ExtractValue();

    // result_2.IsValid();
    // result_2.ExtractValue();

    Result<CustomMoveType, OperationStatus> result_3{CustomMoveType{}};
    result_3.IsValid();
    result_3.ExtractValue();

    Result<string, OperationStatus> result_4{"hello"};
    auto&& v = result_4.ExtractValue();
    std::cout << v << std::endl;

    // auto&& v2 = result_4.GetValue();

    return 0;
}