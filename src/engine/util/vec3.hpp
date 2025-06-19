struct vec3 {
    float x, y, z;
    constexpr vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}
    constexpr vec3(const vec3& v) : x(v.x), y(v.y), z(v.z) {}
    ~vec3(void) = default;
    float dot(const vec3& rhs) const;   // Dot product
    vec3 cross(const vec3& rhs) const;  // Cross product
    float magnitude(void) const;        // Magnitude (length) of the vector
    vec3 normalize(void) const;         // Normalize the vector (make it unit length)
    void print(void) const;
    vec3& operator=(const vec3& rhs);           // Assignment operator
    vec3 operator+(const vec3& rhs) const;      // Vector addition
    vec3 operator-(const vec3& rhs) const;      // Vector subtraction
    vec3 operator*(float scalar) const;         // Scalar multiplication
    vec3 operator/(float scalar) const;         // Scalar division
    bool operator==(const vec3& rhs) const;     // Equality comparison
    bool operator!=(const vec3& rhs) const;     // Inequality comparison
    vec3& operator+=(const vec3& rhs);          // Compound assignment operators
    vec3& operator-=(const vec3& rhs);
    vec3& operator*=(float scalar);
    vec3& operator/=(float scalar);
};