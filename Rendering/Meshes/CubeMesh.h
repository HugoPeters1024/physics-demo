class CubeMesh : public GenericMesh
{
public:
    CubeMesh(const GBufferShader* shader) : GenericMesh("Models/cube.obj", shader) {}
};