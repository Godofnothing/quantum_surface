#pragma once

#include <array>
#include <cmath>
#include <iostream>

namespace geo{
    
    template <typename T, size_t DIM>
    class Vector : public std::array<T, DIM>{
    public:
        Vector& operator+=(const Vector<T, DIM>& v){
            for(size_t i = 0; i < DIM; i++){
                (*this)[i] += v[i];
            } return *this;
        }

        Vector& operator-=(const Vector<T, DIM>& v){
            for(size_t i = 0; i < DIM; i++){
                (*this)[i] -= v[i];
            } return *this;
        }

        template <typename U>
        Vector& operator*=(U c){
            for(size_t i = 0; i < DIM; i++){
                (*this)[i] *= c;
            } return *this;
        }

        Vector& operator/=(T c){
            for(size_t i = 0; i < DIM; i++){
                (*this)[i] /= c;
            } return *this;
        }

        T norm() const;

        T norm2() const;

        Vector& normalize();
    };

    template <typename T, size_t DIM>
    Vector<T, DIM> operator+(const Vector<T, DIM>& a, const Vector<T, DIM>& b){
        return Vector(a) += b;
    }

    template <typename T, size_t DIM>
    Vector<T, DIM> operator-(const Vector<T, DIM>& a, const Vector<T, DIM>& b){
        return Vector(a) -= b;
    }

    template <typename T, typename U, size_t DIM>
    Vector<T, DIM> operator*(U c, const Vector<T, DIM>& v){
        return Vector(v) *= c;
    }

    template <typename T, typename U, size_t DIM>
    Vector<T, DIM> operator*(const Vector<T, DIM>& v, U c){
        return Vector(v) *= c;
    }

    template <typename T, typename U, size_t DIM>
    Vector<T, DIM> operator/(const Vector<T, DIM>& v, U c){
        return Vector(v) /= c;
    }

    template <typename T, size_t DIM>
    T dot(const Vector<T, DIM>& a, const Vector<T, DIM>& b){
        T sum = 0;
        for(size_t i = 0; i < DIM; i++){
            sum += a[i] * b[i];
        } return sum;
    }

    template <typename T>
    Vector<T, 3> cross(const Vector<T, 3>& a, const Vector<T, 3>& b){
        return {
             a[1]*b[2] - a[2]*b[1],
             a[2]*b[0] - a[0]*b[2],
             a[0]*b[1] - a[1]*b[0] 
        };
    }

    template <typename T, size_t DIM>
    T Vector<T, DIM>::norm2() const{
        return dot(*this, *this);
    }

    template <typename T, size_t DIM>
    T Vector<T, DIM>::norm() const{
        return sqrt(norm2());
    }

    template <typename T, size_t DIM>
    Vector<T, DIM>& Vector<T, DIM>::normalize(){
        return *this /= norm();
    }

    template <typename T, size_t DIM>
    std::ostream& operator<<(std::ostream& os, const Vector<T, DIM>& v){
        os << "(";
        for(size_t i = 0; i < DIM; i++){
            if(i != 0) 
                os << " ";
            os << v[i];
        } os << ")";
        return os;
    }

    template <typename T>
    T triangle_area(const Vector<T, 3>& a, const Vector<T, 3>& b){
        return cross(a, b).norm() / 2;
    }

    template <typename T, size_t DIM>
    T cos(const Vector<T, DIM>& a, Vector<T, DIM>& b){
        return dot(a, b) / a.norm() / b.norm();
    }
}