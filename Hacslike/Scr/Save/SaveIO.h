#pragma once

#include <fstream>
#include <string>
#include <cstdint>
#include <type_traits>

// BinaryWriter / BinaryReader をヘッダ化して他の cpp から利用可能にする
class BinaryWriter {
public:
    explicit BinaryWriter(std::ofstream& stream) : s(stream) {}

    // POD 書き込み
    template<typename T>
    typename std::enable_if<std::is_pod<T>::value, void>::type
        WritePOD(const T& v) {
        s.write(reinterpret_cast<const char*>(&v), sizeof(T));
    }

    void WriteString(const std::string& str) {
        uint32_t len = static_cast<uint32_t>(str.size());
        WritePOD(len);
        if (len) s.write(str.data(), len);
    }

private:
    std::ofstream& s;
};

class BinaryReader {
public:
    explicit BinaryReader(std::ifstream& stream) : s(stream) {}

    template<typename T>
    typename std::enable_if<std::is_pod<T>::value, void>::type
        ReadPOD(T& v) {
        s.read(reinterpret_cast<char*>(&v), sizeof(T));
    }

    std::string ReadString() {
        uint32_t len = 0;
        ReadPOD(len);
        if (len == 0) return std::string();
        std::string out(len, '\0');
        s.read(&out[0], len);
        return out;
    }

private:
    std::ifstream& s;
};