#include <unordered_map>
#include <functional>
#include <string>
#include <iostream>

#include <msgpack.hpp>

using namespace std;

template<class TupType, size_t... I>
void print(const TupType& _tup, std::index_sequence<I...>)
{
    std::cout << "(";
    (..., (std::cout << (I == 0? "" : ", ") << std::get<I>(_tup)));
    std::cout << ")\n";
}

template<class... T>
void print (const std::tuple<T...>& _tup)
{
    print(_tup, std::make_index_sequence<sizeof...(T)>());
}

std::string do_nothing(std::string something) {
    return something + "hasdhoishdoiashidosa";
}

// https://en.cppreference.com/w/cpp/utility/integer_sequence
template <template <typename...> class Params, typename... Args, std::size_t... I>
auto convert_helper(const Params<Args...> &params, std::index_sequence<I...>) {
    return std::make_tuple(std::get<I>(params)...);
}

template <template <typename...> class Params, typename... Args>
auto convert(const Params<Args...> &params) {
    return convert_helper(params, std::index_sequence_for<Args...>{});
}

template <typename R, typename... Args>
void my_bind(
        unordered_map<string, function<msgpack::object(msgpack::object)>> &functions,
        string name, R (*func)(Args...)) {
    auto wrapper = std::function<R(Args...)>(func);
    functions.insert({
        name,
        [wrapper](msgpack::object args_) -> msgpack::object {
            msgpack::type::tuple<Args...> args;
            auto res = apply(wrapper, convert(args));
            msgpack::zone z;
            return msgpack::object(res, z);
        }
    });
}

auto getoh() {
    msgpack::type::tuple<std::string> src("example");

    // serialize the object into the buffer.
    // any classes that implements write(const char*,size_t) can be a buffer.
    std::stringstream buffer;
    msgpack::pack(buffer, src);

    // send the buffer ...
    buffer.seekg(0);

    // deserialize the buffer into msgpack::object instance.
    std::string str(buffer.str());

    auto oh = msgpack::unpack(str.data(), str.size());

    return oh;
}

int main() {
    unordered_map<string, function<msgpack::object(msgpack::object)>> functions;
    my_bind(functions, "hehe", &do_nothing);
//    bool a = functions.find("hehe") == functions.end();
//    cout << a << endl;
    auto what = functions["hehe"];
    auto oh = getoh();
    auto res = what(oh.get()).as<std::string>();
    cout << res << endl;
    return 0;
}