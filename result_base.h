#include <type_traits>
#include <utility>

namespace result_details {
template <class T, class U = int>
struct has_ok : std::false_type {};

template <class T>
struct has_ok<T, decltype((void)T::OK, 0)> : std::true_type {};

}

template <class Status>
class ResultBase {
    static_assert(std::is_enum<Status>::value, "Status type is not enum");
    static_assert(result_details::has_ok<Status>::value, "Status type has no OK value");

public:
    ResultBase(Status status);
    ResultBase(ResultBase&& rhs);
    ~ResultBase();

    ResultBase(const ResultBase& rhs) = delete;
    ResultBase& operator=(const ResultBase& rhs) = delete;
    ResultBase& operator=(ResultBase&& rhs) = delete;

    bool IsValid() const;
    Status GetStatus() const;

  protected:
    const Status status_;
    mutable bool is_status_checked_ = false;
};

template <class Status>
ResultBase<Status>::ResultBase(Status status) : status_{status}, is_status_checked_{true} {}

template <class Status>
ResultBase<Status>::ResultBase(ResultBase&& rhs) : status_{rhs.status_}, is_status_checked_{true} {}

template <class Status>
ResultBase<Status>::~ResultBase() {}

template <class Status>
bool ResultBase<Status>::IsValid() const {
    is_status_checked_ = true;
    return status_ == Status::OK;
}

template <class Status>
Status ResultBase<Status>::GetStatus() const {
    is_status_checked_ = true;
    return status_;
}