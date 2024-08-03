#include "result_base.h"
#include <stdexcept>

template <class Value, class Status>
class Result : public ResultBase<Status> {
  public:
    Result(Status status);
    Result(Result&& rhs);
    Result(const Value& value);
    Result(Value&& value);

    const Value& GetValue() const;
    // In case value is only movable object then there is an option to extract it from Result by std::move
    Value&& ExtractValue();

  private:
    Value value_ = {};
    bool is_value_extracted_ = false;
};

template <class Value, class Status>
Result<Value, Status>::Result(Status status) : ResultBase<Status>{status} {}

template <class Value, class Status>
Result<Value, Status>::Result(Result&& rhs) : ResultBase<Status>{std::move(rhs)}, value_{std::move(rhs.value_)} {}

template <class Value, class Status>
Result<Value, Status>::Result(const Value& value) : ResultBase<Status>{Status::OK}, value_{value} {}

template <class Value, class Status>
Result<Value, Status>::Result(Value&& value) : ResultBase<Status>{Status::OK}, value_{std::move(value)} {}

template <class Value, class Status>
const Value& Result<Value, Status>::GetValue() const {
    if (not ResultBase<Status>::is_status_checked_ or ResultBase<Status>::status_ != Status::OK) {
        // should be some type of assert here, like hardware assert or fatal log for example in case of embedded env
        throw std::runtime_error("Getting Result value before checking status or value is not valid");
    }

    // if value was moved already we should not allow to get it
    if (is_value_extracted_) {
        // should be some type of assert here, like hardware assert or fatal log for example in case of embedded env
        throw std::runtime_error("Getting Result value when it already has been moved");
    }

    return value_;
}

template <class Value, class Status>
Value&& Result<Value, Status>::ExtractValue() {
    if (not ResultBase<Status>::is_status_checked_ or ResultBase<Status>::status_ != Status::OK) {
        // should be some type of assert here, like hardware assert or fatal log for example in case of embedded env
        throw std::runtime_error("Getting Result value before checking status or value is not valid");
    }
    is_value_extracted_ = true;
    return std::move(value_);
}


template <class Status>
class Result<void, Status> : public ResultBase<Status> {
  public:
    Result(Status status);
};

template <class Status>
Result<void, Status>::Result(Status status) : ResultBase<Status>{status} {}