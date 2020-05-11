#pragma once

#include <sstream>

template <class T, int N>
AeVector<T, N>::AeVector() {
    std::memset(elements, 0, sizeof elements);
}

template <class T, int N>
template <class T2, int N2>
bool AeVector<T, N>::operator==(const AeVector<T2, N2> &other) const {
    for (int i = 0; i< N && i< N2 ;++i) {
        if (elements[i] != other.elements[i]) {
            return false;
        }
    }
    return true;
}

template <class T, int N>
template <class T2, int N2>
bool AeVector<T, N>::operator!=(const AeVector<T2, N2> &other) const {
    return !(this == other);
}

template <class T, int N>
template <class T2, int N2>
AeVector<T, N> &AeVector<T, N>::operator=(const AeVector<T2, N2> &other) {
    for (int i = 0; i < N && i < N2; ++i) {
        elements[i] = other.elements[i];
    }
    return *this;
}

template <class T, int N>
template <class T2, int N2>
AeVector<T, N> &AeVector<T, N>::operator+=(const AeVector<T2, N2> &other) {
    for (int i = 0; i < N && i < N2; ++i) {
        elements[i] += other.elements[i];
    }
    return *this;
}

template <class T, int N>
template <class T2, int N2>
AeVector<T, N> &AeVector<T, N>::operator-=(const AeVector<T2, N2> &other) {
    for (int i = 0; i < N && i < N2; ++i) {
        elements[i] -= other.elements[i];
    }
    return *this;
}

template <class T, int N>
template <class T2, int N2>
AeVector<T, N> &AeVector<T, N>::operator*=(const AeVector<T2, N2> &other) {
    for (int i = 0; i < N && i < N2; ++i) {
        elements[i] *= other.elements[i];
    }
    return *this;
}

template <class T, int N>
template <class T2, int N2>
AeVector<T, N> &AeVector<T, N>::operator/=(const AeVector<T2, N2> &other) {
    for (int i = 0; i < N && i < N2; ++i) {
        elements[i] /= other.elements[i];
    }
    return *this;
}

template <class T, int N>
template <class T2, int N2>
AeVector<T, N> AeVector<T, N>::operator+(const AeVector<T2, N2> &other) {
    AeVector<T, N> new_;
    for (int i = 0; i < N && i < N2; ++i) {
        new_.elements[i]= elements[i] + other.elements[i];
    }
    return new_;
}

template <class T, int N>
template <class T2, int N2>
AeVector<T, N> AeVector<T, N>::operator-(const AeVector<T2, N2> &other) {
    AeVector<T, N> new_;
    for (int i = 0; i < N && i < N2; ++i) {
        new_.elements[i] = elements[i] - other.elements[i];
    }
    return new_;
}

template <class T, int N>
template <class T2, int N2>
AeVector<T, N> AeVector<T, N>::operator*(const AeVector<T2, N2> &other) {
    AeVector<T, N> new_;
    for (int i = 0; i < N && i < N2; ++i) {
        new_.elements[i] = elements[i] * other.elements[i];
    }
    return new_;
}

template <class T, int N>
template <class T2, int N2>
AeVector<T, N> AeVector<T, N>::operator/(const AeVector<T2, N2> &other) {
    AeVector<T, N> new_;
    for (int i = 0; i < N && i < N2; ++i) {
        new_.elements[i] = elements[i] / other.elements[i];
    }
    return new_;
}

template <class T, int N>
template <class T2>
AeVector<T, N> &AeVector<T, N>::operator=(const T2 &other) {
    for (int i = 0; i < N ; ++i) {
        elements[i] = other;
    }
    return *this;
}

template <class T, int N>
template <class T2>
AeVector<T, N> &AeVector<T, N>::operator+=(const T2 &other) {
    for (int i = 0; i < N; ++i) {
        elements[i] += other;
    }
    return *this;
}

template <class T, int N>
template <class T2>
AeVector<T, N> &AeVector<T, N>::operator-=(const T2 &other) {
    for (int i = 0; i < N; ++i) {
        elements[i] -= other;
    }
    return *this;
}

template <class T, int N>
template <class T2>
AeVector<T, N> &AeVector<T, N>::operator*=(const T2 &other) {
    for (int i = 0; i < N; ++i) {
        elements[i] *= other;
    }
    return *this;
}

template <class T, int N>
template <class T2>
AeVector<T, N> &AeVector<T, N>::operator/=(const T2 &other) {
    for (int i = 0; i < N; ++i) {
        elements[i] /= other;
    }
    return *this;
}

template <class T, int N>
template <class T2>
AeVector<T, N> AeVector<T, N>::operator+(const T2 &other) {
    AeVector<T, N> new_;
    for (int i = 0; i < N; ++i) {
        new_.elements[i] = elements[i] + other;
    }
    return new_;
}

template <class T, int N>
template <class T2>
AeVector<T, N> AeVector<T, N>::operator-(const T2 &other) {
    AeVector<T, N> new_;
    for (int i = 0; i < N; ++i) {
        new_.elements[i] = elements[i] - other;
    }
    return new_;
}

template <class T, int N>
template <class T2>
AeVector<T, N> AeVector<T, N>::operator*(const T2 &other) {
    AeVector<T, N> new_;
    for (int i = 0; i < N; ++i) {
        new_.elements[i] = elements[i] * other;
    }
    return new_;
}

template <class T, int N>
template <class T2>
AeVector<T, N> AeVector<T, N>::operator/(const T2 &other) {
    AeVector<T, N> new_;
    for (int i = 0; i < N; ++i) {
        new_.elements[i] = elements[i] / other;
    }
    return new_;
}

template <class T>
int AeLib::findElementFromVector(std::vector<T> &vec, T element) {
    std::vector<T>::iterator it = std::find(vec.begin(), vec.end(), element);
    if (it == vec.end()) return INDEX_NONE;
    return int(it - vec.begin());
}

template <class T>
bool AeLib::eraseElementFromVector(std::vector<T> &vec, T element) {
    int index = findElementFromVector(vec, element);
    if (index == INDEX_NONE) return false;
    vec.erase(vec.begin() + index);
    return true;
}

template <class T>
T QeEncode::ConvertTo(const std::string &str) {
    std::stringstream ss(str);
    T num;
    ss >> num;
    return num;
}

template <class T>
std::vector<T> QeEncode::split(std::string s, std::string delim) {
    std::vector<T> tokens;
    /*char dup[4096];
    strncpy_s(dup, s, 4096);
    char *context = NULL;
    char *token = strtok_s(dup, delim, &context);
    while (token != NULL) {
        tokens.push_back(std::string(token));
        token = strtok_s(NULL, delim, &context);
    }*/

    size_t pos = 0;
    while ((pos = s.find(delim)) != std::string::npos) {
        tokens.push_back(ConvertTo<T>(s.substr(0, pos)));
        s.erase(0, pos + delim.length());
    }
    tokens.push_back(ConvertTo<T>(s));
    return tokens;
}

template <class T>
T AeXMLNode::getXMLValue(const char *key) {
    T ret;
    getXMLValue<T>(ret, key);
    return ret;
}

template <class T>
AeXMLNode *AeXMLNode::getXMLValue(T &value, const char *key) {
    std::memset((void *)&value, 0, sizeof value);
    auto keys = ENCODE->split<std::string>(key, ".");
    std::vector<std::string> keys1 = keys;
    keys1.pop_back();
    AeXMLNode *current = getXMLNode(keys1);
    if (!current) return nullptr;

    std::string final_key = keys.back();

    for (const auto &node : current->data->elements) {
        if (final_key.compare(node.key) == 0) {
            value = ENCODE->ConvertTo<T>(node.value);
            return current;
        }
    }

    current = getXMLNode(final_key.c_str());
    if (!current) return nullptr;
    if (final_key.compare(current->data->key.c_str()) == 0) {
        value = ENCODE->ConvertTo<T>(current->data->value);
        return current;
    }
    return nullptr;
}

template <class T, int N>
AeVector<T, N> AeXMLNode::getXMLValues(const char *key) {
    AeVector<T, N> ret;
    getXMLValues<T, N>(ret, key);
    return ret;
}

template <class T, int N>
AeXMLNode *AeXMLNode::getXMLValues(AeVector<T, N> &value, const char *key) {
    std::string str_values;
    AeXMLNode *ret = getXMLValue<std::string>(str_values, key);
    auto values = ENCODE->split<T>(str_values, " ");
    for (int i = 0; i < N; ++i) {
        value.elements[i] = values[i];
    }
    return ret;
}