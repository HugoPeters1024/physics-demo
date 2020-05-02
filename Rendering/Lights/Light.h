struct Light {
    Vector3 position;
    Vector3 color;
    float constant = 1.0f;
    float linear = 0.7f;
    float quadratic = 1.8f;

    float radius() const {
        float lightMax = std::fmax(std::fmax(color.x, color.y), color.z);
        float radius =  (-linear +  sqrtf(linear * linear - 4 * quadratic * (constant - (256.0 / 10.0) * lightMax)))
                        / (2 * quadratic);
        return radius;
    }

    Matrix4 getMvp() const {
      return Matrix4::FromTranslation(position) * Matrix4::FromScale(radius() / 18.737);
    }
};