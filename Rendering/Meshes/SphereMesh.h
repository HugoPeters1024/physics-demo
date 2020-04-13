class SphereMesh : public GenericMesh
{
public:
    SphereMesh(const GBufferShader* shader) : GenericMesh("Models/sphere.obj", shader) {};
};