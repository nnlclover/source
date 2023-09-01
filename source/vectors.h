#ifndef __VECTORS_H_
#define __VECTORS_H_

#include <stdint.h>
#include <iostream>

#define float_to_rgb(x) 255 * x

struct MVector2i
{
    int x, y;
    MVector2i operator=(MVector2i vec)
    {
        return MVector2i(this->x + vec.x, this->y + vec.y);
    }
    MVector2i operator+(MVector2i vec)
    {
        return MVector2i(this->x + vec.x, this->y + vec.y);
    }
    MVector2i operator-=(MVector2i obj)
    {
        x -= obj.x;
        y -= obj.y;
        return MVector2i(x, y);
    }
    MVector2i()
    {
        x = 0; y = 0;
    }
    MVector2i(int x, int y)
    {
        this->x = x; this->y = y;
    }
    void set(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
};

struct MVector3f
{
    float x, y, z;
    MVector3f operator+(MVector3f vec)
    {
        return MVector3f(this->x + vec.x, this->y + vec.y, this->z + vec.z);
    }
    MVector3f()
    {
        x = 0; y = 0; z = 0;
    }
    MVector3f(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    MVector3f operator-(MVector3f& obj)
    {
        return MVector3f(x - obj.x, y - obj.y, z - obj.z);
    }

    MVector3f operator-(const MVector3f& obj)
    {
        return MVector3f(x - obj.x, y - obj.y, z - obj.z);
    }

    MVector3f operator*(float obj)
    {
        return MVector3f(x * obj, y * obj, z * obj);
    }

    MVector3f operator*=(float obj)
    {
        x *= obj; 
        y *= obj;
        z *= obj;
        return MVector3f(x, y, z);
    }

    MVector3f operator*(MVector3f& obj)
    {
        return MVector3f(x * obj.x, y * obj.y, z * obj.z);
    }

    //MVector3f operator*(float& obj)
    //{
    //    return MVector3f(x * obj, y * obj, z * obj);
    //}

    MVector3f operator+=(MVector3f obj)
    {
        x += obj.x;
        y += obj.y;
        z += obj.z;
        return MVector3f(x, y, z);
    }

    MVector3f operator-=(MVector3f obj)
    {
        x -= obj.x;
        y -= obj.y;
        z -= obj.z;
        return MVector3f(x, y, z);
    }

    void set(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};


struct MVector2f
{
    float x, y;
    MVector2f operator+(MVector2f vec)
    {
        return MVector2f(this->x + vec.x, this->y + vec.y);
    }
    MVector2f()
    {
        x = 0; y = 0;
    }
    MVector2f(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    MVector2f operator-(MVector2f& obj)
    {
        return MVector2f(x - obj.x, y - obj.y);
    }
    
    MVector2f operator*(float obj)
    {
        return MVector2f(x * obj, y * obj);
    }

    MVector2f operator*=(float obj)
    {
        x *= obj; 
        y *= obj;
        return MVector2f(x, y);
    }

    MVector2f operator*(MVector2f& obj)
    {
        return MVector2f(x * obj.x, y * obj.y);
    }

    MVector2f operator+=(MVector2f obj)
    {
        x += obj.x;
        y += obj.y;
        return MVector2f(x, y);
    }

    MVector2f operator-=(MVector2f obj)
    {
        x -= obj.x;
        y -= obj.y;
        return MVector2f(x, y);
    }

    void set(float x, float y)
    {
        this->x = x;
        this->y = y;
    }
};

struct Color
{
    uint8_t r = 0, g = 0, b = 0, a = 255;
    Color(float _r, float _g, float _b, float _a)
    {
      r = float_to_rgb(_r);
      g = float_to_rgb(_g);
      b = float_to_rgb(_b);
      a = float_to_rgb(_a);
    }

    Color(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a)
    {
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }

    Color(int _r, int _g, int _b, int _a)
    {
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }


    Color(int _r, int _g, int _b)
    {
        r = _r;
        g = _g;
        b = _b;
    }

    Color() = default;

    bool operator!=(const Color& object)
    {
        if (r != object.r || g != object.g || b != object.b || a != object.a)
            return true;
        return false;
    }
    bool operator==(const Color& object)
    {
        if (r == object.r && g == object.g && b == object.b && a == object.a)
            return true;
        return false;
    }
};



inline bool indiap(MVector2i min, MVector2i max, MVector2i pos)
{
     if ( min.x < pos.x && min.y < pos.y && max.x > pos.x && max.y > pos.y )
          return true;
     return false;
}



template<typename T>
class ManualVector {
private:
    T* data;
    size_t size;
    size_t capacity;

    // Функция для увеличения емкости вектора
    void increaseCapacity() {
        capacity = capacity == 0 ? 1 : capacity * 2;
        T* newData = new T[capacity];
        for (size_t i = 0; i < size; ++i) {
            newData[i] = std::move(data[i]);
        }
        delete[] data;
        data = newData;
    }

    // Функция для уменьшения емкости вектора
    void decreaseCapacity() {
        if (capacity == 0) {
            return; // Вектор уже пуст, ничего не делаем
        }
        capacity = capacity / 2;
        T* newData = new T[capacity];
        for (size_t i = 0; i < size; ++i) {
            newData[i] = std::move(data[i]);
        }
        delete[] data;
        data = newData;
    }

public:
    // Конструктор
    ManualVector() : data(nullptr), size(0), capacity(0) {}

    // Деструктор
    ~ManualVector() {
        delete[] data;
    }

    // Добавление элемента в конец вектора
    void push_back(const T& value) {
        if (size >= capacity) {
            increaseCapacity();
        }
        data[size++] = value;
    }

    // Удаление элемента из вектора по индексу
    void remove(size_t index) {
        if (index >= size) {
            return; // Неверный индекс, ничего не делаем
        }
        for (size_t i = index; i < size - 1; ++i) {
            data[i] = std::move(data[i + 1]);
        }
        size--;

        // Если размер стал меньше половины емкости, уменьшаем емкость
        if (size < capacity / 2) {
            decreaseCapacity();
        }
    }

    // Получение размера вектора
    size_t getSize() const {
        return size;
    }

    // Получение элемента по индексу
    T& operator[](size_t index) {
        return data[index];
    }

    // Константная версия оператора []
    const T& operator[](size_t index) const {
        return data[index];
    }
};


#endif
