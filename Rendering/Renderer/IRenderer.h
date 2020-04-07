void error_callback(int error, const char* description)
{
  fprintf(stderr, "Error: %s\n", description);
}

class IRenderer
{
public:
    virtual void loop(const Camera::Camera* camera) = 0;
    virtual GLFWwindow* getWindowPointer() const = 0;
    virtual bool shouldClose() = 0;
    virtual float getWindowRatio() const = 0;
    virtual void addCube(rp3d::ProxyShape* shape, rp3d::BoxShape* box) = 0;
    virtual void addSphere(rp3d::ProxyShape* shape, rp3d::SphereShape* box) = 0;
    virtual void addHeightMap(rp3d::ProxyShape* shape, rp3d::HeightFieldShape* map, float* data) = 0;
};

